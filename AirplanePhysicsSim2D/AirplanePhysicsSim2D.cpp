#include <SFML/Graphics.hpp>
#include <Eigen/Core>
#include "polygon.h"
#include "physicsobject.h"
#include "airplane.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "My airplane works!");
    int cameraLookahead = 300;

    // (Load configuration with auto-scaling)
    std::vector<Eigen::Vector2d> vertices{
        Eigen::Vector2d(0.0, 0.0),
        Eigen::Vector2d(10.0, 10.0),
        Eigen::Vector2d(20.0, 30.0),
        Eigen::Vector2d(20.0, 37.0),
        Eigen::Vector2d(50.0, 35.0),
        Eigen::Vector2d(110.0, 20.0),
        Eigen::Vector2d(160.0, 0.0),
        Eigen::Vector2d(210.0, 0.0),
        Eigen::Vector2d(290.0, 30.0),
        Eigen::Vector2d(420.0, 42.0),
        Eigen::Vector2d(400.0, 47.0),
        Eigen::Vector2d(300.0, 50.0),
        Eigen::Vector2d(0.0, 50.0)
    };
    
    Polygon planeShape = Polygon{ vertices };
    Airplane planeObj = Airplane(planeShape, 1000, 1);

    sf::View playerView(sf::FloatRect(0, 0, (float)window.getSize().x, (float)window.getSize().y));
    playerView.zoom(2.0f);

    sf::CircleShape aCircle(50);
    aCircle.setFillColor(sf::Color(100, 250, 50));
    float circleX = 600;
    float distanceTravelled = 0;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        playerView.setCenter((float) (planeObj.globalPosition().x() + cameraLookahead), (float) (planeObj.globalPosition().y()));
        window.setView(playerView);

        // TODO: Create world with point indicators.

        window.clear();

        window.draw(planeObj.shape());

        distanceTravelled += planeObj.localVelocity().x();
        if (distanceTravelled > 2*window.getSize().x) {
            circleX += 2*distanceTravelled;
            distanceTravelled = 0;
        }

        aCircle.setPosition(circleX, -100);
        window.draw(aCircle);
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            planeObj.thrust(0.5);
        }
        else {
            planeObj.thrust(0);
        }
        
        planeObj.update(1.0);
        //planeObj.globalTranslate(Eigen::Vector2d{ 0.02, 0.01 });
        //planeObj.translateForward(0.02);
        //planeObj.rotateAroundCOM(-0.00003);

        window.display();
    }

    return 0;
}