QT       += core gui sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++17

SOURCES += $$files(./*.cpp)
HEADERS += $$files(./*.h)
FORMS   += $$files(./*.ui)