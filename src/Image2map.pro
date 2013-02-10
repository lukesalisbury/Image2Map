#-------------------------------------------------
#
# Project created by QtCreator 2010-07-10T01:06:04
#
#-------------------------------------------------

QT       += core gui widgets xml

TARGET = Image2map
TEMPLATE = app


SOURCES += main.cpp\
		mainwindow.cpp \
	imageconvert.cpp \
	mapobject.cpp \
	mapsprite.cpp \
	table.cpp \
	imagedelegate.cpp \
	spritesheet.cpp \
	spritematchdialog.cpp \
	spritechooser.cpp \
	reversematchwindow.cpp \
	nexttiledialog.cpp \
	colourmatchdialog.cpp \
	displayobject.cpp

HEADERS  += mainwindow.hpp \
	imageconvert.hpp \
	mapobject.hpp \
	mapsprite.hpp \
	table.hpp \
	tableitem.hpp \
	imagedelegate.hpp \
	spritesheet.hpp \
	spritematchdialog.hpp \
	spritechooser.hpp \
	reversematchwindow.h \
	nexttiledialog.hpp \
	colourmatchdialog.h \
	displayobject.hpp

FORMS    += mainwindow.ui \
	spritematchdialog.ui \
	spritechooser.ui \
	reversematchwindow.ui \
	nexttiledialog.ui \
	colourmatchdialog.ui
