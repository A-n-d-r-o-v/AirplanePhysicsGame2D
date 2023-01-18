#include "Util.h"

double Util::wrapAngle(double theta) 
{
	const double twoPi = 2 * EIGEN_PI;
	return theta - twoPi * floor(theta / twoPi);
}

double Util::angle(Eigen::Vector2d vec) 
{
	return atan2(-vec.y(), vec.x());
}

Eigen::Vector2d Util::unitOrthogonal(const Eigen::Vector2d& pointA, const Eigen::Vector2d& pointB)
{
	const double x{ -pointB.y() + pointA.y() };
	const double y{ pointB.x() - pointA.x() };

	Eigen::Vector2d axisNorm{ x, y };
	return axisNorm / axisNorm.norm();
}

