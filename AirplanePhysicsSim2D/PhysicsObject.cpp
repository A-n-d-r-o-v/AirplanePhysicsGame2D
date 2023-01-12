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
	const double x = delta * cos(-angle_);
	const double y = delta * sin(-angle_);
	Eigen::Vector2d globalDelta{x, y};

	globalTranslate(globalDelta);
}

void PhysicsObject::addLocalVelocity(const Eigen::Vector2d& delta)
{
	worldVelocity_ += toWorldSpace(delta);
	localVelocity_ = toLocalSpace(worldVelocity_);
}


Eigen::Vector2d const PhysicsObject::toWorldSpace(const Eigen::Vector2d& coordinate)
{
	Eigen::Rotation2Dd t(-angle_);
	Eigen::Matrix2d rotMat = t.toRotationMatrix();
	return Eigen::Vector2d { rotMat * coordinate };
}

const Eigen::Vector2d PhysicsObject::toLocalSpace(const Eigen::Vector2d& coordinate)
{
	Eigen::Rotation2Dd t(-angle_);
	Eigen::Matrix2d rotMat = t.inverse().toRotationMatrix();
	return Eigen::Vector2d { rotMat * coordinate };
}

void PhysicsObject::update(double dt)
{
	angle_ = wrapAngle(angle_);
	velocityAngle_ = wrapAngle(angle(worldVelocity_));

	updateLocalVelocity();
	updateWorldVelocity();

	//std::cout << "World velocity: " << worldVelocity_.x() << ", " << worldVelocity_.y() << std::endl;

	globalTranslate(worldVelocity_ * dt);
	rotateAroundCOM(worldAngularVelocity_ * dt);
}

void PhysicsObject::updateLocalVelocity()
{
	localVelocity_ += localAcceleration_;
}

void PhysicsObject::updateWorldVelocity()
{
	worldAcceleration_ = toWorldSpace(localAcceleration_);

	worldVelocity_ += worldAcceleration_;
	worldAngularVelocity_ += worldAngularAcceleration_;
}

Polygon PhysicsObject::shape() const
{
	return shape_;
}

Eigen::Vector2d PhysicsObject::globalPosition() const
{
	return globalPosition_;
}

Eigen::Vector2d PhysicsObject::localVelocity() const
{
	return localVelocity_;
}

void PhysicsObject::rotateAroundCOM(double theta)
{
	Eigen::Vector2d pivot = globalPosition_ + localCenterOfMass_;
	shape_.rotateAroundPivot(-theta, pivot);
	angle_ += theta;
}

double PhysicsObject::wrapAngle(double theta)
{
	const double twoPi = 2 * EIGEN_PI;
	return theta - twoPi * floor(theta / twoPi);
}

double PhysicsObject::angle(Eigen::Vector2d vec)
{
	return atan2(-vec.y(), vec.x());
}
