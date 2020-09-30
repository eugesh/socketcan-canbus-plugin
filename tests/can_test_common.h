#ifndef CAN_TEST_COMMON_H
#define CAN_TEST_COMMON_H

// #include <QtSerialBus/qcanbusdevice.h>
// #include <QtSerialBus/qcanbusframe.h>
// #include <QtSerialBus/qcanbus.h>
// #include <QtSerialBus/qcanbusfactory.h>
#include "qcanbusdevice_v2.h"
#include "qcanbusframe_v2.h"
#include "qcanbus_v2.h"
#include "qcanbusfactory_v2.h"
#include <QString>
#include <QtTest/qtest.h>

QList<QCanBusFrame>
read_long_file(QString const& path) {
    QList<QCanBusFrame> frames;

    QFile file(path);

    if (!file.open(QFile::ReadOnly)) {
        QString msg = QString("Failed to open %1\n%2")
                             .arg(path)
                             .arg(file.errorString());
        qWarning() << QString("Error") + msg;
        return QList<QCanBusFrame>();
    }

    while (!file.atEnd()) {
        QByteArray line = file.readLine().simplified();

        auto fields = line.split(' ');

        int canId = fields[1].toUInt(nullptr, 16);
        QByteArray len_arr = fields[2];
        len_arr.replace('[', "");
        len_arr.replace(']', "");
        int len = len_arr.toUInt();
        QByteArray payload;

        for (int i = 0; i < len; ++i)
            payload.append(QByteArray::fromHex(fields[3 + i]));

        // qDebug() << canId << " [" << len << "] " << payload;

        QCanBusFrame frame = QCanBusFrame(canId, payload);

        frames << frame;
    }

    return frames;
}

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
