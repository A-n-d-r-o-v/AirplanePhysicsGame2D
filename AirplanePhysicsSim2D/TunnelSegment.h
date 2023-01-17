#pragma once

#include <Eigen/Core>
#include <memory>
#include <optional>

#include "Polygon.h"
#include "Bezier.h"

class TunnelSegment
{
public:
	TunnelSegment(Eigen::Vector2f innerStartPoint, Eigen::Vector2f innerEndPoint, const std::pair<double, double>& entryExitAngles);

	std::vector<Eigen::Vector2d> shapePoints() const;
	std::vector<Eigen::Vector2d> toOuterShapePoints(Bezier::Bezier<3> fromBezier);
	void setTunnelThickness(std::pair<uint16_t, uint16_t>& tunnelThickness);

private:
	std::pair<Bezier::Vec2, Bezier::Vec2> innerEndPoints_;
	const std::pair<double, double>& entryExitAngles_;
	//std::vector<Eigen::Vector2d> supportPoints_;
	std::pair<Bezier::Vec2, Bezier::Vec2> supportPoints_; // Using a pair of support points for now
	std::vector<Bezier::Vec2> bezierPoints_; // Points that define the bezier curve, consisting of end points and control points

	Bezier::Bezier<3> cubicBezier_;
	std::vector<Eigen::Vector2d> shapePoints_; // TODO: outer shape: scale bezier?

	const std::pair<uint16_t, uint16_t>& tunnelThickness_{0,0};

	const int spacing_ = 40; // Pixels per vertex in the local x direction

	const int supportPointLength = 200;

	[[nodiscard]]
	std::vector<Eigen::Vector2d> generateCurve();

	Bezier::Point& recalculateSupportPoint(uint16_t pointIndex, const Bezier::Bezier<3>& fromBezier);

	[[nodiscard]]
	std::vector<float> controlPointLengthRatios(); // Ratio from (0->1) that describes how far along the control points are on the curve 
	
	void calculateSupportPoints(std::optional<Bezier::Bezier<3>> fromBezier = std::nullopt);

};

