QT       += core gui sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++17

SOURCES += main.cpp bankapp.cpp database.cpp loginwidget.cpp registerwidget.cpp transferdialog.cpp
HEADERS += bankapp.h database.h loginwidget.h registerwidget.h transferdialog.h
FORMS   += bankapp.ui loginwidget.ui registerwidget.ui transferdialog.ui