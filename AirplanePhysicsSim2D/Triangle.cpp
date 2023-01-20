#include "Triangle.h"

using namespace APGeom;

Triangle::Triangle(Eigen::Vector2d vertexA, Eigen::Vector2d vertexB, Eigen::Vector2d vertexC)
	: vertices_{vertexA, vertexB, vertexC}
{
	findMinMaxVertices();
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

void Triangle::findMinMaxVertices() {

	auto [x_min, x_max] = std::minmax_element(vertices_.begin(), vertices_.end(), [](const auto& a, const auto& b) { return a.x() < b.x(); });
	auto [y_min, y_max] = std::minmax_element(vertices_.begin(), vertices_.end(), [](const auto& a, const auto& b) { return a.y() < b.y(); });

	minXVertex_ = std::make_shared<Eigen::Vector2d>(*x_min);
	maxXVertex_ = std::make_shared<Eigen::Vector2d>(*x_max);

	minYVertex_ = std::make_shared<Eigen::Vector2d>(*y_min);
	maxYVertex_ = std::make_shared<Eigen::Vector2d>(*y_max);
}

bool Triangle::intersects(const Triangle& other) {
	// Later when looping through triangles: set checkedCollision to true in first for loop
	// In PhysicsObject, add collides(std::vector<Polygon> otherPolys)
	// Check collision for all PhysicsObjects in Universe

	// Do quick bounding box collision
	if (!boundingBoxCollides(other)) {
		return false;
	}

	return separatingAxisIntersect(other);
}


bool Triangle::inView(int leftBound, int rightBound, int upperBound, int lowerBound) {
		
	return !(
		minXVertex_->x() > rightBound || 
		maxXVertex_->x() < leftBound ||
		minYVertex_->y() > lowerBound ||
		maxYVertex_->y() < upperBound
	);
}

void Triangle::operator+=(Eigen::Vector2d vec)
{
	for (int i = 0; i < 3; ++i) {
		vertices_[i] += vec;
	}
}

std::array<Eigen::Vector2d, 3>& Triangle::getVertices()
{
	return vertices_;
}

const std::array<Eigen::Vector2d, 3>& Triangle::getVertices() const
{
	return vertices_;
}

void Triangle::setEnableCulling(bool enableCulling) {
	enableCulling_ = enableCulling;
}

bool Triangle::enableCulling() const {
	return enableCulling_;
}

void Triangle::setVisibility(bool visible) {
	isVisible_ = visible;
}

bool Triangle::isVisible() const {
	return isVisible_;
}

std::array<double, 3> Triangle::projections(const Eigen::Vector2d& axis_normalised) const
{
	std::array<double, 3> projections{};

	for (size_t i = 0; i < vertices_.size(); ++i) {
		const double projection = axis_normalised.dot(vertices_[i]);
		projections[i] = projection;
	}
	return projections;
}

bool Triangle::isOverlapping(std::array<double, 3>& projectionsA, std::array<double, 3>& projectionsB) {
	const double minProjectionA = *std::min_element(projectionsA.begin(), projectionsA.end());
	const double maxProjectionA = *std::max_element(projectionsA.begin(), projectionsA.end());
	const double minProjectionB = *std::min_element(projectionsB.begin(), projectionsB.end());
	const double maxProjectionB = *std::max_element(projectionsB.begin(), projectionsB.end());

	// True if projection overlaps but does not necessarily mean the polygons are intersecting yet
	return !(maxProjectionA < minProjectionB or maxProjectionB < minProjectionA);
}

bool Triangle::separatingAxisIntersect(const Triangle& other)
{
	// Check for overlap of projections along the normals of all the edges in this triangle
	for (uint16_t i = 0; i < vertices_.size(); ++i) {
		const Eigen::Vector2d next{ vertices_[(i + 1) % vertices_.size()] };
		const Eigen::Vector2d axisNorm = Util::unitOrthogonal(vertices_[i], next);
		std::array<double, 3> projectionsThis = projections(axisNorm);
		std::array<double, 3> projectionsOther = other.projections(axisNorm);

		if (!isOverlapping(projectionsThis, projectionsOther)) return false;
	}
	// Check for overlap of projections along the normals of all the edges in the other triangle
	for (uint16_t i = 0; i < other.vertices_.size(); ++i) {
		const Eigen::Vector2d next{ other.vertices_[(i + 1) % other.vertices_.size()] };
		const Eigen::Vector2d axisNorm = Util::unitOrthogonal(other.vertices_[i], next);
		std::array<double, 3> projectionsThis = projections(axisNorm);
		std::array<double, 3> projectionsOther = other.projections(axisNorm);

		if (!isOverlapping(projectionsThis, projectionsOther)) return false;
	}
	return true;
}

bool Triangle::boundingBoxCollides(const Triangle& other)
{
	// x values do not overlap
	if (maxXVertex_->x() < other.minXVertex_->x() || other.maxXVertex_->x() < minXVertex_->x()) {
		return false;
	}

	// y values do not overlap
	if (maxYVertex_->y() < other.minYVertex_->y() || other.maxYVertex_->y() < minYVertex_->y()) {
		return false;
	}

	// If the x and y coordinates of the bounding boxes overlap, the triangles may be colliding
	return true;
}

