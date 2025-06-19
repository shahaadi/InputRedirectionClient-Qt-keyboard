#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include "global.h"
#include "touchscreen.h"
#include <QPushButton>
#include <QLabel>
#include <QKeyEvent>
#include <QScrollArea> // NEW: Include for scroll area

class ConfigWindow : public QDialog
{
    Q_OBJECT

private:
    // NEW: Widgets for the scrollable area
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;

    // The layout for the content inside the scroll area
    QVBoxLayout *mainLayout;

    // Labels to show the currently mapped key
    QLabel      *lblKeyA, *lblKeyB, *lblKeyX, *lblKeyY,
                *lblKeyL, *lblKeyR, *lblKeyUp, *lblKeyDown,
                *lblKeyLeft, *lblKeyRight, *lblKeyStart, *lblKeySelect,
                *lblKeyZL, *lblKeyZR, *lblKeyHome, *lblKeyPower,
                *lblKeyPowerLong,
                *lblKeyCPadUp, *lblKeyCPadDown, *lblKeyCPadLeft, *lblKeyCPadRight,
                *lblKeyCStickUp, *lblKeyCStickDown, *lblKeyCStickLeft, *lblKeyCStickRight;

    // Buttons to initiate remapping
    QPushButton *btnMapA, *btnMapB, *btnMapX, *btnMapY,
                *btnMapL, *btnMapR, *btnMapUp, *btnMapDown,
                *btnMapLeft, *btnMapRight, *btnMapStart, *btnMapSelect,
                *btnMapZL, *btnMapZR, *btnMapHome, *btnMapPower,
                *btnMapPowerLong,
                *btnMapCPadUp, *btnMapCPadDown, *btnMapCPadLeft, *btnMapCPadRight,
                *btnMapCStickUp, *btnMapCStickDown, *btnMapCStickLeft, *btnMapCStickRight;

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
    void keyPressEvent(QKeyEvent *event) override;
    void showEvent(QShowEvent* event) override;


public:
    ConfigWindow(QWidget *parent = nullptr, TouchScreen *ts = nullptr);
};

#endif // CONFIGWINDOW_H