#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SFML/Graphics.hpp>
#include "Camera.hpp"

/**
 * Class designed to render what a Camera object is currently simulating.
 */
class Renderer{
    private:
        Camera *cam;
        sf::Vector2f size;
        sf::Vector2f pos;
    public:
        static sf::Color darkenByDepth(double minDepth, double maxDepth, sf::Color original);
        
        Renderer(const Camera &cam, sf::Vector2f size, sf::Vector2f pos);

        void drawRender(const sf::RenderWindow &win) const;
};

#endif