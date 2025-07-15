#include "Camera.hpp"

const double Camera::defaultViewAngle = 45.F;
const double Camera::defaultViewLength = 100.F;
const unsigned short Camera::defaultRayAmount = 100u;

Camera::~Camera(){
    for (unsigned short i=0; i<view.size(); i++){
        delete view[i];
    }
}


void Camera::setupRays(){
    double angleStep = viewAngle / rayAmount;
    
    view = vector<Ray*>(rayAmount);
    
    
    for (unsigned short i=0; i<rayAmount; i++){
        view[i] = new Ray(pos, pos - sf::Vector2f(0, viewLength));
        
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

Camera::Camera(){
    pos = sf::Vector2f(0, 0);
    viewAngle = defaultViewAngle;
    viewLength = defaultViewLength;
    rayAmount = defaultRayAmount;

    setupBody();
    setupRays();
}

Camera::Camera(sf::Vector2f pos,
               double viewAngle,
               double viewLength,
               unsigned short rayAmount){
    
    this->pos = pos;
    this->viewAngle = viewAngle;
    this->viewLength = viewLength;
    this->rayAmount = rayAmount;

    setupBody();
    setupRays();
}
    
void Camera::move(sf::Vector2f offset){
    for (short unsigned i=0; i<view.size(); i++){
        view[i]->move(offset);
    }
    body.setPosition(body.getPosition() + offset);
}

void Camera::rotate(double angle){
    for (short unsigned i=0; i<view.size(); i++){
        view[i]->rotateDegrees(angle);
    }
}

// Maybe change sf::RectangleShape into sf::VertexArray or something similar for more flexibility.
// Perhaps making an overload that accepts a vector of sf::VertexArray??
void Camera::castRays(vector<sf::RectangleShape*> &colliders){
    view[0]->sortColliders(colliders);
    for (short unsigned i=0; i<view.size(); i++){
        view[i]->castIt(vector<sf::RectangleShape*>(colliders));
    }
    
}

void Camera::drawIn(sf::RenderWindow &window){
    for (short unsigned i=0; i<view.size(); i++){
        window.draw(view[i]->makeDrawable());
    }
    window.draw(body);
}
