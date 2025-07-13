#include "Ray.hpp"
using namespace std;

int main(){
    Ray testRay = Ray(sf::Vector2f(1000, 1000), sf::Vector2f(1000, 1300));
    
    sf::RectangleShape a = sf::RectangleShape(sf::Vector2f(100, 100));
    a.setPosition(50, 200);
    a.setFillColor(sf::Color::Red);

    sf::RectangleShape b = sf::RectangleShape(sf::Vector2f(100, 100));
    b.setPosition(150, 300);
    b.setFillColor(sf::Color::Red);

    sf::RectangleShape c = sf::RectangleShape(sf::Vector2f(100, 100));
    c.setPosition(250, 400);
    c.setFillColor(sf::Color::Red);

    vector<sf::Drawable*> colliders = {&a, &b, &c};

    sf::CircleShape circle(300, 30ul);
    circle.setPosition(sf::Vector2f(700, 700));
    circle.setOutlineColor(sf::Color::Cyan);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineThickness(5);


    sf::RenderWindow win(sf::VideoMode::getDesktopMode(), "Main");

    while(win.isOpen()){
        sf::Event event;
        while (win.pollEvent(event)){
            if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))){
                win.close();
            }
        }

        // Direction controls
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
            testRay.move(sf::Vector2f(-2, 0));
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
            testRay.move(sf::Vector2f(2, 0));
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
            testRay.move(sf::Vector2f(0, -2));
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
            testRay.move(sf::Vector2f(0, 2));
        }


        // Rotation controls
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
            testRay.rotateDegrees(1);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
            testRay.rotateDegrees(-1);
        }

        testRay.castIt(colliders);

        win.clear(sf::Color::Black);

        for (int i=0; i<colliders.size(); i++){
            win.draw(*colliders[i]);
        }

        /*for (int i=0; i<Ray::pointNumber; i++){
            sf::CircleShape tc(10, 30);
            tc.setPosition(testRay.points[i]);
            tc.setFillColor(sf::Color(i*20, 10*i, 255 - 20*i));
            win.draw(tc);
        }*/

        win.draw(circle);

        win.draw(testRay.makeDrawable());

        win.display();
    }

    return 0;
}