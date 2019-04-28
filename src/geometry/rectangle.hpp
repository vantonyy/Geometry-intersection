#pragma once

#include "shape.hpp"
#include "polygon.hpp"

namespace geometry {

class Rect
	: public Polygon
{
public:
	Rect() = default;
	Rect(Rect&&) = default;
	Rect& operator=(Rect&&) = default;
	Rect& operator=(const Rect&) = default;

	Rect(const Points& points)
		: Polygon(points)
	{}

	Rect(Points&& points)
		: Polygon(points)
	{
		assert(points.size() == 4);
	}

	Rect(const std::initializer_list<Points::value_type>& pointList)
		: Polygon(pointList)
	{
		assert(pointList.size() == 4);
	}

	Rect(std::initializer_list<Points::value_type>&& pointList)
		: Polygon(pointList)
	{
		assert(pointList.size() == 4);
	}

	Point::Coord left() const
	{
		return getPoint(0).x;
	}

	Point::Coord right() const
	{
		return getPoint(2).x;
	}

	Point::Coord top() const
	{
		return getPoint(2).y;
	}	

	Point::Coord bottom() const
	{
		return getPoint(0).y;
	}

	void left(Point::Coord l)
	{
		getPoint(0).x = l;
		getPoint(1).x = l;
	}

	void right(Point::Coord r)
	{
		getPoint(2).x = r;
		getPoint(3).x = r;
	}

	void top(Point::Coord t)
	{
		getPoint(1).y = t;
		getPoint(2).y = t;
	}

	void bottom(Point::Coord b)
	{
		getPoint(0).y = b;
		getPoint(3).y = b;
	}
public:
	virtual ShapeType getType() const override
	{
		return ShapeType(RectType);
	}
};

}