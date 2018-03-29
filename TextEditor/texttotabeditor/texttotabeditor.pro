include(../include/build.pri)

TEMPLATE = lib
QT += widgets core gui
CONFIG += c++14

DEFINES += TEXT_TO_TAB_EDITOR_LIBRARY

SOURCES += \
	src/filetotabwidget.cpp \
    src/LoaderWidget.cpp \
    src/printelement.cpp \
    src/shell.cpp

HEADERS += \
    src/filetotab_priv.h \
    src/printelement.h \
    ../include/filetotabwidget.h \
    ../include/LoaderWidget.h \
    ../include/shell.h \
    ../include/toolssettings.h

RESOURCES += \
    resources.qrc
