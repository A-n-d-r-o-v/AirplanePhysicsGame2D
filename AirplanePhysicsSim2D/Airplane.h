#pragma once
#include "PhysicsObject.h"
#include "Eigen/Core"
#include <iostream>
#include <algorithm>

class Airplane : public PhysicsObject {
	
public:
	using PhysicsObject::PhysicsObject;

	void update(double dt) override;

	void applyInput();

	void thrust(double forwardForce); // Set and calculate forward acceleration calculated from forwardForce.

	void pitch(double angularForce);

	void calculateGForce(double dt); // Local acceleration

private:
	// TODO: read these values from JSON.
	/* ---- PARAMS ---- */
	double dragAOAMultiplier_ = 1.0; // AOA impact on drag
	double dragAOABias_ = 0.1; // Drag AOA bias
	double dragScale_ = 1.0; // Drag scale

	double liftAOAMultiplier_ = 0.4; // AOA impact on lift
	double liftAOABias_ = 3;
	double liftScale_ = 0;

	double thrustPower_ = 0.9; // Applied thrust force from input
	double pitchPower_ = 0.001; // Applied angular force from input
	/* ---------------- */

	double angleToGround_;
	double angleOfAttack_;

	double dragCoeff_;
	double drag_;

	double liftCoeff_;
	double lift_;

	bool upsideDown_;

	void calculateState(double dt);

	static double toGroundAngle(double theta); // Map angle to 0 -> 90 -> 0 -> -90 -> 0 (one revolution)
	void calculateAngleOfAttack();
	double sampleAOACurve(double angleOfAttack); // Retrieve value from AOA curve, clamped between -pi/2 and pi/2.

	bool isUpsideDown();

	void calculateDrag();
	void applyDrag();

	void calculateLift();
	void applyLift();

};

