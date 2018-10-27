#pragma once

#include "point.hpp"

#include <chrono>

namespace geometry {

enum OrientTypeEnum {
	Colinear = 0,
	Clockwise = 1,
	CounterClockwise = -1
};

enum ShapeTypeEnum {
	PolygonType = 0,
	SegmentType = 1,
	UnknownType
};

}