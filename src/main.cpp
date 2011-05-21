#include <QApplication>

#include <QWidget>
#include <QGridLayout>
#include <QSlider>
#include <QCheckBox>

#include "glWidget.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
	QWidget mainWindow;
	QSlider sliderAngle(Qt::Horizontal);
	QSlider sliderTwist(Qt::Horizontal);
	QSlider sliderParts(Qt::Horizontal);
	QCheckBox chkVertex("Vertex");
	QCheckBox chkFragment("Fragment");

	sliderAngle.setMinimum(0);
	sliderAngle.setMaximum(360 * 16);
	sliderAngle.setPageStep(16);
	sliderAngle.setValue(0);
	
	sliderTwist.setMinimum(-720 * 16);
	sliderTwist.setMaximum(720 * 16);
	sliderTwist.setPageStep(16);
	sliderTwist.setValue(0);
	
	sliderParts.setMinimum(1);
	sliderParts.setMaximum(512);
	sliderParts.setPageStep(8);
	sliderParts.setValue(128);
	
	GlWidget myGlw;
	
	QGridLayout mainLayout;
	mainLayout.addWidget(&myGlw, 0, 0, 1, 2);
	mainLayout.addWidget(&chkVertex, 1, 0, 1, 2);
	mainLayout.addWidget(&chkFragment, 1, 1, 1, 2);
	mainLayout.addWidget(&sliderAngle, 2, 0, 1, 2);
	mainLayout.addWidget(&sliderTwist, 3, 0, 1, 2);
	mainLayout.addWidget(&sliderParts, 4, 0, 1, 2);
	mainWindow.setLayout(&mainLayout);
	mainWindow.show();
	
	QObject::connect(
		&sliderAngle, SIGNAL(valueChanged(int)),
		&myGlw, SLOT(setAngle(int))
	);
	QObject::connect(
		&sliderTwist, SIGNAL(valueChanged(int)),
		&myGlw, SLOT(setTwist(int))
	);
	QObject::connect(
		&sliderParts, SIGNAL(valueChanged(int)),
		&myGlw, SLOT(setParts(int))
	);
	QObject::connect(
		&chkVertex, SIGNAL(stateChanged(int)),
		&myGlw, SLOT(setVertex(int))
	);
	QObject::connect(
		&chkFragment, SIGNAL(stateChanged(int)),
		&myGlw, SLOT(setFragment(int))
	);
	
	chkVertex.setChecked(true);
	chkFragment.setChecked(true);
	
	return app.exec();
}
