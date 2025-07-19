#include "Camera.hpp"
#include <cmath>

const double Camera::defaultViewAngle = 30.F;
const unsigned short Camera::defaultRayAmount = 100u;

Camera::~Camera(){
    for (unsigned short i=0; i<view.size(); i++){
        delete view[i];
    }
}

void Camera::setupRayEndpoints(bool initVector){
    if (initVector){
        rayEndpoints = vector<sf::Vector2f*>(rayAmount);
    }
    
    for (unsigned short i=0; i<rayAmount; i++){
        rayEndpoints[i] = &(view[i]->points[view[i]->pointInCollision]);
    }
}


void Camera::setupRays(const sf::Vector2f winSize){
    double angleStep = viewAngle / rayAmount;
    
    view = vector<Ray*>(rayAmount);
    
    for (unsigned short i=0; i<rayAmount; i++){
        view[i] = new Ray(pos, pos - sf::Vector2f(0, viewLength), winSize);
        
        // This makes it so that the camera starts facing correctly in a single 
        // direction, and the next rays are generated from that initial angle
        view[i]->rotateDegrees(-viewAngle/2 + i*angleStep);
    }
}

void Camera::setupBody(){
    body = sf::CircleShape(100, 30ul);
    body.setPosition(pos - sf::Vector2f(body.getRadius(), body.getRadius()));

    body.setFillColor(sf::Color(246, 175, 90));       // #f6af5a
}

Camera::Camera(const sf::Vector2f winSize){
    pos = sf::Vector2f(0, 0);
    viewAngle = defaultViewAngle;
    viewLength = max(winSize.x, winSize.y);
    rayAmount = defaultRayAmount;

    this->rotation = 0;

    setupBody();
    setupRays(winSize);
    setupRayEndpoints(true);
}

Camera::Camera(sf::Vector2f pos,
               const sf::Vector2f winSize,
               double viewAngle,
               unsigned short rayAmount){
    
    this->pos = pos;
    this->viewAngle = viewAngle;
    this->viewLength = max(winSize.x, winSize.y)*2;
    this->rayAmount = rayAmount;

    this->winSize = winSize;
    this->rotation = 0;

    setupBody();
    setupRays(winSize);
    setupRayEndpoints(true);
}
    
void Camera::move(sf::Vector2f offset, vector<sf::RectangleShape*> &colliders){
    sf::CircleShape bodyCopy = body;
    bodyCopy.setPosition(body.getPosition() + offset);

    if(bodyCopy.getPosition().x < 0 || bodyCopy.getPosition().y < 0){
        return;
    }else if (bodyCopy.getPosition().x + bodyCopy.getRadius()*2 > winSize.x){
        return;
    }else if (bodyCopy.getPosition().y + bodyCopy.getRadius()*2 > winSize.y){
        return;
    }

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

    // Calculates rotation as an angle in the range [0, 360)
    if(this->rotation >= 360){
        // This calculations are equal to using this->rotation % 360
        this->rotation = this->rotation - 360*((int)(this->rotation / 360));
    }
}

// Maybe change sf::RectangleShape into sf::VertexArray or something similar for more flexibility.
// Perhaps making an overload that accepts a vector of sf::VertexArray??
void Camera::castRays(vector<sf::RectangleShape*> &colliders){
    view[0]->sortColliders(colliders);
    for (short unsigned i=0; i<view.size(); i++){
        view[i]->castIt(vector<sf::RectangleShape*>(colliders));
    }
    setupRayEndpoints();
}

void Camera::drawIn(sf::RenderWindow &window, bool debug) const{
    // Drawing the camera's view cone
    sf::VertexArray viewCone(sf::PrimitiveType::TriangleFan, view.size()+1);
    viewCone[0] = this->pos;

    for (short unsigned i=1; i<(rayEndpoints.size()+1); i++){
        viewCone[i] = *rayEndpoints[i-1];
    }
    window.draw(viewCone);

    window.draw(body);

    
    if (debug){
        // Drawing the nearest collision point to the camera
        Ray *iter = view[0];
        double minLength = iter->euclideanDistanceToCollision();

        for (unsigned i=1; i<view.size(); i++){
            minLength = min(minLength, view[i]->euclideanDistanceToCollision());
            if (view[i]->euclideanDistanceToCollision() == minLength){
                iter = view[i];
            }
        }

        sf::Vector2f lr = iter->makeDrawable()[1].position;
        sf::CircleShape c(10, 30ul);
        c.setFillColor(sf::Color::Cyan);
        c.setPosition(lr - sf::Vector2f(c.getRadius(), c.getRadius()));
        window.draw(c);

        // Drawing the individual points that compose each ray
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


double Camera::getMaximumDistance() const{
    return max(
        sqrt(pow(pos.x, 2) + pow(pos.y, 2)),    // distance to window origin
        sqrt(pow(pos.x - winSize.x, 2) + pow(pos.y - winSize.y, 2))     // distance to window end coords
    );
}
