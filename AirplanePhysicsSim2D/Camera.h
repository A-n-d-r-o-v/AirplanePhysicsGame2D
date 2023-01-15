#pragma once

#include <SFML/Graphics.hpp>

#include "PhysicsObject.h"

class Camera
{
public:
	Camera(sf::RenderWindow& window, const PhysicsObject& objectToFollow);

	void updateView();

	std::pair<int, int> renderBoundX() const;
	std::pair<int, int> renderBoundY() const;

private:
	sf::View view_;
	sf::RenderWindow& window_;

	const PhysicsObject& objectToFollow_;

	const int renderBoundMargin_ = 100;
	std::pair<int, int> renderBoundX_; // Min max x bound
	std::pair<int, int> renderBoundY_; // Min max y bound

	/* Params. TODO: retrieve from JSON config file */

	const Eigen::Vector2d offset_{ 0, 0 };

	const Eigen::Vector2d lookAhead_{ 0.7, 0.3 } ; // Look-ahead offset factor relative to window size

	const double angularLookAhead_ = 500.4;
	const double maxAngularLookAhead_ = EIGEN_PI / 8;

	double zoomLevel_ = 2.0;
	double prevZoomLevel_ = zoomLevel_;

	std::pair<double, double> zoomRange_ = std::make_pair(0.75, 3.0);

	/* ---------- */

	Eigen::Vector2d lookAheadVec();

	void calculateRenderBounds();

	void calculateZoomLevel();

};

