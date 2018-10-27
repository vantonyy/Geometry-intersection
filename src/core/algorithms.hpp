#pragma once

#include "shape.hpp"
#include "polygon.hpp"
#include "segment.hpp"
#include "types.hpp"

#include <iostream>
#include <algorithm>

//https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/

namespace core {

/* 
Compute orientation:

Cases:
       Colinear          Clockwise        Counter Clockwise
        + P3		   + P3			+ P3
       /		  /		         \
      /			 /                        \  
     + P2		+ P2                       + P2
    /			|			   | 
   /			|			   |
  + P1         		+ P1                       + P1


  case1: tan(alpha1) - tan(alpha2) = 0
  case2: tan(alpha1) - tan(alpha2) > 0
  case3: tan(alpha1) - tan(alpha2) < 0

tan(alpha) = edge1 / edge2

  Y2 - Y1       Y3 - Y2       (X3 - X2) * (Y2 - Y1) - (Y3 - Y2) * (X2 - X1)
  -------   -   -------   =   ---------------------------------------------
  X2 - X1       X3 - X2                   (X2 - X1) * (X3 - X2) // this can be ignored

Example of clockwise:

	 P3 + 
     	   /|
      	  / |
  alpha2 /\ |
     P2 + --+
 	|   |
 alpha1	|\  |
     P1 +---+

     tan(alpha1) - tan(alpha2) > 0
*/

template <typename Point>
Orient orientation(const Point& p1, const Point& p2, const Point& p3)
{
	typename Point::Coord o = (p3.x - p2.x) * (p2.y - p1.y) -
		(p3.y - p2.y) * (p2.x - p1.x);
	static constexpr typename Point::Coord zero(0);
	return zero == o ? Colinear : o > zero ? Clockwise : CounterClockwise;
}

template <typename Geometry>
typename Geometry::PointType::Coord area(const Geometry&);

template <>
Polygon::PointType::Coord area(const Polygon& p)
{
	size_t pointsNum = p.getPointsNum();
	Polygon::PointType::Coord area = 0;
	for (size_t i = 0, j = pointsNum - 1; i != pointsNum; ++i, j = i) {
		const Polygon::PointType& p1 = p.getPoint(i);
		const Polygon::PointType& p2 = p.getPoint(j);
		area += (p2.x + p2.x) * (p2.y - p1.y);
	}
	return area / Polygon::PointType::Coord(2);
}

Orient orientation(const Polygon& p)
{
	return 0 > area(p) ? Clockwise : CounterClockwise;
}

bool isManhattan(const Shape& s)
{
	size_t pointsNum = s.getPointsNum();
	for (size_t i = 0, j = pointsNum - 1; i != pointsNum; ++i, j = i) {
		const Shape::PointType& p = s.getPoint(i);
		const Shape::PointType& q = s.getPoint(j);
		if (p.x != q.x && p.y != q.y) {
			return false;
		}
	}
	return true;
}


namespace nopt {

template <typename ValueType>
bool inRange(const ValueType& a, const ValueType& b, const ValueType& v)
{
	return (a >= v && b <= v) || (a >= v && b <= v);
}

template <typename Point>
bool onSegment(const Segment& s, const Point& p1)
{
	const Segment::PointType& p = s.getPoint(0);
	const Segment::PointType& q = s.getPoint(1);
	return inRange(p.x, q.x, p1.x) && inRange(p.y, q.y, p1.y);
}

bool isIntersected(const Segment& s1, const Segment& s2)
{
	const Segment::PointType& p1 = s1.getPoint(0);
	const Segment::PointType& q1 = s1.getPoint(1);
	const Segment::PointType& p2 = s2.getPoint(0);
	const Segment::PointType& q2 = s2.getPoint(1);
	Orient o1 = orientation(p1, q1, p2);
	Orient o2 = orientation(p1, q1, q2);
	Orient o3 = orientation(p2, q2, p1);
	Orient o4 = orientation(p2, q2, q1);
	return (o1 != o2 && o3 != o4) ||
	       (Colinear == o1 && onSegment(s1, p2)) ||
	       (Colinear == o2 && onSegment(s1, q2)) ||
	       (Colinear == o3 && onSegment(s2, p1)) ||
	       (Colinear == o4 && onSegment(s2, q1));
}

//@Complexity O(Geometry::edges * Shape::edges)
template <typename Geometry>
bool isIntersected(const Geometry& g, const Shape& sh)
{
	size_t pointsNum = sh.getPointsNum();
	for (size_t i = 0, j = pointsNum - 1; i != pointsNum; ++i, j = i) {
		const Shape::PointType& q1 = sh.getPoint(i);
		const Shape::PointType& q2 = sh.getPoint(j);
		if (isIntersected(Segment{q1, q2}, g)) {
			return true;
		}
	}
	return false;
}

}

namespace opt {

#define inRange(m, M, v) \
		(m <= v && v <= M) || (M <= v && v <= m)

template <typename Point>
bool onSegment(const Point& p, const Point& q, const Point& p1)
{
	return inRange(p.x, q.x, p1.x) && inRange(p.y, q.y, p1.y);
}

bool isIntersected(const DPoint& p1, const DPoint& q1, 
		   const DPoint& p2, const DPoint& q2)
{
	Orient o1 = orientation(p1, q1, p2);
	Orient o2 = orientation(p1, q1, q2);
	Orient o3 = orientation(p2, q2, p1);
	Orient o4 = orientation(p2, q2, q1);
	return (o1 != o2 && o3 != o4) ||
		(Colinear == o1 && onSegment(p1, q1, p2)) ||
		(Colinear == o2 && onSegment(p1, q1, q2)) ||
		(Colinear == o3 && onSegment(p2, q2, p1)) ||
		(Colinear == o4 && onSegment(p2, q2, q1));
}

//@Complexity O(Geometry::edges * Polygon::edges)
bool isIntersected(const Shape& s1, const Shape& s2)
{
	size_t pointsNum1 = s1.getPointsNum();
	size_t pointsNum2 = s2.getPointsNum();
	for (size_t i = 0, i1 = pointsNum1 - 1; i != pointsNum1; ++i, i1 = i) {
		const Shape::PointType& p1 = s1.getPoint(i);
		const Shape::PointType& p2 = s1.getPoint(i1);
		for (size_t j = 0, j1 = pointsNum2 - 1; j != pointsNum2; ++j, j1 = j) {
			const Shape::PointType& q1 = s2.getPoint(j);
			const Shape::PointType& q2 = s2.getPoint(j1);
			if (isIntersected(p1, p2, q1, q2)) {
				return true;
			}
		}
	}
	return false;
}

}

}