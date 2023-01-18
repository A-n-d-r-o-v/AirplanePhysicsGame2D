#include <SFML/Graphics.hpp>
#include <Eigen/Core>
#include "polygon.h"
#include "physicsobject.h"
#include "airplane.h"
#include "TerrainGenerator.h"
#include "Camera.h"
#include "Universe.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1200, 600), "My airplane sounds like this: \"wreeuuummmm!!\"");
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

    Polygon planeShape = Polygon{ vertices, 0.2, false };
    Airplane planeObj = Airplane(planeShape, 6000, 1);

    TerrainGenerator terrainGenerator(TerrainType::Mountains);
    terrainGenerator.generateMountainSegment(0, 5000, false);
    terrainGenerator.generateMountainSegment(5000, 10000, true);
    terrainGenerator.generateMountainSegment(10000, 20000, false);

    Camera camera(window, planeObj);

    Universe universe{ terrainGenerator, camera, planeObj };
    
    // TODO create world object with dynamic circles
    std::vector<sf::CircleShape> circles;
    for (int i = 0; i < 60; ++i) {
        for (int j = -5; j < 40; ++j) {
            sf::CircleShape circle;
            circle.setFillColor(sf::Color(100, 250, 50));
            circle.setRadius(10);
            circle.setPosition(i * 1000, j * 1000);
            circles.emplace_back(circle);
        }
    }

    sf::Clock mainClock;
    const double dt = 1.0 / 60.0;
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
            universe.updatePhysics(dt);

            accumulator -= dt;

        }
        //std::cout << planeObj.globalPosition().x() << ", " << planeObj.globalPosition().y() << std::endl;
        //std::cout << "Fps: "  << (int)(1 / frameTime) << std::endl;

        window.clear();

        universe.renderStep(window);

        for (const auto& circle : circles) {
            window.draw(circle);
        }

        window.display();
    }

    return 0;
}