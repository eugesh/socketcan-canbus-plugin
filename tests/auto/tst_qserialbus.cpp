#include <QtTest/QtTest>
#include <QtSerialBus/qcanbusframe.h>
#include <QtSerialBus/qcanbusdevice.h>
#include <QtSerialBus/qcanbusdeviceinfo.h>
#include <QtSerialBus/qcanbusdevice.h>

#include <QThread>

Q_DECLARE_METATYPE(QIODevice::OpenMode);
Q_DECLARE_METATYPE(QIODevice::OpenModeFlag);
Q_DECLARE_METATYPE(Qt::ConnectionType);

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


protected slots:


private:
    static int loopLevel;
};

int tst_QSerialBus::loopLevel = 0;

//const QByteArray tst_QSerialBus::alphabetArray("ABCDEFGHIJKLMNOPQRSTUVWXUZ");
//const QByteArray tst_QSerialBus::newlineArray("\n\r");

tst_QSerialBus::tst_QSerialBus() {

}

void tst_QSerialBus::initTestCase() {

}


QTEST_MAIN(tst_QSerialBus)
#include "tst_qserialbus.moc"
