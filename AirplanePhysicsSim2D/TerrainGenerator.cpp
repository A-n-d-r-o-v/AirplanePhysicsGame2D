#include "TerrainGenerator.h"

TerrainGenerator::TerrainGenerator(const TerrainType terrainType)
	: terrainType_(terrainType)
{
	perlinNoiseGenerator_ = siv::PerlinNoise{ seed_ };
}

void TerrainGenerator::generateMountainSegment(int startPoint, int endPoint, int hasTunnel)
{	
	if (hasTunnel) {
		std::cout << "Generating tunnel top segment..." << std::endl;
		std::pair<Polygon, TunnelSegment> topSegment = mountainTunnelSegment(startPoint, endPoint);

		polygons_.emplace_back(topSegment.first);

		std::cout << "Generating tunnel bottom segment..." << std::endl;
		TunnelSegment topTunnel = topSegment.second;
		std::pair<Polygon, TunnelSegment> bottomSegment = tunnelBottomSegment(startPoint, endPoint, topTunnel);
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

std::vector<Polygon> TerrainGenerator::polygons()
{
	return polygons_;
}

std::pair<Polygon, TunnelSegment> TerrainGenerator::mountainTunnelSegment(int startPoint, int endPoint)
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

	return std::make_pair(Polygon{ points }, tunnel);
}

std::pair<Polygon, TunnelSegment> TerrainGenerator::tunnelBottomSegment(int startPoint, int endPoint, std::optional<TunnelSegment> innerTunnelOpt)
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

	return std::make_pair(Polygon{ points }, tunnel);
}

Polygon TerrainGenerator::solidMountainSegment(int startPoint, int endPoint)
{
	std::vector<Eigen::Vector2d> points;
	
	for (int i = startPoint; i <= endPoint; i += spacing_) {

		double height = terrainNoise(i);
		points.emplace_back(Eigen::Vector2d{ i, (int)height });
	}
	points.emplace_back(endPoint, bottomLevel_);
	points.emplace_back(startPoint, bottomLevel_);
	
	return Polygon{ points };
}

TunnelSegment TerrainGenerator::generateTunnel(int startPoint, int endPoint)
{
	Eigen::Vector2f tunnelStart{ startPoint, terrainNoise(startPoint) };
	Eigen::Vector2f tunnelEnd{ endPoint, terrainNoise(endPoint) };
	std::pair<double, double> startEndAngles = std::make_pair(0, 0); // TODO
	
	return TunnelSegment(tunnelStart, tunnelEnd, startEndAngles);	
}
