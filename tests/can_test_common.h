#ifndef CAN_TEST_COMMON_H
#define CAN_TEST_COMMON_H

// #include <QtSerialBus/qcanbusdevice.h>
#include "qcanbusdevice_v2.h"
#include <QtSerialBus/qcanbusframe.h>
#include <QtSerialBus/qcanbus.h>
#include <QtSerialBus/qcanbusfactory.h>
#include <QString>
#include <QtTest/qtest.h>

// Returns port name.
QString find_can_device () {
    QString errorMessage;
    QList<QCanBusDeviceInfo> infos = QCanBus::instance()->availableDevices("socketcan", &errorMessage);
    QList<QCanBusDeviceInfo> recommended_devices;

    if (infos.empty()) {
        qCritical() << "No CAN devices available!";
        return "";
    }

    for (int i = 0; i < infos.size(); ++i) {
        QCanBusDeviceInfo info = infos[i];
        if (info.name().compare(QString("%1").arg("can0"), Qt::CaseInsensitive) == 0) {
            qInfo() << "\ncan0 Interface will be used";
            return QString("can0");
        }
    }

    for (int i = 0; i < infos.size(); ++i) {
        QCanBusDeviceInfo info = infos[i];
        if (info.name().compare(QString("%1").arg("vcan0"), Qt::CaseInsensitive) == 0) {
            qInfo() << "\nvcan0 Interface will be used";
            return QString("vcan0");
        }
    }

    for (int i = 0; i < infos.size(); ++i) {
        QCanBusDeviceInfo info = infos[i];
        if (info.name().compare(QString("%1").arg("slcan0"), Qt::CaseInsensitive) == 0) {
            qInfo() << "\nslcan0 Interface will be used";
            return QString("slcan0");
        }
    }

    return "";
}


#endif // CAN_TEST_COMMON_H
