#include "Camera.hpp"

const double Camera::defaultViewAngle = 30.F;
const unsigned short Camera::defaultRayAmount = 100u;

Camera::~Camera(){
    for (unsigned short i=0; i<view.size(); i++){
        delete view[i];
    }
}


void Camera::setupRays(const sf::RenderWindow &win){
    double angleStep = viewAngle / rayAmount;
    
    view = vector<Ray*>(rayAmount);
    
    
    for (unsigned short i=0; i<rayAmount; i++){
        view[i] = new Ray(pos, pos - sf::Vector2f(0, viewLength), win);
        
        // This makes it so that the camera starts facing correctly in a single 
        // direction, and the next rays are generated from that initial angle
        view[i]->rotateDegrees(-viewAngle/2 + i*angleStep);
    }
}

void Camera::setupBody(){
    body = sf::CircleShape(100, 30ul);
    body.setPosition(pos - sf::Vector2f(body.getRadius(), body.getRadius()));

    body.setFillColor(sf::Color(253, 146, 16));       // #fd9210
}

Camera::Camera(const sf::RenderWindow &win){
    pos = sf::Vector2f(0, 0);
    viewAngle = defaultViewAngle;
    viewLength = max(win.getSize().x, win.getSize().y);
    rayAmount = defaultRayAmount;

    this->rotation = 0;

    setupBody();
    setupRays(win);
}

Camera::Camera(sf::Vector2f pos,
               const sf::RenderWindow &win,
               double viewAngle,
               unsigned short rayAmount){
    
    this->pos = pos;
    this->viewAngle = viewAngle;
    this->viewLength = max(win.getSize().x, win.getSize().y)*2;
    cout << "length is " << viewLength << endl;
    this->rayAmount = rayAmount;

    this->rotation = 0;

    setupBody();
    setupRays(win);
}
    
void Camera::move(sf::Vector2f offset, vector<sf::RectangleShape*> &colliders){
    sf::CircleShape bodyCopy = body;
    bodyCopy.setPosition(body.getPosition() + offset);

    for (unsigned i=0; i<colliders.size(); i++){
        if (colliders[i]->getGlobalBounds().intersects(bodyCopy.getGlobalBounds())){
            return;
        }
    }
    
    for (short unsigned i=0; i<view.size(); i++){
        view[i]->move(offset);
    }
    body.setPosition(body.getPosition() + offset);
    this->pos += offset;
}

void Camera::rotate(double angle){
    for (short unsigned i=0; i<view.size(); i++){
        view[i]->rotate(angle);
    }

    this->rotation += angle;
}

// Maybe change sf::RectangleShape into sf::VertexArray or something similar for more flexibility.
// Perhaps making an overload that accepts a vector of sf::VertexArray??
void Camera::castRays(vector<sf::RectangleShape*> &colliders){
    view[0]->sortColliders(colliders);
    for (short unsigned i=0; i<view.size(); i++){
        view[i]->castIt(vector<sf::RectangleShape*>(colliders));
    }
    
}

void Camera::drawIn(sf::RenderWindow &window, bool debug){
    sf::VertexArray viewCone(sf::PrimitiveType::TriangleFan, view.size()+1);
    viewCone[0] = this->pos;

    for (short unsigned i=1; i<(view.size()+1); i++){
        viewCone[i] = view[i-1]->points[view[i-1]->pointInCollision];
    }

    window.draw(viewCone);
    
    window.draw(body);
    
    if (debug){
        for (int j=0; j<view.size(); j++){
            for (int i=0; i<view[j]->pointInCollision; i++){
                sf::CircleShape pt(10, 30);
                pt.setPosition(view[j]->points[i]);
                pt.setFillColor(sf::Color(i*20, 10*i, 255 - 20*i));
                window.draw(pt);
            }
        }
    }
}
