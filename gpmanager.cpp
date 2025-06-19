#include "gpmanager.h"

GamepadMonitor::GamepadMonitor(QObject *parent) : QObject(parent)
{
    // The button press/release events are now handled by Widget::keyPressEvent
    // This code is now obsolete for button handling.
    connect(QGamepadManager::instance(), &QGamepadManager::gamepadButtonPressEvent, this,
        [](int deviceId, QGamepadManager::GamepadButton button, double value)
    {
        (void)deviceId;
        (void)button;
        (void)value;
    });

    connect(QGamepadManager::instance(), &QGamepadManager::gamepadButtonReleaseEvent, this,
        [](int deviceId, QGamepadManager::GamepadButton button)
    {
        (void)deviceId;
        (void)button;
    });

    // --- ANALOG AXIS HANDLING (UNCHANGED FROM ORIGINAL) ---
    // This allows a controller's analog sticks to be used alongside the keyboard.
    connect(QGamepadManager::instance(), &QGamepadManager::gamepadAxisEvent, this,
        [](int deviceId, QGamepadManager::GamepadAxis axis, double value)
    {
        (void)deviceId;

        if (btnSettings.isMonsterHunterCamera() || btnSettings.isRightStickSmash() || btnSettings.isRightStickFaceButtons())
        {
            // The logic for these special modes is complex and relies on the old
            // QGamepadManager button system (hidButtonsAB, hidButtonsMiddle etc.).
            // This hybrid functionality is not supported by the new keyboard-only system.
            // We now only process the raw axis values.
        }

        if(axis == QGamepadManager::AxisLeftX)
        {
           worker.setLeftAxis(value, worker.getLeftAxis().y);
        }
        else if(axis == QGamepadManager::AxisLeftY)
        {
            worker.setLeftAxis(worker.getLeftAxis().x, -value);
        }
        else if(axis == QGamepadManager::AxisRightX)
        {
            worker.setRightAxis(value, worker.getRightAxis().y);
        }
        else if(axis == QGamepadManager::AxisRightY)
        {
            worker.setRightAxis(worker.getRightAxis().x, -value);
        }
    });
}