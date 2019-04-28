#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "graphicsview.hpp"

#include <QtWidgets/QMainWindow>
#include <QMainWindow>
#include <QtGui>

class QMenu;
class QAction;
class QToolBar;
class QComboBox;
class QCheckBox;
class QListWidgetItem;

class MainWindow
	:public QMainWindow
{
	Q_OBJECT
public:
	MainWindow();
	~MainWindow();
private:
	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *drawMenu;
private:
	QAction *drawLine;
	QAction *drawPol;
	QAction *drawRect;
	QAction *drawPoint;
private:
	QAction *newAction;
	QAction *exitAction;
	QAction *clearAction;

	QAction *findSectsAction;

	QAction *copyAction;
	QAction *cutAction;
	QAction *pasteAction;
	QAction *deleteAction;

	QAction *routing;
	QAction *randompoints;
	
	QAction* pol_rect_tree;
	QComboBox* combo;

	QToolBar *findSectsToolbar;
	QToolBar *fileToolbar;
	QToolBar *editToolbar;
	QToolBar *drawToolbar;
	QToolBar *cordToolbar;


	GraphicsView *view;
	//BlinkThread* blinkthread;

	QListWidgetItem* item;

	//RandomPoints * r_points;

	int m_tree_type;
	bool m_fullgraph;


	QAction * analyze;

private:
	void createMenu();
	void createAction();
	void createToolbars();
	void createNew();
	void createExit();
	void createDrawLine();
	void createDrawPol();
	void createDrawRect();
	void createCopy();
	void createPaste();
	void createCut();
	void createDelete();
	void createLayouts();
	void createRand();
	void test(int count, int s, double& s1, double& s2);
	void test_only_RSG(int count, int s, double& s1);
	private slots:


	void newFile();
	void set_enum();
	void reset();
	void re_1();
	void re_2();
	void re_3();
	void reset_selection();
	void setcolor(QListWidgetItem*);
	void delete_items();
	void copy_items();
	void past_items();
	void cut_items();
	void randomPoints();
	void set_tree_type(int);
	void generatetree();
	void generategraph();
	void setfullgraph(bool);

	void analyz();
	friend GraphicsView;
};

#endif // MAINWINDOW_HPP