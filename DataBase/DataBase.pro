QT -= gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += xml sql

TEMPLATE = lib
DEFINES += DATABASE_LIBRARY

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

MOC_DIR = ../temp/DataBase/moc/
UI_DIR = ../temp/DataBase/uic/
OBJECTS_DIR = ../temp/DataBase/obj/

QMAKE_LIBDIR = ../bin

TARGET = DataBase
DESTDIR = ../bin

SOURCES += \
    CDataBase.cpp \
    CDataSQL.cpp \
    CMsgInfo.cpp

HEADERS += \
    CDataSQL.h \
    CMsgInfo.h \
    DataBase_global.h \
    CDataBase.h \
    GSEConfig.h

TRANSLATIONS += \
    DataBase_zh_CN.ts

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
