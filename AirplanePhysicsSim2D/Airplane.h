#pragma once
#include "PhysicsObject.h"
#include "Eigen/Core"
#include <iostream>

class Airplane : public PhysicsObject {
	
public:
	using PhysicsObject::PhysicsObject;

	void update(double dt) override;

	void calculateGForce(double dt); // Local acceleration

private:
	double dragAOAMultiplier_ = 1.0; // AOA impact on drag
	double dragAOABias_ = 0.5; // Drag AOA bias
	double dragScale_ = 2; // Drag scale

	double angleOfAttack_;

	double dragCoeff_;
	double drag_;


	void calculateState(double dt);
	void calculateAngleOfAttack(); // Map angle to 0 -> 90 -> 0 -> -90 -> 0 (one revolution)
	void calculateDrag();

};

