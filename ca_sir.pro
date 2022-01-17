#-------------------------------------------------
#
# Project created by QtCreator 2014-06-17T00:18:16
#
#-------------------------------------------------

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = casim
TEMPLATE = app

SOURCES += \
    main.cpp\
    mainwindow.cpp \
    model.cpp \
    modelproperties.cpp \
    cellviewer.cpp \
    fsmmodel.cpp \
    sirmodel.cpp \
    fsmmodelinfo.cpp \
    modelgraph.cpp \
    choleramodel.cpp \
    hexagonalgeometrymodel.cpp

HEADERS += \
    mainwindow.hpp \
    model.hpp \
    modelproperties.h \
    fsmmodel.hpp \
    sirmodel.hpp \
    cellviewer.hpp \
    fsmmodelinfo.hpp \
    modelgraph.hpp \
    choleramodel.hpp \
    hexagonalgeometrymodel.hpp

FORMS += \
    mainwindow.ui \
    cellinfo.ui \
    modelgraph.ui \
    model.ui

LIBS += -lqwt-qt5

INCLUDEPATH += /usr/include/qwt

QMAKE_CXXFLAGS += -std=c++11

RESOURCES += \
    tango-icons.qrc
