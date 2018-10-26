#pragma once

#include "types.hpp"

#include <vector>
#include <string>
#include <initializer_list>

namespace core {

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
		static const std::string names[] = {"Polygon", "Segment", "Unknown"};
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
	typedef DPoint PointType;
	typedef std::vector<PointType> Points;
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

	const PointType& getPoint(unsigned index) const
	{
		return m_points.at(index);
	}

	void setPoint(unsigned index, const PointType& point)
	{
		m_points.at(index) = point;
	}

	void setPoint(unsigned index, PointType&& point)
	{
		m_points.at(index) = point;
	}

	size_t getPointsNum() const
	{
		return m_points.size();
	}

	unsigned getId() const
	{
		return m_id;
	}	
private:
	Points m_points;
	ShapeId m_id;
};

}
