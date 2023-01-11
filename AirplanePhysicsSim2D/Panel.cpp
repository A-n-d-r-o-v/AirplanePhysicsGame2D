#include "Panel.h"

Panel::Panel(std::pair <Eigen::Vector2d, Eigen::Vector2d> vertices) : vertices_(vertices) {
	Eigen::Vector2d v1 = vertices_.first;
	Eigen::Vector2d v2 = vertices_.second;

	dot_ = v1.dot(v2);
	v1_norm_ = v1.norm();
	v2_norm_ = v2.norm();

	midpoint_ = calcMidpoint_();
	angle_ = calcAngle_();
	normalVec_ = calcNormalVector_();
}

Eigen::Vector2d Panel::midpoint() const {
	return midpoint_;
}

double Panel::angle() const {
	return angle_;
}

Eigen::Vector2d Panel::normalVector() const{
	return normalVec_;
}


// ------- Private functions -------

Eigen::Vector2d Panel::calcMidpoint_() const {
	return ((std::get<0>(vertices_) + std::get<1>(vertices_))) / 2; // (a+b)/2
}

double Panel::calcAngle_() const {
	Eigen::Vector2d v1 = vertices_.first;
	Eigen::Vector2d v2 = vertices_.second;

	double dx = v2.x() - v1.x();
	double dy = v2.y() - v1.y();
	return std::atan2(dy, dx);
}

Eigen::Vector2d Panel::calcNormalVector_() const {
	Eigen::Vector2d normalVector { std::cos(angle_ + M_PI/2), std::sin(angle_ + M_PI/2)};
	return normalVector;
}

double Panel::v1_norm() const {
	return v1_norm_;
}

double Panel::v2_norm() const {
	return v2_norm_;
}

double Panel::dot() const {
	return dot_;
}