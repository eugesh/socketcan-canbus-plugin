QT = core testlib serialbus
TARGET = write_test
CONFIG += testcase
CONFIG -= app_bundle
QT += core-private

CONFIG += c++11 debug

INCLUDEPATH += $$PWD/../src
#QTPLUGIN += qtcanbustestgeneric

HEADERS += \
           $$PWD/../src/socketcanbackend_v2.h \
           can_test_common.h \
           ./../src/qcanbusdevice_v2.h
           #./../src/qcanbusdevice_v2_p.h

SOURCES += write_test.cpp \
           ./../src/socketcanbackend_v2.cpp \
           ./../src/qcanbusdevice_v2.cpp
