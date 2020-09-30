#include <QtTest/QtTest>
#include <QtSerialBus/qcanbusframe.h>
#include <QtSerialBus/qcanbusdevice.h>
#include <QtSerialBus/qcanbusdeviceinfo.h>
#include <QtSerialBus/qcanbus.h>
#include <QtSerialBus/qcanbusfactory.h>

Q_DECLARE_METATYPE(QIODevice::OpenMode)
Q_DECLARE_METATYPE(QIODevice::OpenModeFlag)
Q_DECLARE_METATYPE(Qt::ConnectionType)

#define BIGN 100500

#ifdef WIN32
    const QString Plugin = "virtualcan";
#else
    const QString Plugin = "socketcan";
#endif

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
    void on_write_error(QCanBusDevice::CanBusError err);

private slots:
    void initTestCase();
    void createDevice();
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
}

void tst_QSerialBus::createDevice()
{
    QString errorString;

#ifdef USE_LOCAL_PLUGIN
    m_canDeviceR = new SocketCanBackend_v2(m_receiverPortName);
    m_canDeviceW = new SocketCanBackend_v2(m_senderPortName);
#else
    m_canDeviceR = QCanBus::instance()->createDevice(Plugin, m_receiverPortName, &errorString);
    m_canDeviceW = QCanBus::instance()->createDevice(Plugin, m_senderPortName, &errorString);
#endif
    QVERIFY (m_canDeviceR && m_canDeviceW);

    if (! m_canDeviceR ) {
        qCritical() << "Error: QSocketCAN_connector::HW_init: Socket wasn't initialized! Error string: " << errorString;
        qInfo() << "Plugin: " << Plugin << "Port" << m_receiverPortName;
        return;
    }

    if (! m_canDeviceW ) {
        qCritical() << "Error: QSocketCAN_connector::HW_init: Socket wasn't initialized! Error string: " << errorString;
        qInfo() << "Plugin: " << Plugin << "Port" << m_senderPortName;
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

    connect (m_canDeviceW, &QCanBusDevice::errorOccurred, this, &tst_QSerialBus::on_write_error);
}

void
tst_QSerialBus::on_write_error(QCanBusDevice::CanBusError err) {
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
        default:
            break;
    }

    err_string.append(QString(", Errno: %1. Do you use the latest patchset of socketcan plugin?").arg(errno));

    QFAIL(err_string.toStdString().c_str());
}

void
tst_QSerialBus::ReadWriteLoop() {
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

            // qInfo() << "Loop #" << tst_QSerialBus::loopLevel << ", Receive: " << frameR.toString();
        }
    }
    qInfo() << "write_count = " << write_count;
    qInfo() << "read_count = " << read_count;
    QVERIFY (write_count == read_count);
}

QTEST_MAIN(tst_QSerialBus)

#include "tst_qserialbus.moc"
