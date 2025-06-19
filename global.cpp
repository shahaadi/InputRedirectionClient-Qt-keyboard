#include "global.h"
#include <math.h>
#include "gpmanager.h"
#include "configwindow.h" // Include header for ConfigWindow definition

QSettings settings("TuxSH", "InputRedirectionClient-Qt");

Worker worker;
Settings btnSettings;
double tsRatio;

std::vector<ShortCut> listShortcuts;

QGamepadManager::GamepadButtons buttons;
u32 interfaceButtons = 0;
int yAxisMultiplier = 1, yAxisMultiplierCpp = 1;
bool shouldSwapStick = false;
int CPAD_BOUND;
int CPP_BOUND;

GamepadConfigurator *gpConfigurator;
ConfigWindow *settingsConfig = nullptr; // NEW: Define the global pointer

QString ipAddress;

bool touchScreenPressed;
QSize touchScreenSize = QSize(TOUCH_SCREEN_WIDTH, TOUCH_SCREEN_HEIGHT);
QPoint touchScreenPosition;

// --- Global keyboard mapping variables ---
int keyA, keyB, keyX, keyY;
int keyUp, keyDown, keyLeft, keyRight;
int keyL, keyR, keyZL, keyZR;
int keyStart, keySelect;
int keyHome, keyPower, keyPowerLong;
QSet<int> pressedKeys;

void loadKeysFromSettingsIntoGlobals() {
    CPAD_BOUND = settings.value("StickBound", 1488).toInt();
    CPP_BOUND = settings.value("CppBound", 127).toInt();

    yAxisMultiplier = settings.value("invertY", false).toBool() ? -1 : 1;
    yAxisMultiplierCpp = settings.value("invertCPPY", false).toBool() ? -1 : 1;
    btnSettings.setShouldSwapStick(settings.value("swapSticks", false).toBool());
    btnSettings.setRightStickSmash(settings.value("rightStickSmash", false).toBool());
    btnSettings.setMonsterHunterCamera(settings.value("monsterHunterCamera", false).toBool());
    btnSettings.setCStickDisabled(settings.value("cStickDisable", false).toBool());
    btnSettings.setRightStickFaceButtons(settings.value("rightStickABXY", false).toBool());

    keyA = settings.value("ButtonA", Qt::Key_J).toInt();
    keyB = settings.value("ButtonB", Qt::Key_K).toInt();
    keyX = settings.value("ButtonX", Qt::Key_I).toInt();
    keyY = settings.value("ButtonY", Qt::Key_L).toInt();
    keyUp = settings.value("ButtonUp", Qt::Key_W).toInt();
    keyDown = settings.value("ButtonDown", Qt::Key_S).toInt();
    keyLeft = settings.value("ButtonLeft", Qt::Key_A).toInt();
    keyRight = settings.value("ButtonRight", Qt::Key_D).toInt();
    keyL = settings.value("ButtonL", Qt::Key_Q).toInt();
    keyR = settings.value("ButtonR", Qt::Key_E).toInt();
    keyZL = settings.value("ButtonZL", Qt::Key_1).toInt();
    keyZR = settings.value("ButtonZR", Qt::Key_3).toInt();
    keyStart = settings.value("ButtonStart", Qt::Key_Return).toInt();
    keySelect = settings.value("ButtonSelect", Qt::Key_Shift).toInt();
    keyHome = settings.value("ButtonHome", Qt::Key_Home).toInt();
    keyPower = settings.value("ButtonPower", 0).toInt();
    keyPowerLong = settings.value("ButtonPowerLong", 0).toInt();
}

void Worker::sendFrame(void)
{
    u32 hidPad = 0xFFF;
    if (pressedKeys.contains(keyA))       hidPad &= ~(1 << 0);
    if (pressedKeys.contains(keyB))       hidPad &= ~(1 << 1);
    if (pressedKeys.contains(keySelect))  hidPad &= ~(1 << 2);
    if (pressedKeys.contains(keyStart))   hidPad &= ~(1 << 3);
    if (pressedKeys.contains(keyRight))   hidPad &= ~(1 << 4);
    if (pressedKeys.contains(keyLeft))    hidPad &= ~(1 << 5);
    if (pressedKeys.contains(keyUp))      hidPad &= ~(1 << 6);
    if (pressedKeys.contains(keyDown))    hidPad &= ~(1 << 7);
    if (pressedKeys.contains(keyR))       hidPad &= ~(1 << 8);
    if (pressedKeys.contains(keyL))       hidPad &= ~(1 << 9);
    if (pressedKeys.contains(keyX))       hidPad &= ~(1 << 10);
    if (pressedKeys.contains(keyY))       hidPad &= ~(1 << 11);

    u32 irButtonsState = 0;
    if (pressedKeys.contains(keyZR))      irButtonsState |= (1 << 1);
    if (pressedKeys.contains(keyZL))      irButtonsState |= (1 << 2);

    u32 touchScreenState = 0x2000000;
    u32 circlePadState = 0x7ff7ff;
    u32 cppState = 0x80800081;

    if(touchScreenPressed)
    {
        u32 x = (u32)(0xfff * std::min(std::max(0, touchScreenPosition.x()), touchScreenSize.width())) / touchScreenSize.width();
        u32 y = (u32)(0xfff * std::min(std::max(0, touchScreenPosition.y()), touchScreenSize.height())) / touchScreenSize.height();
        touchScreenState = (1 << 24) | (y << 12) | x;
    }

    MyAxis lAxis = btnSettings.isShouldSwapStick() ? rightAxis : leftAxis;
    MyAxis rAxis = btnSettings.isShouldSwapStick() ? leftAxis : rightAxis;

    if(lAxis.x != 0.0 || lAxis.y != 0.0)
    {
          u32 x = (u32)(lAxis.x * CPAD_BOUND + 0x800);
          u32 y = (u32)(lAxis.y * yAxisMultiplier * CPAD_BOUND + 0x800);
          x = x >= 0xfff ? (lAxis.x < 0.0 ? 0x000 : 0xfff) : x;
          y = y >= 0xfff ? (lAxis.y * yAxisMultiplier < 0.0 ? 0x000 : 0xfff) : y;
          circlePadState = (y << 12) | x;
    }

    if((rAxis.x != 0.0 || rAxis.y != 0.0 || irButtonsState != 0) && !btnSettings.isCStickDisabled())
    {
          u32 x = (u32)(M_SQRT1_2 * (rAxis.x + rAxis.y * yAxisMultiplierCpp) * CPP_BOUND + 0x80);
          u32 y = (u32)(M_SQRT1_2 * (rAxis.y * yAxisMultiplierCpp - rAxis.x) * CPP_BOUND + 0x80);
          x = x >= 0xff ? (rAxis.x < 0.0 ? 0x00 : 0xff) : x;
          y = y >= 0xff ? (rAxis.y * yAxisMultiplierCpp < 0.0 ? 0x00 : 0xff) : y;
          cppState = (y << 24) | (x << 16) | (irButtonsState << 8) | 0x81;
    }

    QByteArray ba(20, 0);
    qToLittleEndian(hidPad, (uchar *)ba.data());
    qToLittleEndian(touchScreenState, (uchar *)ba.data() + 4);
    qToLittleEndian(circlePadState, (uchar *)ba.data() + 8);
    qToLittleEndian(cppState, (uchar *)ba.data() + 12);
    qToLittleEndian(interfaceButtons, (uchar *)ba.data() + 16);
    QUdpSocket().writeDatagram(ba, QHostAddress(ipAddress), 4950);
}


void Worker::setLeftAxis(double x, double y) { leftAxis.x = x; leftAxis.y = y; }
void Worker::setRightAxis(double x, double y) { rightAxis.x = x; rightAxis.y = y; }
void Worker::setPreviousLAxis(double x, double y) { previousLeftAxis.x = x; previousLeftAxis.y = y; }
MyAxis Worker::getLeftAxis() { return leftAxis; }
MyAxis Worker::getRightAxis() { return rightAxis; }
MyAxis Worker::getPreviousLAxis() { return previousLeftAxis; }
int appScreenTo3dsX(int posX) { return TOUCH_SCREEN_WIDTH*((touchScreenSize.height()*posX)/TOUCH_SCREEN_HEIGHT)/touchScreenSize.width(); }
int appScreenTo3dsY(int posY) { return TOUCH_SCREEN_HEIGHT*((touchScreenSize.width()*posY)/TOUCH_SCREEN_WIDTH)/touchScreenSize.height(); }