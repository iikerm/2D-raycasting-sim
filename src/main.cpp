#include "Ray.hpp"
#include "Camera.hpp"
using namespace std;

// Distance moved by the camera for every frame where the necessary key is pressed
constexpr double DISTANCE_MOVED = 8;

// Angle by which the camera will rotate for every frame where the necessary key is pressed
constexpr double ANGLE_ROTATED = 2;

constexpr unsigned short MAX_FPS = 120;  // Maximum framerate allowed for the main window


int main(){
    // Ray testRay = Ray(sf::Vector2f(1000, 1000), sf::Vector2f(1000, 1300));
    Camera testCamera = Camera(sf::Vector2f(1000, 1000), 30, 1000, 320u);
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
    win.setFramerateLimit(MAX_FPS);

    sf::Clock fpsClock;
    fpsClock.restart();

    // true if the rays should be casted in the current iteration 
    // (starts as true so they are casted in the first iteration)
    bool castRaysNow = true;

    int fpsCount = 0;

    while(win.isOpen()){
        
        // Shows FPS (for performance improvements)
        fpsCount++;
        if (fpsCount == 100){
            unsigned t = fpsClock.getElapsedTime().asMilliseconds();
            cout << t << "ms since last frame (" << 100.f / (double)(t * 1.f/1000.f) << "fps)" << endl;
            fpsCount = 0;
            fpsClock.restart();
        }

        sf::Event event;
        while (win.pollEvent(event)){
            if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))){
                win.close();
            }
        }

        // Direction controls
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
            testCamera.move(sf::Vector2f(-DISTANCE_MOVED, 0));
            castRaysNow = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
            testCamera.move(sf::Vector2f(DISTANCE_MOVED, 0));
            castRaysNow = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
            testCamera.move(sf::Vector2f(0, -DISTANCE_MOVED));
            castRaysNow = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
            testCamera.move(sf::Vector2f(0, DISTANCE_MOVED));
            castRaysNow = true;
        }


        // Rotation controls
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
            testCamera.rotate(ANGLE_ROTATED);
            castRaysNow = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
            testCamera.rotate(-ANGLE_ROTATED);
            castRaysNow = true;
        }

        // Casting rays
        if (castRaysNow){
            testCamera.castRays(colliders);
        }

        // Drawing to window
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


        castRaysNow = false;
    }

    return 0;
}