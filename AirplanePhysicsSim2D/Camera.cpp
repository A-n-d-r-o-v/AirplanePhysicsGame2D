#include "Camera.h"

Camera::Camera(sf::RenderWindow& window, const PhysicsObject& objectToFollow) 
	: objectToFollow_(objectToFollow), window_(window)
{
	view_ = sf::View(sf::FloatRect(0, 0, (float)window.getSize().x, (float)window.getSize().y));
	view_.zoom((float)zoomLevel_);

	calculateRenderBounds();
}

void Camera::updateView()
{
	//Eigen::Vector2d lookAheadVec = PhysicsObject::toWorldSpace(lookAhead_, objectToFollow_.angle());
	Eigen::Vector2d viewCenter = objectToFollow_.globalPosition() + zoomLevel_ * (lookAheadVec() + offset_);
	view_.setCenter((float)(viewCenter.x()), (float)(viewCenter.y()));

	calculateZoomLevel();
	view_.zoom((float)zoomLevel_);

	calculateRenderBounds();

	window_.setView(view_);
}

std::pair<int, int> Camera::renderBoundX() const
{
	return renderBoundX_;
}

std::pair<int, int> Camera::renderBoundY() const
{
	return renderBoundY_;
}

Eigen::Vector2d Camera::lookAheadVec()
{
	const Eigen::Vector2d halfViewSize{ view_.getSize().x / 2, view_.getSize().y / 2 };
	double angVelocityAngle = objectToFollow_.angularVelocity() * angularLookAhead_;
	angVelocityAngle = std::max(angVelocityAngle, maxAngularLookAhead_);
	angVelocityAngle = std::min(angVelocityAngle, -maxAngularLookAhead_);

	double angle = -objectToFollow_.angle() + angVelocityAngle;
	angle = Util::wrapAngle(angle);

	Eigen::Vector2d lookAheadVec{ 
		halfViewSize.x() * lookAhead_.x() * cos(angle) ,
		halfViewSize.y() * lookAhead_.y() * sin(angle)
	};
	return lookAheadVec;
}

void Camera::calculateRenderBounds()
{
	const float width = view_.getSize().x;
	const float height = view_.getSize().y;

	renderBoundX_.first = (int) (view_.getCenter().x - (width / 2.0f) - renderBoundMargin_);
	renderBoundX_.second = (int) (view_.getCenter().x + (width / 2.0f) + renderBoundMargin_);
	
	renderBoundY_.first = (int) (view_.getCenter().y - (height / 2.0f) - renderBoundMargin_);
	renderBoundY_.second = (int) (view_.getCenter().y + (height / 2.0f) + renderBoundMargin_);
}

void Camera::calculateZoomLevel()
{
	double zoomRangeInterval = zoomRange_.second - zoomRange_.first;
	double scaledVelocity = objectToFollow_.velocityNorm() / objectToFollow_.maxVelocityNorm(); // Scale to range [0,1]
	if (objectToFollow_.maxVelocityNorm() == 0) {
		scaledVelocity = 0.0;
	}

	double newZoom = (zoomRange_.first + (zoomRangeInterval * scaledVelocity));
	zoomLevel_ = newZoom / (prevZoomLevel_);

	prevZoomLevel_ = newZoom;
}

