QT = core testlib
TARGET = tst_qserialbus
#CONFIG += testcase

QT += serialbus

#DEFINES += USE_LOCAL_PLUGIN

#INCLUDEPATH += ../../socketcan-canbus-plugin/src
INCLUDEPATH += ../../src

SOURCES = tst_qserialbus.cpp

contains (DEFINES, USE_LOCAL_PLUGIN): {
#QT -= serialbus
SOURCES += ../../src/socketcanbackend_v2.cpp

HEADERS += ../../src/socketcanbackend_v2.h
}
