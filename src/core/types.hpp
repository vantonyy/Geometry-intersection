#pragma once

#include "point.hpp"

#include <chrono>

namespace core {

enum Orient {
	Colinear = 0,
	Clockwise = 1,
	CounterClockwise = -1
};

enum ShapeTypeEnum {
	PolygonType = 0,
	SegmentType = 1,
	UnknownType
};

typedef Point<double> DPoint;

typedef std::chrono::nanoseconds nanoseconds;
typedef std::chrono::microseconds microseconds;
typedef std::chrono::milliseconds milliseconds;
typedef std::chrono::seconds seconds;
typedef std::chrono::minutes minutes;
typedef std::chrono::hours hours;

}