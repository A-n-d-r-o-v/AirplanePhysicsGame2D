#pragma once

#include <SFML/Graphics.hpp>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <vector>
#include <iostream>
#include "triangle.h"


class Polygon : public sf::Drawable
{
public:
	Polygon(const std::vector<Eigen::Vector2d>& vertices);

	Polygon(std::vector<Eigen::Vector2d>& vertices, double scale);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	
	void rotateAroundPivot(double theta, Eigen::Vector2d pivotPoint); // Rotate around center of mass.

	// Getters
	std::vector<Eigen::Vector2d> vertices() const; // Get vertices
	std::vector<Triangle>& triangles(); // Maybe create SFML Triangles object??
	uint16_t triangleCount() const;

private:
	std::vector<Eigen::Vector2d> vertices_;

	std::vector<Triangle> triangles_; // Vector of triangles from triangulation
	uint16_t triangleCount_ = 0;

	std::vector<Triangle> triangulate(); // Triangulate polygon using ear-clipping triangulation
										// TODO save index list to make it fast to call again? eg. in case of scaling

	const std::vector<sf::CircleShape> cornerCircles() const ; // Return a list of SFML circles corresponding to all the corner vertices

};

