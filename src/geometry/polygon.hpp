#pragma once

#include "shape.hpp"

namespace geometry {

class Polygon
	: public Shape
{
public:
	Polygon() = default;
	Polygon(Polygon&&) = default;
	Polygon& operator=(Polygon&&) = default;
	Polygon& operator=(const Polygon&) = default;

	Polygon(const Points& points)
		: Shape(points)
	{}

	Polygon(Points&& points)
		: Shape(points)
	{}

	Polygon(const std::initializer_list<Points::value_type>& pointList)
		: Shape(pointList)
	{
	}

	Polygon(std::initializer_list<Points::value_type>&& pointList)
		: Shape(pointList)
	{
	}
public:
	virtual ShapeType getType() const override
	{
		return ShapeType(PolygonType);
	}
};

}