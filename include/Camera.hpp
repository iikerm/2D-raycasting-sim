#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Ray.hpp"
#include <vector>
using namespace std;

/**
 * This class is meant to be useful for grouping Ray class objects into one,
 * making group rotations and translations easier to handle.
 */
class Camera{
    private:
        sf::Vector2f pos;
        double viewAngle;       // How wide the camera sees
        double viewLength;      // How far the camera sees
        unsigned short rayAmount;   // "Detail" with which the camera sees

        double rotation;        // Sum of all the angles in all the rotations applied
        
        sf::CircleShape body;
        
        void setupRays(const sf::RenderWindow &win);
        void setupBody();
    
    public:
        vector<Ray*> view;
        const static double defaultViewAngle;
        const static double defaultViewLength;
        const static unsigned short defaultRayAmount;

        Camera(const sf::RenderWindow &win);
        Camera(sf::Vector2f pos,
            const sf::RenderWindow &win,
            double viewAngle = defaultViewAngle,
            double viewLength = defaultViewLength,
            unsigned short rayAmount = defaultRayAmount);
        
        ~Camera();
            
        void move(sf::Vector2f offset, vector<sf::RectangleShape*> &colliders);
        void rotate(double angle);
        void castRays(vector<sf::RectangleShape*> &colliders);

        void drawIn(sf::RenderWindow &window);

        sf::Vector2f getPos(){return pos;}
        double getViewAngle(){return viewAngle;}
        double getViewLength(){return viewLength;}
        unsigned short getRayAmount(){return rayAmount;}
        double getRotation(){return rotation;}
};

#endif