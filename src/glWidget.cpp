#include "glWidget.h"

#include <QtOpenGL>
#include <QDebug>

GlWidget::GlWidget(QWidget *parent)
    :QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
  _refreshTimer.setSingleShot(false);
  QObject::connect(
    &_refreshTimer, SIGNAL(timeout()),
    this, SLOT(update())
  );
  _refreshTimer.start(20);

    setAttribute(Qt::WA_NoSystemBackground);
    setMinimumSize(480, 480);
    setWindowTitle("Cg Tests");

  _fragment_on = false;
  _vertex_on = false;

  _angle = 0.0;
  _twist = 0.0;
  _parts = 128;
}

GlWidget::~GlWidget()
{
  this->makeCurrent();
}

QSize GlWidget::sizeHint() const
{
  return QSize(480, 480);
}

void GlWidget::initializeGL()
{
  glEnable(GL_DEPTH_TEST);

  if ((_context = cgCreateContext()) == 0)
    qDebug() << "context         [ERROR]";
  else
    qDebug() << "context         [OK]";

  _vertex_profile = cgGLGetLatestProfile(CG_GL_VERTEX);
  _fragment_profile = cgGLGetLatestProfile(CG_GL_FRAGMENT);

  cgGLSetOptimalOptions(_vertex_profile);
  cgGLSetOptimalOptions(_fragment_profile);

  if (_vertex_profile == CG_PROFILE_UNKNOWN)
    qDebug() << "vertex profile  [ERRORS]";
  else
    qDebug() << "vertex profile  [OK]";
  if (_fragment_profile == CG_PROFILE_UNKNOWN)
    qDebug() << "fragment profile[ERRORS]";
  else
    qDebug() << "fragment profile[OK]";

  if ((_vertex_program = cgCreateProgramFromFile(
      _context,
      CG_SOURCE,
      "../shaders/vertex.cg",
      _vertex_profile,
      "C3E2v_varying",
      0)) == 0)
    qDebug() << "vertex shader   [ERROR]";
  else
    qDebug() << "vertex shader   [OK]";
  if ((_fragment_program = cgCreateProgramFromFile(
      _context,
      CG_SOURCE,
      "../shaders/fragment.cg",
      _fragment_profile,
      "C2E2f_passthrough",
      0)) == 0)
    qDebug() << "fragment shader [ERROR]";
  else
    qDebug() << "fragment shader [OK]";

  cgGLLoadProgram(_vertex_program);
  cgGLLoadProgram(_fragment_program);

  _position_v			= cgGetNamedParameter(_vertex_program, "IN.position");
  _color_v			= cgGetNamedParameter(_vertex_program, "IN.color");
  _texCoord_v			= cgGetNamedParameter(_vertex_program, "IN.texCoord");
  _modelViewMatrix_v	= cgGetNamedParameter(_vertex_program, "ModelViewProj");
  _twist_v			= cgGetNamedParameter(_vertex_program, "twisting");

  _texCoord_f		= cgGetNamedParameter(_fragment_program, "texCoord");
  _decal_f		= cgGetNamedParameter(_fragment_program, "decal");

  cgSetParameter4f(_color_v, 0.0, 0.0, 1.0, 1.0);
}

void GlWidget::paintGL()
{
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);

  resizeGL(width(), height());

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

  if (_vertex_on)
    cgGLEnableProfile(_vertex_profile);

  if (_fragment_on)
    cgGLEnableProfile(_fragment_profile);

  cgGLBindProgram(_vertex_program);
  cgGLBindProgram(_fragment_program);

  glPushMatrix();
  glRotatef(_angle, 0.0, 0.0, 1.0);

  cgGLSetStateMatrixParameter(_modelViewMatrix_v,
    CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_IDENTITY);
  cgGLSetParameter1f(_twist_v, _twist);

  GLuint texture;
    texture = bindTexture(QPixmap(QString(":/images/texture.png")));
  glBindTexture(GL_TEXTURE_2D, texture);

  glColor3f(1.0, 1.0, 1.0);
  for (int i = 0; i <= (_parts - 1); i++)
  {
    glBegin(GL_QUAD_STRIP);
      // textures
      float xt1 = ((float)i		/ (float)_parts);
      float xt2 = ((float)(i + 1)	/ (float)_parts);
      // vertices
      float x1 = 1.4 * xt1 - 0.7;
      float x2 = 1.4 * xt2 - 0.7;
      for (int j = 0; j <= _parts; j++)
      {
        float yt = ((float)j / float(_parts));
        float y = 1.4 * yt - 0.7;
        glTexCoord2f(xt2, yt);	glVertex3f(x2,  y, -0.2);
        glTexCoord2f(xt1, yt);	glVertex3f(x1,  y, -0.2);
      }
    glEnd();
  }

  glPopMatrix();

  cgGLDisableProfile(_vertex_profile);
  cgGLDisableProfile(_fragment_profile);

  glPushMatrix();

  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_QUADS);
    glTexCoord2f( 0, 0);	glVertex3f( 0.7,  0.7, -0.2);
    glTexCoord2f( 1, 0);	glVertex3f(-0.7,  0.7, -0.2);
    glTexCoord2f( 1, 1);	glVertex3f(-0.7, -0.7, -0.2);
    glTexCoord2f( 0, 1);	glVertex3f( 0.7, -0.7, -0.2);
  glEnd();

  glPopMatrix();

  glFlush();

  glPopAttrib();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
}

void GlWidget::resizeGL(int width, int height)
{
  int side = qMin(width, height);
  glViewport((width - side) / 2, (height - side) / 2, side, side);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
}

void GlWidget::paintEvent(QPaintEvent */*event*/)
{
  QPainter painter;
    painter.begin(this);

  paintGL();

  painter.end();
}

void GlWidget::setAngle(int angle)
{
  _angle = (double)angle / 16.;
}

void GlWidget::setTwist(int twist)
{
  _twist = (double)twist * 3.141592654 / 16. / 180.;
}

void GlWidget::setParts(int parts)
{
  if (parts > 0)
    _parts = parts;
}

void GlWidget::setVertex(int state)
{
  _vertex_on = (state == Qt::Checked);
}

void GlWidget::setFragment(int state)
{
  _fragment_on = (state == Qt::Checked);
}
