#pragma once

#include <Eigen/Core>
#include <utility>
#include <iostream>

enum class FluidMode {
	Constant,
	Oscillating
};

class FluidField
{
public:
	FluidField(FluidMode fluidMode);

	Eigen::Vector2d sampleField(Eigen::Vector2d position); // sample wind velocity at position

	Eigen::Vector2d constantWind();

private:
	FluidMode fluidMode_;

};

