#include "polygon.h"

Polygon::Polygon(const std::vector<Eigen::Vector2d>& vertices) : vertices_(vertices) {
	panels_ = Polygon::generatePanels();

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

std::vector<Panel> Polygon::generatePanels() const {
	std::vector<Panel> panels;
	panels.reserve(vertices_.size());
	
	for (int i = 0; i < vertices_.size(); ++i) {
		// Get next vertex
		auto& v1 = vertices_[i];
		auto& v2 = vertices_[(i+1) % vertices_.size()];

		panels.emplace_back(std::make_pair(v1, v2));
	}
	
	return panels;
}

void Polygon::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& triangle : triangles_) {
		target.draw(triangle.toConvexShape(), states);
	}
	for (const auto& circle : normalCircles()) {
		target.draw(circle, states);
	}
	for (const auto& circle : cornerCircles()) {
		target.draw(circle, states);
	}
	target.draw(COMCircle(), states);

}

std::vector<Eigen::Vector2d> Polygon::vertices() const {
	return vertices_;
}

std::vector<Triangle>& Polygon::triangles()
{
	return triangles_;
}

std::vector<Panel> Polygon::panels() const {
	return panels_;
}

uint16_t Polygon::triangleCount() const
{
	return triangleCount_;
}

// ------- Private functions -------

std::vector<sf::CircleShape> Polygon::normalCircles() const {
	std::vector<sf::CircleShape> circleVec;
	circleVec.reserve(panels_.size());

	for (const auto& panel : panels_) {
		sf::CircleShape normalCircle;
		normalCircle.setRadius(3);
		normalCircle.setOrigin(normalCircle.getRadius(), normalCircle.getRadius());

		// Midpoint indicator
		normalCircle.setFillColor(sf::Color::Red);
		normalCircle.setPosition(sf::Vector2f(panel.midpoint().x(), panel.midpoint().y()));
		circleVec.emplace_back(normalCircle);

		Eigen::Vector2d normalEndpoint = panel.midpoint() + panel.normalVector();

		// Endpoint indicator (midpoint + normal vector)
		normalCircle.setFillColor(sf::Color::Blue);
		normalCircle.setPosition(sf::Vector2f(normalEndpoint.x(), normalEndpoint.y()));
		circleVec.emplace_back(normalCircle);


	}
	return circleVec;
}

std::vector<sf::CircleShape> Polygon::cornerCircles() const {
	std::vector<sf::CircleShape> circleVec;
	circleVec.reserve(vertices_.size());

	for (const auto& vertex : vertices_) {
		sf::CircleShape vertexCircle;
		vertexCircle.setRadius(3);
		vertexCircle.setOrigin(vertexCircle.getRadius(), vertexCircle.getRadius());

		vertexCircle.setFillColor(sf::Color::Green);
		vertexCircle.setPosition((float)vertex.x(), (float)vertex.y());
		circleVec.emplace_back(vertexCircle);
	}
	return circleVec;
}

sf::CircleShape Polygon::COMCircle() const {
	sf::CircleShape COMCircle;
	COMCircle.setRadius(10);
	COMCircle.setOrigin(COMCircle.getRadius(), COMCircle.getRadius());
	COMCircle.setPosition(128, 31);
	COMCircle.setFillColor(sf::Color::Green);

	return COMCircle;
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
