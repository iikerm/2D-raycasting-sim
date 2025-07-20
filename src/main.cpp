#include "Ray.hpp"
#include "Camera.hpp"
#include "Renderer.hpp"
#include <cmath>
using namespace std;

// Distance moved by the camera for every frame where the necessary key is pressed
constexpr double DISTANCE_MOVED = 15;

// Angle by which the camera will rotate for every frame where the necessary key is pressed
constexpr double ANGLE_ROTATED = 2;

// Maximum framerate allowed for the main window
constexpr unsigned short MAX_FPS = 120;

// The fps that the program is running at will be calculated once every n frames
constexpr unsigned short CHECK_FPS_EVERY_N_FRAMES = 30;

// Path to the font file used to render the information texts
const string FONT_PATH = "fonts/pixel-font.ttf";


constexpr double CAMERA_VIEW_ANGLE = 50.f;    // View angle of the camera in degrees
constexpr unsigned CAMERA_RAY_AMOUNT = 200u; // Amount of rays coming out of the camera


int main(){
    sf::RenderWindow win(sf::VideoMode::getDesktopMode(), "Main");
    win.setFramerateLimit(MAX_FPS);

    Camera testCamera = Camera(sf::Vector2f(100, 100), 
                               sf::Vector2f(win.getSize()), 
                               CAMERA_VIEW_ANGLE, 
                               CAMERA_RAY_AMOUNT);
    testCamera.rotate(200);

    Renderer mainRenderer(testCamera, sf::Vector2f(500, 200), 
                            sf::Vector2f(win.getSize().x - 500, 0));

    vector<vector<unsigned>> maze = {{0, 0, 0, 1, 0, 0, 0, 1},
                                     {0, 1, 0, 1, 1, 0, 0, 0},
                                     {0, 0, 0, 0, 0, 0, 1, 1},
                                     {1, 1, 1, 0, 0, 1, 1, 0},
                                     {1, 0, 0, 0, 0, 1, 0, 0}};

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

    sf::Font cameraInfoFont;
    if (!cameraInfoFont.loadFromFile(FONT_PATH)){
        cerr << "Unable to load font from: " << FONT_PATH << endl;
    }

    sf::Text cameraInfo("Ray point density (per 100px): "
                        + to_string((int)Ray::pointDensity)
                        +"\nAngle of vision: "
                        + to_string(static_cast<int>(testCamera.getViewAngle()))
                        + " deg\nNumber of rays: "
                        + to_string(testCamera.getRayAmount())
                        + "\n Capped at" + to_string(MAX_FPS)
                        + " fps max", cameraInfoFont, 30u);
    cameraInfo.setFillColor(sf::Color(246, 175, 90));
    cameraInfo.setPosition(5, 5);
    cameraInfo.setOutlineColor(sf::Color::Black);
    cameraInfo.setOutlineThickness(1);

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
        if (fpsCount == CHECK_FPS_EVERY_N_FRAMES){
            unsigned t = fpsClock.getElapsedTime().asMilliseconds();
            double fps = ((double)CHECK_FPS_EVERY_N_FRAMES) / (double)(t / 1000.f);
            // cout << t << "ms since last frame (" << fps << "fps)" << endl;
            fpsCount = 0;
            fpsClock.restart();

            cameraInfo.setString("Ray point density (per 100px): "
                                + to_string((int)Ray::pointDensity)
                                + "\nAngle of vision: "
                                + to_string(static_cast<int>(testCamera.getViewAngle()))
                                + " deg\nNumber of rays: "
                                + to_string(testCamera.getRayAmount()) 
                                + "\n"
                                + to_string(static_cast<int>(fps))
                                + " fps");
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
        mainRenderer.drawRenderInfo(FONT_PATH, win);

        win.draw(cameraInfo);

        win.display();


        castRaysNow = false;
    }

    return 0;
}