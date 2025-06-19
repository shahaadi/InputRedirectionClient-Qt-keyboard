#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif

#include "mainwidget.h"
#include "gpmanager.h"
#include <QTimer>
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Load keybindings from settings into global variables on startup
    loadKeysFromSettingsIntoGlobals();

    Widget w;
    GamepadMonitor m(&w); // GamepadMonitor now only handles analog stick input

    w.show();
    QThread* thread = new QThread();
    QTimer timer;
    timer.setInterval(20);

    timer.moveToThread(thread);
    worker.moveToThread(thread);

    // Reverted to the original SIGNAL/SLOT macro syntax to ensure compatibility
    QObject::connect(thread, SIGNAL(started()), &timer, SLOT(start()));
    QObject::connect(&worker, SIGNAL(finished()), thread, SLOT(quit()));
    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    QObject::connect(&timer, SIGNAL(timeout()), &worker, SLOT(sendFrame()));

    thread->start();
    a.exec();
    worker.closeThread();
    return 0;
}