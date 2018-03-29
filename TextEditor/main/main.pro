include(../include/build.pri)

TEMPLATE = app
QT += widgets gui core

CONFIG += c++14

SOURCES += main.cpp

LIBS += \
    -ltexttotabeditor
