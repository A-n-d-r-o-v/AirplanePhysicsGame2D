#include "TerrainGenerator.h"

TerrainGenerator::TerrainGenerator(const TerrainType terrainType)
	: terrainType_(terrainType)
{
	perlinNoiseGenerator_ = siv::PerlinNoise{ seed_ };
}

void TerrainGenerator::generateMountainSegment(int startPoint, int endPoint, int hasTunnel)
{
	std::cout << "Generating mountain segment..." << std::endl;
	polygons_.emplace_back(solidMountainSegment(startPoint, endPoint));
}

double TerrainGenerator::terrainNoise(int x)
{
	return amplitude_ * perlinNoiseGenerator_.octave1D(x * frequency_, numOctaves_);
}

std::vector<Polygon> TerrainGenerator::polygons()
{
	return polygons_;
}

std::vector<Eigen::Vector2d> TerrainGenerator::solidMountainSegment(int startPoint, int endPoint)
{
	std::vector<Eigen::Vector2d> points;
	
	for (int i = startPoint; i <= endPoint; i += spacing_) {

		double height = -terrainNoise(i) + heightOffset_;
		points.emplace_back(Eigen::Vector2d{ i, height });
	}
	points.emplace_back(endPoint, bottomLevel);
	points.emplace_back(startPoint, bottomLevel);
	
	return points;
}
