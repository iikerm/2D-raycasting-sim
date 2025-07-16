#include "../include/Ray.hpp"
#include <cmath>
#include <queue>
using namespace std;

/**
 * Calculates the distance between two consecutive "points" from the ray (i.e. points where the ray
 * will check for any collision).
 */
void Ray::calculatePointDistance(){
    sf::Vector2f tempFinish = finish;
    
    bool finishOutsideWindow = (finish.x < 0 || finish.y < 0 || finish.x > winSize.x || finish.y > winSize.y);
    if (finishOutsideWindow){ 
        while (finishOutsideWindow){
            tempFinish -= ((finish - start) / (pointNumber/5.f));
            finishOutsideWindow = (tempFinish.x < 0 
                || tempFinish.y < 0 
                || tempFinish.x > winSize.x 
                || tempFinish.y > winSize.y);
        }
        tempFinish += ((finish - start) / (pointNumber/5.f));
    }

    pointDistance = sf::Vector2f(
        (tempFinish.x - start.x) / (double)(pointNumber),
        (tempFinish.y - start.y) / (double)(pointNumber)
    );
}

/**
 * Calculates the positions of all the points in the ray.
 */
void Ray::calculatePointsArray(){

    points[0] = start;

    for (short unsigned i=1; i<pointNumber; i++){
        points[i] = start + sf::Vector2f(i*pointDistance.x, i*pointDistance.y);
    }
}

/**
 * Default constructor
 */
Ray::Ray(){
    start = sf::Vector2f(0,0);
    finish = start;

    calculatePointDistance();
    calculatePointsArray();
}

/**
 * Constructor used in the Camera class.
 * It creates a ray going from the specified start position to the specified end (finish) position.
 * The sf::RenderWindow reference is needed in order to calculate the window size.
 */
Ray::Ray(sf::Vector2f start, sf::Vector2f finish, const sf::RenderWindow &win){
    this->start = start;
    this->finish = finish;

    this->winSize = win.getSize();
    pointInCollision = pointNumber-1;
    calculatePointDistance();
    calculatePointsArray();
}

/**
 * Constructor that takes a start position, and the distance & angle, which are then used to calculate
 * the position of the end point of the ray.
 */
Ray::Ray(sf::Vector2f start, double angleDegrees, double lineOfSight, const sf::RenderWindow &win){
    this->start = start;
    this->finish = start + sf::Vector2f(lineOfSight, lineOfSight);

    this->winSize = win.getSize();
    pointInCollision = pointNumber-1;
    
    this->rotateDegrees(angleDegrees);
}

Ray::Ray(Ray& other){
    this->start = other.start;
    this->finish = other.finish;

    /*for (int i=0; i<pointNumber; i++){
        this->points[i] = other.points[i];
    }*/

    this->winSize = other.winSize;

    calculatePointsArray();
    this->pointDistance = other.pointDistance;
    this->pointInCollision = other.pointInCollision;
}

/**
 * Moves the entire ray the specified distance
 */
void Ray::move(sf::Vector2f offset){
    this->start += offset;
    this->finish += offset;

    calculatePointDistance();
    calculatePointsArray();
}

/**
 * Rotates the entire ray by the specified angle in degrees.
 * In reality this method only rotates the final point of the ray, and then uses
 * calculatePointDistance() and calculatePointsArray() to calculate the position of all the other
 * points in the ray to avoid having to call this method for each of those points.
 */
void Ray::rotateDegrees(double angle){
    // We need the angle to be in radians in order to work with cmath's sin() and cos() functions so:
    angle = angle * M_PI / 180;

    // The rotation matrix below will rotate a vector counterclockwise if the angle of rotation is 
    // positive, which is the opposite of what I want to happen so we invert the angle's sign.
    angle = -1 * angle;
    
    /*
    We can rotate a point around the origin coordinates by multiplying its coordinates by the
    rotation matrix:
    
    | cos()  -sin() | * | finish.x | = | finish.x*cos() - finish.y*sin() |
    | sin()   cos() |   | finish.y |   | finish.x*sin() + finish.y*cos() |
    
    Because we want to rotate the finish point around the ray's starting point and not the origin 
    coordinates, we must subtract the start coordinates from the finish coordinates.
    */

    sf::Vector2f tempFinish = finish - start;

    // We now have the coordinates of the finish point treating the start point as the origin
    // coordinates, so we can do the matrix multiplication

    finish = sf::Vector2f(
        tempFinish.x * cos(angle) - tempFinish.y * sin(angle),
        tempFinish.x * sin(angle) + tempFinish.y * cos(angle)
    );

    // We now have the coordinates of the rotated point with respect to (0, 0), so we need to 
    // transform them to represent coordinates with respect to the start point. For this we
    // just add back the start coordinates.

    finish += start;

    calculatePointDistance();
    calculatePointsArray();
}


/**
 * It is meant to be an improvement from rotateDegrees. 
 * By pre-computing the value of sin(-1°) and cos(-1°), some computation will be avoided, more so
 * when this process is done for multiple rays. 
 * This method is equivalent to using rotateDegrees(1).
 * The boolean argument must be true if the rotation will be -1°, and false otherwise.
 * 
 * If this method is called, it needs to be followed by 
 * calculatePointDistance() and calculatePointsArray(), otherwise it will do nothing.
 */
void Ray::rotateInStep(bool negativeStep){
    // Pre-computed values using python math.sin(), math.cos(), and math.radians() functions
    double sin_minus1_deg = -0.01745240643728351;
    constexpr double cos_minus1_deg = 0.9998476951563913;

    // If we want to rotate in the other direction:
    if (negativeStep){
        sin_minus1_deg = -1 * sin_minus1_deg;
        // cos doesn't change depending on the sign of the angle
    }

    // The rest of the process is the same as in the rotateDegrees method

     sf::Vector2f tempFinish = finish - start;

    finish = sf::Vector2f(
        tempFinish.x * cos_minus1_deg - tempFinish.y * sin_minus1_deg,
        tempFinish.x * sin_minus1_deg + tempFinish.y * cos_minus1_deg
    );

    finish += start;
}

/**
 * Method that will rotate the ray by the specified angle in degrees. 
 * It unifies the rotateDegrees() and rotateInStep() methods and decides which one is best to use
 * in each individual case.
 */
void Ray::rotate(double angle){

    if (angle == 0){
        return;
    }

    if (angle == static_cast<int>(angle)){
        // If the angle is a whole number, rotateInStep is more efficient
        for (int i=0; i<abs(angle); i++){
            this->rotateInStep(angle<0);
        }
        calculatePointDistance();
        calculatePointsArray();

    }else{
        this->rotateDegrees(angle);
    }
}


/**
 * This method is intended to be called from an instance of the Camera class, so that the colliders 
 * vector only has to be ordered once for a group of rays.
 * This works because all the rays coming out of a Camera will have the same starting point
 */
void Ray::sortColliders(vector<sf::RectangleShape*> &colliders){
    /*
    We order the colliders vector from nearest (first) to furthest away (last) from the
    starting point of the ray. This ensures that most rays will perform few iterations of the 
    following loops because they are more probable to find a collision early on.
    */

    sort(colliders.begin(), colliders.end(), 
        [this](sf::RectangleShape *a, sf::RectangleShape *b){
            return sqrt(pow(a->getPosition().x - this->start.x, 2) + pow(a->getPosition().y - this->start.y, 2)) 
                   < sqrt(pow(b->getPosition().x - this->start.x, 2) + pow(b->getPosition().y - this->start.y, 2));
        });
}


/**
 * Calculates the ray's collision with the first object of the list that comes across its path.
 * If using outside of a Camera method, call Ray::sortColliders method first for better performance.
 */
void Ray::castIt(vector<sf::RectangleShape*> colliders){
    sf::FloatRect colliderBounds;

    for (long unsigned i=0; i<colliders.size(); i++){

        sf::RectangleShape* collider = dynamic_cast<sf::RectangleShape*>(colliders[i]);
        if (collider != NULL){
            colliderBounds = collider->getGlobalBounds();
        }else{
            cerr << "Ignored collider in position " << i 
                 << " because of unsupported sf::Drawable subclass" << endl;
            continue;
        }
        
        for (unsigned short i=0; i<pointNumber-1; i++){
            if (colliderBounds.contains(points[i])){
                
                if (i==0){
                    pointInCollision = 0;
                }else{
                    pointInCollision = i-1;
                }
                return;
            }else{
                pointInCollision = pointNumber-1;
            }
        }
    }
}

sf::VertexArray Ray::makeDrawable(){
    sf::VertexArray tempVertexArray(sf::Lines, 2);

    tempVertexArray[0] = points[0];

    if (pointInCollision < 0 || pointInCollision >= pointNumber){
        cerr << "Invalid point index in collision: " << pointInCollision << endl;
        exit(0);
    }

    tempVertexArray[1] = points[pointInCollision];
    
    return tempVertexArray;
}