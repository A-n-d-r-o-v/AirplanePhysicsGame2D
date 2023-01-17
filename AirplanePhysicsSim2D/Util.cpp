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