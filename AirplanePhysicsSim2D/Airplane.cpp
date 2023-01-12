#include "Airplane.h"

void Airplane::update(double dt)
{
	PhysicsObject::update(dt); // Call base class update
	calculateAngleOfAttack();

	// Apply drag
	calculateDrag();
	double dragAcceleration = drag_ / mass_; // a = F/m
	localVelocity_.x() -= dragAcceleration;
}

// TODO: Calculate difference between current AOA and direction
void Airplane::calculateAngleOfAttack()
{
	angleOfAttack_ = fmod(angle_, 2*EIGEN_PI);
	short sign = 1;

	// Bottom half: invert sign
	if (angleOfAttack_ > EIGEN_PI) {
		angleOfAttack_ -= EIGEN_PI;
		sign = -1;
	}
	// Mirror values that are >pi rads
	if (angleOfAttack_ > EIGEN_PI / 2 && angleOfAttack_ <= EIGEN_PI) {
		angleOfAttack_ = EIGEN_PI - angleOfAttack_;
	}
	angleOfAttack_ *= sign;
}

void Airplane::calculateDrag()
{
	double velXSquared = localVelocity_.x() * localVelocity_.x();
	dragCoeff_ = dragAOAMultiplier_ * (abs(angleOfAttack_) + dragAOABias_);
	drag_ = 0.5 * velXSquared * dragCoeff_ * dragScale_ * copysign(1.0, localVelocity_.x());

	std::cout << "Drag acc: " << drag_ << ", vel x: " << localVelocity_.x() << std::endl;

}
