#include "TunnelSegment.h"


TunnelSegment::TunnelSegment(const std::pair<Eigen::Vector2d, Eigen::Vector2d>& innerEndPoints, bool isInner)
	: innerEndPoints_(innerEndPoints), isInner_(isInner)
{
}

TunnelSegment::TunnelSegment(Eigen::Vector2d innerStartPoint, Eigen::Vector2d innerEndPoint, bool isInner) 
	: isInner_(isInner)
{
	innerEndPoints_ = std::make_pair(innerStartPoint, innerEndPoint);
}

std::vector<Eigen::Vector2d> TunnelSegment::generateInnerCurve()
{
	std::vector<Eigen::Vector2d> points;



	return points;
}