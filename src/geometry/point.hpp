#pragma once

#include <iostream>

namespace geometry {

template <typename CoordType>
class Point
{
public:
	typedef CoordType Coord;
public:
	Point()
		: x(Coord())
		, y(Coord())
	{
	}

	Point(const Coord& x, const Coord& y)
		: x(x)
		, y(y)
	{}
public:
	bool operator< (const Point& rhs) const
	{
		if (x != rhs.x) {
			return x < rhs.x;
		}
		return y < rhs.y;
	}

	bool operator> (const Point& rhs) const
	{
		if (x != rhs.x) {
			return m_x > rhs.x;
		}
		return y > rhs.y;
	}

	bool operator<= (const Point& rhs) const
	{
		return *this < rhs || *this == rhs;
	}

	bool operator>= (const Point& rhs) const
	{
		return *this > rhs || *this == rhs;
	}

	bool operator== (const Point& rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}

	bool operator!= (const Point& rhs) const
	{
		return !(*this == rhs);
	}
public:
	Coord x;
	Coord y;
};

template <typename CoordType>
std::ostream& operator<<(std::ostream& out, const Point<CoordType>& p)
{
	out << "{" << p.x << ", " << p.y << "}";
	return out;
}

}