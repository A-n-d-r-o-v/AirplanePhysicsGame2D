#pragma once

#include <Eigen/Core>
#include <iostream>

#include "perlinnoise.hpp"
#include "Polygon.h"
#include "bezier.h"
#include "TunnelSegment.h"

enum class TerrainType {
	Mountains,	// Mountains with tunnels using 1D noise
	Clouds,		// Cloud-like structures from thresholded 2D noise
	Void		// Nothing but indicator dots
};

class TerrainGenerator
{
public:
	TerrainGenerator(const TerrainType terrainType);

	void generateMountainSegment(int startPoint, int endPoint, int hasTunnel);

	double terrainNoise(int x);
	
	std::vector<Polygon> polygons();

private:
	siv::PerlinNoise perlinNoiseGenerator_;
	TerrainType terrainType_;

	std::vector<Polygon> polygons_;

	/* Parameters, TODO load from JSON config file */
	const siv::PerlinNoise::seed_type seed_ = 123456u;
	const uint16_t heightOffset_ = 8000;

	const int bottomLevel = 30000; // Height of map bottom

	int spacing_ = 10; // Pixels per vertex in the local x direction

	const double amplitude_ = 13000.0;
	const double frequency_ = 0.00005;
	const int numOctaves_ = 10;
	/* ---------------- */

	std::vector<Eigen::Vector2d> tunnelBottomSegment(int startPoint, int endPoint, TunnelSegment& tunnel); // The part underneath the tunnel that extends to the bottom

	std::vector<Eigen::Vector2d> mountainTunnelSegment(int startPoint, int endPoint, TunnelSegment& tunnel); // Solid mountain with a tunnel underneath

	std::vector<Eigen::Vector2d> solidMountainSegment(int startPoint, int endPoint); // Solid mountain with no connected tunnel

};

