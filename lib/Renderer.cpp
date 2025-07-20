#include "Renderer.hpp"
#include <cmath>

const sf::Color CAMERA_COLOR = sf::Color(246, 175, 90);       //rgb(246, 175, 90)

/**
 * Call only when camera moves to avoid unnecessary calculations
 */
void Renderer::computeRenderDistances(){
    this->minRenderedDepth = cam->view[0]->euclideanDistanceToCollision();
    this->maxRenderedDepth = minRenderedDepth;

    minDepthIndex = 0;
    maxDepthIndex = 0;

    double currentDistance = 0.f;

    for (unsigned long i=1; i<cam->view.size(); i++){
        currentDistance = cam->view[i]->euclideanDistanceToCollision();

        minRenderedDepth = min(minRenderedDepth, currentDistance);
        maxRenderedDepth = max(maxRenderedDepth, currentDistance);

        if(minRenderedDepth == currentDistance){
            minDepthIndex = i;
            continue;       // Because if it is the minimum, it wont also be the maximum
        }

        if(maxRenderedDepth == currentDistance){
            maxDepthIndex = i;
        }
    }

}

Renderer::Renderer(Camera &cam, sf::Vector2f size, sf::Vector2f pos){
    this->cam = &cam;
    this->size = size;
    this->pos = pos;

    this->renderBorderSize = 10.f;

    computeRenderDistances();
}

sf::Color Renderer::darkenByDepth(double actualDepth, sf::Color original){
    // original will be the brightest color
    double maxValue = cam->getMaximumDistance();

    return sf::Color(
        original.r * (1 - ((actualDepth)/(maxValue*2))),
        original.g * (1 - ((actualDepth)/(maxValue*2))),
        original.b * (1 - ((actualDepth)/(maxValue*2)))
    );
}

void Renderer::drawRender(sf::RenderWindow &win){

    sf::RectangleShape borders(size);
    borders.setPosition(pos - sf::Vector2f(renderBorderSize, -renderBorderSize));
    borders.setOutlineColor(CAMERA_COLOR);
    borders.setOutlineThickness(renderBorderSize);
    borders.setFillColor(CAMERA_COLOR);
    win.draw(borders);
    
    unsigned lineIndex=0;
    double step = (((double)cam->rayAmount) / size.x) + 0.001;
    sf::Vector2f linePosition;
    unsigned previousIdx=1;

    for (double i=0; i<cam->view.size(); i+=step){
        unsigned idx = static_cast<int>(i);
        
        if (static_cast<double>(idx) == i){
            continue;
        }
        if (previousIdx == idx){
            // To ensure that each cam->view[] point is rendered only once
            continue;
        }

        sf::RectangleShape line(sf::Vector2f(max(1.f, size.x / cam->rayAmount), size.y));
        if (!cam->view[idx]->getCollidedWithObject()){
            line.setFillColor(sf::Color::Black);
        }else{
            line.setFillColor(
                darkenByDepth(cam->view[idx]->euclideanDistanceToCollision(), sf::Color::Red)
            );
        }

        linePosition = sf::Vector2f(size.x-(lineIndex*line.getSize().x), 0);
        linePosition += pos - sf::Vector2f(renderBorderSize + line.getSize().x, -renderBorderSize);
        line.setPosition(linePosition);
        win.draw(line);

        lineIndex++;

        previousIdx = idx;
    }
}


void Renderer::drawRenderInfo(string fontPath, sf::RenderWindow &win){
    sf::Font renderFont;
    if (!renderFont.loadFromFile(fontPath)){
        cerr << "Unable to render font at: " << fontPath << endl;
        return;
    }

    sf::Text infoBelowDisplay("CAMERA VIEW - FACING: " 
                                + to_string(static_cast<int>(cam->getRotation())) + " deg", 
                                renderFont, 
                                30u
                            );
    infoBelowDisplay.setFillColor(CAMERA_COLOR);
    infoBelowDisplay.setPosition(pos + sf::Vector2f(-renderBorderSize - 7, 
                                                    size.y + renderBorderSize + 7)
                                );
    infoBelowDisplay.setOutlineColor(sf::Color::Black);
    infoBelowDisplay.setOutlineThickness(1);
    win.draw(infoBelowDisplay);
}
