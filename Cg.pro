TEMPLATE =		app

TARGET =		Cg_Test

DEPENDPATH =	src \
				.
INCLUDEPATH =	src \
				.

QT +=			opengl
LIBS +=			-lCg -lCgGL

HEADERS =		src/glWidget.h
SOURCES =		src/glWidget.cpp \
				src/main.cpp
CONFIG +=		release \
				warn_on \
				qt \
				console \
				gui
RESOURCES =		res/textures.qrc
DESTDIR =		bin
MOC_DIR =		build/moc
UI_DIR =		build/uic
OBJECTS_DIR =	build/obj
RCC_DIR =		build/rcc