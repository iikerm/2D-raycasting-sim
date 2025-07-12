#include "Ray.hpp"
using namespace std;

int main(){
    Ray testRay = Ray(sf::Vector2f(100, 100), sf::Vector2f(100, 100));
    
    sf::RectangleShape* a = new sf::RectangleShape(sf::Vector2f(100, 100));

    sf::RectangleShape* b = new sf::RectangleShape(sf::Vector2f(100, 100));
    
    sf::RectangleShape* c = new sf::RectangleShape(sf::Vector2f(100, 100));

    vector<sf::RectangleShape*> colliders = {a, b, c};

    return 0;
}