QT       += core gui openglwidgets

include(../libs/QtGifImage/gifimage/qtgifimage.pri)
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
QMAKE_CXXFLAGS += -g

#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    back/AffineApplier.cc \
    back/SettingsHandler.cc \
    main.cc \
    back/Model.cc \
    back/ObjectFile.cc \
    back/Controller.cc \
    front/GLView.cc \
    front/MainWindow.cc \
    front/MenuView.cc

HEADERS += \
    back/AffineApplier.hh \
    back/Model.hh \
    back/ObjectFile.hh \
    back/ObserverInterface.hh \
    back/Controller.hh \
    back/SettingsHandler.hh \
    front/GLView.hh \
    front/MainWindow.hh \
    front/MenuView.hh

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
