#pragma once

#include "shape.hpp"
#include "polygon.hpp"
#include "segment.hpp"
#include "types.hpp"

#include <iostream>
#include <algorithm>
#include <cassert>

//https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/

namespace geometry {

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
OrientTypeEnum orientation(const Point& p1, const Point& p2, const Point& p3)
{
	typename Point::Coord o = (p3.x - p2.x) * (p2.y - p1.y) -
		(p3.y - p2.y) * (p2.x - p1.x);
	static constexpr typename Point::Coord zero(0);
	return zero == o ? Colinear : o > zero ? Clockwise : CounterClockwise;
}

template <typename Geometry>
typename Geometry::Point::Coord area(const Geometry&);

template <>
Polygon::Point::Coord area(const Polygon& p)
{
	size_t pointsNum = p.getPointsNum();
	Polygon::Point::Coord area = 0;
	for (size_t i = 0, iPrev = pointsNum - 1; i != pointsNum; ++i, iPrev = i) {
		const Polygon::Point& p1 = p.getPoint(i);
		const Polygon::Point& p2 = p.getPoint(iPrev);
		area += (p2.x + p2.x) * (p2.y - p1.y);
	}
	return area / Polygon::Point::Coord(2);
}

OrientTypeEnum orientation(const Polygon& p)
{
	return 0 > area(p) ? Clockwise : CounterClockwise;
}

bool isManhattan(const Shape& s)
{
	size_t pointsNum = s.getPointsNum();
	for (size_t i = 0, iPrev = pointsNum - 1; i != pointsNum; ++i, iPrev = i) {
		const Shape::Point& p = s.getPoint(i);
		const Shape::Point& q = s.getPoint(iPrev);
		if (p.x != q.x && p.y != q.y) {
			return false;
		}
	}
	return true;
}

void getIntersectPoint(const Shape::Point& A, const Shape::Point& B, const Shape::Point& C, const Shape::Point& D, Shape::Point& p)
{
	Shape::Point::Coord a1 = B.y - A.y;
	Shape::Point::Coord b1 = A.x - B.x;
	Shape::Point::Coord c1 = a1 * A.x + b1 * A.y;
	Shape::Point::Coord a2 = D.y - C.y;
	Shape::Point::Coord b2 = C.x - D.x;
	Shape::Point::Coord c2 = a2 * C.x + b2 * C.y;
	Shape::Point::Coord determinant = a1 * b2 - a2 * b1;
	if (determinant == 0) {
		p.makeInvalid();
	} else {
		p.x = (b2 * c1 - b1 * c2) / determinant;
		p.y = (a1 * c2 - a2 * c1) / determinant;
	}
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
	const Segment::Point& p = s.getPoint(0);
	const Segment::Point& q = s.getPoint(1);
	return inRange(p.x, q.x, p1.x) && inRange(p.y, q.y, p1.y);
}

bool isIntersected(const Segment& s1, const Segment& s2)
{
	const Segment::Point& p1 = s1.getPoint(0);
	const Segment::Point& q1 = s1.getPoint(1);
	const Segment::Point& p2 = s2.getPoint(0);
	const Segment::Point& q2 = s2.getPoint(1);
	OrientTypeEnum o1 = orientation(p1, q1, p2);
	OrientTypeEnum o2 = orientation(p1, q1, q2);
	OrientTypeEnum o3 = orientation(p2, q2, p1);
	OrientTypeEnum o4 = orientation(p2, q2, q1);
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
		const Shape::Point& q1 = sh.getPoint(i);
		const Shape::Point& q2 = sh.getPoint(j);
		if (isIntersected(Segment{q1, q2}, g)) {
			return true;
		}
	}
	return false;
}

void getIntersectPoint(const Segment& s1, const Segment& s2, Shape::Point& p)
{
	assert(nopt::isIntersected(s1, s2));
	const Segment::Point& p1 = s1.getPoint(0);
	const Segment::Point& q1 = s1.getPoint(1);
	const Segment::Point& p2 = s2.getPoint(0);
	const Segment::Point& q2 = s2.getPoint(1);

	//TODO:
	const Segment::Point::Coord& k1 = (q1.y - p1.y) / (q1.x - p1.x);
	const Segment::Point::Coord& k2 = (q2.y - p2.y) / (q2.x - p2.x);
	const Segment::Point::Coord& b1 = p1.y - k1 * p1.x;
	const Segment::Point::Coord& b2 = p2.y - k2 * p2.x;

	p.x = (b2 - b1) / (k1 - k2);
	p.y = p.x * k1 + b1;
}

Segment::Point getIntersectPoint(const Segment& s1, const Segment& s2)
{
	assert(nopt::isIntersected(s1, s2));
	Segment::Point p;
	getIntersectPoint(s1, s2, p);
	return p;
}

}

namespace opt {

#define isInRange(m, M, v) \
		(m <= v && v <= M) || (M <= v && v <= m)

bool onSegment(const Shape::Point& p, const Shape::Point& q, const Shape::Point& p1)
{
	return isInRange(p.x, q.x, p1.x) && isInRange(p.y, q.y, p1.y);
}

bool isIntersected(const Shape::Point& p1, const Shape::Point& q1,
		   const Shape::Point& p2, const Shape::Point& q2)
{
	OrientTypeEnum o1 = orientation(p1, q1, p2);
	OrientTypeEnum o2 = orientation(p1, q1, q2);
	OrientTypeEnum o3 = orientation(p2, q2, p1);
	OrientTypeEnum o4 = orientation(p2, q2, q1);
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
	for (size_t i = 0, iPrev = pointsNum1 - 1; i != pointsNum1; ++i, iPrev = i) {
		const Shape::Point& p1 = s1.getPoint(i);
		const Shape::Point& p2 = s1.getPoint(iPrev);
		for (size_t j = 0, jPrev = pointsNum2 - 1; j != pointsNum2; ++j, jPrev = j) {
			const Shape::Point& q1 = s2.getPoint(j);
			const Shape::Point& q2 = s2.getPoint(jPrev);
			if (isIntersected(p1, p2, q1, q2)) {
				return true;
			}
		}
	}
	return false;
}

}

namespace scanLineImpl {

bool isIntersected(const Shape&, const Shape&)
{
	//TODO: implement
	return false;
}

}

}
