#pragma once

#include <Eigen/Core>

#include "polygon.h"
#include <iostream>

class PhysicsObject
{
public:
	PhysicsObject(Polygon& shape, double mass, double gravityScale);

	void calculateLocalCOM();

	// Create world object
	// Apply gravity to PhysicsObject
	// Triangulate polygon
	// Find center of mass
	// Rotate object around center of mass

	void globalTranslate(const Eigen::Vector2d &delta);

	void translateForward(double delta);

	void applyForce(Eigen::Vector2d &force); // Apply force, move and rotate accordingly

	Polygon shape() const;

	void rotateAroundCOM(double theta); // TODO: Public for testing purposes, should be private

private:
	double mass_;
	double gravityScale_ = 1.0;
	double angle_ = 0.0;
	Polygon shape_;

	Eigen::Vector2d globalPosition_;
	Eigen::Vector2d localCenterOfMass_;

};

