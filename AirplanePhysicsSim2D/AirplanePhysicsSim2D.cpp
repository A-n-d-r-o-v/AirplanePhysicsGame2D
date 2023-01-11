#include <SFML/Graphics.hpp>
#include <Eigen/Core>
#include "polygon.h"
#include "physicsobject.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "My airplane works!");
    sf::CircleShape shape(100.f);

    // (Load configuration with auto-scaling)
    std::vector<Eigen::Vector2d> vertices{
        Eigen::Vector2d(0.0, 0.0),
        Eigen::Vector2d(20.0, 30.0),
        //Eigen::Vector2d(20.0, 40.0),
        Eigen::Vector2d(200.0, 0.0),
        Eigen::Vector2d(400.0, 36.0),
        Eigen::Vector2d(300.0, 50.0),
        Eigen::Vector2d(0.0, 50.0)
    };
    
    Polygon planeShape = Polygon{ vertices };
    PhysicsObject planeObj = PhysicsObject(planeShape, 100, 1);

    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        window.draw(planeObj.shape());
        //planeObj.globalTranslate(Eigen::Vector2d{ 0.005, 0 });
        planeObj.translateForward(0.05);
        planeObj.rotateAroundCOM(-0.0003);

        window.display();
    }

    return 0;
}