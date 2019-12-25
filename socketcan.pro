TARGET = qtsocketcanbus_v2

QT = core serialbus

HEADERS += \
    src/socketcanbackend_v2.h

SOURCES += \
    src/main.cpp \
    src/socketcanbackend_v2.cpp

DISTFILES = plugin.json

PLUGIN_TYPE = canbus
PLUGIN_CLASS_NAME = SocketCanBusPluginV2
load(qt_plugin)
