#pragma once

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <utility>
#include <iostream>

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif
#include <cmath>

struct Panel
{
	Panel(std::pair <Eigen::Vector2d, Eigen::Vector2d> vertices);

	Eigen::Vector2d midpoint() const; // Midpoint getter.

	double angle() const; // Angle getter.

	Eigen::Vector2d normalVector() const; // Normal vector getter.

	// Getters
	[[nodiscard]] double v1_norm() const;
	[[nodiscard]] double v2_norm() const;
	[[nodiscard]] double dot() const;

private:

	std::pair <Eigen::Vector2d, Eigen::Vector2d> vertices_;

	[[nodiscard]] Eigen::Vector2d calcMidpoint_() const; // The point between the edge polygons.

	[[nodiscard]] double calcAngle_() const; // Angle of panel.

	[[nodiscard]] Eigen::Vector2d calcNormalVector_() const; // Normal vector. (add midpoint to get normal vector endpoint)

	double v1_norm_, v2_norm_;
	double dot_;
	Eigen::Vector2d midpoint_;
	double angle_;
	Eigen::Vector2d normalVec_;
};

