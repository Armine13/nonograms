#-------------------------------------------------
#
# Project created by QtCreator 2015-04-20T18:58:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = nonograms
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gridbutton.cpp \
    nonogram.cpp \
    picturedata.cpp \
    solver.cpp \
    databutton.cpp \
    colorselectionbutton.cpp

HEADERS  += mainwindow.h \
    gridbutton.h \
    nonogram.h \
    picturedata.h \
    solver.h \
    databutton.h \
    colorselectionbutton.h

SUBDIRS += \
    projectv3.pro

RESOURCES += \
    resources.qrc
