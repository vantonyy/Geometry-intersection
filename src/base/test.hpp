#pragma once

#include "polygon.hpp"
#include "algorithms.hpp"
#include "timer.hpp"

#include <iostream>
#include <numeric>

namespace tools {

class PerfTest
{
public:
	tools::microseconds::rep testIntersection(unsigned count)
	{
		std::vector<tools::microseconds::rep> is(count);
		for (unsigned i = 0; i != count; ++i) {
			std::cout << (1 + i) << ") ";
			is[i] = testIntersection();
		}
		tools::microseconds::rep m = 0;
		m = std::accumulate(is.begin(), is.end(), m) / count;
		std::cout << "Middle value: " << m << "\n";
		return m;
	}

	microseconds::rep testIntersection()
	{
		std::cout << "Start performance test for intersection(polygon1, polygon2)\n";
		geometry::Polygon p1, p2;
		getPolygons(p1, p2);
		tools::microseconds::rep m1 = testIntersectionOpt(p1, p2);
		tools::microseconds::rep m2 = testIntersectionNOpt(p1, p2);
		tools::microseconds::rep r = (m2 / m1);
		std::cout << "Improved: " << r << "x\n\n";
		return r;
	}
private:
	tools::microseconds::rep testIntersectionNOpt(const geometry::Polygon& p1, const geometry::Polygon& p2) const
	{
		std::cout << "NOpt: ";
		tools::Timer t;
		geometry::nopt::isIntersected(p1, p2);
		tools::microseconds::rep m = t.elapsed<tools::microseconds>();
		std::cout << m << "\n";
		return m;
	}

	tools::microseconds::rep testIntersectionOpt(const geometry::Polygon& p1, const geometry::Polygon& p2) const
	{
		std::cout << "Opt: ";
		tools::Timer t;
		geometry::opt::isIntersected(p1, p2);
		tools::microseconds::rep m = t.elapsed<tools::microseconds>();
		std::cout << m << "\n";
		return m;
	}

	void getPolygons(geometry::Polygon& p1, geometry::Polygon& p2) const
	{
		constexpr int m = 9000;
		geometry::Polygon::Points ps(m);
		for (int i = 0; i < m; ++i) {
			ps.push_back({ double(i), double(i + 1) });
		}
		p1 = ps;
		p2 = ps;
	}
};

class UnitTest
{
public:
	void testManhattan()
	{
		geometry::Polygon p = { { 0, 0 },{ 0, 1 },{ 1, 1 },{ 1, 0 } };
		std::cout << "Manhattan: " << geometry::isManhattan(p) << "\n";
	}

	void testIntersectedPoint()
	{
		geometry::Segment s1({ 1, 0 }, { 1, 3 }), s2({0, 2}, {3, 2});
		geometry::Segment::Point p = geometry::nopt::getIntersectPoint(s1, s2);
		std::cout << p << "\n";
	}
};

}
