TARGET = qtsocketcanbus

QT = core serialbus

HEADERS += \
    src/socketcanbackend.h

SOURCES += \
    src/main.cpp \
    src/socketcanbackend.cpp

DISTFILES = plugin.json

PLUGIN_TYPE = canbus
PLUGIN_CLASS_NAME = SocketCanBusPlugin
load(qt_plugin)
