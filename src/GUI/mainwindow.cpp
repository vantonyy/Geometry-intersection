#include "mainwindow.hpp"

#include <QMenuBar>
#include <QMenu>
#include <iostream>
#include <QToolBar>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
//#include "layer.h"
#include <QString>
#include <QDialog>
#include <QBoxLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QSplitter>
#include <QListWidget>
#include <QTableWidget>
#include <QHeaderView>

MainWindow::MainWindow() :QMainWindow(), view(0),  m_fullgraph(0), m_tree_type(0)
{
	//setWindowState(Qt::WindowMaximized);
	createAction();
	createMenu();
	createToolbars();
	setMinimumSize(1400, 1000);
	setWindowTitle("Geometry Intersection");

}

MainWindow::~MainWindow()
{
}

void MainWindow::createRand()
{
	combo = new QComboBox(this);
	combo->addItem("Random Rect", QVariant(0));
	combo->addItem("Variable Density", QVariant(1));
	combo->addItem("Fency Rect", QVariant(2));
	//combo->setVisible(false);
	connect(combo, SIGNAL(activated(int)), this, SLOT(set_tree_type(int)));

	pol_rect_tree = new QAction("Poligon's minimal rect tree", this);
	pol_rect_tree->setVisible(false);

	routing = new QAction("Generate shapes", this);
	routing->setIcon(QIcon("../Resources/images/routing.png"));
	routing->setVisible(false);
}

void MainWindow::generategraph()
{
}

void MainWindow::generatetree()
{
}

void MainWindow::set_tree_type(int i)
{
	m_tree_type = i;
	if (view) {
		view->setTestType(i);
	}
}

void MainWindow::randomPoints()
{	
}

void MainWindow::createNew()
{
	//********NEW*********
	newAction = new QAction("&New", this);
	newAction->setIcon(QIcon("../Resources/images/new.png"));
	newAction->setShortcut(QKeySequence::New);
	connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));
}

void MainWindow::createExit()
{
	//*******EXIT*********
	exitAction = new QAction("E&xit", this);
	exitAction->setShortcut(QKeySequence::Close);
	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

}

void MainWindow::createDrawLine()
{
	//******Point****************
	drawPoint = new QAction("Point", this);
	drawPoint->setIcon(QIcon("../Resources/images/point.png"));
	drawPoint->setCheckable(true);
	connect(drawPoint, SIGNAL(toggled(bool)), this, SLOT(set_enum()));
	connect(drawPoint, SIGNAL(changed()), this, SLOT(reset()));

	//********LINE*******
	drawLine = new QAction("Line", this);
	drawLine->setIcon(QIcon("../Resources/images/line.png"));
	drawLine->setCheckable(true);
	connect(drawLine, SIGNAL(toggled(bool)), this, SLOT(set_enum()));
	connect(drawLine, SIGNAL(changed()), this, SLOT(reset()));
}

void MainWindow::createDrawPol()
{
	//*******POLYGON**********
	drawPol = new QAction("Polygon", this);
	drawPol->setIcon(QIcon("../Resources/images/polygon.png"));
	drawPol->setCheckable(true);
	connect(drawPol, SIGNAL(toggled(bool)), this, SLOT(set_enum()));
	connect(drawPol, SIGNAL(changed()), this, SLOT(reset()));
}

void MainWindow::createDrawRect()
{

	//*******RECTANGLE*********
	drawRect = new QAction("Rectangle", this);
	drawRect->setIcon(QIcon("../Resources/images/rectangle.png"));
	drawRect->setCheckable(true);
	connect(drawRect, SIGNAL(toggled(bool)), this, SLOT(set_enum()));
	connect(drawRect, SIGNAL(changed()), this, SLOT(reset()));

}

void MainWindow::createCopy()
{
	copyAction = new QAction("Copy", this);
	copyAction->setIcon(QIcon("../Resources/images/copy.png"));
	copyAction->setShortcut(QKeySequence::Copy);
	connect(copyAction, SIGNAL(triggered()), this, SLOT(copy_items()));
}

void MainWindow::createCut()
{
	cutAction = new QAction("Cut", this);
	cutAction->setIcon(QIcon("../Resources/images/cut.png"));
	cutAction->setShortcut(QKeySequence::Cut);
	connect(cutAction, SIGNAL(triggered()), this, SLOT(cut_items()));
}

void MainWindow::createPaste()
{
	pasteAction = new QAction("Paste", this);
	pasteAction->setIcon(QIcon("../Resources/images/paste.png"));
	pasteAction->setShortcut(QKeySequence::Paste);
	connect(pasteAction, SIGNAL(triggered()), this, SLOT(past_items()));
}

void MainWindow::createDelete()
{
	deleteAction = new QAction("Delete", this);
	deleteAction->setIcon(QIcon("../Resources/images/delete.png"));
	deleteAction->setShortcut(QKeySequence::Delete);
	connect(deleteAction, SIGNAL(triggered()), this, SLOT(delete_items()));
}

void MainWindow::delete_items()
{
	if (view) {
		view->delete_items();
	}
}


void MainWindow::copy_items()
{
	if (view) {
		view->copy_items();
	}

}

void MainWindow::cut_items()
{
	if (view) {
		view->cut_items();
	}

}

void MainWindow::past_items()
{
	if (view)
	{
		view->past_items();
	}
}

void MainWindow::createAction()
{
	createNew();
	createExit();
	createDrawLine();
	createDrawPol();
	createDrawRect();
	createCopy();
	createCut();
	createPaste();
	createDelete();
	createRand();

	//********Connections********
	connect(drawLine, SIGNAL(triggered()), this, SLOT(re_1()));
	connect(drawRect, SIGNAL(triggered()), this, SLOT(re_2()));
	connect(drawPol, SIGNAL(triggered()), this, SLOT(re_3()));

	clearAction = new QAction("Clear Scene", this);
	clearAction->setIcon(QIcon("../Resources/images/clear.png"));
	clearAction->setVisible(false);



	findSectsAction = new QAction("Find &Intersections", this);
	findSectsAction->setCheckable(true);

	analyze = new QAction("Analyze algorithms", this);
	analyze->setIcon(QIcon("../Resources/images/analiz.png"));
	connect(analyze, SIGNAL(triggered()), this, SLOT(analyz()));
}

void MainWindow::re_1()
{
	if (drawPol->isChecked())
		drawPol->setChecked(false);
	if (drawRect->isChecked())
		drawRect->setChecked(false);
}

void MainWindow::re_2()
{
	if (drawLine->isChecked())
		drawLine->setChecked(false);
	if (drawPol->isChecked())
		drawPol->setChecked(false);
}

void MainWindow::re_3()
{
	if (drawLine->isChecked())
		drawLine->setChecked(false);
	if (drawRect->isChecked())
		drawRect->setChecked(false);
}

void MainWindow::set_enum()
{
	if (view) {
		if (drawLine->isChecked()) {
			view->set(Shapes(line));
		}
		if (drawPol->isChecked()) {
			view->set(Shapes(polygon));
		}
		if (drawRect->isChecked()) {
			view->set(Shapes(rectangle));
		}
		if (drawPoint->isChecked()) {
			view->set(Shapes(point1));
		}
	}
}

void MainWindow::reset()
{
	if (view)
		view->set(Shapes(pointq));
}

void MainWindow::createToolbars()
{
	fileToolbar = addToolBar(tr("File"));
	fileToolbar->addAction(newAction);

	editToolbar = addToolBar(tr("Edit"));

	editToolbar->addAction(copyAction);
	editToolbar->addAction(cutAction);
	editToolbar->addAction(pasteAction);
	editToolbar->addAction(deleteAction);
	editToolbar->addAction(clearAction);

	drawToolbar = addToolBar(tr("Draw"));
	drawToolbar->addAction(drawPoint);
	drawToolbar->addAction(drawLine);
	drawToolbar->addAction(drawRect);
	drawToolbar->addAction(drawPol);
	drawToolbar->addAction(routing);
	drawToolbar->addWidget(combo);
	drawToolbar->addAction(analyze);

	//drawToolbar->addAction(findSectsAction);
	drawToolbar->setVisible(false);
	connect(newAction, SIGNAL(triggered(bool)), drawToolbar, SLOT(show()));


}

void MainWindow::createMenu()
{
	fileMenu = menuBar()->addMenu(tr("File"));
	fileMenu->addAction(newAction);
	fileMenu->addAction(exitAction);

	editMenu = menuBar()->addMenu(tr("Edit"));
	editMenu->addAction(copyAction);
	editMenu->addAction(cutAction);
	editMenu->addAction(pasteAction);
	editMenu->addAction(deleteAction);
	editMenu->addAction(clearAction);

	drawMenu = menuBar()->addMenu(tr("Draw"));
	drawMenu->addAction(drawPoint);
	drawMenu->addAction(drawLine);
	drawMenu->addAction(drawPol);
	drawMenu->addAction(drawRect);
}




void MainWindow::newFile()
{
	if (view) {
		delete view;
		delete cordToolbar;
	}
	view = new GraphicsView(this, m_tree_type);
	view->setBaseSize(500, 500);
	createLayouts();
	//createRand();
	pol_rect_tree->setVisible(true);
	routing->setVisible(true);

	connect(pol_rect_tree, SIGNAL(triggered()), view, SLOT(generate_rect_tree_with_poligons()));
	connect(routing, SIGNAL(triggered()), view, SLOT(generate_tree()));
	connect(clearAction, SIGNAL(triggered()), view->getScene(), SLOT(clear()));
	clearAction->setVisible(true);

	// setCentralWidget(view);
	drawLine->setChecked(false);
	drawPol->setChecked(false);
	drawRect->setChecked(false);
	cordToolbar = addToolBar(tr("Cordinates"));
	cordToolbar->addWidget(view->get_x());
	cordToolbar->addSeparator();
	cordToolbar->addWidget(view->get_y());
	connect(view, SIGNAL(reset_s()), this, SLOT(reset_selection()));

	connect(findSectsAction, SIGNAL(triggered(bool)), view, SLOT(showIntersections(bool)));
	// connect(view->getScene(),SIGNAL(changed(QList<QRectF>)),blinkthread,SLOT(get_intersection_list()));
	// connect(findSectsAction, SIGNAL(triggered(bool)), blinkthread, SLOT(set_blink(bool)));
	//  connect(findSectsAction, SIGNAL(triggered(bool)), blinkthread, SLOT(start()));
}


void MainWindow::setfullgraph(bool t)
{
	std::cout << t << std::endl;
	m_fullgraph = t;
}


void MainWindow::createLayouts()
{
	QSplitter* main_spl = new QSplitter(Qt::Horizontal);
	QSplitter* vert_spl = new QSplitter(Qt::Vertical);

	main_spl->addWidget(view);

	QListWidget* listwidget = new QListWidget(vert_spl);

	listwidget->setMaximumWidth(200);

	item = new QListWidgetItem("Layer_2", listwidget);

	item->setBackground(Qt::DiagCrossPattern);
	item->setBackgroundColor(Qt::blue);
	item->setForeground(Qt::DiagCrossPattern);

	listwidget->addItem(item);

	item = new QListWidgetItem("Layer_3", listwidget);

	item->setBackground(Qt::DiagCrossPattern);
	item->setBackgroundColor(Qt::green);
	item->setForeground(Qt::DiagCrossPattern);


	item = new QListWidgetItem("Layer_4", listwidget);

	item->setBackground(Qt::DiagCrossPattern);
	item->setBackgroundColor(Qt::white);
	item->setForeground(Qt::DiagCrossPattern);

	listwidget->addItem(item);


	item = new QListWidgetItem("Layer_5", listwidget);

	item->setBackground(Qt::DiagCrossPattern);
	item->setBackgroundColor(Qt::magenta);
	item->setForeground(Qt::DiagCrossPattern);

	listwidget->addItem(item);


	listwidget->addItem(item);

	connect(listwidget, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(setcolor(QListWidgetItem*)));

	main_spl->addWidget(listwidget);
	setCentralWidget(main_spl);

}


void MainWindow::reset_selection()
{
	drawLine->setChecked(false);
	drawPol->setChecked(false);
	drawRect->setChecked(false);
}

void MainWindow::setcolor(QListWidgetItem* i)
{
	std::cout << "Act" << std::endl;
	view->setcolor(i->backgroundColor());
}


void MainWindow::analyz()
{
	QGraphicsView* local_view = new GraphicsView(0, m_tree_type);
	QGraphicsScene* l_scene = new QGraphicsScene(local_view);

	QTableWidget* table = new QTableWidget(10, 3);

	QStringList labels;
	labels << tr("Vertices count") << tr("Working time of full graph (sec)") << tr("Working time of RSG (sec)");
	table->setHorizontalHeaderLabels(labels);
	table->verticalHeader()->hide();
	table->horizontalHeader()->resize(700, 300);

	l_scene->setBackgroundBrush(Qt::Dense1Pattern);


	QString str = "Full graph ";
	QString str1 = "Algo graph ";

	double s1, s2;

	double temp1 = 0, temp2 = 0;

	test(100, 5, s1, s2);

	table->setItem(0, 0, new QTableWidgetItem(QString::number(100)));
	table->setItem(0, 1, new QTableWidgetItem(QString::number(s1 / 1000)));
	table->setItem(0, 2, new QTableWidgetItem(QString::number(s2 / 1000)));



	l_scene->addLine(0, 0, 10, -s1 / 10, QPen(Qt::red));
	l_scene->addLine(0, 0, 10, -s2 / 10, QPen(Qt::green));

	temp1 = s1;
	temp2 = s2;

	str += QString::number(s1) + " ";
	str1 += QString::number(s2) + " ";

	test(1000, 2, s1, s2);

	table->setItem(1, 0, new QTableWidgetItem(QString::number(1000)));
	table->setItem(1, 1, new QTableWidgetItem(QString::number(s1 / 1000)));
	table->setItem(1, 2, new QTableWidgetItem(QString::number(s2 / 1000)));


	l_scene->addLine(10, -temp1 / 10, 100, -s1 / 10, QPen(Qt::red));
	l_scene->addLine(10, -temp2 / 10, 100, -s2 / 10, QPen(Qt::green));

	temp1 = s1;
	temp2 = s2;


	str += QString::number(s1) + " ";
	str1 += QString::number(s2) + " ";

	test(2000, 2, s1, s2);

	table->setItem(2, 0, new QTableWidgetItem(QString::number(2000)));
	table->setItem(2, 1, new QTableWidgetItem(QString::number(s1 / 1000)));
	table->setItem(2, 2, new QTableWidgetItem(QString::number(s2 / 1000)));


	l_scene->addLine(100, -temp1 / 10, 200, -s1 / 10, QPen(Qt::red));
	l_scene->addLine(100, -temp2 / 10, 200, -s2 / 10, QPen(Qt::green));

	temp1 = s1;
	temp2 = s2;



	str += QString::number(s1) + " ";
	str1 += QString::number(s2) + " ";


	test(3000, 1, s1, s2);

	table->setItem(3, 0, new QTableWidgetItem(QString::number(3000)));
	table->setItem(3, 1, new QTableWidgetItem(QString::number(s1 / 1000)));
	table->setItem(3, 2, new QTableWidgetItem(QString::number(s2 / 1000)));


	l_scene->addLine(200, -temp1 / 10, 300, -s1 / 10, QPen(Qt::red));
	l_scene->addLine(200, -temp2 / 10, 300, -s2 / 10, QPen(Qt::green));

	temp1 = s1;
	temp2 = s2;


	str += QString::number(s1) + " ";
	str1 += QString::number(s2) + " ";

	test(4000, 1, s1, s2);

	table->setItem(4, 0, new QTableWidgetItem(QString::number(4000)));
	table->setItem(4, 1, new QTableWidgetItem(QString::number(s1 / 1000)));
	table->setItem(4, 2, new QTableWidgetItem(QString::number(s2 / 1000)));



	l_scene->addLine(300, -temp1 / 10, 400, -s1 / 10, QPen(Qt::red));
	l_scene->addLine(300, -temp2 / 10, 400, -s2 / 10, QPen(Qt::green));

	temp1 = s1;
	temp2 = s2;


	str += QString::number(s1) + " ";
	str1 += QString::number(s2) + " ";




	test_only_RSG(5000, 1, s2);


	table->setItem(5, 0, new QTableWidgetItem(QString::number(5000)));
	table->setItem(5, 1, new QTableWidgetItem("Not enough resources."));
	table->setItem(5, 2, new QTableWidgetItem(QString::number(s2 / 1000)));


	test_only_RSG(7000, 1, s2);


	table->setItem(6, 0, new QTableWidgetItem(QString::number(7000)));
	table->setItem(6, 1, new QTableWidgetItem("Not enough resources."));
	table->setItem(6, 2, new QTableWidgetItem(QString::number(s2 / 1000)));



	test_only_RSG(8000, 1, s2);


	table->setItem(7, 0, new QTableWidgetItem(QString::number(8000)));
	table->setItem(7, 1, new QTableWidgetItem("Not enough resources."));
	table->setItem(7, 2, new QTableWidgetItem(QString::number(s2 / 1000)));

	
	//test_only_RSG(10000,1,s2);


	//table->setItem(8,0,new QTableWidgetItem(QString::number(10000)));
	//table->setItem(8,1,new QTableWidgetItem("Not enough resources."));
	//table->setItem(8,2,new QTableWidgetItem(QString::number(s2)));




	QWidget* graphicWidget = new QWidget();


	QBoxLayout* mylayout = new QBoxLayout(QBoxLayout::LeftToRight);


	local_view->setScene(l_scene);

	mylayout->addWidget(local_view);
	mylayout->addWidget(table);

	graphicWidget->setLayout(mylayout);

	graphicWidget->setGeometry(50, 50, 800, 500);

	graphicWidget->show();
}


void MainWindow::test(int count, int s, double& s1, double& s2)
{
	double sum1 = 0;
	double sum2 = 0;

	for (int i = 0;i<s;++i)
	{
		//if (r_points != 0)
		//{
		//	delete r_points;
		//}

		//r_points = new RandomPoints(view->getScene(), m_fullgraph, count);
		m_tree_type = 2;
		QTime time;
		time.start();
		generatetree();
		sum1 += time.elapsed();

		m_tree_type = 1;
		QTime time1;
		time1.start();
		generatetree();

		sum2 = time1.elapsed();
	}
	view->getScene()->clear();
	s1 = sum1 / s;
	s2 = sum2 / s;
}


void MainWindow::test_only_RSG(int count, int s, double& s1)
{
}
