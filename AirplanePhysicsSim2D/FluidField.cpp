#include "FluidField.h"

FluidField::FluidField(FluidMode fluidMode) : fluidMode_(fluidMode) { }

// TODO: Get values from json file
Eigen::Vector2d FluidField::sampleField(Eigen::Vector2d position)
{
	switch (fluidMode_)
	{
	case FluidMode::Constant:
		return constantWind();
	case FluidMode::Oscillating:
		std::cout << "Fluid mode \'oscillating\' is not yet supported, ruh roh!" << std::endl;
		break;
	default:
		break;
	}
	return Eigen::Vector2d();
}

Eigen::Vector2d FluidField::constantWind()
{
	return Eigen::Vector2d{ 2.0, 1.0 }; // TODO
}


