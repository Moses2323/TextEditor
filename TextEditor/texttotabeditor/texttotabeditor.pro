include(../include/build.pri)

TEMPLATE = lib
QT += widgets core gui
CONFIG += c++14

DEFINES += TEXT_TO_TAB_EDITOR_LIBRARY

SOURCES += \
	src/filetotabwidget.cpp \
    src/LoaderWidget.cpp \
    src/printelement.cpp \
    src/texttotabeditor.cpp

HEADERS += \
    src/filetotab_priv.h \
    src/printelement.h \
    src/LoaderWidget.h \
    ../include/filetotabwidget.h \
    ../include/texttotabeditorsettings.h \
    ../include/texttotabeditor.h \
    src/formistakes.h

RESOURCES += \
    resources.qrc
