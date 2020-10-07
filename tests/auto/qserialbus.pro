QT = core testlib
TARGET = tst_qserialbus
QT += serialbus

#DEFINES += USE_LOCAL_PLUGIN

SOURCES = tst_qserialbus.cpp

INCLUDEPATH += ../../src


contains (DEFINES, USE_LOCAL_PLUGIN): {
QT -= serialbus
QT += core-private
SOURCES += ../../src/socketcanbackend_v2.cpp \
           ../../src/qcanbusdevice_v2.cpp \
           ../../src/qcanbusframe_v2.cpp \
           ../../src/qcanbus_v2.cpp \
           ../../src/qcanbusdeviceinfo_v2.cpp \
           ../../src/qcanbusfactory_v2.cpp

HEADERS += ../../src/socketcanbackend_v2.h \
           ../../src/qcanbusdevice_v2.h \
           ../../src/qcanbusframe_v2.h \
           ../../src/qcanbus_v2.h \
           ../../src/qcanbusdeviceinfo_v2.h \
           ../../src/qcanbusfactory_v2.h
}
