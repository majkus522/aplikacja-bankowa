QT       += core gui sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++17

SOURCES += main.cpp $$files(./source/*.cpp)
HEADERS += $$files(./headers/*.h)
FORMS   += $$files(./gui/*.ui)