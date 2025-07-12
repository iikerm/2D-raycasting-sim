#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
using namespace std;

/*
Create a laser class that represents a non-drawable line that will check for every point since start
to end (both defined). If there is a point that is contained in the bounds of another object
(presumably from a vector of objects), the process will finish.
*/


int main(){
    sf::RenderWindow win(sf::VideoMode::getDesktopMode(), "Tests");
    win.setFramerateLimit(120);

    sf::RectangleShape viewLaser(sf::Vector2f(5, win.getSize().y));
    viewLaser.setFillColor(sf::Color(253, 146, 16));       // #fd9210
    viewLaser.setPosition(sf::Vector2f(0, 0));

    sf::RectangleShape diag(sf::Vector2f(1, 350));
    diag.setFillColor(sf::Color::Red);       // #fd9210
    diag.setPosition(sf::Vector2f(400, 100));

    sf::RectangleShape diag2(sf::Vector2f(-400, -400));
    diag2.setPosition(sf::Vector2f(win.getSize().x/2, win.getSize().y/2));

    sf::CircleShape cir(100.f, 50);
    cir.setFillColor(sf::Color::Red);
    cir.setPosition(sf::Vector2f(900, 100));

    sf::FloatRect bounds = diag2.getGlobalBounds();
    sf::RectangleShape drawBounds(bounds.getSize());
    drawBounds.setPosition(bounds.getPosition());
    drawBounds.setFillColor(sf::Color::Transparent);
    drawBounds.setOutlineColor(sf::Color::Blue);
    drawBounds.setOutlineThickness(3);

    

    while (win.isOpen()){
        sf::Event event;
        while (win.pollEvent(event)){
            if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))){
                win.close();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
            viewLaser.setPosition(viewLaser.getPosition() + sf::Vector2f(-5, 0));
            if (viewLaser.getPosition().x < 0){
                viewLaser.setPosition(sf::Vector2f(0, viewLaser.getPosition().y));
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
            viewLaser.setPosition(viewLaser.getPosition() + sf::Vector2f(5, 0));
            if (viewLaser.getPosition().x > win.getSize().x-5){
                viewLaser.setPosition(sf::Vector2f(win.getSize().x-5, viewLaser.getPosition().y));
            }
        }

        win.clear(sf::Color::Black);

        win.draw(viewLaser);
        win.draw(cir);
        win.draw(diag);
        win.draw(diag2);
        win.draw(drawBounds);

        win.display();
    }
}