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
	setPolygons();
	
	for (auto& poly : polygons_) {
		window.draw(*poly);

		for (auto& triangle : poly->triangles()) {
			bool inView = inRenderView(*triangle);
			triangle->setVisibility(inView);
		}
	}	
}

bool Universe::inRenderView(APGeom::Triangle& triangle)
{
	const int leftBound = camera_->renderBoundX().first;
	const int rightBound = camera_->renderBoundX().second;

	const int upperBound = camera_->renderBoundY().first;
	const int lowerBound = camera_->renderBoundY().second;

	return triangle.inView(leftBound, rightBound, upperBound, lowerBound);
}

void Universe::setPolygons()
{
	polygons_.clear();

	for (auto& poly : terrainGenerator_->polygons()) {
		polygons_.emplace_back(std::make_shared<APGeom::Polygon>(poly));
	}

	polygons_.emplace_back(std::make_shared<APGeom::Polygon>(player_->shape()));
}
