QT -= gui
QT = core testlib serialbus
QT += core-private
TARGET = read_test
CONFIG += testcase

CONFIG += c++11 console debug
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/../../src
INCLUDEPATH += $$PWD/../
#QTPLUGIN += qtcanbustestgeneric

HEADERS += \
           $$PWD/../../src/socketcanbackend_v2.h \
           ../can_test_common.h \
           ../../src/qcanbusdevice_v2.h \
           ../../src/qcanbusframe_v2.h \
           ../../src/qcanbus_v2.h \
           ../../src/qcanbusdeviceinfo_v2.h \
           ../../src/qcanbusfactory_v2.h

SOURCES += \
           main.cpp \
           ../../src/socketcanbackend_v2.cpp \
           ../../src/qcanbusdevice_v2.cpp \
           ../../src/qcanbusframe_v2.cpp \
           ../../src/qcanbus_v2.cpp \
           ../../src/qcanbusdeviceinfo_v2.cpp \
           ../../src/qcanbusfactory_v2.cpp
