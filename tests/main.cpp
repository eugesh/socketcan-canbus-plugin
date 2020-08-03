#include <QCoreApplication>
#include <QObject>
#include <iostream>
#include "can_test_common.h"
#include "socketcanbackend_v2.h"
#include <QtCore/qtimer.h>
#include <unistd.h>


#define LONG_MSG_TEST_NUM_LOOP 50000

class tst_QCanBus : public QObject
{
    Q_OBJECT
public:
    explicit tst_QCanBus();

public slots:
    void initTestCase();
    // void conf();
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
}

void tst_QCanBus::longUniCANWrite() {
    // Name of file with long message
    QString test_file = "long_CAN.txt";

    QList<QCanBusFrame> frames = read_long_file(test_file);

    for (int k = 0; k < LONG_MSG_TEST_NUM_LOOP; k++) {
        for (int n = 0; n < frames.count(); ++n) {
            QVERIFY(m_canDevice->writeFrame(frames[n]));
            // usleep(1);
        }
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    tst_QCanBus test;

    test.initTestCase();
    test.longUniCANWrite();

    return a.exec();
}

#include "main.moc"
