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

	bool inView(int leftBound, int rightBound, int upperBound, int lowerBound);

	void operator += (Eigen::Vector2d vec);

	// Getters
	std::array<Eigen::Vector2d, 3>& getVertices();
	const std::array<Eigen::Vector2d, 3>& getVertices() const;

	void setEnableCulling(bool enableCulling);
	bool enableCulling() const;

	void setVisibility(bool visible);
	bool isVisible() const;

private:
	std::array<Eigen::Vector2d, 3> vertices_;

	std::shared_ptr<Eigen::Vector2d> minXVertex_;
	std::shared_ptr<Eigen::Vector2d> minYVertex_;

	std::shared_ptr<Eigen::Vector2d> maxXVertex_;
	std::shared_ptr<Eigen::Vector2d> maxYVertex_;

	bool isVisible_ = true;
	bool enableCulling_ = true;
};

