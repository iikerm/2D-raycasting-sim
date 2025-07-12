#include "../include/Ray.hpp"
using namespace std;

void Ray::calculatePointDistance(){
    pointDistance = sf::Vector2f(
        (finish.x - start.x) / (double)pointNumber-1,
        (finish.y - start.y) / (double)pointNumber-1
    );
}

void Ray::calculatePointsArray(){

    points[0] = start;

    for (short unsigned i=1; i<pointNumber; i++){
        points[i] = sf::Vector2f(i*pointDistance.x, i*pointDistance.y);
    }
}


Ray::Ray(sf::Vector2f start, sf::Vector2f finish){
    this->start = start;
    this->finish = finish;

    pointInCollision = pointNumber-1;
    calculatePointDistance();
    calculatePointsArray();
}

Ray::Ray(sf::Vector2f start, double angleDegrees, double lineOfSight){
    this->start = start;
    this->finish = start;

    sf::RectangleShape tempRect(sf::Vector2f(1, lineOfSight));
    tempRect.setPosition(start);
    tempRect.rotate(angleDegrees);
    
    // So that the angle is in [0-360]
    angleDegrees = tempRect.getRotation();

    if (angleDegrees <= 180){
        finish.x = start.x - tempRect.getGlobalBounds().width;
    }else{
        finish.x = start.x + tempRect.getGlobalBounds().width;
    }

    if (angleDegrees <= 90 && angleDegrees >= 270){
        finish.y = start.y + tempRect.getGlobalBounds().height;
    }else{
        finish.y = start.y - tempRect.getGlobalBounds().height;
    }


    // So we will render only what we are seeing
    if (finish.x > lineOfSight){
        finish.x = lineOfSight;
    }
    if (finish.y > lineOfSight){
        finish.y = lineOfSight;
    }

    pointInCollision = pointNumber-1;
    calculatePointDistance();
    calculatePointsArray();
}

Ray::Ray(Ray& other){
    this->start = other.start;
    this->finish = other.finish;

    calculatePointDistance();
    calculatePointsArray();

    this->pointInCollision = other.pointInCollision;
}


void Ray::castIt(vector<sf::Drawable*> colliders){
    /*Suggested performance optimizations:
        - Use a priority queue that orders each collider according to their distance from the start
          (so that we find a collision early and avoid checking most of the colliders).
        
        - Use a priority queue that orders each collider according to their surface area (bigger
          colliders have a higher chance to cause a collision).
    */

    sf::FloatRect colliderBounds;

    for (long unsigned i=0; i<colliders.size(); i++){

        try{
            sf::RectangleShape* collider = dynamic_cast<sf::RectangleShape*>(colliders[i]);
            colliderBounds = collider->getGlobalBounds();
        }catch(bad_cast){
            try{
                sf::CircleShape* collider = dynamic_cast<sf::CircleShape*>(colliders[i]);
                colliderBounds = collider->getGlobalBounds();
            }catch(bad_cast){
                cout << "Ignored collider because of unsupported Drawable subclass" << endl;
                continue;
            }
        }

        for (short unsigned i=0; i<pointNumber; i++){
            if (colliderBounds.contains(points[i])){
                pointInCollision = i;
                return;
            }else{
                pointInCollision = pointNumber-1;
            }
        }
    }
}

sf::VertexArray Ray::makeDrawable(){
    sf::VertexArray tempVertexArray(sf::Lines, 2);
    tempVertexArray[0] = points[0];
    tempVertexArray[1] = points[pointInCollision];
    return tempVertexArray;
}