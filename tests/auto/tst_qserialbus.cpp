/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Evgeny Shtanov
** Denis Shienkov
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtSerialBus module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/
#ifdef USE_LOCAL_PLUGIN
#include "qcanbusdevice_v2.h"
#include "qcanbusframe_v2.h"
#include "qcanbus_v2.h"
#include "qcanbusfactory_v2.h"
#include "socketcanbackend_v2.h"
#else
#include <QtSerialBus/qcanbus.h>
#include <QtSerialBus/qcanbusdevice.h>
#include <QtSerialBus/qcanbusdeviceinfo.h>
#include <QtSerialBus/qcanbusfactory.h>
#include <QtSerialBus/qcanbusframe.h>
#endif
#include <QtTest/QtTest>

#define MAX_TIMEOUT 1000000

class tst_QSerialBus : public QObject
{
    Q_OBJECT
public:
    explicit tst_QSerialBus();

    static void enterLoop(int secs)
    {
        ++loopLevel;
        QTestEventLoop::instance().enterLoop(secs);
        --loopLevel;
    }

    static void enterLoopMsecs(int msecs)
    {
        ++loopLevel;
        QTestEventLoop::instance().enterLoopMSecs(msecs);
        --loopLevel;
    }

    static void exitLoop()
    {
        if (loopLevel > 0)
            QTestEventLoop::instance().exitLoop();
    }

    static bool timeout()
    {
        return QTestEventLoop::instance().timeout();
    }

protected slots:
    void on_error_occured(QCanBusDevice::CanBusError err);

private slots:
    void initTestCase();
    void createDevice();
    void ReadWriteLoop();

private:
    QString m_sender = "vcan0";
    QString m_receiver = "vcan0";
    QString m_plugin = "socketcan";
    QCanBusDevice * m_canDeviceR;
    QCanBusDevice * m_canDeviceW;
    static int loopLevel;
};

int tst_QSerialBus::loopLevel = 0;

tst_QSerialBus::tst_QSerialBus()
{

}

void tst_QSerialBus::initTestCase()
{
    qputenv("QTEST_FUNCTION_TIMEOUT", QByteArray("90000000"));
    m_sender = QString::fromLocal8Bit(qgetenv("QTEST_SERIALBUS_SENDER"));
    m_receiver = QString::fromLocal8Bit(qgetenv("QTEST_SERIALBUS_RECEIVER"));
    m_plugin = QString::fromLocal8Bit(qgetenv("QTEST_SERIALBUS_PLUGIN_NAME"));

    if (m_sender.isEmpty() || m_receiver.isEmpty()) {
        static const char message[] =
              "Test doesn't work because the names of CAN ports aren't found in env.\n"
              "Please set environment variables:\n"
              " QTEST_SERIALBUS_SENDER to name of output CAN port\n"
              " QTEST_SERIALBUS_RECEIVER to name of input CAN port\n"
              " QTEST_SERIALBUS_PLUGIN_NAME to name CAN plugin (backend)\n"
              "Specify short names of port, like: vcan0\n"
              "Specify name of backend, like: socketcan\n";

        QSKIP(message);
    }

    qDebug() << "m_senderPortName = " << m_sender;
    qDebug() << "m_receiverPortName = " << m_receiver;
}

void tst_QSerialBus::createDevice()
{
    QString errorString;

#ifdef USE_LOCAL_PLUGIN
    m_canDeviceR = new SocketCanBackend_v2(m_receiver);
    m_canDeviceW = new SocketCanBackend_v2(m_sender);
#else
    m_canDeviceR = QCanBus::instance()->createDevice(Plugin, m_receiver, &errorString);
    m_canDeviceW = QCanBus::instance()->createDevice(Plugin, m_sender, &errorString);
#endif
    QVERIFY (m_canDeviceR && m_canDeviceW);

    if (! m_canDeviceR ) {
        qCritical() << "Error: QSocketCAN_connector::HW_init: Socket wasn't initialized! Error string: " << errorString;
        qInfo() << "Plugin: " << m_plugin << "Port" << m_receiver;
        return;
    }

    if (! m_canDeviceW ) {
        qCritical() << "Error: QSocketCAN_connector::HW_init: Socket wasn't initialized! Error string: " << errorString;
        qInfo() << "Plugin: " << m_plugin << "Port" << m_sender;
        return;
    }

    if (m_canDeviceR->state() == QCanBusDevice::UnconnectedState)
        m_canDeviceR->connectDevice();

    if (m_canDeviceR->state() == QCanBusDevice::UnconnectedState)  {
        qCritical() << "Error: Read Socket wasn't initialized!";
    }

    if (m_canDeviceW->state() == QCanBusDevice::UnconnectedState)
        m_canDeviceW->connectDevice();

    if (m_canDeviceW->state() == QCanBusDevice::UnconnectedState)  {
        qCritical() << "Error: Write Socket wasn't initialized!";
        return;
    }

    connect (m_canDeviceW, &QCanBusDevice::errorOccurred, this, &tst_QSerialBus::on_error_occured);
}

void
tst_QSerialBus::on_error_occured(QCanBusDevice::CanBusError err) {
    QString err_string;
    switch (err) {
        case QCanBusDevice::NoError:
            err_string = "NoError";
            break;
        case QCanBusDevice::ReadError :
            err_string = "ReadError";
            break;
        case QCanBusDevice::WriteError:
            err_string = "WriteError";
            break;
        case QCanBusDevice::ConnectionError:
            err_string = "ConnectionError";
            break;
        case QCanBusDevice::ConfigurationError:
            err_string = "ConfigurationError";
            break;
        case QCanBusDevice::UnknownError:
            err_string = "UnknownError";
            break;
    }

    err_string.append(QString(", Errno: %1. Do you use the latest patchset of socketcan plugin?").arg(errno));

    QFAIL(err_string.toStdString().c_str());
}

void tst_QSerialBus::ReadWriteLoop() {
    long unsigned int currentReadFrameNumber = 0;
    //long unsigned int
    qulonglong currentWriteFrameNumber = 0;
    const int kMaxFramesCount = 100500;
    QCanBusFrame frameW;
    frameW.setFrameId(0x123);

    auto frameWriter = [&]() {
        if (currentWriteFrameNumber >= kMaxFramesCount)
            return;
        QByteArray payload = QByteArray::number(++currentWriteFrameNumber, 16);
        frameW.setPayload(payload); // Add current frame number to payload
        m_canDeviceW->writeFrame(frameW);
     };

    connect(m_canDeviceW, &QCanBusDevice::framesWritten, frameWriter);
    /*connect(m_canDeviceW, &QCanBusDevice::errorOccurred, [on_write_error]() {
        QFAIL("");
    });*/

    auto frameReader = [&]() {
        const auto frames = m_canDeviceR->readAllFrames();
        for (const auto &frame : frames) {
            QCOMPARE(frame.frameId(), 0x123);
            bool *ok = nullptr;
            const qulonglong frameNumber = frame.payload().toULongLong(ok, 16); // extract frame number from payload
            QCOMPARE(frameNumber, ++currentReadFrameNumber);
        }

        if (currentReadFrameNumber == kMaxFramesCount)
             exitLoop();
     };

    connect(m_canDeviceR, &QCanBusDevice::framesReceived, frameReader);
    connect(m_canDeviceR, &QCanBusDevice::errorOccurred, this, &tst_QSerialBus::on_error_occured); //( ) {
        //QFAIL();
    //});

    frameWriter();

    enterLoop(MAX_TIMEOUT);

    qInfo() << "write_count = " << currentWriteFrameNumber;
    qInfo() << "read_count = " << currentReadFrameNumber;
    QVERIFY (currentWriteFrameNumber == currentReadFrameNumber);
}

QTEST_MAIN(tst_QSerialBus)

#include "tst_qserialbus.moc"
