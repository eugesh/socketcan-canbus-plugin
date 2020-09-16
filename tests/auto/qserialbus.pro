QT = core testlib
TARGET = tst_qserialbus
#CONFIG += testcase

QT += serialbus

#INCLUDEPATH += ../../socketcan-canbus-plugin/src
INCLUDEPATH += ../../src

SOURCES = tst_qserialbus.cpp

SOURCES += ../../src/socketcanbackend_v2.cpp

HEADERS += ../../src/socketcanbackend_v2.h
