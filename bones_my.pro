#-------------------------------------------------
#
# Project created by QtCreator 2015-01-01T21:24:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets \
opengl

TARGET = bones_my
TEMPLATE = app


SOURCES += main.cpp\
        crgpic.cpp \
    cmainwidget.cpp \
    qmyscene.cpp \
    cglwidget.cpp

HEADERS  += crgpic.h \
    cmainwidget.h \
    qmyscene.h \
    cglwidget.h

LIBS += -lOpengl32
