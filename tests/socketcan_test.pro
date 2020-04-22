QT -= gui
QT = core testlib #serialbus
TARGET = write_test
CONFIG += testcase
CONFIG -= app_bundle
QT += core-private

CONFIG += c++11 console debug

INCLUDEPATH += $$PWD/../src
INCLUDEPATH += $$PWD
#QTPLUGIN += qtcanbustestgeneric

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
           ../src/socketcanbackend_v2.h \
           can_test_common.h \
           ../src/qcanbusdevice_v2.h \
           ../src/qcanbusframe_v2.h \
           ../src/qcanbus_v2.h \
           ../src/qcanbusdeviceinfo_v2.h \
           ../src/qcanbusfactory_v2.h

SOURCES += \
           main.cpp \
           #write_test.cpp \
           ../src/socketcanbackend_v2.cpp \
           ../src/qcanbusdevice_v2.cpp \
           ../src/qcanbusframe_v2.cpp \
           ../src/qcanbus_v2.cpp \
           ../src/qcanbusdeviceinfo_v2.cpp \
           ../src/qcanbusfactory_v2.cpp

DISTFILES += \
    CMakeLists.txt
