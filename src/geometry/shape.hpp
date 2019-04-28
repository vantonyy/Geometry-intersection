#pragma once

#include "types.hpp"

#include <vector>
#include <string>
#include <initializer_list>
#include <cassert>

namespace geometry {

class ShapeType
{
public:
	ShapeType(ShapeTypeEnum type)
		: m_type(type)
		, m_typeNeme(getName(type))
	{}

	operator ShapeTypeEnum() const
	{
		return m_type;
	}

	const std::string& getName() const
	{
		return m_typeNeme;
	}
private:
	static const std::string& getName(ShapeTypeEnum type)
	{
		static const std::string names[] = {"Polygon", "Rect", "Segment", "Unknown"};
		return isValid(type) ? names[type] : names[UnknownType];
	}

	static bool isValid(ShapeTypeEnum type)
	{
		return type <= UnknownType && type >= PolygonType;
	}
private:
	ShapeTypeEnum m_type;
	std::string m_typeNeme;
};

class ShapeId
{
	typedef unsigned IdType;
public:	
	ShapeId()
		: m_id(newID())
	{
	}

	operator IdType() const
	{
		return m_id;
	}
private:
	IdType newID()
	{
		static IdType id = 0;
		return id++;
	}
private:
	IdType m_id;
};

class Shape
{
public:
	typedef Point<double> Point;
	typedef std::vector<Point> Points;
	typedef std::shared_ptr<Shape> Ptr;
public:
	Shape() = default;

	Shape(const Points& points)
		: m_points(points)
	{
	}

	Shape(Points&& points)
		: m_points(points)
	{
	}

	Shape(const std::initializer_list<Points::value_type>& pointList)
		: m_points(pointList.begin(), pointList.end())
	{
	}

	Shape(std::initializer_list<Points::value_type>&& pointList)
		: m_points(std::make_move_iterator(pointList.begin()),
			   std::make_move_iterator(pointList.end()))
	{
	}

	virtual ShapeType getType() const = 0;

	void getPoints(Points& points) const
	{
		points = m_points;
	}

	const Points& getPoints() const
	{
		return m_points;
	}

	void setPoints(const Points& points)
	{
		m_points = points;
	}

	void setPoints(Points&& points)
	{
		m_points = points;
	}

	const Point& getPoint(unsigned index) const
	{
		assert(m_points.size() > index);
		return m_points[index];
	}

	Point& getPoint(unsigned index)
	{
		assert(m_points.size() > index);
		return m_points[index];
	}

	void setPoint(unsigned index, const Point& point)
	{
		assert(m_points.size() > index);
		m_points[index] = point;
	}

	void setPoint(unsigned index, Point&& point)
	{
		assert(m_points.size() > index);
		m_points[index] = point;
	}

	size_t getPointsNum() const
	{
		return m_points.size();
	}

	unsigned getId() const
	{
		return m_id;
	}

	void addPoint(const Point& point)
	{
		m_points.push_back(point);
	}
private:
	Points m_points;
	ShapeId m_id;
};

std::ostream& operator<<(std::ostream& out, const Shape& s)
{
	out << "============ Dump shape =============" << std::endl;
	out << "Type: " << s.getType().getName() << std::endl;
	out << "ID: " << s.getId() << std::endl;
	size_t pointsNum = s.getPointsNum();
	out << "Num of points: " << pointsNum << std::endl;
	out << "Points: {x, y}" << std::endl;
	out << "        ------" << std::endl;
	for (const Shape::Point& p : s.getPoints()) {
		out << "\t" << p << std::endl;
	}
	out << "=====================================" << std::endl;
	return out;
}

}