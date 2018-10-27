#pragma once

#include "point.hpp"
#include "types.hpp"

namespace geometry {

class Segment
	: public Shape
{
public:
	Segment()
		: Shape()
	{}

	Segment(const Point& p1, const Point& p2)
		: Shape({ p1, p2 })
	{}

	Segment(const std::pair<Point, Point>&& points)
		: Shape({points.first, points.second})
	{}

public:
	virtual ShapeType getType() const override
	{
		return ShapeType(SegmentType);
	}
};

}