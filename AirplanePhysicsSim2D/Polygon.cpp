#include "polygon.h"

Polygon::Polygon(const std::vector<Eigen::Vector2d>& vertices) : vertices_(vertices) {
	triangles_ = triangulate();
}

Polygon::Polygon(std::vector<Eigen::Vector2d>& vertices, double scale)
{
	std::transform(vertices.begin(), vertices.end(), vertices.begin(),
		[scale](const Eigen::Vector2d& v) { return Eigen::Vector2d{ v[0] * scale, v[1] * scale }; });

	vertices_ = vertices;

	//for (Eigen::Vector2d & element : vertices) {
	//	element.x() *= scale;
	//	element.y() *= scale;
	//}

	triangles_ = triangulate();
}

void Polygon::rotateAroundPivot(double theta, Eigen::Vector2d pivotPoint)
{
	Eigen::Rotation2Dd t(theta);
	Eigen::Matrix2d rotMat = t.toRotationMatrix();

	for (auto& triangle : triangles_) {
		for (auto& v : triangle.getVertices()) {
			v -= pivotPoint;
			v = rotMat * v;
			v += pivotPoint;
		}
	}
}

void Polygon::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& triangle : triangles_) {
		target.draw(triangle.toConvexShape(), states);
	}
	//for (auto& circle : cornerCircles()) {
	//	target.draw(circle, states);
	//}
}

std::vector<Eigen::Vector2d> Polygon::vertices() const {
	return vertices_;
}

std::vector<Triangle>& Polygon::triangles()
{
	return triangles_;
}

uint16_t Polygon::triangleCount() const
{
	return triangleCount_;
}

// ------- Private functions -------

const std::vector<sf::CircleShape> Polygon::cornerCircles() const {
	std::vector<sf::CircleShape> circleVec;
	circleVec.reserve(vertices_.size());

	for (const auto& triangle : triangles_) {
		for (const auto& vertex : triangle.getVertices()) {
			sf::CircleShape vertexCircle;
			vertexCircle.setRadius(3);
			vertexCircle.setOrigin(vertexCircle.getRadius(), vertexCircle.getRadius());

			vertexCircle.setFillColor(sf::Color::Green);
			vertexCircle.setPosition((float)vertex.x(), (float)vertex.y());
			circleVec.emplace_back(vertexCircle);
		}
	}
	return circleVec;
}

std::vector<Triangle> Polygon::triangulate()
{
	std::vector<Eigen::Vector2d> vertices = vertices_;

	std::vector<Triangle> triangles;
	triangleCount_ = std::size(vertices) - 2;
	triangles.reserve(triangleCount_);

	while (std::size(vertices) > 3) {
		uint16_t vertexCount = std::size(vertices);

		for (uint16_t i = 0; i < vertexCount; ++i) {
			int prevIndex = i == 0				? vertexCount-1 : i - 1;
			int nextIndex = i == vertexCount-1	? 0				: i + 1;
			Eigen::Vector2d currVec = vertices[i];
			Eigen::Vector2d prevVec = vertices[prevIndex];
			Eigen::Vector2d nextVec = vertices[nextIndex];
			
			if (Triangle::isConvex(prevVec, currVec, nextVec)) {
				continue;
			}
			
			bool isEar = true;

			// Check if test ear contains any polygon vertices
			for (uint16_t j = 0; j < std::size(vertices); ++j) {
				// Skip currVec, prevVec, nextVec
				if (j == i || j == prevIndex || j == nextIndex) {
					continue;
				}
				Eigen::Vector2d point = vertices[j];
				if (Triangle::isPointInside(point, prevVec, currVec, nextVec)) {
					isEar = false;
					break;
				}
			}
			if (isEar) {
				Triangle t = Triangle(prevVec, currVec, nextVec);
				triangles.emplace_back(t);

				vertices.erase(vertices.begin() + i); // Remove vertex at index i
				vertexCount--;
				break;
			}
		}
	}

	Triangle t = Triangle(vertices[0], vertices[1], vertices[2]); // emplace last triangle
	triangles.emplace_back(t);

	return triangles;
}
