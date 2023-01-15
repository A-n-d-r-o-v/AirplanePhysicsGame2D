#pragma once

#include <Eigen/Core>
#include <SFML/Graphics.hpp>
#include <iostream>

struct Triangle
{
	Triangle(Eigen::Vector2d vertexA, Eigen::Vector2d vertexB, Eigen::Vector2d vertexC);

	sf::ConvexShape toConvexShape() const;

	static bool isConvex(Eigen::Vector2d vertexA, Eigen::Vector2d vertexB, Eigen::Vector2d vertexC); // Return true if angle ABC is convex

	float area() const;

	static float area(Eigen::Vector2d vertexA, Eigen::Vector2d vertexB, Eigen::Vector2d vertexC);

	bool isPointInside(Eigen::Vector2d point) const;

	static bool isPointInside(Eigen::Vector2d point, Eigen::Vector2d vertexA, Eigen::Vector2d vertexB, Eigen::Vector2d vertexC);

	Eigen::Vector2d center() const;

	void findMinMaxVertices();

	void operator += (Eigen::Vector2d vec);

	// Getters
	std::array<Eigen::Vector2d, 3>& getVertices();
	const std::array<Eigen::Vector2d, 3>& getVertices() const;
	bool isVisible() const;

private:
	std::array<Eigen::Vector2d, 3> vertices_;

	Eigen::Vector2d* minXVertex_;
	Eigen::Vector2d* minYVertex_;

	Eigen::Vector2d* maxXVertex_;
	Eigen::Vector2d* maxYVertex_;

	bool isVisible_ = true;
};

