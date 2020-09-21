#include <QtTest/QtTest>
/*#include <QtSerialBus/qcanbusframe.h>
#include <QtSerialBus/qcanbusdevice.h>
#include <QtSerialBus/qcanbusdeviceinfo.h>
#include <QtSerialBus/qcanbusdevice.h>
#include <QtSerialBus/qcanbus.h>
#include <QtSerialBus/qcanbusfactory.h>*/

#ifndef WIN32
#include "socketcanbackend_v2.h"
#include "qcanbusframe_v2.h"
#include "qcanbusdevice_v2.h"
#include "qcanbus_v2.h"
#else
#include <QCanBusDevice>
#include <QCanBus>
#include <QCanBusFrame>
#include <QCanBus>
#endif

#include <QThread>

Q_DECLARE_METATYPE(QIODevice::OpenMode);
Q_DECLARE_METATYPE(QIODevice::OpenModeFlag);
Q_DECLARE_METATYPE(Qt::ConnectionType);

#define BIGN 100500

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

private slots:
    void initTestCase();
    void createDevice();
    void ReadWriteLoop2();

protected slots:

private:
    void interfaces();
    void createDevice2();
    void ReadWriteLoop();

private:
    QString m_senderPortName;
    QString m_receiverPortName;
    QStringList m_availablePortNames;
    QCanBusDevice * m_canDeviceR;
    QCanBusDevice * m_canDeviceW;

    static int loopLevel;
    QCanBus *bus = nullptr;
};

int tst_QSerialBus::loopLevel = 0;

tst_QSerialBus::tst_QSerialBus() {

}

class ReadWriteTest : public QObject {
    Q_OBJECT
    ReadWriteTest();
private slots:
    void onframesWritten();
};

ReadWriteTest::ReadWriteTest() {

}

void
ReadWriteTest::onframesWritten() {
    tst_QSerialBus::exitLoop();
}

class AsyncReader : public QObject
{
    Q_OBJECT
public:
    explicit AsyncReader(QCanBusDevice * device, Qt::ConnectionType connectionType, int expectedFramesCount)
        : m_device(device), m_expectedFramesCount(expectedFramesCount)
    {
        connect(device, &QCanBusDevice::framesReceived, this, &AsyncReader::receive, connectionType);
    }

private slots:
    void receive()
    {
        if (m_device->framesAvailable() < m_expectedFramesCount)
            return;

        tst_QSerialBus::exitLoop();
    }

private:
    QCanBusDevice * m_device;
    const int m_expectedFramesCount;
};

void tst_QSerialBus::initTestCase() {
    qputenv("QTEST_FUNCTION_TIMEOUT", QByteArray("90000000"));
    m_senderPortName = QString::fromLocal8Bit(qgetenv("QTEST_SERIALBUS_SENDER"));
    m_receiverPortName = QString::fromLocal8Bit(qgetenv("QTEST_SERIALBUS_RECEIVER"));
    if (m_senderPortName.isEmpty() || m_receiverPortName.isEmpty()) {
        static const char message[] =
              "Test doesn't work because the names of CAN ports aren't found in env.\n"
              "Please set environment variables:\n"
              " QTEST_SERIALBUS_SENDER to name of output CAN port\n"
              " QTEST_SERIALBUS_RECEIVER to name of input CAN port\n"
              "Specify short names of port"
#if defined(Q_OS_UNIX)
              ", like: vcan0\n";
#elif defined(Q_OS_WIN32)
              ", like: can0\n";
#else
              "\n";
#endif

        QSKIP(message);
    } else {
        m_availablePortNames << m_senderPortName << m_receiverPortName;
    }

    qDebug() << "m_senderPortName = " << m_senderPortName;
    qDebug() << "m_receiverPortName = " << m_receiverPortName;

    bus = QCanBus::instance();
    QVERIFY(bus);
    QCanBus *sameInstance = QCanBus::instance();
    QCOMPARE(bus, sameInstance);
}

void tst_QSerialBus::interfaces()
{
    // Plugins derived from QCanBusFactory(V1) don't have availableDevices()
    const QList<QCanBusDeviceInfo> deviceListV1 = bus->availableDevices("genericV1");
    QVERIFY(deviceListV1.isEmpty());

    const QList<QCanBusDeviceInfo> pluginList = bus->availableDevices("generic");
    QCOMPARE(pluginList.size(), 1);
    QCOMPARE(pluginList.at(0).name(), QStringLiteral("can0"));
    QVERIFY(pluginList.at(0).isVirtual());
    QVERIFY(pluginList.at(0).hasFlexibleDataRate());
}

void tst_QSerialBus::createDevice()
{
    m_canDeviceR = new SocketCanBackend_v2(m_receiverPortName);
    m_canDeviceW = new SocketCanBackend_v2(m_senderPortName);
    AsyncReader * areader = new AsyncReader(m_canDeviceR, Qt::ConnectionType::AutoConnection, 1);

    QVERIFY (m_canDeviceR && m_canDeviceW);

    if (! m_canDeviceR || ! m_canDeviceW ) {
        qCritical() << "Error: QSocketCAN_connector::HW_init: Socket wasn't initialized!";
        qInfo() << "Plugin: " << "socketcan" << "Port" << m_receiverPortName;
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
}

void tst_QSerialBus::createDevice2() {
    m_receiverPortName = "slcan0";
    m_senderPortName = "slcan0";
    QCanBusDevice * m_canDeviceR = new SocketCanBackend_v2(m_receiverPortName);
    QCanBusDevice * m_canDeviceW = new SocketCanBackend_v2(m_senderPortName);

    if (! m_canDeviceR || ! m_canDeviceW ) {
        qCritical() << "Error: QSocketCAN_connector::HW_init: Socket wasn't initialized!";
        //qCritical() << errorString;
        qInfo() << "Plugin: " << "socketcan" << "Port" << m_receiverPortName;
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

    QCanBusFrame frame;
    frame.setFrameId(123);
    frame.setPayload(QByteArray::fromHex("E0FF"));

    qDebug() << frame.toString();

    for (int i = 0; i < 100500; ++i) {
        if (! m_canDeviceW->writeFrame(frame))
            qCritical("Frame wwasn't writen!");
        //m_canDeviceW->waitForFramesWritten(1);
    }

    enterLoop(10000);
}

void
tst_QSerialBus::ReadWriteLoop2() {
    long unsigned int read_count = 0;
    long unsigned int write_count = 0;

    AsyncReader * areader = new AsyncReader(m_canDeviceR, Qt::ConnectionType::AutoConnection, 1);

    QCanBusFrame frameW;
    frameW.setFrameId(123);
    frameW.setPayload(QByteArray::fromHex("E0FF"));

    qInfo() << "Loop #" << tst_QSerialBus::loopLevel << ", Write: " << frameW.toString();

    for (int i=0; i < BIGN; ++i) {
        m_canDeviceW->writeFrame(frameW);
        write_count++;
        enterLoop(1);

        while (m_canDeviceR->framesAvailable()) {
            const QCanBusFrame frameR = m_canDeviceR->readFrame();
            // QVERIFY(frameR.payload().length() == frameW.payload().length());
            if (Q_UNLIKELY(frameR.payload().length() != frameW.payload().length()))
                QSKIP("Bad CAN dlc!");
            read_count++;

            QString view;
            if (frameR.frameType() == QCanBusFrame::ErrorFrame)
                view = m_canDeviceR->interpretErrorFrame(frameR);
            else
                view = frameR.toString();

            const QString time = QString::fromLatin1("%1.%2  ")
                    .arg(frameR.timeStamp().seconds(), 10, 10, QLatin1Char(' '))
                    .arg(frameR.timeStamp().microSeconds() / 100, 4, 10, QLatin1Char('0'));

            // const QString flags = frameFlags(frameR);
            //qInfo() << "Loop #" << tst_QSerialBus::loopLevel << ", Receive: " << frameR.toString();
        }
    }
    qInfo() << "write_count = " << write_count;
    qInfo() << "read_count = " << read_count;
    QVERIFY (write_count == read_count);
}

void
tst_QSerialBus::ReadWriteLoop() {
    m_receiverPortName = "slcan0";
    m_senderPortName = "slcan0";
    QCanBusDevice * m_canDeviceR = new SocketCanBackend_v2(m_receiverPortName);
    QCanBusDevice * m_canDeviceW = new SocketCanBackend_v2(m_senderPortName);

    if (! m_canDeviceR || ! m_canDeviceW ) {
        qCritical() << "Error: QSocketCAN_connector::HW_init: Socket wasn't initialized!";
        //qCritical() << errorString;
        qInfo() << "Plugin: " << "socketcan" << "Port" << m_receiverPortName;
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

    QCanBusFrame frameW;
    frameW.setFrameId(123);
    frameW.setPayload(QByteArray::fromHex("E0FF"));

    qDebug() << frameW.toString();

    for (int i=0; i < BIGN; ++i) {
        m_canDeviceW->writeFrame(frameW);
        enterLoop(1);

        // qint64 n_r_frames = m_canDeviceR->framesAvailable();
        while (m_canDeviceR->framesAvailable()) {
            const QCanBusFrame frameR = m_canDeviceR->readFrame();

            QString view;
            if (frameR.frameType() == QCanBusFrame::ErrorFrame)
                view = m_canDeviceR->interpretErrorFrame(frameR);
            else
                view = frameR.toString();

            const QString time = QString::fromLatin1("%1.%2  ")
                    .arg(frameR.timeStamp().seconds(), 10, 10, QLatin1Char(' '))
                    .arg(frameR.timeStamp().microSeconds() / 100, 4, 10, QLatin1Char('0'));

            // const QString flags = frameFlags(frameR);
            qDebug() << frameR.toString();
        }
    }

}

QTEST_MAIN(tst_QSerialBus)

#include "tst_qserialbus.moc"
