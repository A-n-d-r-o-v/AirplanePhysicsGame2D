#pragma once

#include <stdexcept>
#include <Eigen/Core>

namespace APGeom {
	
	template<typename T, template<typename> class PointContainer,
		template<typename> class CoordContainer>
	struct BoundingRect
	{
		BoundingRect(T left, T top, T right, T bottom);
		BoundingRect(const PointContainer<T>& topLeft, const PointContainer<T>& bottomRight);
		BoundingRect(const CoordContainer<T>& points);

		T xMin, yMin, xMax, yMax;
	};

	template<typename T>
	struct Point {
		Point(T x, T y);
		Point<T>(const Eigen::Vector2d& vec);

		T x, y;
	};
}
