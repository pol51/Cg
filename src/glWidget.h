#ifndef __GLWIDGET_H
#define __GLWIDGET_H

#include <QGLWidget>
#include <QTimer>

#include <Cg/cg.h>
#include <Cg/cgGL.h>

class GlWidget : public QGLWidget
{
	Q_OBJECT
	
	protected:
		QTimer _refreshTimer;
		CGcontext _context;
		CGprogram _vertex_program, _fragment_program;
		CGprofile _vertex_profile, _fragment_profile;
		CGparameter	_position_v, _color_v, _texCoord_v,
			_modelViewMatrix_v,	_twist_v, _texCoord_f, _decal_f;
		double _angle;
		double _twist;
		int _parts;
		bool _vertex_on;
		bool _fragment_on;
	
	public:
		GlWidget(QWidget *parent = NULL);
		~GlWidget();
	
		QSize sizeHint() const;
	
	protected:
		void initializeGL();
		void paintGL();
		void resizeGL(int width, int height);
		void paintEvent(QPaintEvent *event);
	
	public slots:
		void setAngle(int angle);
		void setTwist(int twist);
		void setParts(int parts);
		void setVertex(int state);
		void setFragment(int state);
};

#endif
