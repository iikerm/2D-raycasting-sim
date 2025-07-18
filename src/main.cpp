#include "Ray.hpp"
#include "Camera.hpp"
#include "Renderer.hpp"
using namespace std;

// Distance moved by the camera for every frame where the necessary key is pressed
constexpr double DISTANCE_MOVED = 20;

// Angle by which the camera will rotate for every frame where the necessary key is pressed
constexpr double ANGLE_ROTATED = 2;

constexpr unsigned short MAX_FPS = 120;  // Maximum framerate allowed for the main window


int main(){
    sf::RenderWindow win(sf::VideoMode::getDesktopMode(), "Main");
    win.setFramerateLimit(MAX_FPS);

    // Ray testRay = Ray(sf::Vector2f(1000, 1000), sf::Vector2f(1000, 1300));
    Camera testCamera = Camera(sf::Vector2f(100, 100), sf::Vector2f(win.getSize()), 30, 500u);
    testCamera.rotate(200);

    Renderer mainRenderer(testCamera, sf::Vector2f(500, 200), 
                            sf::Vector2f(win.getSize().x - 500, 0));

    vector<vector<unsigned>> maze = {{0, 1, 0, 1, 0, 0, 0, 1},
                                     {0, 1, 0, 1, 1, 0, 0, 0},
                                     {0, 0, 0, 0, 0, 0, 1, 1},
                                     {1, 1, 1, 0, 0, 1, 1, 0},
                                     {0, 0, 0, 0, 0, 1, 0, 0}};

    vector<sf::RectangleShape*> colliders;
    sf::RectangleShape* maze_wall;

    sf::Vector2f wall_dimensions = sf::Vector2f(win.getSize().x/maze[0].size(), 
                                                win.getSize().y/maze.size());
    for (unsigned i=0; i<maze.size(); i++){
        for (unsigned j=0; j<maze[i].size(); j++){
            if (maze[i][j] == 1){
                maze_wall = new sf::RectangleShape(wall_dimensions);
                maze_wall->setFillColor(sf::Color::Red);
                maze_wall->setPosition(wall_dimensions.x*j, wall_dimensions.y*i);

                colliders.push_back(maze_wall);
            }
        }
    }


    sf::Clock fpsClock;
    fpsClock.restart();

    sf::Clock moveClock;
    moveClock.restart();

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
        if (moveClock.getElapsedTime().asMicroseconds() > 10000){
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
                testCamera.move(sf::Vector2f(-DISTANCE_MOVED, 0), colliders);
                castRaysNow = true;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
                testCamera.move(sf::Vector2f(DISTANCE_MOVED, 0), colliders);
                castRaysNow = true;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
                testCamera.move(sf::Vector2f(0, -DISTANCE_MOVED), colliders);
                castRaysNow = true;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
                testCamera.move(sf::Vector2f(0, DISTANCE_MOVED), colliders);
                castRaysNow = true;
            }
            moveClock.restart();
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
            mainRenderer.computeRenderDistances();
        }

        // Drawing to window
        win.clear(sf::Color::Black);

        for (unsigned long i=0; i<colliders.size(); i++){
            win.draw(*colliders[i]);
        }
        // win.draw(testRay.makeDrawable());
        testCamera.drawIn(win, false);

        mainRenderer.drawRender(win);

        win.display();


        castRaysNow = false;
    }

    return 0;
}