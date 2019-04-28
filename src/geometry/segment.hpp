#pragma once

#include "point.hpp"
#include "shape.hpp"
#include "types.hpp"

namespace geometry {

class Segment
	: public Shape
{
public:
	Segment()
		: Shape()
		, m_owner(0)
	{}

	Segment(const Point& p1, const Point& p2)
		: Shape({ p1, p2 })
		, m_owner(0)
		, m_value((p2.y - p1.y) / (p2.x - p1.x))
	{
	}

	Segment(const std::pair<Point, Point>&& points)
		: Shape({points.first, points.second})
		, m_owner(0)
		, m_value((points.second.y - points.first.y) /
			  (points.second.x - points.first.x))
	{}

	bool operator <(const Segment& rhs) const
	{
		if (getPoint(0) != rhs.getPoint(0)) {
			return getPoint(0) < rhs.getPoint(0);
		}
		return getPoint(1) < rhs.getPoint(1);
	}
public:
	virtual ShapeType getType() const override
	{
		return ShapeType(SegmentType);
	}

	bool hasSameOwner(const Segment& other) const
	{
		return nullptr != m_owner && other.m_owner == m_owner;
	}

	void setOwner(const Shape* o)
	{
		m_owner = o;
	}

	int value() const
	{
		return m_value;
	}
private:
	const Shape* m_owner;
	int m_value;
};

}