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

	void operator += (Eigen::Vector2d vec);

	// Getters
	std::vector<Eigen::Vector2d>& getVertices();

private:
	std::vector<Eigen::Vector2d> vertices_;
};

