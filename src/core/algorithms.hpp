#pragma once

#include "shape.hpp"
#include "polygon.hpp"
#include "segment.hpp"
#include "types.hpp"

#include <algorithm>

//https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/

namespace core {

/* 
Compute orientation:

Cases:
       Colinear          Clockwise        Counter Clockwise
        + P3		   + P3			+ P3
       /                  /                      \
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
	int o = (p3.x() - p2.x()) * (p2.y() - p1.y()) -
		(p3.y() - p2.y()) * (p2.x() - p1.x());
	return 0 == o ? Colinear : o > 0 ? Clockwise : CounterClockwise;
}

template <typename ValueType>
bool inRange(const ValueType& lower, const ValueType& upper, const ValueType& value)
{
	return lower >= value && upper <= value;
}

template <typename Point>
bool onSegment(const Segment& s, const Point& p1)
{
	const Segment::PointType& p = s.getPoint(0);
	const Segment::PointType& q = s.getPoint(1);
	return inRange(std::min(p.x(), q.x()), std::max(p.x(), q.x()), p1.x()) &&
	       inRange(std::min(p.y(), q.y()), std::max(p.y(), q.y()), p1.y());
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

//@Complexity O(Geometry::edges * Polygon::edges)
template <typename Geometry>
bool isIntersected(const Geometry& g, const Polygon& p)
{
	size_t pointsNum = p.getPointsNum();
	for (size_t i = 0; i != pointsNum; ++i) {
		Segment s(p.getPoint(i), p.getPoint((i + 1) % pointsNum));
		if (isIntersected(s, g)) {
			return true;
		}
	}
	return false;
}

namespace test {

template <typename Point>
bool onSegment1(const Point& p, const Point& q, const Point& p1)
{
	return inRange(std::min(p.x(), q.x()), std::max(p.x(), q.x()), p1.x()) &&
		inRange(std::min(p.y(), q.y()), std::max(p.y(), q.y()), p1.y());
}

bool isIntersected1(const DPoint& p1, const DPoint& q1, 
		    const DPoint& p2, const DPoint& q2)
{
	Orient o1 = orientation(p1, q1, p2);
	Orient o2 = orientation(p1, q1, q2);
	Orient o3 = orientation(p2, q2, p1);
	Orient o4 = orientation(p2, q2, q1);
	return (o1 != o2 && o3 != o4) ||
		(Colinear == o1 && onSegment1(p1, q1, p2)) ||
		(Colinear == o2 && onSegment1(p1, q1, q2)) ||
		(Colinear == o3 && onSegment1(p2, q2, p1)) ||
		(Colinear == o4 && onSegment1(p2, q2, q1));
}

//@Complexity O(Geometry::edges * Polygon::edges)
bool isIntersected1(const Shape& s1, const Shape& s2)
{
	size_t pointsNum1 = s1.getPointsNum();
	size_t pointsNum2 = s2.getPointsNum();
	for (size_t i = 0; i != pointsNum1; ++i) {
		const DPoint& p1 = s1.getPoint(i);
		const DPoint& p2 = s1.getPoint((i + 1) % pointsNum1);
		for (size_t j = 0; j != pointsNum2; ++j) {
			const DPoint& q1 = s2.getPoint(j);
			const DPoint& q2 = s2.getPoint((j + 1) % pointsNum2);
			if (isIntersected1(p1, p2, q1, q2)) {
				return true;
			}
		}
	}
	return false;
}

}

}
