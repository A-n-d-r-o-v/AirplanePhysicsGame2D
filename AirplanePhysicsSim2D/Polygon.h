#pragma once

#include <SFML/Graphics.hpp>
#include <Eigen/Core>
#include <vector>
#include <iostream>
#include "panel.h"
#include "triangle.h"


class Polygon : public sf::Drawable
{
public:
	Polygon(const std::vector<Eigen::Vector2d>& vertices);

	std::vector<Panel> generatePanels() const;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	
	void rotateAroundPivot(double theta, Eigen::Vector2d pivotPoint); // Rotate around center of mass.

	// Getters
	std::vector<Eigen::Vector2d> vertices() const; // Get vertices
	std::vector<Triangle>& triangles(); // Maybe create SFML Triangles object??
	std::vector<Panel> panels() const; // Get panels
	uint16_t triangleCount() const;

private:
	std::vector<Eigen::Vector2d> vertices_;
	std::vector<Panel> panels_;

	std::vector<Triangle> triangles_; // Vector of triangles from triangulation
	uint16_t triangleCount_ = 0;

	std::vector<Triangle> triangulate(); // Triangulate polygon using ear-clipping triangulation
										// TODO save index list to make it fast to call again? eg. in case of scaling

	std::vector<sf::CircleShape> normalCircles() const; // Return a list of SFML circles corresponding to all the midpoints and normal vectors
	std::vector<sf::CircleShape> cornerCircles() const; // Return a list of SFML circles corresponding to all the corner vertices
	sf::CircleShape COMCircle() const;


};

