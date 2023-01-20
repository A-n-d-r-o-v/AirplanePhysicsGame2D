#include "GeomUtil.h"

/* Bounding rect */

template <typename T, 
	template<typename> class PointContainer, 
	template<typename> class CoordContainer>
inline APGeom::BoundingRect<T, PointContainer, CoordContainer>::BoundingRect(T left, T top, T right, T bottom)
	: xMin(left), yMin(top), xMax(right), yMax(bottom)
{ }

template <typename T, 
	template<typename> class PointContainer, 
	template<typename> class CoordContainer>
inline APGeom::BoundingRect<T, PointContainer, CoordContainer>::BoundingRect(
	const PointContainer<T>& topLeft, const PointContainer<T>& bottomRight)
	: xMin(topLeft.first), yMin(topLeft.second), xMax(bottomRight.first), yMax(bottomRight.second)
{ }

template <typename T, 
	template<typename> class PointContainer, 
	template<typename> class CoordContainer>
inline APGeom::BoundingRect<T, PointContainer, CoordContainer>::BoundingRect(
	const CoordContainer<T>& points)
{ 
	if (points.size() != 4) {
		throw std::invalid_argument("BoundingRect constructor requires exactly 4 points in the container.");
	}

	xMin = points[0];
	yMin = points[1];
	xMax = points[2];
	yMax = points[3];
}

/* Point */

template<typename T>
inline APGeom::Point<T>::Point(T x, T y)
	: x(this.x), y(this.y)
{ }

template<typename T>
inline APGeom::Point<T>::Point<T>(const Eigen::Vector2d& vec)
	: x(vec.x()), y(vec.y())
{ }