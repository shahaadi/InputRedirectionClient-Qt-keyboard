#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include "global.h"
#include "touchscreen.h"
#include <QPushButton>
#include <QLabel>
#include <QKeyEvent>

class ConfigWindow : public QDialog
{
    Q_OBJECT

private:
    QGridLayout *layout;

    // Labels to show the currently mapped key
    QLabel      *lblKeyA, *lblKeyB, *lblKeyX, *lblKeyY,
                *lblKeyL, *lblKeyR, *lblKeyUp, *lblKeyDown,
                *lblKeyLeft, *lblKeyRight, *lblKeyStart, *lblKeySelect,
                *lblKeyZL, *lblKeyZR, *lblKeyHome, *lblKeyPower,
                *lblKeyPowerLong;

    // Buttons to initiate remapping
    QPushButton *btnMapA, *btnMapB, *btnMapX, *btnMapY,
                *btnMapL, *btnMapR, *btnMapUp, *btnMapDown,
                *btnMapLeft, *btnMapRight, *btnMapStart, *btnMapSelect,
                *btnMapZL, *btnMapZR, *btnMapHome, *btnMapPower,
                *btnMapPowerLong;

    QPushButton *saveButton;

    QCheckBox   *invertYCheckbox, *invertYCppCheckbox, *swapSticksCheckbox,
                *mhCameraCheckbox, *rsSmashCheckbox,
                *disableCStickCheckbox, *rsFaceButtonsCheckbox;

    QLineEdit *txtStickVal, *txtCppVal;
    QValidator *validator;

    // Variable to track which button we are currently remapping
    QString     buttonToRemap;
    QLabel      *lblDirections; // To prompt the user

    // Helper function to update UI labels with current key maps
    void loadKeysFromSettings();
    void setKeyMapping(const QString& buttonName, int key, bool saveSetting = true);

protected:
    // Override keyPressEvent to capture the key
    void keyPressEvent(QKeyEvent *event) override;
    void showEvent(QShowEvent* event) override;


public:
    ConfigWindow(QWidget *parent = nullptr, TouchScreen *ts = nullptr);
};

#endif // CONFIGWINDOW_H