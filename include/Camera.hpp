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
        
        sf::Vector2f winSize;

        sf::CircleShape body;
        
        void setupRays(const sf::Vector2f winSize);
        void setupRayEndpoints(bool initVector=false);
        void setupBody();
    
    public:
        vector<Ray*> view;
        vector<sf::Vector2f*> rayEndpoints;
        const static double defaultViewAngle;
        const static unsigned short defaultRayAmount;

        Camera(const sf::Vector2f winSize);
        Camera(sf::Vector2f pos,
            const sf::Vector2f winSize,
            double viewAngle = defaultViewAngle,
            unsigned short rayAmount = defaultRayAmount);
        
        ~Camera();
            
        void move(sf::Vector2f offset, vector<sf::RectangleShape*> &colliders);
        void rotate(double angle);
        void castRays(vector<sf::RectangleShape*> &colliders);

        void drawIn(sf::RenderWindow &window, bool debug=false) const;

        const sf::Vector2f getPos() const {return pos;}
        const double getViewAngle() const {return viewAngle;}
        const double getViewLength() const {return viewLength;}
        const unsigned short getRayAmount() const {return rayAmount;}
        const double getRotation() const {return rotation;}
};

#endif