#include "TerrainGenerator.h"

TerrainGenerator::TerrainGenerator(const TerrainType terrainType)
	: terrainType_(terrainType)
{
	perlinNoiseGenerator_ = siv::PerlinNoise{ seed_ };
}

void TerrainGenerator::generateMountainSegment(int startPoint, int endPoint, int hasTunnel)
{	
	if (hasTunnel) {
		std::cout << "Generating tunnel segment..." << std::endl;
		std::pair<APGeom::Polygon, TunnelSegment> topSegment = mountainTunnelSegment(startPoint, endPoint);

		if (topSegment.first.triangles().empty()) {
			std::cout << "Could not generate segment with tunnel. Attempting to generate segment without tunnel..." << std::endl;
			return generateMountainSegment(startPoint, endPoint, false);
		}
		polygons_.emplace_back(topSegment.first);

		TunnelSegment topTunnel = topSegment.second;
		std::pair<APGeom::Polygon, TunnelSegment> bottomSegment = tunnelBottomSegment(startPoint, endPoint, topTunnel);
		polygons_.emplace_back(bottomSegment.first);
	}
	else {
		std::cout << "Generating mountain segment..." << std::endl;
		polygons_.emplace_back(solidMountainSegment(startPoint, endPoint));
	}
}

double TerrainGenerator::terrainNoise(int x)
{
	return -(amplitude_ * perlinNoiseGenerator_.octave1D(x * frequency_, numOctaves_)) + heightOffset_;
}

std::vector<APGeom::Polygon> TerrainGenerator::polygons()
{
	return polygons_;
}

std::pair<APGeom::Polygon, TunnelSegment> TerrainGenerator::mountainTunnelSegment(int startPoint, int endPoint)
{
	std::vector<Eigen::Vector2d> points;
	
	const int start = startPoint + tunnelThickness_.first;
	const int end = endPoint - tunnelThickness_.second;
	TunnelSegment tunnel = generateTunnel(start, end);

	// Mountain points
	for (int i = start; i <= end; i += spacing_) {

		double height = terrainNoise(i);
		points.emplace_back(Eigen::Vector2d{ i, (int)height });
	}
	// Traverse tunnel points in reverse order to create a loop
	std::vector shapePoints = tunnel.shapePoints();
	for (auto it = shapePoints.rbegin(); it != shapePoints.rend(); ++it) {
		points.emplace_back(*it);
	}

	return std::make_pair(APGeom::Polygon{ points }, tunnel);
}

std::pair<APGeom::Polygon, TunnelSegment> TerrainGenerator::tunnelBottomSegment(int startPoint, int endPoint, const std::optional<TunnelSegment>& innerTunnelOpt)
{
	std::vector<Eigen::Vector2d> points;

	TunnelSegment tunnel = generateTunnel(startPoint, endPoint);
	
	if (innerTunnelOpt.has_value()) {
		// Transform tunnel to outer tunnel
		tunnel.toOuterShapePoints(tunnelThickness_, innerTunnelOpt.value().cubicBezier());
	}

	// Traverse tunnel points
	std::vector shapePoints = tunnel.shapePoints();
	for (auto it = shapePoints.begin(); it != shapePoints.end(); ++it) {
		points.emplace_back(*it);
	}
	points.emplace_back(endPoint, bottomLevel_);
	points.emplace_back(startPoint, bottomLevel_);

	return std::make_pair(APGeom::Polygon{ points }, tunnel);
}

APGeom::Polygon TerrainGenerator::solidMountainSegment(int startPoint, int endPoint)
{
	std::vector<Eigen::Vector2d> points;
	
	for (int i = startPoint; i <= endPoint; i += spacing_) {

		double height = terrainNoise(i);
		points.emplace_back(Eigen::Vector2d{ i, (int)height });
	}
	points.emplace_back(endPoint, bottomLevel_);
	points.emplace_back(startPoint, bottomLevel_);
	
	return APGeom::Polygon{ points };
}

TunnelSegment TerrainGenerator::generateTunnel(int startPoint, int endPoint)
{
	Eigen::Vector2f tunnelStart{ startPoint, terrainNoise(startPoint) };
	Eigen::Vector2f tunnelEnd{ endPoint, terrainNoise(endPoint) };
	std::pair<double, double> startEndAngles = std::make_pair(0, 0); // TODO
	
	return TunnelSegment(tunnelStart, tunnelEnd, startEndAngles);	
}
