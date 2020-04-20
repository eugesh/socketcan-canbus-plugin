TARGET = qtsocketcanbus_v2

QT = core serialbus
QT += core-private

HEADERS += \
    src/socketcanbackend_v2.h \
    src/qcanbusdevice_v2.h \
    src/qcanbusdevice_v2_p.h

SOURCES += \
    src/main.cpp \
    src/socketcanbackend_v2.cpp \
    src/qcanbusdevice_v2.cpp

DISTFILES = plugin.json \
    CMakeLists.txt

PLUGIN_TYPE = canbus
PLUGIN_CLASS_NAME = SocketCanBusPluginV2
load(qt_plugin)
