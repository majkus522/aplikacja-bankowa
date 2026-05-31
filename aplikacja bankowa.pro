QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += main.cpp bankapp.cpp database.cpp login.cpp register.cpp
HEADERS += bankapp.h database.h login.h register.h
FORMS   += bankapp.ui login.ui register.ui