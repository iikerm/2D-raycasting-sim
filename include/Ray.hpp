#ifndef RAY_HPP
#define RAY_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;


// When drawing a Ray object, draw from start either to the finishing point, 
// or to the point in collision.
class Ray{
    
    private:
        double length;
        sf::Vector2f pointDistance;     // Distance from one point in the ray to the next
        bool collidedWithObject;
    
    public:
        constexpr static double pointDensity = 60.f;   // Points in the ray for every 100px of distance
        short unsigned pointNumber;  // Number of points that the ray is "composed of"
        sf::Vector2f* points;
        sf::Vector2f start;
        sf::Vector2f finish;
        
        sf::Vector2f winSize;   // Size of the window where the ray is drawn
        
        short unsigned pointInCollision;    // Index of the point which collided with an object

        void calculatePointDistance();
        void calculatePointsArray();
    
        Ray();
        Ray(sf::Vector2f start, sf::Vector2f finish, const sf::Vector2f winSize);
        Ray(sf::Vector2f start, double angleDegrees, double lineOfSight, const sf::Vector2f winSize);
        Ray(Ray& other);

        ~Ray();
        
        void move(sf::Vector2f offset);
        void rotateDegrees(double angle);
        void rotateInStep(bool negativeStep=false);
        void rotate(double angle);
        void sortColliders(vector<sf::RectangleShape*> &colliders);

        void castIt(vector<sf::RectangleShape*> colliders);

        const sf::VertexArray makeDrawable() const;

        const double getLength() const {return length;}
        const bool getCollidedWithObject() const {return collidedWithObject;}
        double euclideanDistanceToCollision();
};


#endif