#ifndef GLOBAL_H
#define GLOBAL_H

#include <QWidget>
#include <QApplication>
#include <QGamepadManager>
#include <QGamepad>
#include <QtEndian>
#include <QUdpSocket>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QDialog>
#include <QSettings>
#include <QMouseEvent>
#include <QCloseEvent>
#include <QFileDialog>
#include <QLabel>
#include <QSlider>
#include <QThread>
#include <QMetaObject>
#include <QSet>
#include "gpconfigurator.h"
#include "settings.h"
#include "shortcut.h"

// Forward-declare the ConfigWindow class to avoid circular dependencies
class ConfigWindow;

#define TOUCH_SCREEN_WIDTH  320
#define TOUCH_SCREEN_HEIGHT 240

extern int CPAD_BOUND;
extern int CPP_BOUND;

extern Settings btnSettings;

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

int appScreenTo3dsX(int);
int appScreenTo3dsY(int);

struct TouchButton
{
    int x, y;
};

extern std::vector<ShortCut> listShortcuts;

ShortCut variantToShortCut(QVariant variant);

extern int id, fid;
extern QSettings settings;

extern QGamepadManager::GamepadButtons buttons;
extern u32 interfaceButtons;

extern int yAxisMultiplier, yAxisMultiplierCpp;
extern QString ipAddress;
extern bool timerEnabled;

extern GamepadConfigurator *gpConfigurator;
extern ConfigWindow *settingsConfig;

extern bool touchScreenPressed;
extern QSize touchScreenSize;
extern QPoint touchScreenPosition;
extern double tsRatio;

// --- Keyboard mapping variables ---
extern int keyA, keyB, keyX, keyY;
extern int keyUp, keyDown, keyLeft, keyRight; // D-Pad
extern int keyL, keyR, keyZL, keyZR;
extern int keyStart, keySelect;
extern int keyHome, keyPower, keyPowerLong;
// NEW: Keybinds for analog stick directions
extern int keyCPadUp, keyCPadDown, keyCPadLeft, keyCPadRight;
extern int keyCStickUp, keyCStickDown, keyCStickLeft, keyCStickRight;
extern QSet<int> pressedKeys;

void loadKeysFromSettingsIntoGlobals();


extern TouchButton tbOne, tbTwo, tbThree, tbFour;

struct MyAxis
{
    double x, y;
};

class Worker : public QObject {
    Q_OBJECT
 public:

    MyAxis getLeftAxis();
    MyAxis getRightAxis();
    MyAxis getPreviousLAxis();

    void setLeftAxis(double x, double y);
    void setRightAxis(double x, double y);
    void setPreviousLAxis(double x, double y);
    void closeThread() { emit finished(); }

    explicit Worker(QObject *parent = 0) : QObject(parent)
    {
        previousLeftAxis.x = leftAxis.x;
        previousLeftAxis.y = leftAxis.y;
    }
    ~Worker() {}

 public slots:
    void sendFrame();

 signals:
    void finished();
    void error(QString err);

 private:
    MyAxis leftAxis;
    MyAxis rightAxis;
    MyAxis previousLeftAxis;
};

extern Worker worker;

#endif // GLOBAL_H