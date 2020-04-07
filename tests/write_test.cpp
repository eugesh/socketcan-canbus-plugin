#include <QtSerialBus/qcanbusdevice.h>
#include <QtSerialBus/qcanbusframe.h>
#include <QtSerialBus/qcanbus.h>
#include <QtSerialBus/qcanbusfactory.h>

#include <QtCore/qscopedpointer.h>
#include <QtCore/qtimer.h>
#include <QtTest/qsignalspy.h>
#include <QtTest/qtest.h>
#include "socketcanbackend_v2.h"

class tst_QCanBus : public QObject
{
    Q_OBJECT
public:
    explicit tst_QCanBus();

private slots:
    void initTestCase();
    // void conf();
    void floodTrafficWrite();

private:
    QCanBus *bus = nullptr;
};

tst_QCanBus::tst_QCanBus() {

}

void tst_QCanBus::initTestCase()
{
#if QT_CONFIG(library)
    /*
     * Set custom path since CI doesn't install test plugins
     */
    QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath()
                                     + QStringLiteral("/../../../plugins"));
#ifdef Q_OS_WIN
    QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath()
                                     + QStringLiteral("/../../../../plugins"));
#endif
#endif // QT_CONFIG(library)
    bus = QCanBus::instance();
    QVERIFY(bus);
    QCanBus *sameInstance = QCanBus::instance();
    QCOMPARE(bus, sameInstance);
}

void tst_QCanBus::floodTrafficWrite() {
    QCanBusDevice *canDevice = nullptr;
    canDevice = new SocketCanBackend_v2("vcan0");

    QVERIFY ( canDevice);
    if (! canDevice) {
        qCritical() << "Error: QSocketCAN_connector::HW_init: Socket wasn't initialized!";
    }

    if (canDevice->state() == QCanBusDevice::UnconnectedState)
        canDevice->connectDevice();

    QVERIFY(canDevice->state() == QCanBusDevice::ConnectedState);
    if (canDevice->state() == QCanBusDevice::UnconnectedState)  {
        qCritical() << "Error: QSocketCAN_connector::HW_init: Socket wasn't initialized!";

        return;
    }

    for (int i=0; i < 50000; ++i) {
        const uint frameId = 0x22;
        QString data = QString("E0FF");
        const QByteArray payload = QByteArray::fromHex(data.toLatin1());

        QCanBusFrame frame = QCanBusFrame(frameId, payload);
        // frame.setExtendedFrameFormat(false);
        // frame.setFlexibleDataRateFormat(false);
        // frame.setBitrateSwitch(m_ui->bitrateSwitchBox->isChecked());

        /*if (m_ui->errorFrame->isChecked())
            frame.setFrameType(QCanBusFrame::ErrorFrame);
        else if (m_ui->remoteFrame->isChecked())
            frame.setFrameType(QCanBusFrame::RemoteRequestFrame);*/

        QVERIFY(canDevice->writeFrame(frame));
    }
}

QTEST_MAIN(tst_QCanBus)

#include "write_test.moc"
