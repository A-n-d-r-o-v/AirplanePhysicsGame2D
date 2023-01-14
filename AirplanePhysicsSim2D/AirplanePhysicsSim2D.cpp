#include <SFML/Graphics.hpp>
#include <Eigen/Core>
#include "polygon.h"
#include "physicsobject.h"
#include "airplane.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1200, 900), "My airplane works!");
    int cameraLookahead = 800;
    int timeScale = 500;

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
    Airplane planeObj = Airplane(planeShape, 1100, 1);

    std::vector<Airplane> otherPlanes;
    for (int i = 0; i < 18; ++i) {
        Airplane otherPlane = Airplane(planeShape, 1200, 1);
        otherPlane.rotateAroundCOM(-EIGEN_PI / (0.3*i));
        otherPlane.thrust(5 * i);
        otherPlanes.emplace_back(otherPlane);
    }

    sf::View playerView(sf::FloatRect(0, 0, (float)window.getSize().x, (float)window.getSize().y));
    playerView.zoom(8.0f);

    // TODO create world object with dynamic circles
    std::vector<sf::CircleShape> circles;
    for (int i = 0; i < 60; ++i) {
        for (int j = -5; j < 40; ++j) {
            sf::CircleShape circle;
            circle.setFillColor(sf::Color(100, 250, 50));
            circle.setRadius(10);
            circle.setPosition(i * 5000, j * 5000);
            circles.emplace_back(circle);
        }
    }

    sf::Clock mainClock;
    double dt = 1.0 / 60.0;
    double accumulator = 0.0;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        double frameTime = mainClock.restart().asSeconds();
        accumulator += frameTime;

        while (accumulator >= dt) {
            playerView.setCenter((float)(planeObj.globalPosition().x() + cameraLookahead), (float)(planeObj.globalPosition().y()));
            window.setView(playerView);

            planeObj.applyInput();

            planeObj.update(dt* timeScale);

            for (auto& otherPlane : otherPlanes) {
                otherPlane.update(dt * timeScale);
                otherPlane.rotateAroundCOM(EIGEN_PI / 900);
            }

            accumulator -= dt;

        }

        std::cout << "Fps: "  << (int)(1 / frameTime) << std::endl;

        // TODO: Create world with point indicators.

        window.clear();

        window.draw(planeObj.shape());

        for (auto& otherPlane : otherPlanes) {
            window.draw(otherPlane.shape());
        }
        for (const auto& circle : circles) {
            window.draw(circle);
        }

        window.display();
    }

    return 0;
}