#include <QCoreApplication>
#include <QObject>
#include <iostream>
#include "can_test_common.h"
#include "socketcanbackend_v2.h"


class tst_QCanBusRead : public QObject
{
    Q_OBJECT

public:
    explicit tst_QCanBusRead();
    void listen_can();
    void listen_long_can();

private slots:
    void processReceivedFrames();
    void processReceivedFrames_long();

private:
    bool compare_received_frame(const QCanBusFrame & frame) const;

private:
    QCanBus       *bus = nullptr;
    QCanBusDevice *m_canDevice = nullptr;
    quint32        m_rxCount = 0;
    QList<QCanBusFrame> m_frames;
    quint32        m_error_frame_count = 0;
};

tst_QCanBusRead::tst_QCanBusRead() {

}

void tst_QCanBusRead::listen_can() {
    m_canDevice = new SocketCanBackend_v2(find_can_device ());

    QVERIFY (m_canDevice);
    if (! m_canDevice) {
        qCritical() << "Error: QSocketCAN_connector::HW_init: Socket wasn't initialized!";
    }

    if (m_canDevice->state() == QCanBusDevice::UnconnectedState)
        m_canDevice->connectDevice();

    QVERIFY(m_canDevice->state() == QCanBusDevice::ConnectedState);
    if (m_canDevice->state() == QCanBusDevice::UnconnectedState)  {
        qCritical() << "Error: QSocketCAN_connector::HW_init: Socket wasn't initialized!";

        return;
    }

    connect(m_canDevice, &QCanBusDevice::framesReceived, this, &tst_QCanBusRead::processReceivedFrames);
}

void tst_QCanBusRead::listen_long_can() {
    // Read file with long frame.
    // Name of file with long message
    QString test_file = "long_CAN.txt";

    m_frames = read_long_file(test_file);

    if (! m_frames.count())
        return;

    m_canDevice = new SocketCanBackend_v2(find_can_device ());

    QVERIFY (m_canDevice);
    if (! m_canDevice) {
        qCritical() << "Error: QSocketCAN_connector::HW_init: Socket wasn't initialized!";
    }

    if (m_canDevice->state() == QCanBusDevice::UnconnectedState)
        m_canDevice->connectDevice();

    QVERIFY(m_canDevice->state() == QCanBusDevice::ConnectedState);
    if (m_canDevice->state() == QCanBusDevice::UnconnectedState)  {
        qCritical() << "Error: QSocketCAN_connector::HW_init: Socket wasn't initialized!";

        return;
    }

    connect(m_canDevice, &QCanBusDevice::framesReceived, this, &tst_QCanBusRead::processReceivedFrames_long);
}

void tst_QCanBusRead::processReceivedFrames_long() {
    if (!m_canDevice)
        return;

    while (m_canDevice->framesAvailable()) {
        const QCanBusFrame frame = m_canDevice->readFrame();

        if (frame.frameType() == QCanBusFrame::ErrorFrame)
            qCritical() << m_canDevice->interpretErrorFrame(frame);

        if (! compare_received_frame(frame))
            m_error_frame_count++;

        const QString time = QString::fromLatin1("%1.%2  ")
                .arg(frame.timeStamp().seconds(), 10, 10, QLatin1Char(' '))
                .arg(frame.timeStamp().microSeconds() / 100, 4, 10, QLatin1Char('0'));

        int m_bytesCount = frame.toString().count();
        m_rxCount++;

        /*message.can_extbit = frame.hasExtendedFrameFormat();

        if (message.can_extbit) {
            message.can_identifier = frame.frameId() & CAN_EFF_MASK;
        }
        else {
            message.can_identifier = frame.frameId() & CAN_SFF_MASK;
        }

        // message.can_rtr = (frame.frameId() & CAN_RTR_FLAG) >> 30;
        if (frame.frameType() == QCanBusFrame::RemoteRequestFrame)
            message.can_rtr = 1;
        else
            message.can_rtr = 0;

        message.can_dlc = frame.payload().count();

        for (int data_index = 0; data_index < frame.payload().count(); data_index++){
            message.data[data_index] = frame.payload()[data_index];
        }

        can_receive_message(&message);

        emit can_message_ready(message);*/
        qInfo() << m_rxCount << "( " << m_error_frame_count << " )" << frame.toString(); //QString::number(frame.frameId(), 16) << "#" << QString(frame.payload());
    }

}

bool tst_QCanBusRead::compare_received_frame(const QCanBusFrame & frame) const {
    QCanBusFrame expected_frame = m_frames[m_rxCount % m_frames.count()];

    if (expected_frame.payload() == frame.payload() &&
        expected_frame.frameId() == frame.frameId())
    {
        return true;
    }
    else {
        qCritical() << "Frame was received in inapropriate order!";
        return false;
    }
}

void tst_QCanBusRead::processReceivedFrames() {
    if (!m_canDevice)
        return;

    while (m_canDevice->framesAvailable()) {
        const QCanBusFrame frame = m_canDevice->readFrame();

        if (frame.frameType() == QCanBusFrame::ErrorFrame)
            qCritical() << m_canDevice->interpretErrorFrame(frame);

        const QString time = QString::fromLatin1("%1.%2  ")
                .arg(frame.timeStamp().seconds(), 10, 10, QLatin1Char(' '))
                .arg(frame.timeStamp().microSeconds() / 100, 4, 10, QLatin1Char('0'));

        int m_bytesCount = frame.toString().count();
        m_rxCount++;

        qInfo() << m_rxCount << frame.toString();
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    tst_QCanBusRead test;

    test.listen_can();
    // test.listen_long_can();

    return a.exec();
}

#include "main.moc"
