#pragma once

#include <Eigen/Core>
#include <memory>

#include "Polygon.h"
#include "Bezier.h"

class TunnelSegment
{
public:
	TunnelSegment(Eigen::Vector2f innerStartPoint, Eigen::Vector2f innerEndPoint, const std::pair<double, double>& entryExitAngles, bool isInner);

	double getStartThickness() const;
	double getEndThickness() const;
	bool isInner() const;

	std::vector<Eigen::Vector2d> shapePoints() const;

private:
	std::pair<Bezier::Vec2, Bezier::Vec2> innerEndPoints_;
	const std::pair<double, double>& entryExitAngles_;
	//std::vector<Eigen::Vector2d> supportPoints_;
	std::pair<Bezier::Vec2, Bezier::Vec2> supportPoints_; // Using a pair of support points for now

	Bezier::Bezier<3> cubicBezier_;
	std::vector<Eigen::Vector2d> shapePoints_; // TODO: outer shape: scale bezier?

	const int supportPointLength = 200;

	double startThickness_ = 50, endThickness_ = 50;
	bool isInner_;

	std::vector<Eigen::Vector2d> generateInnerCurve();
	void transformToOuterCurve(); // Transform inner curve to outer curve, accounting for thickness

	void calculateSupportPoints();

};

