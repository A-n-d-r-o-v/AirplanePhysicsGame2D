#include "Universe.h"

Universe::Universe(TerrainGenerator& terrainGenerator, Camera& camera, Airplane& player)
	: terrainGenerator_(&terrainGenerator), camera_(&camera), player_(&player)
{
	setPolygons();
}

void Universe::updatePhysics(double dt)
{
	player_->applyInput();
	player_->update(dt * timeScale_);

	camera_->updateView();
}

void Universe::renderStep(sf::RenderWindow& window)
{
	//int numVisible = 0;
	//int numInvisible = 0;

	setPolygons();
	
	for (auto& poly : polygons_) {
		window.draw(*poly);

		for (auto& triangle : poly->triangles()) {
			if (inRenderView(*triangle)) {
				triangle->setVisibility(true);
				//++numVisible;
			}
			else {
				triangle->setVisibility(false);
				//++numInvisible;
			}
		}
	}
	
	//std::cout << "Visible: " << numVisible << ", invisible: " << numInvisible << std::endl;
}

bool Universe::inRenderView(Triangle& triangle)
{
	int leftBound = camera_->renderBoundX().first;
	int rightBound = camera_->renderBoundX().second;

	int upperBound = camera_->renderBoundY().first;
	int lowerBound = camera_->renderBoundY().second;

	return triangle.inView(leftBound, rightBound, upperBound, lowerBound);
}

void Universe::setPolygons()
{
	polygons_.clear();

	for (auto& poly : terrainGenerator_->polygons()) {
		polygons_.emplace_back(std::make_shared<Polygon>(poly));
	}

	polygons_.emplace_back(std::make_shared<Polygon>(player_->shape()));
}
