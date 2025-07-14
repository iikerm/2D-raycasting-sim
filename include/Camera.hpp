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
    
    sf::CircleShape body;
    
    void setupRays();
    void setupBody();
    
    public:
        vector<Ray> view;
        const static double defaultViewAngle;
        const static double defaultViewLength;
        const static unsigned short defaultRayAmount;

        Camera();
        Camera(sf::Vector2f pos,
            double viewAngle = defaultViewAngle,
            double viewLength = defaultViewLength,
            unsigned short rayAmount = defaultRayAmount);
            
        void move(sf::Vector2f offset);
        void rotate(double angle);
        void castRays(vector<sf::RectangleShape*> &colliders);

        void drawIn(sf::RenderWindow &window);
};

#endif