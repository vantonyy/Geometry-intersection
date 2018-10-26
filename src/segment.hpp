#pragma once

#include "point.hpp"
#include "types.hpp"

namespace core {

class Segment
	: public Shape
{
public:
	Segment()
		: Shape()
	{}

	Segment(const PointType& p1, const PointType& p2)
		: Shape({ p1, p2 })
	{}

	Segment(const std::pair<PointType, PointType>&& points)
		: Shape({points.first, points.second})
	{}

public:
	virtual ShapeType getType() const override
	{
		return ShapeType(SegmentType);
	}
};

}