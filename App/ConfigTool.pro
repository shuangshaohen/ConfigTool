QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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

MOC_DIR = ../temp/cfgtool/moc
UI_DIR = ../temp/cfgtool/uic
OBJECTS_DIR = ../temp/cfgtool/obj

QMAKE_LIBDIR = ../bin
INCLUDEPATH += $$PWD/../bin ../CommGui ../DataBase
DEPENDPATH += $$PWD/../bin

TARGET = CfgTool
DESTDIR = ../bin

SOURCES += \
    CCfgActions.cpp \
    CCfgCenterWnd.cpp \
    CCfgMainWnd.cpp \
    CCfgToolBox.cpp \
    CDefaultWnd.cpp \
    CResourceTreeWnd.cpp \
    main.cpp \

HEADERS += \
    CCfgActions.h \
    CCfgCenterWnd.h \
    CCfgToolBox.h \
    CDefaultWnd.h \
    CMainWnd.h \
    CDataBase.h \
    CCfgMainWnd.h \ \
    CResourceTreeWnd.h

FORMS += \
    DefaultWnd.ui \
    ToolBox.ui

TRANSLATIONDebugS += \
    ConfigTool_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/ -lCommGui -lDataBase
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/ -lCommGui -lDataBase
else:unix: LIBS += -L$$PWD/../bin/ -lCommGui -lDataBase

RESOURCES += \
    src.qrc


