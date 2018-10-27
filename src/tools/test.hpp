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
	core::microseconds::rep testIntersection(unsigned count)
	{
		std::vector<core::microseconds::rep> is(count);
		for (unsigned i = 0; i != count; ++i) {
			std::cout << (1 + i) << ") ";
			is[i] = testIntersection();
		}
		core::microseconds::rep m = 0;
		m = std::accumulate(is.begin(), is.end(), m) / count;
		std::cout << "Middle value: " << m << "\n";
		return m;
	}

	core::microseconds::rep testIntersection()
	{
		std::cout << "Start performance test for intersection(polygon1, polygon2)\n";
		core::Polygon p1, p2;
		getPolygons(p1, p2);
		core::microseconds::rep m1 = testIntersectionOpt(p1, p2);
		core::microseconds::rep m2 = testIntersectionNOpt(p1, p2);
		core::microseconds::rep r = (m2 / m1);
		std::cout << "Improved: " << r << "x\n\n";
		return r;
	}
private:
	core::microseconds::rep testIntersectionNOpt(const core::Polygon& p1, const core::Polygon& p2) const
	{
		std::cout << "NOpt: ";
		core::Timer t;
		core::nopt::isIntersected(p1, p2);
		core::microseconds::rep m = t.elapsed<core::microseconds>();
		std::cout << m << "\n";
		return m;
	}

	core::microseconds::rep testIntersectionOpt(const core::Polygon& p1, const core::Polygon& p2) const
	{
		std::cout << "Opt: ";
		core::Timer t;
		core::opt::isIntersected(p1, p2);
		core::microseconds::rep m = t.elapsed<core::microseconds>();
		std::cout << m << "\n";
		return m;
	}

	void getPolygons(core::Polygon& p1, core::Polygon& p2) const
	{
		core::Polygon::Points ps(1000);
		for (int i = 0; i < 1000; ++i) {
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
		core::Polygon p = { { 0, 0 },{ 0, 1 },{ 1, 1 },{ 1, 0 } };
		std::cout << "Manhattan: " << core::isManhattan(p) << "\n";
	}
};

}
