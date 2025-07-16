#ifndef RAY_HPP
#define RAY_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;


// When drawing a Ray object, draw from start either to the finishing point, 
// or to the point in collision.
class Ray{

    friend class Camera;
    
    private:
        double length;
        sf::Vector2f pointDistance;     // Distance from one point in the ray to the next
    
    public:
        const static short unsigned pointNumber = 800u;  // Number of points that the ray is "composed of"
        sf::Vector2f points[pointNumber];
        sf::Vector2f start;
        sf::Vector2f finish;
        
        sf::Vector2u winSize;   // Size of the window where the ray is drawn
        
        short unsigned pointInCollision;    // Index of the point which collided with an object

        void calculatePointDistance();
        void calculatePointsArray();
    
        Ray();
        Ray(sf::Vector2f start, sf::Vector2f finish, const sf::RenderWindow &win);
        Ray(sf::Vector2f start, double angleDegrees, double lineOfSight, const sf::RenderWindow &win);
        Ray(Ray& other);
        
        void move(sf::Vector2f offset);
        void rotateDegrees(double angle);
        void rotateInStep(bool negativeStep=false);
        void rotate(double angle);
        void sortColliders(vector<sf::RectangleShape*> &colliders);

        void castIt(vector<sf::RectangleShape*> colliders);

        sf::VertexArray makeDrawable();
};


#endif