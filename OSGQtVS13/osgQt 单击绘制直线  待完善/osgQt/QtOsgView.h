#ifndef QTOSGVIEW_H  
#define QTOSGVIEW_H  

#include <osgViewer/Viewer>  
#include <osgViewer/CompositeViewer>  
#include <osgViewer/ViewerEventHandlers>  
#include <osgGA/TrackballManipulator>  
#include <osgDB/ReadFile>  
#include <QString>  
#include <QKeyEvent>  
#include <QMouseEvent>  
#include <QWheelEvent>  
#include <QOpenGLWidget>  
#include <QMainWindow>  
#include <iostream>  

#pragma comment(lib,"OpenThreadsd.lib")
#pragma comment(lib,"osgViewerd.lib")
#pragma comment(lib,"osgDBd.lib")
#pragma comment(lib,"osgd.lib")
#pragma comment(lib,"osgGAd.lib")

class AdapterWidget :public QOpenGLWidget
{
public:
	AdapterWidget(QWidget *parent = 0, const char* name = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0);

	virtual ~AdapterWidget(){}

	osgViewer::GraphicsWindow* getGraphicsWindow()
	{
		return mGw.get();
	}

	const osgViewer::GraphicsWindow* getGraphicsWidow()const
	{
		return mGw.get();
	}
protected:
	virtual void resizeGL(int width, int height);
	virtual void keyPressEvent(QKeyEvent* event);
	virtual void keyReleaseEvent(QKeyEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void wheelEvent(QWheelEvent* event);

	osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> mGw;
};

class QtOsgView : public osgViewer::Viewer, public AdapterWidget
{
public:
	QtOsgView(QWidget * parent = 0, const char * name = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0);

	~QtOsgView(){}

	virtual void paintGL();

protected:
	osg::Timer mTimer;          //��ʱ��������֡��  
	double mStrTime;            //��ʼtickʱ��  
	double mEndTime;            //����tickʱ��  

	double mSleepTime;          //��Ҫ˯�ߵ�ʱ��  
	int num;
};



#endif // QTOSGWIDGET_H  