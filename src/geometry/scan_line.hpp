#pragma once

#include "segment.hpp"
#include "polygon.hpp"
#include "algorithms.hpp"
#include "rectangle.hpp"
#include "interval_tree.hpp"

#include <set>
#include <list>
#include <memory>

using geometry::Segment;
using geometry::Shape;
using geometry::Polygon;
using geometry::Rect;

typedef Segment::Point Point;

enum SegmentPointType {
	Left,
	Right,
	Intersect
};

class EventBase
{
public:
	typedef std::shared_ptr<EventBase> ptr;

	EventBase(const Point& p, SegmentPointType t)
		: point(p)
		, type(t)
	{
	}

	SegmentPointType type;
	Point point;
};

class Event : public EventBase
{
public:
	Event(const Point& p, SegmentPointType t, const Segment* s)
		: EventBase(p, t)
		, ref(s)
	{
	}

	const Segment* ref;
};

class EventIntersect : public EventBase
{
public:
	EventIntersect(const Point& p, const Segment* s1, const Segment* s2)
		: EventBase(p, Intersect)
		, iSeg1(s1)
		, iSeg2(s2)
	{
	}
	
	const Segment* iSeg1;
	const Segment* iSeg2;
};


class XCompare
{
public:
	bool operator() (const Point& p, const Point& q) const
	{
		return p < q;
	}
};

class ECompare
{
public:
	bool operator() (const Event::ptr& e1, const Event::ptr& e2) const
	{
		return XCompare()(e1->point, e2->point);
	}
};

class EventQueue
{
public:
	EventQueue()
	{
	}
	
	void insert(const Event::ptr e)
	{
		m_EQ.insert(e);
	}

	const Event::ptr pop()
	{
		Event::ptr p;
		if (!m_EQ.empty()) {
			auto i = m_EQ.begin();
			p = *i;
			m_EQ.erase(i);
		}
		return p;
	}

	void dump()
	{
		for (const EventBase::ptr e : m_EQ) {
			std::cout << e->point << " - S: " << *(static_cast<const Event*>(e.get())->ref) << " - ";
			if (e->type == Left) {
				std::cout << "Left\n";
			} else {
				std::cout << "Right\n";
			}
		}
	}
private:
	std::multiset<Event::ptr, ECompare> m_EQ;
};


/*
Runtime
Sweep line status updates: O(log n)
Event queue operations: O(log n), as the total number of stored events is <= 2n + k,
			and each operation takes time O(log(2n+k)) = O(log n2) = O(log n)
								k = O(n2)
There are O(n+k) events. Hence the total runtime is O((n+k) log n)
*/
class SweepLine // by Bentley–Ottmann
{
public:
	SweepLine()
	{
	}

	bool getIntersects(std::list<Point>& IL)
	{
		IL.clear();
		std::set<const Segment*> SL;
		while (const EventBase::ptr& e = m_EQ.pop()) {
			switch (e->type) {
			case Left:
				handleLeftPoint(static_cast<const Event*>(e.get()), SL);
				break;
			case Right:
				handleRightPoint(static_cast<const Event*>(e.get()), SL);
				break;
			case Intersect:
				handleIntersectPoint(static_cast<const EventIntersect*>(e.get()), IL);
				break;
			}
		}
		return !IL.empty();
	}

	void initShape(const geometry::Shape& s)
	{
		unsigned num = s.getPointsNum();
		for (unsigned i = 0; i < num; ++i) {
			const Point& p = s.getPoint(i);
			const Point& q = s.getPoint(((i + 1) % num));
			m_segments.push_back({ p, q });
			Segment* seg = &(m_segments.back());
			seg->setOwner(&s);
			if (p < q) {
				m_EQ.insert(Event::ptr(new Event(p, Left, seg)));
				m_EQ.insert(Event::ptr(new Event(q, Right, seg)));
			}
			else {
				m_EQ.insert(Event::ptr(new Event(p, Right, seg)));
				m_EQ.insert(Event::ptr(new Event(q, Left, seg)));
			}
		}
	}

public:
	void handleLeftPoint(const Event* e, std::set<const Segment*>& SL)
	{
		const Segment& segE = *e->ref;
		auto i = SL.insert(&segE).first;
		//above intercestion
		const Segment* above = upper(SL, i);
		Point I;
		if (0 != above && getIntersect(segE, *above, I)) {
			m_EQ.insert(Event::ptr(new EventIntersect(I, *i, above)));
		}
		//below intercestion
		const Segment* below = lower(SL, i);
		if (0 != below && getIntersect(segE, *below, I)) {
			m_EQ.insert(Event::ptr(new EventIntersect(I, *i, below)));
		}
	}

	void handleRightPoint(const Event* e, std::set<const Segment*>& SL)
	{
		auto i = SL.find(e->ref);
		if (i != SL.end()) {
			const Segment* above = upper(SL, i);
			const Segment* below = lower(SL, i);
			Point I;
			if (0 != above && 0 != below && getIntersect(*above, *below, I)) {
				m_EQ.insert(Event::ptr(new EventIntersect(I, above, below)));
			}
			SL.erase(i);
		}
	}

	void handleIntersectPoint(const EventIntersect* e, std::list<Point>& IL)
	{
		IL.push_back(e->point);
		//TODO:
	}

	static bool getIntersect(const Segment& s1, const Segment& s2, Point& p)
	{
		if (s1.hasSameOwner(s2)) {
			return false;
		}
		return geometry::nopt::intersect(s1, s2, p);
	}

	static const Segment* upper(const std::set<const Segment*>& SL, std::set<const Segment*>::iterator i)
	{
		if (i != SL.end()) {
			return ++i == SL.end() ? 0 : *i;
		}
		return 0;
	}

	static const Segment* lower(const std::set<const Segment*>& SL, std::set<const Segment*>::iterator i)
	{
		if (i != SL.end()) {
			return i == SL.begin() ? 0 : *(--i);
		}
		return 0;
	}
private:
	EventQueue m_EQ;
	std::list<Segment> m_segments;
};