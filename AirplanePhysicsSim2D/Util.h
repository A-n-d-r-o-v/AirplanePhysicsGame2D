#pragma once

#include <Eigen/Core>
#include <cmath>

namespace Util {

	double wrapAngle(double theta);

	double angle(Eigen::Vector2d vec);

	Eigen::Vector2d unitOrthogonal(const Eigen::Vector2d& pointA, const Eigen::Vector2d& pointB);
}