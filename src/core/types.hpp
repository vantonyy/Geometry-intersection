#pragma once

#include "point.hpp"

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

}
