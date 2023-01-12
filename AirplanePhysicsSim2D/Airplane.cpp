#include "Airplane.h"

void Airplane::update(double dt)
{
	PhysicsObject::update(dt); // Call base class update

	calculateState();

	// Apply drag
	applyDrag();
	applyAngularDrag();
	applyLift();
}

void Airplane::applyInput()
{
	// Thrust
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		thrust(thrustPower_);
	}
	else {
		thrust(0);
	}
	// Pitch up
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		pitch(pitchPower_);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		pitch(-pitchPower_);
	}
	else {
		pitch(0);
	}
}

void Airplane::thrust(double forwardForce)
{
	Eigen::Vector2d thrustVec{ forwardForce / mass_, 0 };

	localAcceleration_.x() = thrustVec.x();
}

// TODO implement angular drag
void Airplane::pitch(double angularForce)
{
	double angularAcceleration = angularForce / mass_;

	worldAngularAcceleration_ = angularAcceleration;
}

void Airplane::calculateState()
{
	angleToGround_ = toGroundAngle(angle_);
	calculateAngleOfAttack();

	upsideDown_ = isUpsideDown();

	calculateDrag();
	calculateAngularDrag();
	calculateLift();

}

// TODO: AOA should calculate difference between angle (currently AOA) and velocity direction (from global velocity)
double Airplane::toGroundAngle(double theta)
{
	double groundAngle = wrapAngle(theta); // later: calculate if plane is upside-down like this (pi > angle > 3pi or something)
	short sign = 1;

	// Bottom half: invert sign
	if (groundAngle > EIGEN_PI) {
		groundAngle -= EIGEN_PI;
		sign = -1;
	}
	// Mirror values that are >pi rads
	if (groundAngle > EIGEN_PI / 2 && groundAngle <= EIGEN_PI) {
		groundAngle = EIGEN_PI - groundAngle;
	}
	groundAngle *= sign;
	return groundAngle;
}

void Airplane::calculateAngleOfAttack()
{
	double aoa = toGroundAngle(angle_ - velocityAngle_);
	angleOfAttack_ = aoa;
}

double Airplane::sampleAOACurve(double angleOfAttack)
{
	double value;

	if (angleOfAttack > EIGEN_PI/6) {
		// Curve from approx. (x : pi/6 -> pi/2) , (y : 1 -> 0)
		value = sin( ((2 - EIGEN_PI/6) * angleOfAttack) + (EIGEN_PI/4 + 0.038));
	}
	else if (angleOfAttack < -EIGEN_PI/6) {
		// Curve from approx. (x : -pi/6 -> -pi/2) , (y : -1 -> 0)
		value = sin( ((2 - EIGEN_PI/6) * angleOfAttack) - (EIGEN_PI/4 + 0.038));
	}
	else {
		value = sin(3 * angleOfAttack);
	}

	// Clamp value between [-pi/2, pi/2]
	value = fmin(value, EIGEN_PI/2);
	value = fmax(value, -EIGEN_PI / 2);
	return value;
}

bool Airplane::isUpsideDown()
{
	return angle_ > EIGEN_PI / 2 && angle_ < 1.5 * EIGEN_PI;
}

void Airplane::calculateDrag()
{
	// TODO: should also look at local y velocity
	double localVelocityX = localVelocity().x();
	double velXSquared = localVelocityX * localVelocityX;
	double dragCoeff = dragAOAMultiplier_ * (abs(angleOfAttack_) + dragAOABias_);
	drag_ = 0.5 * velXSquared * dragCoeff * dragScale_ * copysign(1.0, localVelocityX);

	//std::cout << "Drag acc: " << drag_ << ", \t vel x: " << localVelocityX << std::endl; // Bugged.

}

void Airplane::applyDrag()
{
	double dragAcceleration = drag_ / mass_; // a = F/m
	addLocalVelocity(Eigen::Vector2d{ -dragAcceleration, 0 });
}

void Airplane::calculateLift()
{
	double localVelocityX = localVelocity().x();
	double velXSquared = localVelocityX * localVelocityX;
	double liftCoeff = liftAOAMultiplier_ * (sampleAOACurve(angleOfAttack_) + liftAOABias_);
	lift_ = 0.5 * velXSquared * liftCoeff * liftScale_ * copysign(1.0, localVelocityX);
}

void Airplane::applyLift()
{
	double liftAcceleration = lift_ / mass_; // a = F/m
	addLocalVelocity(Eigen::Vector2d{ 0, -liftAcceleration });
}

void Airplane::calculateAngularDrag()
{
	double angularVelocitySquared = worldAngularVelocity_ * worldAngularVelocity_;
	angularDrag_ = angularVelocitySquared * angularDragScale * copysign(1.0, worldAngularVelocity_);
}

void Airplane::applyAngularDrag()
{
	double dragAcceleration = angularDrag_ / mass_;
	worldAngularVelocity_ -= dragAcceleration;
}
