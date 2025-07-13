#include "../include/Ray.hpp"
#include <cmath>
using namespace std;


Ray::Ray(){
    start = sf::Vector2f(0,0);
    finish = start;
    calculatePointDistance();
    calculatePointsArray();
}

void Ray::calculatePointDistance(){
    pointDistance = sf::Vector2f(
        (finish.x - start.x) / (double)(pointNumber),
        (finish.y - start.y) / (double)(pointNumber)
    );
    /*
    cout << pointNumber << " points" << endl;
    cout << "Math: (" << finish.x << " - " << start.x << ") / " << pointNumber << " = " << pointDistance.x << endl;
    cout << "Math: (" << finish.y << " - " << start.y << ") / " << pointNumber << " = " << pointDistance.y << endl;
    cout << "Start: " << start.x << "x" << start.y << endl;
    cout << "End: " << finish.x << "x" << finish.y << endl;
    cout << "Point distance: " << pointDistance.x << "x" << pointDistance.y << endl;
    */
}

void Ray::calculatePointsArray(){

    points[0] = start;

    for (short unsigned i=1; i<pointNumber; i++){
        points[i] = start + sf::Vector2f(i*pointDistance.x, i*pointDistance.y);
    }
}


Ray::Ray(sf::Vector2f start, sf::Vector2f finish){
    this->start = start;
    this->finish = finish;

    pointInCollision = pointNumber-1;
    calculatePointDistance();
    calculatePointsArray();
}

Ray::Ray(sf::Vector2f start, double angleDegrees, double lineOfSight){
    this->start = start;
    this->finish = start + sf::Vector2f(lineOfSight, lineOfSight);

    pointInCollision = pointNumber-1;
    
    this->rotateDegrees(angleDegrees);
}

Ray::Ray(Ray& other){
    this->start = other.start;
    this->finish = other.finish;

    calculatePointDistance();
    calculatePointsArray();

    this->pointInCollision = other.pointInCollision;
}


void Ray::move(sf::Vector2f offset){
    this->start += offset;
    this->finish += offset;

    // No need to recalculate point distance because 
    // the distance between start and finish has not changed

    calculatePointsArray();
}


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


void Ray::castIt(vector<sf::Drawable*> colliders){
    /*Suggested performance optimizations:

        DO THIS ONE TO FIX TRANSPARENT COLLIDER PROBLEM
        - Use a priority queue that orders each collider according to their distance from the start
          (so that we find a collision early and avoid checking most of the colliders).
        
        Would not fix transparent collider problem
        - Use a priority queue that orders each collider according to their surface area (bigger
          colliders have a higher chance to cause a collision).
    */

    sf::FloatRect colliderBounds;

    for (long unsigned i=0; i<colliders.size(); i++){

        try{
            sf::RectangleShape* collider = dynamic_cast<sf::RectangleShape*>(colliders[i]);
            colliderBounds = collider->getGlobalBounds();
        }catch(bad_cast){
            try{
                sf::CircleShape* collider = dynamic_cast<sf::CircleShape*>(colliders[i]);
                colliderBounds = collider->getGlobalBounds();
            }catch(bad_cast){
                cerr << "Ignored collider because of unsupported Drawable subclass" << endl;
                continue;
            }
        }

        for (int i=0; i<pointNumber; i++){
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