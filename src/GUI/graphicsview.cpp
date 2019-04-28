#include "graphicsview.hpp"

#include "mainwindow.hpp"

#include <QGraphicsItem>
#include <qtextedit.h>
#include <QMouseEvent>
#include <iostream>
#include <cmath>
#include <thread>
#include <set>

#include "segment.hpp"
#include "polygon.hpp"
#include "scan_line.hpp"
#include "interval_tree.hpp"
#include "runtime_intersection_marker.hpp"

RuntimeIntersectionHighlighter runtime_intersection_highlighter;

GraphicsView::GraphicsView(QWidget* q, int t) : 
	QGraphicsView(q), m_testType(t), point(0), m_point(0), m_cor_num(0), m_curitem(0), b(pointq), m_timer(0)
{
	scene = new QGraphicsScene();
	runtime_intersection_highlighter.scene = scene;
	setSceneRect(0, 0, 350, 350);
	create_bg();
	setScene(scene);
	x = new QLabel;
	y = new QLabel;
	x->setText("0");
	y->setText("0");
	layer_c = Qt::red;
	runtime_intersection_highlighter.clear();
	//  connect(scene,SIGNAL(changed(QList<QRectF>)),this,SLOT(showIntersections()));
	setMouseTracking(true);
}

void GraphicsView::create_bg()
{
	static const QColor bg_color(30, 30, 30);
	//this function is slow
	QPixmap* b_pixmap = new QPixmap(QSize(4000, 4000));
	b_pixmap->fill(bg_color);
	QPainter* painter = new QPainter(b_pixmap);
	painter->setPen(QPen(Qt::white, Qt::SolidLine));
	for (int i = 0; i < 4000; i += 50) {
		for (int j = 0; j < 4000; j += 50) {
			painter->drawPoint(i, j);
		}
	}
	painter->drawLine(QPointF(-4000, 0), QPointF(4000, 0));
	painter->drawLine(QPointF(0, -4000), QPointF(0, 4000));

	QBrush brush(bg_color, *b_pixmap);
	setBackgroundBrush(brush);
}

QPointF* GraphicsView::mapTo_bg(QPointF point)
{
	int x = point.x() / 10;
	int y = point.y() / 10;
	if (10 * x + 10 - point.x() < point.x() - 10 * x)
		x += 1;
	if (fabs(10 * y + 10 - point.y())<fabs(point.y() - 10 * y))
		y += 1;
//	std::cout << x << "\t" << x * 10 << std::endl;
	return new QPointF(x * 10, y * 10);
}

void GraphicsView::set(Shapes t)
{
	b = t;
	if (b == polygon)
		m_vector.clear();
}

void GraphicsView::drawLine(QMouseEvent* e)
{
	if (!point) {
		point = new QPointF(mapToScene(e->pos()));
		point = mapTo_bg(*point);
		setMouseTracking(true);
	} else
	{
		QGraphicsItem* s;
		setMouseTracking(false);
		if (e->button() != Qt::NoButton) {
			QLineF ll(*point, *mapTo_bg(mapToScene((e->pos()))));
			s = scene->addLine(ll, QPen(layer_c));
			runtime_intersection_highlighter.commit(&ll);
		} else {
			QLineF ll(*point, *mapTo_bg(QPointF((e->pos()))));
			s = scene->addLine(ll, QPen(layer_c));
			runtime_intersection_highlighter.commit(&ll);
		}
		s->setFlag(QGraphicsItem::ItemIsSelectable);
		s->setFlag(QGraphicsItem::ItemIsMovable, true);
		delete point;
		point = 0;
		if (m_curitem)
			delete m_curitem;
		m_curitem = 0;
	}
}

void GraphicsView::drawLineMove(QMouseEvent * e)
{
	if (m_curitem)
	{
		if (scene->items().contains(m_curitem))
			scene->removeItem(m_curitem);
		delete m_curitem;
		m_curitem = 0;
	}
	if (point) {
		QPointF p = *mapTo_bg(mapToScene(e->pos()));
		QLineF ll(*point, p);
		m_curitem = scene->addLine(ll, QPen(layer_c));
		runtime_intersection_highlighter.update(&ll);
	}
}

void GraphicsView::drawRec(QMouseEvent *e)
{
	if (!m_point) {
		m_point = new QPointF(mapToScene(e->pos()));
		m_point = mapTo_bg(*m_point);
		setMouseTracking(true);
	}
	else {
		QPointF c;
		setMouseTracking(false);
		if (e->button() != Qt::NoButton) {
			c = mapToScene(e->pos());
		}
		else {
			c = e->pos();
		}
		createRect(c);
	}
}

void GraphicsView::createRect(QPointF c)
{
	if (!m_point) {
		m_point = new QPointF();
	}
	c = *mapTo_bg(c);
	QGraphicsItem *s = 0;
	if (m_point->x()>c.x() && m_point->y()>c.y())
	{
		s = scene->addRect(c.x(), c.y(), fabs(c.x() - m_point->x()), fabs(c.y() - m_point->y()), QPen(layer_c), QBrush(layer_c, Qt::DiagCrossPattern));
	}
	else {
		if (m_point->x() < c.x() && m_point->y() < c.y()) {
			s = scene->addRect(m_point->x(), m_point->y(), fabs(c.x() - m_point->x()), fabs(c.y() - m_point->y()), QPen(layer_c), QBrush(layer_c, Qt::DiagCrossPattern));
		}
		else {
			if (m_point->x() > c.x() && m_point->y() < c.y()) {
				s = scene->addRect(c.x(), m_point->y(), fabs(c.x() - m_point->x()), fabs(c.y() - m_point->y()), QPen(layer_c), QBrush(layer_c, Qt::DiagCrossPattern));
			}
			else if (m_point->x() < c.x() && m_point->y() > c.y()) {
				s = scene->addRect(m_point->x(), c.y(), fabs(c.x() - m_point->x()), fabs(c.y() - m_point->y()), QPen(layer_c), QBrush(layer_c, Qt::DiagCrossPattern));
			}
		}
	}
	s->setFlag(QGraphicsItem::ItemIsSelectable);
	s->setFlag(QGraphicsItem::ItemIsMovable, true);
	runtime_intersection_highlighter.commit(static_cast<QGraphicsRectItem*>(s));

	delete m_point;
	m_point = 0;
	if (m_curitem)
		delete m_curitem;
	m_curitem = 0;
}

void GraphicsView::drawRecMove(QMouseEvent* e, Qt::BrushStyle style)
{
	if (m_curitem) {
		if (scene->items().contains(m_curitem))
			scene->removeItem(m_curitem);
		delete m_curitem;
		m_curitem = 0;
	}
	QPointF  c = mapToScene(e->pos());
	c = *mapTo_bg(c);
	if (m_point) {
		if (m_point->x()>c.x() && m_point->y()>c.y()) {
			m_curitem = scene->addRect(c.x(), c.y(), fabs(c.x() - m_point->x()), fabs(c.y() - m_point->y()), QPen(layer_c), QBrush(layer_c, style));
		} else {
			if (m_point->x()<c.x() && m_point->y()<c.y()) {
				m_curitem = scene->addRect(m_point->x(), m_point->y(), fabs(c.x() - m_point->x()), fabs(c.y() - m_point->y()), QPen(layer_c), QBrush(layer_c, style));
			} else {
				if (m_point->x() > c.x() && m_point->y() < c.y()) {
					m_curitem = scene->addRect(c.x(), m_point->y(), fabs(c.x() - m_point->x()), fabs(c.y() - m_point->y()), QPen(layer_c), QBrush(layer_c, style));
				} else if (m_point->x() < c.x() && m_point->y() > c.y()) {
					m_curitem = scene->addRect(m_point->x(), c.y(), fabs(c.x() - m_point->x()), fabs(c.y() - m_point->y()), QPen(layer_c), QBrush(layer_c, style));
				}
			}
		}
		runtime_intersection_highlighter.update(static_cast<QGraphicsRectItem*>(m_curitem));
	}
}
void GraphicsView::drawPol(QMouseEvent *e)
{
	m_vector.push_back(*mapTo_bg(mapToScene(e->pos())));
	k = new QPolygonF(m_vector);
	setMouseTracking(true);
}

void GraphicsView::drawPolMove(QMouseEvent *e)
{
	if (m_curitem)
	{
		if (scene->items().contains(m_curitem))
			scene->removeItem(m_curitem);
		delete m_curitem;
		m_curitem = 0;
		if (!m_vector.empty())
			m_vector.pop_back();
	}
	if (!m_vector.empty())
	{
		QPointF  p = mapToScene(e->pos());
		p = *mapTo_bg(p);
		m_vector.push_back(p);
	}
	if (m_vector.size() != 0)
	{
		swap();
		QPolygonF vv(m_vector);
		item2 = m_curitem = scene->addPolygon(vv, QPen(layer_c), QBrush(layer_c, Qt::DiagCrossPattern));
		runtime_intersection_highlighter.update(&vv);
		item2->setFlag(QGraphicsItem::ItemIsSelectable);
		item2->setFlag(QGraphicsItem::ItemIsMovable, true);
	}
}

void GraphicsView::swap()
{
	item = item2;
}

void GraphicsView::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_F) {
		QRectF bounds = scene->itemsBoundingRect();
		bounds.setWidth(bounds.width()*0.9);         // to tighten-up margins
		bounds.setHeight(bounds.height()*0.9);       // same as above
		fitInView(bounds, Qt::KeepAspectRatio);
		centerOn(0, 0);
		return;
	}

	if (event->key() == Qt::Key_Escape)//ESC
	{

		if ((m_vector.size()) && (b == polygon)) {
			std::cout << "ESC" << "\t" << event->key() << "\t" << Qt::Key_Enter << std::endl;
			scene->removeItem(item2);
			QGraphicsItem* s = scene->addPolygon(*k, QPen(layer_c), QBrush(layer_c, Qt::DiagCrossPattern));
			s->setFlag(QGraphicsItem::ItemIsSelectable);
			s->setFlag(QGraphicsItem::ItemIsMovable, true);
			std::cout << m_vector.size() << "\t" << m_vector.count() << std::endl;
			m_vector.clear();
			std::cout << m_vector.size() << "\t" << m_vector.count() << std::endl;

		} else
		{
			if (m_point == 0 && point == 0)
				reset_s();
			if (b == line || b == rectangle)
				reset();

		}
	}
	else
		if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Enter - 1)//ENTER
		{
			std::cout << "Enter" << std::endl;
			switch (b)
			{
			case line:
				drawLine(new QMouseEvent(QEvent::None, QPoint(mouse_pos.rx(), mouse_pos.ry()), Qt::NoButton, Qt::NoButton, Qt::NoModifier));
				break;
			case rectangle:
				drawRec(new QMouseEvent(QEvent::None, QPoint(mouse_pos.rx(), mouse_pos.ry()), Qt::NoButton, Qt::NoButton, Qt::NoModifier));
				break;
			case polygon:
				swap();
				QPolygonF vv(m_vector);
				QGraphicsItem* s = item2 = scene->addPolygon(vv, QPen(layer_c), QBrush(layer_c, Qt::DiagCrossPattern));
				s->setFlag(QGraphicsItem::ItemIsSelectable);
				s->setFlag(QGraphicsItem::ItemIsMovable, true);
				runtime_intersection_highlighter.commit(&vv);
				reset();
				std::cout << "double click\t" << m_vector.size() << std::endl;
				m_vector.clear();
				std::cout << "double click\t" << m_vector.size() << std::endl;
				break;

			};
		}
}

void GraphicsView::reset()
{
	if (0 != m_point)
		delete m_point;
	m_point = 0;
	if (0 != point)
		delete point;
	point = 0;
}

void GraphicsView::mouseDoubleClickEvent(QMouseEvent *e)
{
	switch (b) {
	case line:
		reset();
		break;
	case rectangle:
		reset();
		break;
	case polygon:
		swap();
		QPolygonF vv(m_vector);
		QGraphicsItem* s = item2 = scene->addPolygon(vv, QPen(layer_c), QBrush(layer_c, Qt::DiagCrossPattern));
		s->setFlag(QGraphicsItem::ItemIsSelectable);
		s->setFlag(QGraphicsItem::ItemIsMovable, true);
		runtime_intersection_highlighter.commit(&vv);
		reset();
		m_vector.clear();
		break;

	};

}

void GraphicsView::mousePressEvent(QMouseEvent *e)
{
	QPointF c;
	switch (b) {
	case line:
		drawLine(e);
		break;
	case rectangle:
		drawRec(e);
		break;
	case polygon:
		drawPol(e);
		break;
	case point1:
		c = mapToScene(e->pos());
		c = *mapTo_bg(c);
		scene->addEllipse(c.x() - 10, c.y() - 10, 20, 20, QPen(Qt::green), QBrush(Qt::green, Qt::SolidPattern));
	case pointq:
		//drawRec(e);
		QGraphicsView::mousePressEvent(e);
	};
}

void GraphicsView::mouseMoveEvent(QMouseEvent *e)
{
	mouse_pos = mapToScene(e->pos());
	mapTo();
	switch (b)
	{
	case line:
		drawLineMove(e);
		break;
	case rectangle:
		drawRecMove(e);
		break;
	case polygon:
		drawPolMove(e);
		break;
	default:
		//drawRecMove(e, Qt::BrushStyle::VerPattern);
		QGraphicsView::mouseMoveEvent(e);
	};
}

void GraphicsView::wheelEvent(QWheelEvent *event)
{
	static int scale_out = 0;
	static int scale_in = 0;


	QPointF pointBeforeScale(mapToScene(event->pos()));

	double scaleFactor = 2; //How fast we zoom
	if (event->delta() > 0) {
		//Zoom in
		if (scale_in<2)
		{
			scale_in++;
			scale(scaleFactor, scaleFactor);
			scale_out--;
		}
	}
	else {
		//Zooming out
		if (scale_out<1)
		{
			scale_in--;
			scale(1.0 / scaleFactor, 1.0 / scaleFactor);
			scale_out++;
		}
	}
	/*
	ensureVisible(event->pos().x(),event->pos().y(),1,1,1,1);
	event->accept();
	ensureVisible(pointBeforeScale.x(),pointBeforeScale.y(),10,10,0,0);
	centerOn(pointBeforeScale);
	QGraphicsView::wheelEvent(event);

	//fitInView();
	// centerOn(viewport()->rect().center(),Qt::KeepAspectRatio);
	QPointF pointAfterScale(mapToScene(event->pos()));

	QPointF offset = pointBeforeScale - pointAfterScale;
	QPointF center = mapToScene(viewport()->rect().center()) + offset;
	centerOn(center);*/
}

QGraphicsScene* GraphicsView::getScene()
{
	return scene;
}

void GraphicsView::mapTo()
{
	QString s;
	s.setNum(mouse_pos.rx());
	x->setText(s);
	s.setNum(mouse_pos.ry());
	y->setText(s);
}

QLabel* GraphicsView::get_x()
{
	return x;
}

QLabel* GraphicsView::get_y()
{	return y;

}

void GraphicsView::setcolor(QColor color)
{
	layer_c = color.toRgb();
	std::cout << layer_c.name().toStdString() << "\n";
}

void GraphicsView::delete_items()
{
	runtime_intersection_highlighter.clear();
	QList<QGraphicsItem*> item_list = scene->selectedItems();
	for (QList<QGraphicsItem*>::iterator i = item_list.begin();i != item_list.end();++i) {
		scene->removeItem(*i);
	}
}

void GraphicsView::copy_items()
{
	/*selected_item_list.clear();
	QList<QGraphicsItem*> item_list=scene->selectedItems();
	for(QList<QGraphicsItem*>::iterator i=item_list.begin();i!=item_list.end();++i)
	{

	QPointF pos=(*i)->pos();
	//  std::cout<<pos.x()<<"\t"<<pos.y()<<std::endl;
	(*i)->setPos(pos.x()+100,pos.y()-10);
	//  pos=(*i)->pos();
	//  std::cout<<pos.x()<<"\t"<<pos.y()<<std::endl;
	selected_item_list.push_back(*i);
	}*/

}


void GraphicsView::past_items()
{
	for (QList<QGraphicsItem*>::iterator i = selected_item_list.begin(); i != selected_item_list.end(); ++i) {
		scene->addItem(*i);
	}
}

void GraphicsView::cut_items()
{
	selected_item_list = scene->selectedItems();
	delete_items();
}

void GraphicsView::generate_tree()
{
	switch (m_testType) {
	case RandomRect: {

		break;
	}
	case VariableDensity: {

		break;
	}
	case FencyRect: {
		int n = 1000;
		//Horizontal
		for (int i = 0; i < n / 2; i += 60) {
			runtime_intersection_highlighter.commit
				(scene->addRect(qreal(0), qreal(i), qreal(n / 2), qreal(40), QPen(layer_c), QBrush(layer_c, Qt::DiagCrossPattern)));
		}
		//Vertical
		for (int i = 40; i < n / 2; i += 60) {
			runtime_intersection_highlighter.commit
				(scene->addRect(qreal(i), qreal(0), qreal(40), qreal(n / 2), QPen(layer_c), QBrush(layer_c, Qt::DiagCrossPattern)));
		}
		break;
	}
	default:;
	}
}