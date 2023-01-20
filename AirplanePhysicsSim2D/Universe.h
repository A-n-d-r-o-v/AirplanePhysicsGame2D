#pragma once

#include "Camera.h"
#include "TerrainGenerator.h"
#include "Airplane.h"

class Universe
{
public:
	Universe(TerrainGenerator& terrainGenerator, Camera& camera, Airplane& player);

	void updatePhysics(double dt);

	void renderStep(sf::RenderWindow& window);

	bool inRenderView(APGeom::Triangle& triangle);



private:

	TerrainGenerator* terrainGenerator_;
	Camera* camera_;
	Airplane* player_;

	std::vector<std::shared_ptr<APGeom::Polygon>> polygons_;
	//std::vector<Polygon> polygons_;

	const double timeScale_ = 500;

	void setPolygons();

};

