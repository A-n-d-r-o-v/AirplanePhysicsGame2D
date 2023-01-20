#pragma once

#include <SFML/Graphics.hpp>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <vector>
#include <iostream>
#include <execution>

#include "Triangle.h"

namespace APGeom {

	class Polygon : public sf::Drawable
	{
	public:
		Polygon(const std::vector<Eigen::Vector2d>& vertices, bool enableCulling = true);

		Polygon(std::vector<Eigen::Vector2d>& vertices, double scale, bool enableCulling = true);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		void rotateAroundPivot(double theta, Eigen::Vector2d pivotPoint); // Rotate around center of mass.

		// Getters
		std::vector<Eigen::Vector2d> vertices() const; // Get vertices
		std::vector<std::shared_ptr<Triangle>> triangles();
		uint16_t triangleCount() const;

	private:
		std::vector<Eigen::Vector2d> vertices_;

		std::vector<std::shared_ptr<Triangle>> triangles_; // Vector of triangles from triangulation
		uint16_t triangleCount_ = 0;

		std::vector<std::shared_ptr<Triangle>> triangulate(bool enableCulling); // Triangulate polygon using ear-clipping triangulation
		// TODO save index list to make it fast to call again? eg. in case of scaling

		const std::vector<sf::CircleShape> cornerCircles() const; // Return a list of SFML circles corresponding to all the corner vertices

	};
}
