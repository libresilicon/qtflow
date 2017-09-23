#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent):
	m_mainWindow(parent),
	m_fAngle1(20.0f),
	m_fAngle2(20.0f),
	m_fScale(1.0),
	m_offsetX(0),
	m_offsetY(0),
	magicdata(NULL),
	lefdata(NULL),
	project(NULL)
{
	setFocusPolicy(Qt::StrongFocus);
	grabKeyboard();
}

void GLWidget::loadFile(QString file)
{
	layer_rects_t layers;
	rects_t layer;
	QString filedest;
	QTemporaryDir temporaryDir;
	filePath = file;
	if(magicdata) delete magicdata;
	magicdata = new magic::MagicData(file);

	if(project->getTechnology()==magicdata->getTechnology()) {
		if(lefdata) delete lefdata;
		lefdata = new lef::LEFData();
		foreach(QString filename, project->getProcessFiles()) {
			filedest = temporaryDir.path()+"/cells.lef";
			QFile::copy(filename, filedest);
			if(QFile(filedest).exists()) {
				lefdata->loadFile(filedest);
			}
		}
	}

	layers = magicdata->getRectangles();

	m_highestX = 0;
	m_highestY = 0;
	foreach(QString layerN, layers.keys()) {
		layer = layers[layerN];
		foreach (rect_t e, layer)
		{
			if(e.x1 > m_highestX) {
				m_highestX = e.x1;
			}
			if(e.x2 > m_highestX) {
				m_highestX = e.x2;
			}
			if(e.y1 > m_highestY) {
				m_highestY = e.y1;
			}
			if(e.y2 > m_highestY) {
				m_highestY = e.y2;
			}
		}
	}

	m_lowestX = m_highestX;
	m_lowestY = m_highestY;
	foreach(QString layerN, layers.keys()) {
		layer = layers[layerN];
		foreach (rect_t e, layer)
		{
			if(e.x1 < m_lowestX) {
				m_lowestX = e.x1;
			}
			if(e.x2 < m_lowestX) {
				m_lowestX = e.x2;
			}
			if(e.y1 < m_lowestY) {
				m_lowestY = e.y1;
			}
			if(e.y2 < m_lowestY) {
				m_lowestY = e.y2;
			}
		}
	}

}

void GLWidget::paintGL()
{
	QColor color;
	rects_t layer;
	layer_rects_t layers = magicdata->getRectangles();

	foreach(QString layerN, layers.keys()) {
		color = project->colorMat(layerN);
		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(color.redF(), color.greenF(), color.blueF());
		layer = layers[layerN];
		foreach (rect_t e, layer) {
			addWire(layerN, e.x1, e.y1, e.x2, e.y2);
		}
	}

	glRotatef( 1, 0.0f, 1.0f, 0.0f );
	glRotatef( 1, 0.0f, 0.0f, 1.0f );

	update();
}

void GLWidget::initializeGL()
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glEnable(GL_POLYGON_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	qglClearColor( Qt::white );
}

void GLWidget::addWire(QString layerN, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
	qreal m_wireScaleX = m_highestX-m_lowestX;
	qreal m_wireScaleY = m_highestY-m_lowestY;

	qreal z = project->posMat(layerN);
	qreal th = project->thicknessMat(layerN);

	x1-=m_lowestX;
	y1-=m_lowestY;
	x2-=m_lowestX;
	y2-=m_lowestY;

	x1-=m_wireScaleX/2;
	y1-=m_wireScaleY/2;
	x2-=m_wireScaleX/2;
	y2-=m_wireScaleY/2;

	x1/=m_wireScaleX;
	y1/=m_wireScaleY;
	x2/=m_wireScaleX;
	y2/=m_wireScaleY;

	x1*=m_fScale;
	y1*=m_fScale;
	x2*=m_fScale;
	y2*=m_fScale;

	x1+=m_offsetX;
	x2+=m_offsetX;
	y1+=m_offsetY;
	y2+=m_offsetY;

	//qDebug() << __FUNCTION__ << '\t' << x1 << '\t' << y1 << '\t' << x2 << '\t' << y2;

	// bottom
	glBegin(GL_POLYGON);
	glVertex3f( x1, y1, z );
	glVertex3f( x1, y2, z );
	glVertex3f( x2, y2, z );
	glVertex3f( x2, y1, z );
	glEnd();

	// top
	glBegin(GL_POLYGON);
	glVertex3f( x1, y1, z+th );
	glVertex3f( x1, y2, z+th );
	glVertex3f( x2, y2, z+th );
	glVertex3f( x2, y1, z+th );
	glEnd();

	// side 1
	glBegin(GL_POLYGON);
	glVertex3f( x1, y1, z+th );
	glVertex3f( x2, y1, z+th );
	glVertex3f( x2, y1, z );
	glVertex3f( x1, y1, z );
	glEnd();

	// side 2
	glBegin(GL_POLYGON);
	glVertex3f( x1, y1, z+th );
	glVertex3f( x1, y2, z+th );
	glVertex3f( x1, y2, z );
	glVertex3f( x1, y1, z );
	glEnd();

	// side 3
	glBegin(GL_POLYGON);
	glVertex3f( x1, y2, z+th );
	glVertex3f( x2, y2, z+th );
	glVertex3f( x2, y2, z );
	glVertex3f( x1, y2, z );
	glEnd();
}

void GLWidget::setProject(Project *p)
{
	project = p;
}

void GLWidget::resizeGL(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	int dx = event->x() - lastPos.x();
	int dy = event->y() - lastPos.y();

	if (event->buttons() & Qt::LeftButton) {
		m_fAngle1+=dx;
		m_fAngle2+=dy;
	}

	update();

   lastPos = event->pos();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
	int numSteps = event->delta()/120;
	if( event->modifiers() & Qt::ShiftModifier ) {
		m_offsetX+=numSteps;
	} else if( event->modifiers() & Qt::ControlModifier ) {
		m_offsetY+=numSteps;
	} else {
		if((m_fScale-numSteps)>0) {
			m_fScale-=numSteps;
			event->accept();
		}
	}

	update();
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
	switch(event->key()) {
		case Qt::Key_Left:
			if(event->modifiers() & Qt::ShiftModifier) {
				m_offsetX--;
			} else {
				m_fAngle1--;
			}
			break;
		case Qt::Key_Right:
			if(event->modifiers() & Qt::ShiftModifier) {
				m_offsetX++;
			} else {
				m_fAngle1++;
			}
			break;
		case Qt::Key_Down:
			if(event->modifiers() & Qt::ShiftModifier) {
				m_offsetY--;
			} else {
				m_fAngle2--;
			}
			break;
		case Qt::Key_Up:
			if(event->modifiers() & Qt::ShiftModifier) {
				m_offsetY++;
			} else {
				m_fAngle2++;
			}
			break;
		case Qt::Key_Plus:
			m_fScale++;
			break;
		case Qt::Key_Minus:
			m_fScale--;
			break;
	}
	update();
}
