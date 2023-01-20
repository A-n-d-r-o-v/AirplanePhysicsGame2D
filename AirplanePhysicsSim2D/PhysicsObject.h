#pragma once

#include <Eigen/Core>
#include <iostream>

#include "Polygon.h"
#include "Util.h"

class PhysicsObject
{
public:
	PhysicsObject(APGeom::Polygon& shape, double mass, double gravityScale);

	void calculateLocalCOM(); // Calculate local center of mass. Does not need to be re-calculated after rotation.

	// Create world object
	// Apply gravity to PhysicsObject

	void globalTranslate(const Eigen::Vector2d& delta);

	void translateForward(double delta);

	void addLocalVelocity(const Eigen::Vector2d& delta);

	//void applyForce(const Eigen::Vector2d& force); // Apply force, move and rotate accordingly

	static const Eigen::Vector2d toWorldSpace(const Eigen::Vector2d& coordinate, double theta);

	static const Eigen::Vector2d toLocalSpace(const Eigen::Vector2d& coordinate, double theta);

	virtual void update(double dt);

	APGeom::Polygon shape() const;

	Eigen::Vector2d globalPosition() const;

	Eigen::Vector2d localVelocity() const;

	Eigen::Vector2d worldVelocity() const;

	double angle() const;

	double angularVelocity() const;

	double velocityNorm() const;

	double maxVelocityNorm() const;

	void rotateAroundCOM(double theta);

protected:
	double mass_;
	double gravityScale_ = 1.0;
	double angle_ = 0.0;
	APGeom::Polygon shape_;

	Eigen::Vector2d globalPosition_;
	Eigen::Vector2d localCenterOfMass_;

	Eigen::Vector2d worldVelocity_;
	Eigen::Vector2d worldAcceleration_;

	Eigen::Vector2d localAcceleration_;

	double angularVelocity_;
	double angularAcceleration_;

	double velocityNorm_ = 0;		// Velocity magnitude
	double maxVelocityNorm_ = 0;	// Highest velocity magnitude

	double velocityAngle_;

	// ------------------

private:
	double gravityConstant_ = 9.82;

	Eigen::Vector2d localVelocity_;

	void updateLocalVelocity(); // Update local velocity.

	void updateWorldVelocity(); // Calculate and update world velocity, including angular velocity.

	void applyGravity(double dt);

	void updateVelocityNorm();

};

