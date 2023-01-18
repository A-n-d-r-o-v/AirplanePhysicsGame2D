#include "TunnelSegment.h"

TunnelSegment::TunnelSegment(Eigen::Vector2f innerStartPoint, Eigen::Vector2f innerEndPoint, const std::pair<double, double>& entryExitAngles)
	: entryExitAngles_(entryExitAngles)
{
	// This is cursed
	innerEndPoints_ = std::make_pair(
		Bezier::Vec2{ innerStartPoint.x(), innerStartPoint.y() }, 
		Bezier::Vec2{ innerEndPoint.x(), innerEndPoint.y() }
	);

	calculateSupportPoints();
	setBezierPoints();
	setBezierCurve(bezierPoints_);
	shapePoints_ = generateCurve();
}

std::vector<Eigen::Vector2d> TunnelSegment::shapePoints() const
{
	return shapePoints_;
}

void TunnelSegment::toOuterShapePoints(const std::pair<uint16_t, uint16_t>& tunnelThickness, Bezier::Bezier<3> fromBezier)
{
	calculateSupportPoints(tunnelThickness, fromBezier);
	setBezierPoints();
	setBezierCurve(bezierPoints_);
	shapePoints_ = generateCurve();
}

Bezier::Bezier<3> TunnelSegment::cubicBezier()
{
	return cubicBezier_;
}

std::vector<Eigen::Vector2d> TunnelSegment::generateCurve()
{
	std::vector<Eigen::Vector2d> points;

	float numPoints = (innerEndPoints_.second.x - innerEndPoints_.first.x) / spacing_;

	// Sample points from bezier curve (0 -> 1)
	for (float i = 0; i <= 1; i += 1.0 / (numPoints - 1)) {
		Bezier::Point bezierPoint = cubicBezier_.valueAt(i);
		float flatX = innerEndPoints_.first.x + (i * (innerEndPoints_.second.x - innerEndPoints_.first.x)); // Scale i instead of taking bezierPoint.x
		points.emplace_back(flatX, bezierPoint.y);
	}
	return points;
}

Bezier::Point TunnelSegment::recalculateSupportPoint(uint16_t pointIndex, const std::pair<uint16_t, uint16_t>& tunnelThickness, const Bezier::Bezier<3>& fromBezier)
{
	Bezier::Point point = bezierPoints_[pointIndex];

	float lengthRatio = controlPointLengthRatios()[pointIndex];
	float newLength = (tunnelThickness.first * lengthRatio) + (tunnelThickness.second * (1 - lengthRatio));

	Bezier::Vec2 scaledNormal = fromBezier.normalAt(lengthRatio) * (newLength * widthScale_);

	point.x += scaledNormal.x;
	point.y += scaledNormal.y;

	return point;
}

std::vector<float> TunnelSegment::controlPointLengthRatios()
{
	float totalLen = 0;

	std::vector<float> lengthVec{0};
	for (size_t i = 1; i < std::size(bezierPoints_); ++i) {
		float len = (bezierPoints_[i] - bezierPoints_[i - 1]).length();		
		totalLen += len;
		lengthVec.emplace_back(totalLen);
	}
	for (auto& len : lengthVec) {
		len /= totalLen;
	}
	return lengthVec;
}

void TunnelSegment::calculateSupportPoints(const std::pair<uint16_t, uint16_t>& tunnelThickness, std::optional<Bezier::Bezier<3>> fromBezier)
{
	// Temp solution
	supportPoints_.first = { Bezier::Point(innerEndPoints_.first.x + 500, innerEndPoints_.first.y + 2000) };
	supportPoints_.second = { Bezier::Point(innerEndPoints_.second.x - 500, innerEndPoints_.second.y + 2000) };

	if (fromBezier.has_value()) {
		Bezier::Bezier<3>& bezier = fromBezier.value();

		// Re-calculate support points
		supportPoints_.first = recalculateSupportPoint(1, tunnelThickness, bezier);
		supportPoints_.second = recalculateSupportPoint(2, tunnelThickness, bezier);
	}
}

void TunnelSegment::setBezierPoints()
{
	bezierPoints_.clear();
	bezierPoints_.reserve(4);
	bezierPoints_.emplace_back(innerEndPoints_.first);
	bezierPoints_.emplace_back(supportPoints_.first);
	bezierPoints_.emplace_back(supportPoints_.second);
	bezierPoints_.emplace_back(innerEndPoints_.second);
}

void TunnelSegment::setBezierCurve(std::vector<Bezier::Vec2> points)
{
	cubicBezier_ = Bezier::Bezier<3>(points);
}
