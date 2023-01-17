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
	bezierPoints_.reserve(4);
	bezierPoints_.emplace_back(innerEndPoints_.first);
	bezierPoints_.emplace_back(supportPoints_.first);
	bezierPoints_.emplace_back(supportPoints_.second);
	bezierPoints_.emplace_back(innerEndPoints_.second);
	
	cubicBezier_ = Bezier::Bezier<3>(bezierPoints_);
	shapePoints_ = generateCurve();
}

std::vector<Eigen::Vector2d> TunnelSegment::shapePoints() const
{
	return shapePoints_;
}

std::vector<Eigen::Vector2d> TunnelSegment::toOuterShapePoints(Bezier::Bezier<3> fromBezier)
{
	return std::vector<Eigen::Vector2d>();
}

std::vector<Eigen::Vector2d> TunnelSegment::generateCurve()
{
	std::vector<Eigen::Vector2d> points;

	int numPoints = (innerEndPoints_.second.x - innerEndPoints_.first.x) / spacing_;
	std::cout << "points: " << numPoints << std::endl;

	for (float i = 0; i <= 1; i += (1.0 / numPoints)) {
		Bezier::Point bezierPoint = cubicBezier_.valueAt(i);
		points.emplace_back(bezierPoint.x, bezierPoint.y);
	}
	// Sample points from bezier curve (0 -> 1)

	return points;
}



std::vector<float> TunnelSegment::controlPointLengthRatios()
{
	float totalLen = 0;

	std::vector<float> lengthVec{0};
	for (size_t i = 1; i < std::size(bezierPoints_) - 1; ++i) {
		float len = (bezierPoints_[i] - bezierPoints_[i - 1]).length();		
		totalLen += len;
		lengthVec.emplace_back(totalLen);
	}
	for (auto& len : lengthVec) {
		len /= totalLen;
	}
	return lengthVec;
}

void TunnelSegment::calculateSupportPoints(std::optional<Bezier::Bezier<3>> fromBezier)
{
	// Temp solution
	supportPoints_.first = { Bezier::Point(innerEndPoints_.first.x + 100, innerEndPoints_.first.y + 5000) };
	supportPoints_.second = { Bezier::Point(innerEndPoints_.second.x - 100, innerEndPoints_.second.y + 5000) };

	if (fromBezier.has_value()) {
		Bezier::Bezier<3>& bezier = fromBezier.value();

		// Re-calculate support points
		std::vector lengthRatioVec = controlPointLengthRatios();

		// First and last indices correspond to endPoints, we're looking for the support points
		// TODO. Fix this ugly mess when implementing std::vector for support points
		std::pair<Bezier::Point, Bezier::Point> normalsAtSupportPoints = std::make_pair(
			bezier.normalAt(lengthRatioVec[1]),
			bezier.normalAt(lengthRatioVec[2])
		);
	
		//float controlCurveLen = 

		//bezier.norm

	}
}
