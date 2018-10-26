#pragma once

namespace core {

template <typename CoordType>
class Point
{
public:
	typedef CoordType Coord;
public:
	Point()
		: m_x(Coord())
		, m_y(Coord())
	{
	}

	Point(const Coord& x, const Coord& y)
		: m_x(x)
		, m_y(y)
	{}

	Coord& x()
	{
		return m_x;
	}

	const Coord& x() const
	{
		return m_x;
	}

	Coord& y()
	{
		return m_y;
	}

	const Coord& y() const
	{
		return m_y;
	}

	void x(const Coord& x)
	{
		m_x = x;
	}

	void y(const Coord& y)
	{
		m_y = y;
	}
public:
	bool operator< (const Point& rhs) const
	{
		if (m_x != rhs.m_x) {
			return m_x < rhs.m_x;
		}
		return m_y < rhs.m_y;
	}

	bool operator> (const Point& rhs) const
	{
		if (m_x != rhs.m_x) {
			return m_x > rhs.m_x;
		}
		return m_y > rhs.m_y;
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
		return m_x == rhs.m_x && m_y == rhs.m_y;
	}

	bool operator!= (const Point& rhs) const
	{
		return !(*this == rhs);
	}
private:
	Coord m_x;
	Coord m_y;
};

}