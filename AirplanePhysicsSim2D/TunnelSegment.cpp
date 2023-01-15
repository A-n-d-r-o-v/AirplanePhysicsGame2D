#include "TunnelSegment.h"


TunnelSegment::TunnelSegment(Eigen::Vector2f innerStartPoint, Eigen::Vector2f innerEndPoint, const std::pair<double, double>& entryExitAngles, bool isInner)
	: isInner_(isInner), entryExitAngles_(entryExitAngles)
{
	innerEndPoints_ = std::make_pair(
		Bezier::Vec2{ innerStartPoint.x(), innerStartPoint.y() }, 
		Bezier::Vec2{ innerStartPoint.x(), innerStartPoint.y() }
	);

	calculateSupportPoints();
	std::vector<Bezier::Vec2> bezierPoints;
	bezierPoints.emplace_back(innerEndPoints_.first);
	bezierPoints.emplace_back(supportPoints_.first);
	bezierPoints.emplace_back(supportPoints_.second);
	bezierPoints.emplace_back(innerEndPoints_.second);
	
	cubicBezier_ = Bezier::Bezier<3>(bezierPoints);
}

double TunnelSegment::getStartThickness() const
{
	return startThickness_;
}

double TunnelSegment::getEndThickness() const
{
	return endThickness_;
}

bool TunnelSegment::isInner() const
{
	return isInner_;
}

std::vector<Eigen::Vector2d> TunnelSegment::shapePoints() const
{
	return shapePoints_;
}

std::vector<Eigen::Vector2d> TunnelSegment::generateInnerCurve()
{
	std::vector<Eigen::Vector2d> points;

	// Sample points from bezier curve (0 -> 1)

	return points;
}

void TunnelSegment::calculateSupportPoints()
{
	// Calculate positions from end points, angle, length

	supportPoints_.first = { 0, 0 };
	supportPoints_.second = { 0, 0 };
}
