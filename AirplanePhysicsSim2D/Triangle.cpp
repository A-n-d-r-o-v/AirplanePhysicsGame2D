#include "Triangle.h"

Triangle::Triangle(Eigen::Vector2d vertexA, Eigen::Vector2d vertexB, Eigen::Vector2d vertexC)
{
	this->vertices_.emplace_back(vertexA);
	this->vertices_.emplace_back(vertexB);
	this->vertices_.emplace_back(vertexC);
}

sf::ConvexShape Triangle::toConvexShape() const {
	sf::ConvexShape convex;
	convex.setPointCount(3);

	convex.setPoint(0, sf::Vector2f(vertices_[0].x(), vertices_[0].y()));
	convex.setPoint(1, sf::Vector2f(vertices_[1].x(), vertices_[1].y()));
	convex.setPoint(2, sf::Vector2f(vertices_[2].x(), vertices_[2].y()));

	return convex;
}

bool Triangle::isConvex(Eigen::Vector2d vertexA, Eigen::Vector2d vertexB, Eigen::Vector2d vertexC)
{
	Eigen::Vector2d ab = vertexB - vertexA;
	Eigen::Vector2d ac = vertexC - vertexA;
	
	float cross = ab.x() * ac.y() - ab.y() * ac.x();
	return cross < 0.0;
}

float Triangle::area() const
{
	return area(vertices_[0], vertices_[1], vertices_[2]);
}

float Triangle::area(Eigen::Vector2d vertexA, Eigen::Vector2d vertexB, Eigen::Vector2d vertexC)
{
	return abs((vertexA.x() * (vertexB.y() - vertexC.y())
		+ vertexB.x() * (vertexC.y() - vertexA.y())
		+ vertexC.x() * (vertexA.y() - vertexB.y())) / 2.0);
}

bool Triangle::isPointInside(Eigen::Vector2d point) const
{
	return isPointInside(point, vertices_[0], vertices_[1], vertices_[2]);
}

bool Triangle::isPointInside(Eigen::Vector2d point, Eigen::Vector2d vertexA, Eigen::Vector2d vertexB, Eigen::Vector2d vertexC)
{
	float ABC_area = area(vertexA, vertexB, vertexC);
	float PBC_area = area(point, vertexB, vertexC);
	float PAC_area = area(point, vertexA, vertexC);
	float PAB_area = area(point, vertexA, vertexB);

	return ABC_area == PBC_area + PAC_area + PAB_area;
}

Eigen::Vector2d Triangle::center() const{
	double x = (vertices_[0].x() + vertices_[1].x() + vertices_[2].x()) / 3.0;
	double y = (vertices_[0].y() + vertices_[1].y() + vertices_[2].y()) / 3.0;

	return Eigen::Vector2d(x, y);
}

void Triangle::operator+=(Eigen::Vector2d vec)
{
	for (int i = 0; i < std::size(vertices_); ++i) {
		vertices_[i] += vec;
	}
}

std::vector<Eigen::Vector2d>& Triangle::getVertices()
{
	return vertices_;
}

const std::vector<Eigen::Vector2d>& Triangle::getVertices() const
{
	return vertices_;
}


