#pragma once

#include <Eigen/Core>

#include "polygon.h"
#include <iostream>

class PhysicsObject
{
public:
	PhysicsObject(Polygon& shape, double mass, double gravityScale);

	void calculateLocalCOM(); // Calculate local center of mass. Does not need to be re-calculated after rotation.

	// Create world object
	// Apply gravity to PhysicsObject

	void globalTranslate(const Eigen::Vector2d& delta);

	void translateForward(double delta);

	void addLocalVelocity(const Eigen::Vector2d& delta);

	//void applyForce(const Eigen::Vector2d& force); // Apply force, move and rotate accordingly

	const Eigen::Vector2d toWorldSpace(const Eigen::Vector2d& coordinate, double theta);

	const Eigen::Vector2d toLocalSpace(const Eigen::Vector2d& coordinate, double theta);

	virtual void update(double dt);

	Polygon shape() const;

	Eigen::Vector2d globalPosition() const;

	Eigen::Vector2d localVelocity() const;

	void rotateAroundCOM(double theta); // TODO: Public for testing purposes, should be private

protected:
	double mass_;
	double gravityScale_ = 1.0;
	double angle_ = 0.0;
	Polygon shape_;

	Eigen::Vector2d globalPosition_;
	Eigen::Vector2d localCenterOfMass_;

	Eigen::Vector2d worldVelocity_;
	Eigen::Vector2d worldAcceleration_;

	Eigen::Vector2d localAcceleration_;

	double worldAngularVelocity_;
	double worldAngularAcceleration_;

	double velocityAngle_;

	// ------------------

	static double angle(Eigen::Vector2d vec);

	static double wrapAngle(double theta); // Wrap angle from 0 to 2*pi

private:
	double gravityConstant_ = 0.006;

	Eigen::Vector2d localVelocity_;

	void updateLocalVelocity(); // Update local velocity.

	void updateWorldVelocity(); // Calculate and update world velocity, including angular velocity.

	void applyGravity(double dt);

};

