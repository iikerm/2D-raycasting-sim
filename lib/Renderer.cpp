#include "Renderer.hpp"

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
    // for (unsigned long i=0; i<this->cam->rayEndpoints.size())

    double borderSize = 10.f;
    sf::RectangleShape borders(size);
    borders.setPosition(pos - sf::Vector2f(borderSize, -borderSize));
    borders.setOutlineColor(sf::Color::White);
    borders.setOutlineThickness(borderSize);
    borders.setFillColor(sf::Color::Black);
    win.draw(borders);
    
    for (unsigned i=0; i<cam->view.size(); i++){
        sf::RectangleShape line(sf::Vector2f(max(1.f, size.x / cam->rayAmount), size.y));
        if (!cam->view[i]->getCollidedWithObject()){
            line.setFillColor(sf::Color::Black);
        }else{
            line.setFillColor(
                darkenByDepth(cam->view[i]->euclideanDistanceToCollision(), sf::Color(255, 0, 0))
            );
        }
        line.setPosition(pos + sf::Vector2f(cam->view.size()-i * line.getSize().x, 0));
        line.setPosition(line.getPosition() - sf::Vector2f(borderSize, -borderSize));
        win.draw(line);
    }

    /*
    sf::RectangleShape minLine(sf::Vector2f(max(1.f, size.x / cam->rayAmount), size.y));
    minLine.setPosition(pos + sf::Vector2f((cam->view.size() - minDepthIndex)*minLine.getSize().x, 0));
    minLine.setFillColor(sf::Color::Cyan);
    win.draw(minLine);

    sf::RectangleShape maxLine(sf::Vector2f(max(1.f, size.x / cam->rayAmount), size.y));
    maxLine.setPosition(pos + sf::Vector2f((cam->view.size() - maxDepthIndex)*maxLine.getSize().x, 0));
    maxLine.setFillColor(sf::Color::Green);
    win.draw(maxLine);
    */
}
