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
    CActRecWnd.cpp \
    CAlmRecWnd.cpp \
    CAnaAttrDialog.cpp \
    CAnaPhaseDialog.cpp \
    CAnaTypeDialog.cpp \
    CAnaWnd.cpp \
    CBIAttrDialog.cpp \
    CBIIndexDPSDialog.cpp \
    CBITypeDialog.cpp \
    CBIWnd.cpp \
    CCfgActions.cpp \
    CCfgCenterWnd.cpp \
    CCfgMainWnd.cpp \
    CCfgToolBox.cpp \
    CDefaultWnd.cpp \
    CGSBIAttrDialog.cpp \
    CGSBIWnd.cpp \
    CGSBOAttrDialog.cpp \
    CGSBOSrcTabDialog.cpp \
    CGSBOWnd.cpp \
    CResourceTreeWnd.cpp \
    CSettingWnd.cpp \
    CSoftYBWnd.cpp \
    CTripMatrixWnd.cpp \
    main.cpp \

HEADERS += \
    CActRecWnd.h \
    CAlmRecWnd.h \
    CAnaAttrDialog.h \
    CAnaPhaseDialog.h \
    CAnaTypeDialog.h \
    CAnaWnd.h \
    CBIAttrDialog.h \
    CBIIndexDPSDialog.h \
    CBITypeDialog.h \
    CBIWnd.h \
    CCfgActions.h \
    CCfgCenterWnd.h \
    CCfgToolBox.h \
    CDefaultWnd.h \
    CGSBIAttrDialog.h \
    CGSBIWnd.h \
    CGSBOAttrDialog.h \
    CGSBOSrcTabDialog.h \
    CGSBOWnd.h \
    CMainWnd.h \
    CDataBase.h \
    CCfgMainWnd.h \ \
    CResourceTreeWnd.h \
    CSettingWnd.h \
    CSoftYBWnd.h \
    CTripMatrixWnd.h

FORMS += \
    ActRecWnd.ui \
    AlmRecWnd.ui \
    AnaAttrDialog.ui \
    AnaPhaseDialog.ui \
    AnaTypeDialog.ui \
    AnaWnd.ui \
    BIAttrDialog.ui \
    BIIndexDPSDialog.ui \
    BITypeDialog.ui \
    BIWnd.ui \
    DefaultWnd.ui \
    GSBIAttrDialog.ui \
    GSBIWnd.ui \
    GSBOAttrDialog.ui \
    GSBOSrcTabDialog.ui \
    GSBOWnd.ui \
    SettingWnd.ui \
    SoftYBWnd.ui \
    ToolBox.ui \
    TripMatrixWnd.ui

TRANSLATIONDebugS += \
    ConfigTool_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/ -lCommGui -lDataBase
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/ -lCommGui -lDataBase
else:unix: LIBS += -L$$PWD/../bin/ -lCommGui -lDataBase


