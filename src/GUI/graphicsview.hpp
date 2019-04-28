#pragma once

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QLabel>
//#include "blinkthread.h"

#include <QMouseEvent>
#include <iostream>
#include <cmath>
#include <QGraphicsItem>

enum Shapes { point1, line, rectangle, polygon, pointq };

enum TestTypes {
	RandomRect, VariableDensity, FencyRect
};

class GraphicsView : public QGraphicsView
{
	Q_OBJECT
public:
	GraphicsView(QWidget* p = 0, int t = RandomRect);
	QGraphicsScene* getScene();
	void set(Shapes);
	QLabel* get_x();
	QLabel* get_y();
	void setcolor(QColor);
	void delete_items();
	void copy_items();
	void cut_items();
	void past_items();

	void setTestType(int i) {
		m_testType = i;
	}
signals:
	void reset_s();
	void run_blink();

	public slots:

	void showIntersections(bool) {}
	void mergerect() {}
	void generate_rect_tree_with_poligons() {}
	void generate_tree();

	private slots:
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mouseDoubleClickEvent(QMouseEvent *);
	void keyPressEvent(QKeyEvent *);
	void wheelEvent(QWheelEvent *);

	void blink() {}
	//QList<QRectF *> findIntersections() {}

private:
	int m_testType;
	QTimer* m_timer;

	QGraphicsItem* item;
	QGraphicsItem* item2;
	QPolygonF* k;
	QGraphicsScene *scene;
	QPointF *point;
	QPointF *m_point;
	QVector<QPointF> m_vector;
	Shapes b;
	QPointF mouse_pos;
	QLabel *x;
	QLabel *y;
	QGraphicsItem *m_curitem;

	QColor layer_c;

	QList<QGraphicsItem*> selected_item_list;
	//QList<QRectF*> intersections_rect_list;
	QList<QGraphicsRectItem*> intersections_rect_list_items;


	int m_cor_num;
	void mapTo();
	void drawRec(QMouseEvent*);
	void drawRecMove(QMouseEvent *, Qt::BrushStyle = Qt::DiagCrossPattern);
	void drawLine(QMouseEvent*);
	void drawLineMove(QMouseEvent *);
	void drawPol(QMouseEvent*);
	void drawPolMove(QMouseEvent*);

	void createRect(QPointF c);
	void swap();
	void reset();
	void create_bg();
	void merge(QList<QGraphicsPolygonItem *> &) {}
	QPointF *mapTo_bg(QPointF);
	// void drawBackground(QPainter *, const QRectF &);
};
