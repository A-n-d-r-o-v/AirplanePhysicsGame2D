#pragma once

#include <Eigen/Core>
#include <memory>

#include "Polygon.h"

class TerrainGenerator;

class TunnelSegment
{
public:
	TunnelSegment(const std::pair<Eigen::Vector2d, Eigen::Vector2d>& innerEndPoints, bool isInner);
	TunnelSegment(Eigen::Vector2d innerStartPoint, Eigen::Vector2d innerEndPoint, bool isInner);

private:
	std::pair<Eigen::Vector2d, Eigen::Vector2d> innerEndPoints_;
	std::vector<Eigen::Vector2d> supportPoints_;

	std::shared_ptr<TerrainGenerator> generator_;

	std::vector<Eigen::Vector2d> shapePoints; // TODO: outer shape: scale bezier?

	double startThickness_, endThickness_;
	bool isInner_;

	std::vector<Eigen::Vector2d> generateInnerCurve();
	void transformToOuterCurve(); // Transform inner curve to outer curve, accounting for thickness
	
	[[nodiscard]]
	Polygon outerTunnelShape();  // Create a shape from tunnel perimeter --> bedrock
};

