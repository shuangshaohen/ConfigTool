QT -= core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib
DEFINES += OUTPUT_LIBRARY

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

MOC_DIR = ../temp/OutputWnd/moc/
UI_DIR = ../temp/OutputWnd/uic/
OBJECTS_DIR = ../temp/OutputWnd/obj/

QMAKE_LIBDIR = ../bin

INCLUDEPATH += $$PWD/../bin ../DataBase
DEPENDPATH += $$PWD/../bin

TARGET = Output
DESTDIR = ../bin

SOURCES += \
    COutputPageWnd.cpp \
    COutputWnd.cpp

HEADERS += \
    COutputPageWnd.h \
    Output_global.h \
    COutputWnd.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/ -lDataBase
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/ -lDataBase
else:unix: LIBS += -L$$PWD/../bin/ -lDataBase

FORMS += \
    OutputWnd.ui

RESOURCES += \
    ../Icon/Icon.qrc
