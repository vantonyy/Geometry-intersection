#pragma once

#include "segment.hpp"
#include "polygon.hpp"
#include "scan_line.hpp"
#include "interval_tree.hpp"

#include <iostream>

class RuntimeIntersectionHighlighter
{
public:
	void commit(const QLineF* p)
	{
		if (!m_points.empty()) {
			scene->removeItem(m_points.back());
		}
		Shape::Ptr s(new geometry::Segment{ { p->pointAt(0).x(), p->pointAt(0).y() },
		{ p->pointAt(1).x(), p->pointAt(1).y() } });
		mark_intersections(s, false);
	}

	void update(const QLineF* p)
	{
		for (auto pp : m_points) {
			if (scene->items().contains(pp)) {
				scene->removeItem(pp);
			}
		}
		m_points.clear();
		Shape::Ptr s(new geometry::Segment{ { p->pointAt(0).x(), p->pointAt(0).y() },
		{ p->pointAt(1).x(), p->pointAt(1).y() } });
		mark_intersections(s, true);
	}

	void commit(const QGraphicsRectItem* p)
	{
		QRectF r = p->rect();
		double x1, x2, y1, y2;
		r.getCoords(&x1, &y1, &x2, &y2);
		Shape::Ptr s(new geometry::Rect{ { x1, y1 },
		{ x1, y2 },
		{ x2, y2 },
		{ x2, y1 }
		});
		mark_intersections(s, false);
	}

	void commit(const QPolygonF* p)
	{
		Shape::Ptr s(new geometry::Polygon);
		for (QPointF f : *p) {
			s->addPoint({ f.x(), f.y() });
		}
		mark_intersections(s, false);
	}

	void update(const QGraphicsRectItem* p)
	{
		if (!p) {
			return;
		}
		for (auto pp : m_points) {
			if (scene->items().contains(pp)) {
				scene->removeItem(pp);
			}
		}
		QRectF r = p->rect();
		double x1, x2, y1, y2;
		r.getCoords(&x1, &y1, &x2, &y2);
		m_points.clear();
		Shape::Ptr s(new geometry::Rect{ { x1, y1 },
		{ x1, y2 },
		{ x2, y2 },
		{ x2, y1 }
		});
		mark_intersections(s, true);
	}

	void update(const QPolygonF* p)
	{
		if (!p) {
			return;
		}
		for (auto pp : m_points) {
			if (scene->items().contains(pp)) {
				scene->removeItem(pp);
			}
		}
		Shape::Ptr pp(new geometry::Polygon);
		for (QPointF f : *p) {
			pp->addPoint({ f.x(), f.y() });
		}
		m_points.clear();
		mark_intersections(pp, true);
	}

	void mark_intersections(Shape::Ptr s, bool track)
	{
		std::list <geometry::Segment::Point> ps;
		getIntersections(s, ps, track);
		for (geometry::Segment::Point p : ps) {
			auto pp = scene->addEllipse(p.x - 10, p.y - 10, 15, 15, QPen(Qt::green),
				QBrush(Qt::green, Qt::SolidPattern));
			if (track) {
				m_points.push_back(pp);
			}
		}
	};

	void clear()
	{
		m_points.clear();
		m_tree.clear();
	}

	typedef IntervalTree<std::list<geometry::Shape::Ptr>, double> Tree;
	typedef typename Tree::Interval Interval;
	typedef typename Tree::IntervalNode INode;

	Interval get_interval_by_y(const Shape& b)
	{
		switch (b.getType()) {
		case geometry::PolygonType: {
			auto min = std::min_element(b.getPoints().begin(), b.getPoints().end(),
				[](const Point& p1, const Point& p2) { return p1.y < p2.y; });

			auto max = std::max_element(b.getPoints().begin(), b.getPoints().end(),
				[](const Point& p1, const Point& p2) { return p1.y < p2.y; });
			return{ min->y, max->y };
		}
		case geometry::RectType: {
			const Rect& r = static_cast<const Rect&>(b);
			return{ r.top(), r.bottom() };
		}
		case geometry::SegmentType: {
			return{ b.getPoint(0).y, b.getPoint(1).y };
		}
		default:
			return{};
		}
	}

	void getIntersections(Shape::Ptr b, std::list<Point>& IP, bool update)
	{
		Interval in(get_interval_by_y(*b));
		std::list<INode*> nn;
		m_tree.searchOverlaps(in, nn);

		SweepLine SL;
		//init Sweep Line
		SL.initShape(*b);
		for (INode* n : nn) {
			for (Shape::Ptr r : n->getData()) {
				SL.initShape(*r);
			}
		}
		SL.getIntersects(IP);
		std::cout << "Count: " << IP.size() << "\n";

#if 1 //DEBUG
		int cc = 0;
		for (auto n : nn) {
			cc += n->getData().size();
		}
		std::cout << cc << "  " << b->getType().getName() << "\n";
#endif //!DEBUG

		if (!update) {
			INode* n = m_tree.insert(in);
			n->getData().push_back(b);
		}
	}
public:
	QGraphicsScene* scene;
	std::list<QGraphicsItem*> m_points;
	Tree m_tree;

};
