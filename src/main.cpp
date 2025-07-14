#include "Ray.hpp"
#include "Camera.hpp"
using namespace std;

int main(){
    // Ray testRay = Ray(sf::Vector2f(1000, 1000), sf::Vector2f(1000, 1300));
    Camera testCamera = Camera(sf::Vector2f(1000, 1000), 45, 400, 320u);
    testCamera.rotate(45);
    
    sf::RectangleShape a = sf::RectangleShape(sf::Vector2f(100, 100));
    a.setPosition(50, 200);
    a.setFillColor(sf::Color::Red);

    sf::RectangleShape b = sf::RectangleShape(sf::Vector2f(100, 100));
    b.setPosition(150, 300);
    b.setFillColor(sf::Color::Red);

    sf::RectangleShape c = sf::RectangleShape(sf::Vector2f(100, 1000));
    c.setPosition(250, 400);
    c.setFillColor(sf::Color::Red);

    sf::CircleShape circle = sf::CircleShape(300, 30ul);
    circle.setPosition(sf::Vector2f(700, 700));
    circle.setOutlineColor(sf::Color::Cyan);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineThickness(5);

    vector<sf::RectangleShape*> colliders = {&a, &b, &c};


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
            testCamera.move(sf::Vector2f(-2, 0));
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
            testCamera.move(sf::Vector2f(2, 0));
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
            testCamera.move(sf::Vector2f(0, -2));
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
            testCamera.move(sf::Vector2f(0, 2));
        }


        // Rotation controls
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
            testCamera.rotate(1);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
            testCamera.rotate(-1);
        }

        testCamera.castRays(colliders);

        win.clear(sf::Color::Black);

        for (int i=0; i<colliders.size(); i++){
            win.draw(*colliders[i]);
        }

        /*for (int j=0; j<10; j++){
            for (int i=0; i<testCamera.view[j].pointInCollision; i++){
                sf::CircleShape tc(10, 30);
                tc.setPosition(testCamera.view[j].points[i]);
                tc.setFillColor(sf::Color(i*20, 10*i, 255 - 20*i));
                win.draw(tc);
            }
        }*/

        win.draw(circle);

        // win.draw(testRay.makeDrawable());
        testCamera.drawIn(win);

        win.display();
    }

    return 0;
}