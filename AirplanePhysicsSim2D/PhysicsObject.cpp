#include "PhysicsObject.h"

// Gravity
// Current x,y velocity and acceleration
// Lift, drag, gravity, thrust

// Future - Think: how to represent wind? Calculate from simplex noise, similar to how simplex worms work?

PhysicsObject::PhysicsObject(Polygon& shape, double mass, double gravityScale=1.0) : shape_(shape), mass_(mass), gravityScale_(gravityScale)
{
	calculateLocalCOM();
	std::cout << localCenterOfMass_.x() << ", " << localCenterOfMass_.y() << std::endl;
}

void PhysicsObject::calculateLocalCOM()
{
	localCenterOfMass_ = Eigen::Vector2d{0, 0};
	for (const auto& triangle : shape_.triangles()) {
		localCenterOfMass_ += (triangle.center() - globalPosition_);
	}
	localCenterOfMass_ /= shape_.triangleCount();
}

void PhysicsObject::globalTranslate(const Eigen::Vector2d& delta)
{
	globalPosition_ += delta;
	for (auto& triangle : shape_.triangles()) {
		triangle += delta;
	}
}

void PhysicsObject::translateForward(double delta)
{
	double x = delta * cos(-angle_);
	double y = delta * sin(-angle_);
	Eigen::Vector2d globalDelta{x, y};

	globalTranslate(globalDelta);
}


Polygon PhysicsObject::shape() const
{
	return shape_;
}

Eigen::Vector2d PhysicsObject::globalPosition() const
{
	return globalPosition_;
}

void PhysicsObject::rotateAroundCOM(double theta)
{
	//calculateLocalCOM();
	Eigen::Vector2d pivot = globalPosition_ + localCenterOfMass_;
	shape_.rotateAroundPivot(-theta, pivot);
	angle_ += theta;
}
