#include "configwindow.h"
#include "gpmanager.h"
#include <QKeySequence>
#include <QGroupBox>
#include <QScrollArea> // NEW: Include for scroll area

// Helper function to create a row in the grid
void addMappingRow(QGridLayout *layout, int row, const QString& name, QLabel*& keyLabel, QPushButton*& mapButton) {
    keyLabel = new QLabel("Not Mapped");
    keyLabel->setMinimumWidth(80);
    mapButton = new QPushButton("Change");
    layout->addWidget(new QLabel(name), row, 0, Qt::AlignRight);
    layout->addWidget(keyLabel, row, 1);
    layout->addWidget(mapButton, row, 2);
}

ConfigWindow::ConfigWindow(QWidget *parent, TouchScreen *ts) : QDialog(parent)
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    this->setWindowTitle(tr("InputRedirectionClient-Qt - Keyboard Config"));

    // --- NEW: Create a scroll area to contain all the widgets ---
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true); // This is crucial for the layout to work correctly
    scrollAreaWidgetContents = new QWidget();

    // The mainLayout now applies to the widget *inside* the scroll area
    mainLayout = new QVBoxLayout(scrollAreaWidgetContents);

    lblDirections = new QLabel("Click 'Change' to map a key. Click Save to apply changes.");
    mainLayout->addWidget(lblDirections, 0, Qt::AlignCenter);

    // --- C-Pad and D-Pad Group ---
    QGroupBox *cpadGroup = new QGroupBox("Circle Pad & D-Pad");
    QGridLayout *cpadLayout = new QGridLayout();
    int cpadRow = 0;
    addMappingRow(cpadLayout, cpadRow++, "C-Pad Up", lblKeyCPadUp, btnMapCPadUp);
    addMappingRow(cpadLayout, cpadRow++, "C-Pad Down", lblKeyCPadDown, btnMapCPadDown);
    addMappingRow(cpadLayout, cpadRow++, "C-Pad Left", lblKeyCPadLeft, btnMapCPadLeft);
    addMappingRow(cpadLayout, cpadRow++, "C-Pad Right", lblKeyCPadRight, btnMapCPadRight);
    cpadLayout->setRowStretch(cpadRow++, 10); // Spacer
    addMappingRow(cpadLayout, cpadRow++, "D-Pad Up", lblKeyUp, btnMapUp);
    addMappingRow(cpadLayout, cpadRow++, "D-Pad Down", lblKeyDown, btnMapDown);
    addMappingRow(cpadLayout, cpadRow++, "D-Pad Left", lblKeyLeft, btnMapLeft);
    addMappingRow(cpadLayout, cpadRow++, "D-Pad Right", lblKeyRight, btnMapRight);
    cpadGroup->setLayout(cpadLayout);
    mainLayout->addWidget(cpadGroup);

    // --- Buttons Group ---
    QGroupBox *buttonsGroup = new QGroupBox("Buttons");
    QGridLayout *buttonsLayout = new QGridLayout();
    int buttonRow = 0;
    addMappingRow(buttonsLayout, buttonRow++, "A Button", lblKeyA, btnMapA);
    addMappingRow(buttonsLayout, buttonRow++, "B Button", lblKeyB, btnMapB);
    addMappingRow(buttonsLayout, buttonRow++, "X Button", lblKeyX, btnMapX);
    addMappingRow(buttonsLayout, buttonRow++, "Y Button", lblKeyY, btnMapY);
    buttonsLayout->setRowStretch(buttonRow++, 10); // Spacer
    addMappingRow(buttonsLayout, buttonRow++, "L Button", lblKeyL, btnMapL);
    addMappingRow(buttonsLayout, buttonRow++, "R Button", lblKeyR, btnMapR);
    addMappingRow(buttonsLayout, buttonRow++, "ZL Button", lblKeyZL, btnMapZL);
    addMappingRow(buttonsLayout, buttonRow++, "ZR Button", lblKeyZR, btnMapZR);
    buttonsGroup->setLayout(buttonsLayout);
    mainLayout->addWidget(buttonsGroup);

    // --- C-Stick Group ---
    QGroupBox *cstickGroup = new QGroupBox("C-Stick (Right Stick)");
    QGridLayout *cstickLayout = new QGridLayout();
    int cstickRow = 0;
    addMappingRow(cstickLayout, cstickRow++, "C-Stick Up", lblKeyCStickUp, btnMapCStickUp);
    addMappingRow(cstickLayout, cstickRow++, "C-Stick Down", lblKeyCStickDown, btnMapCStickDown);
    addMappingRow(cstickLayout, cstickRow++, "C-Stick Left", lblKeyCStickLeft, btnMapCStickLeft);
    addMappingRow(cstickLayout, cstickRow++, "C-Stick Right", lblKeyCStickRight, btnMapCStickRight);
    cstickGroup->setLayout(cstickLayout);
    mainLayout->addWidget(cstickGroup);

    // --- System Buttons Group ---
    QGroupBox *systemGroup = new QGroupBox("System");
    QGridLayout *systemLayout = new QGridLayout();
    int systemRow = 0;
    addMappingRow(systemLayout, systemRow++, "Start", lblKeyStart, btnMapStart);
    addMappingRow(systemLayout, systemRow++, "Select", lblKeySelect, btnMapSelect);
    addMappingRow(systemLayout, systemRow++, "Home", lblKeyHome, btnMapHome);
    addMappingRow(systemLayout, systemRow++, "Power", lblKeyPower, btnMapPower);
    addMappingRow(systemLayout, systemRow++, "Power (Long)", lblKeyPowerLong, btnMapPowerLong);
    systemGroup->setLayout(systemLayout);
    mainLayout->addWidget(systemGroup);


    auto createMapConnection = [this](QPushButton* button, const QString& buttonName) {
        connect(button, &QPushButton::clicked, this, [this, buttonName]() {
            buttonToRemap = buttonName;
            lblDirections->setText(QString("Press a key for: %1...").arg(buttonName));
            this->setFocus();
        });
    };

    createMapConnection(btnMapA, "ButtonA");
    createMapConnection(btnMapB, "ButtonB");
    createMapConnection(btnMapX, "ButtonX");
    createMapConnection(btnMapY, "ButtonY");
    createMapConnection(btnMapUp, "ButtonUp");
    createMapConnection(btnMapDown, "ButtonDown");
    createMapConnection(btnMapLeft, "ButtonLeft");
    createMapConnection(btnMapRight, "ButtonRight");
    createMapConnection(btnMapL, "ButtonL");
    createMapConnection(btnMapR, "ButtonR");
    createMapConnection(btnMapZL, "ButtonZL");
    createMapConnection(btnMapZR, "ButtonZR");
    createMapConnection(btnMapStart, "ButtonStart");
    createMapConnection(btnMapSelect, "ButtonSelect");
    createMapConnection(btnMapHome, "ButtonHome");
    createMapConnection(btnMapPower, "ButtonPower");
    createMapConnection(btnMapPowerLong, "ButtonPowerLong");
    createMapConnection(btnMapCPadUp, "CPadUp");
    createMapConnection(btnMapCPadDown, "CPadDown");
    createMapConnection(btnMapCPadLeft, "CPadLeft");
    createMapConnection(btnMapCPadRight, "CPadRight");
    createMapConnection(btnMapCStickUp, "CStickUp");
    createMapConnection(btnMapCStickDown, "CStickDown");
    createMapConnection(btnMapCStickLeft, "CStickLeft");
    createMapConnection(btnMapCStickRight, "CStickRight");

    // --- Other Settings Group ---
    QGroupBox *otherGroup = new QGroupBox("Other Settings");
    QGridLayout *otherLayout = new QGridLayout();
    txtCppVal = new QLineEdit();
    txtStickVal = new QLineEdit();
    validator = new QIntValidator();
    txtCppVal->setValidator(validator);
    txtStickVal->setValidator(validator);
    invertYCheckbox = new QCheckBox("Invert C-Pad Y");
    invertYCppCheckbox = new QCheckBox("Invert C-Stick Y");
    swapSticksCheckbox = new QCheckBox("Swap C-Pad/C-Stick");
    disableCStickCheckbox = new QCheckBox("Disable C-Stick");
    otherLayout->addWidget(new QLabel("Stick Range:"), 0, 0);
    otherLayout->addWidget(txtStickVal, 0, 1);
    otherLayout->addWidget(new QLabel("CPP Range:"), 1, 0);
    otherLayout->addWidget(txtCppVal, 1, 1);
    otherLayout->addWidget(invertYCheckbox, 2, 0);
    otherLayout->addWidget(invertYCppCheckbox, 2, 1);
    otherLayout->addWidget(swapSticksCheckbox, 3, 0);
    otherLayout->addWidget(disableCStickCheckbox, 3, 1);
    otherGroup->setLayout(otherLayout);
    mainLayout->addWidget(otherGroup);
    
    saveButton = new QPushButton(tr("&SAVE and Apply"), this);
    mainLayout->addWidget(saveButton);

    // --- Finalize Layout ---
    // Place the widget containing all our settings inside the scroll area
    scrollArea->setWidget(scrollAreaWidgetContents);

    // Create the final layout for the dialog window and add the scroll area to it
    QVBoxLayout *dialogLayout = new QVBoxLayout(this);
    dialogLayout->addWidget(scrollArea);


    connect(invertYCheckbox, &QCheckBox::stateChanged, [](int state){
        settings.setValue("invertY", state == Qt::Checked);
    });
    connect(invertYCppCheckbox, &QCheckBox::stateChanged, [](int state){
        settings.setValue("invertCPPY", state == Qt::Checked);
    });
    connect(swapSticksCheckbox, &QCheckBox::stateChanged, [](int state){
        settings.setValue("swapSticks", state == Qt::Checked);
    });
    connect(disableCStickCheckbox, &QCheckBox::stateChanged, [](int state){
        settings.setValue("cStickDisable", state == Qt::Checked);
    });

    connect(saveButton, &QPushButton::pressed, this, [this, ts]() {
        settings.setValue("StickBound", txtStickVal->text().toInt());
        settings.setValue("CppBound", txtCppVal->text().toInt());
        loadKeysFromSettingsIntoGlobals();
        ts->updatePixmap();
        lblDirections->setText("Settings Saved and Applied!");
    });
}

void ConfigWindow::showEvent(QShowEvent* event) {
    loadKeysFromSettings();
    txtStickVal->setText(settings.value("StickBound", 1488).toString());
    txtCppVal->setText(settings.value("CppBound", 127).toString());
    invertYCheckbox->setChecked(settings.value("invertY", false).toBool());
    invertYCppCheckbox->setChecked(settings.value("invertCPPY", false).toBool());
    swapSticksCheckbox->setChecked(settings.value("swapSticks", false).toBool());
    disableCStickCheckbox->setChecked(settings.value("cStickDisable", false).toBool());
    QDialog::showEvent(event);
}

void ConfigWindow::keyPressEvent(QKeyEvent *event) {
    if (!buttonToRemap.isEmpty() && event->key() != Qt::Key_unknown) {
        setKeyMapping(buttonToRemap, event->key());
        buttonToRemap.clear();
        lblDirections->setText("Click 'Change' to map a key. Click Save to apply.");
    } else {
        QDialog::keyPressEvent(event);
    }
}

void ConfigWindow::setKeyMapping(const QString& buttonName, int key, bool saveSetting) {
    if (saveSetting) {
        settings.setValue(buttonName, key);
    }
    QString keyText = QKeySequence(key).toString();
    if (keyText.isEmpty() || key == 0) keyText = "None";

    if (buttonName == "ButtonA") lblKeyA->setText(keyText);
    else if (buttonName == "ButtonB") lblKeyB->setText(keyText);
    else if (buttonName == "ButtonX") lblKeyX->setText(keyText);
    else if (buttonName == "ButtonY") lblKeyY->setText(keyText);
    else if (buttonName == "ButtonUp") lblKeyUp->setText(keyText);
    else if (buttonName == "ButtonDown") lblKeyDown->setText(keyText);
    else if (buttonName == "ButtonLeft") lblKeyLeft->setText(keyText);
    else if (buttonName == "ButtonRight") lblKeyRight->setText(keyText);
    else if (buttonName == "ButtonL") lblKeyL->setText(keyText);
    else if (buttonName == "ButtonR") lblKeyR->setText(keyText);
    else if (buttonName == "ButtonZL") lblKeyZL->setText(keyText);
    else if (buttonName == "ButtonZR") lblKeyZR->setText(keyText);
    else if (buttonName == "ButtonStart") lblKeyStart->setText(keyText);
    else if (buttonName == "ButtonSelect") lblKeySelect->setText(keyText);
    else if (buttonName == "ButtonHome") lblKeyHome->setText(keyText);
    else if (buttonName == "ButtonPower") lblKeyPower->setText(keyText);
    else if (buttonName == "ButtonPowerLong") lblKeyPowerLong->setText(keyText);
    else if (buttonName == "CPadUp") lblKeyCPadUp->setText(keyText);
    else if (buttonName == "CPadDown") lblKeyCPadDown->setText(keyText);
    else if (buttonName == "CPadLeft") lblKeyCPadLeft->setText(keyText);
    else if (buttonName == "CPadRight") lblKeyCPadRight->setText(keyText);
    else if (buttonName == "CStickUp") lblKeyCStickUp->setText(keyText);
    else if (buttonName == "CStickDown") lblKeyCStickDown->setText(keyText);
    else if (buttonName == "CStickLeft") lblKeyCStickLeft->setText(keyText);
    else if (buttonName == "CStickRight") lblKeyCStickRight->setText(keyText);
}

void ConfigWindow::loadKeysFromSettings() {
    setKeyMapping("ButtonA", settings.value("ButtonA", Qt::Key_J).toInt(), false);
    setKeyMapping("ButtonB", settings.value("ButtonB", Qt::Key_K).toInt(), false);
    setKeyMapping("ButtonX", settings.value("ButtonX", Qt::Key_U).toInt(), false);
    setKeyMapping("ButtonY", settings.value("ButtonY", Qt::Key_I).toInt(), false);
    setKeyMapping("ButtonUp", settings.value("ButtonUp", Qt::Key_Up).toInt(), false);
    setKeyMapping("ButtonDown", settings.value("ButtonDown", Qt::Key_Down).toInt(), false);
    setKeyMapping("ButtonLeft", settings.value("ButtonLeft", Qt::Key_Left).toInt(), false);
    setKeyMapping("ButtonRight", settings.value("ButtonRight", Qt::Key_Right).toInt(), false);
    setKeyMapping("ButtonL", settings.value("ButtonL", Qt::Key_Q).toInt(), false);
    setKeyMapping("ButtonR", settings.value("ButtonR", Qt::Key_E).toInt(), false);
    setKeyMapping("ButtonZL", settings.value("ButtonZL", Qt::Key_1).toInt(), false);
    setKeyMapping("ButtonZR", settings.value("ButtonZR", Qt::Key_3).toInt(), false);
    setKeyMapping("ButtonStart", settings.value("ButtonStart", Qt::Key_Return).toInt(), false);
    setKeyMapping("ButtonSelect", settings.value("ButtonSelect", Qt::Key_Shift).toInt(), false);
    setKeyMapping("ButtonHome", settings.value("ButtonHome", Qt::Key_Home).toInt(), false);
    setKeyMapping("ButtonPower", settings.value("ButtonPower", 0).toInt(), false);
    setKeyMapping("ButtonPowerLong", settings.value("ButtonPowerLong", 0).toInt(), false);
    setKeyMapping("CPadUp", settings.value("CPadUp", Qt::Key_W).toInt(), false);
    setKeyMapping("CPadDown", settings.value("CPadDown", Qt::Key_S).toInt(), false);
    setKeyMapping("CPadLeft", settings.value("CPadLeft", Qt::Key_A).toInt(), false);
    setKeyMapping("CPadRight", settings.value("CPadRight", Qt::Key_D).toInt(), false);
    setKeyMapping("CStickUp", settings.value("CStickUp", Qt::Key_T).toInt(), false);
    setKeyMapping("CStickDown", settings.value("CStickDown", Qt::Key_G).toInt(), false);
    setKeyMapping("CStickLeft", settings.value("CStickLeft", Qt::Key_F).toInt(), false);
    setKeyMapping("CStickRight", settings.value("CStickRight", Qt::Key_H).toInt(), false);
}