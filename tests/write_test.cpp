#include <QtCore/qscopedpointer.h>
#include <QtCore/qtimer.h>
#include <QtTest/qsignalspy.h>
#include <QtTest/qtest.h>
#include "socketcanbackend_v2.h"
#include "can_test_common.h"

#define LONG_MSG_TEST_NUM_LOOP 500

class tst_QCanBus : public QObject
{
    Q_OBJECT
public:
    explicit tst_QCanBus();

private slots:
    void initTestCase();
    // void conf();
    // Write many messages without time gap.
    void floodTrafficWrite();
    // Write long UniCAN message.
    void longUniCANWrite();

private:
    QCanBus *bus = nullptr;
    QCanBusDevice *m_canDevice = nullptr;
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
    m_canDevice = new SocketCanBackend_v2(find_can_device());

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

    for (int i = 0; i < 50000; ++i) {
        const uint frameId = 0x22;
        QString data = QString("E0FF");
        const QByteArray payload = QByteArray::fromHex(data.toLatin1());

        // qInfo() << frameId << "#" << data;

        QCanBusFrame frame = QCanBusFrame(frameId, payload);
        // frame.setExtendedFrameFormat(false);
        // frame.setFlexibleDataRateFormat(false);
        // frame.setBitrateSwitch(m_ui->bitrateSwitchBox->isChecked());

        /*if (m_ui->errorFrame->isChecked())
            frame.setFrameType(QCanBusFrame::ErrorFrame);
        else if (m_ui->remoteFrame->isChecked())
            frame.setFrameType(QCanBusFrame::RemoteRequestFrame);*/

        QVERIFY(m_canDevice->writeFrame(frame));
    }
}

void tst_QCanBus::longUniCANWrite() {
    // Name of file with long message
    QString test_file = "long_CAN.txt";

    for (int k = 0; k < LONG_MSG_TEST_NUM_LOOP; k++) {

        QFile file(test_file);

        if (!file.open(QFile::ReadOnly)) {
            QString msg = tr("Failed to open %1\n%2")
                            .arg(test_file)
                            .arg(file.errorString());
            qWarning() << tr("Error") + msg;
            return;
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

            // qInfo () << canId << " [" << len << "] " << payload;

            QCanBusFrame frame = QCanBusFrame(canId, payload);

            QVERIFY(m_canDevice->writeFrame(frame));
        }
    }
}

QTEST_MAIN(tst_QCanBus)

#include "write_test.moc"
