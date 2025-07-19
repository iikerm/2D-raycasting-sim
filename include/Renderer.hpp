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
        double renderBorderSize;
    public:
        double minRenderedDepth;
        double maxRenderedDepth;

        unsigned long minDepthIndex;
        unsigned long maxDepthIndex;

        sf::Color darkenByDepth(double actualDepth, sf::Color original);
        
        Renderer(Camera &cam, sf::Vector2f size, sf::Vector2f pos);

        void computeRenderDistances();

        void drawRender(sf::RenderWindow &win);
        void drawRenderInfo(string fontPath, sf::RenderWindow &win);
};

#endif