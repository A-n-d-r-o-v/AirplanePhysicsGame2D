#include "Airplane.h"

void Airplane::update(double dt)
{
	PhysicsObject::update(dt); // Call base class update
	
	angleToGround_ = toGroundAngle(angle_);
	calculateAngleOfAttack();
	double testAOA = sampleAOACurve(angleOfAttack_);
	std::cout << testAOA << std::endl;

	// Apply drag
	calculateDrag();
	applyDrag();
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

	localAcceleration_ = thrustVec;
}

// TODO implement angular drag
void Airplane::pitch(double angularForce)
{
	double angularAcceleration = angularForce / mass_;

	worldAngularAcceleration_ = angularAcceleration;
}

// TODO: AOA should calculate difference between angle (currently AOA) and velocity direction (from global velocity)
double Airplane::toGroundAngle(double theta)
{
	double groundAngle = fmod(theta, 2*EIGEN_PI); // later: calculate if plane is upside-down like this (pi > angle > 3pi or something)
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
	double planeDir = fmod(angle_, 2 * EIGEN_PI);
	double velocityDir = atan2(-worldVelocity_.y(), worldVelocity_.x());
	if (velocityDir < 0) { velocityDir += 2*EIGEN_PI; }
	velocityDir = fmod(velocityDir, 2 * EIGEN_PI);
	
	double aoa = toGroundAngle(planeDir - velocityDir);
	aoa = fmod(aoa, 2 * EIGEN_PI);

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

void Airplane::calculateDrag()
{
	// TODO: should also look at local y velocity
	double velXSquared = localVelocity_.x() * localVelocity_.x();
	dragCoeff_ = dragAOAMultiplier_ * (abs(angleOfAttack_) + dragAOABias_);
	drag_ = 0.5 * velXSquared * dragCoeff_ * dragScale_ * copysign(1.0, localVelocity_.x());

	//std::cout << "Drag acc: " << drag_ << ", vel x: " << localVelocity_.x() << std::endl;

}

void Airplane::applyDrag()
{
	double dragAcceleration = drag_ / mass_; // a = F/m
	localVelocity_.x() -= dragAcceleration;
}
