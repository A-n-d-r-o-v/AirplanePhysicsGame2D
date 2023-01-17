#pragma once

#include <Eigen/Core>
#include <cmath>

namespace Util {

	double wrapAngle(double theta);

	double angle(Eigen::Vector2d vec);
}