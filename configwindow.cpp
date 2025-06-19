#include "configwindow.h"
#include "gpmanager.h"
#include <QKeySequence>

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

    layout = new QGridLayout(this);
    lblDirections = new QLabel("Click 'Change' to map a key. Click Save to apply changes.");
    layout->addWidget(lblDirections, 0, 0, 1, 4, Qt::AlignCenter);

    // Create the new UI rows
    int currentRow = 1;
    addMappingRow(layout, currentRow++, "A Button", lblKeyA, btnMapA);
    addMappingRow(layout, currentRow++, "B Button", lblKeyB, btnMapB);
    addMappingRow(layout, currentRow++, "X Button", lblKeyX, btnMapX);
    addMappingRow(layout, currentRow++, "Y Button", lblKeyY, btnMapY);
    addMappingRow(layout, currentRow++, "DPad-Up", lblKeyUp, btnMapUp);
    addMappingRow(layout, currentRow++, "DPad-Down", lblKeyDown, btnMapDown);
    addMappingRow(layout, currentRow++, "DPad-Left", lblKeyLeft, btnMapLeft);
    addMappingRow(layout, currentRow++, "DPad-Right", lblKeyRight, btnMapRight);
    addMappingRow(layout, currentRow++, "L Button", lblKeyL, btnMapL);
    addMappingRow(layout, currentRow++, "R Button", lblKeyR, btnMapR);
    addMappingRow(layout, currentRow++, "ZL Button", lblKeyZL, btnMapZL);
    addMappingRow(layout, currentRow++, "ZR Button", lblKeyZR, btnMapZR);
    addMappingRow(layout, currentRow++, "Start", lblKeyStart, btnMapStart);
    addMappingRow(layout, currentRow++, "Select", lblKeySelect, btnMapSelect);
    addMappingRow(layout, currentRow++, "Home", lblKeyHome, btnMapHome);
    addMappingRow(layout, currentRow++, "Power", lblKeyPower, btnMapPower);
    addMappingRow(layout, currentRow++, "Power (Long)", lblKeyPowerLong, btnMapPowerLong);

    auto createMapConnection = [this](QPushButton* button, const QString& buttonName) {
        connect(button, &QPushButton::clicked, this, [this, buttonName]() {
            buttonToRemap = buttonName;
            lblDirections->setText(QString("Press a key for: %1...").arg(buttonName));
            this->setFocus();
        });
    };

    // Corrected variable names here
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


    // --- Checkboxes and other settings ---
    txtCppVal = new QLineEdit();
    txtStickVal = new QLineEdit();
    validator = new QIntValidator();
    txtCppVal->setValidator(validator);
    txtStickVal->setValidator(validator);
    invertYCheckbox = new QCheckBox(this);
    invertYCppCheckbox = new QCheckBox(this);
    swapSticksCheckbox = new QCheckBox(this);
    mhCameraCheckbox = new QCheckBox(this);
    rsSmashCheckbox = new QCheckBox(this);
    rsFaceButtonsCheckbox = new QCheckBox();
    disableCStickCheckbox = new QCheckBox();
    saveButton = new QPushButton(tr("&SAVE and Apply"), this);

    layout->addWidget(new QLabel("Stick Range:"), currentRow, 0, Qt::AlignRight);
    layout->addWidget(txtStickVal, currentRow++, 1, 1, 2);
    layout->addWidget(new QLabel("CPP Range:"), currentRow, 0, Qt::AlignRight);
    layout->addWidget(txtCppVal, currentRow++, 1, 1, 2);

    QGridLayout* checkboxLayout = new QGridLayout();
    checkboxLayout->addWidget(new QLabel("Invert Y axis"), 0, 0);
    checkboxLayout->addWidget(invertYCheckbox, 0, 1);
    checkboxLayout->addWidget(new QLabel("Invert CPP Y"), 0, 2);
    checkboxLayout->addWidget(invertYCppCheckbox, 0, 3);
    checkboxLayout->addWidget(new QLabel("Disable C-Stick"), 1, 0);
    checkboxLayout->addWidget(disableCStickCheckbox, 1, 1);
    checkboxLayout->addWidget(new QLabel("Swap Sticks"), 1, 2);
    checkboxLayout->addWidget(swapSticksCheckbox, 1, 3);
    checkboxLayout->addWidget(new QLabel("RS as DPad"), 2, 0);
    checkboxLayout->addWidget(mhCameraCheckbox, 2, 1);
    checkboxLayout->addWidget(new QLabel("RS as ABXY"), 2, 2);
    checkboxLayout->addWidget(rsFaceButtonsCheckbox, 2, 3);
    checkboxLayout->addWidget(new QLabel("RS as Smash"), 3, 0);
    checkboxLayout->addWidget(rsSmashCheckbox, 3, 1);

    layout->addLayout(checkboxLayout, currentRow++, 0, 1, 4);
    layout->addWidget(saveButton, currentRow, 0, 1, 4);

    connect(invertYCheckbox, &QCheckBox::stateChanged, [](int state){
        settings.setValue("invertY", state == Qt::Checked);
    });
    connect(invertYCppCheckbox, &QCheckBox::stateChanged, [](int state){
        settings.setValue("invertCPPY", state == Qt::Checked);
    });
    connect(swapSticksCheckbox, &QCheckBox::stateChanged, [](int state){
        settings.setValue("swapSticks", state == Qt::Checked);
    });
    connect(rsSmashCheckbox, &QCheckBox::stateChanged, [](int state){
        settings.setValue("rightStickSmash", state == Qt::Checked);
    });
    connect(mhCameraCheckbox, &QCheckBox::stateChanged, [](int state){
        settings.setValue("monsterHunterCamera", state == Qt::Checked);
    });
    connect(disableCStickCheckbox, &QCheckBox::stateChanged, [](int state){
        settings.setValue("cStickDisable", state == Qt::Checked);
    });
    connect(rsFaceButtonsCheckbox, &QCheckBox::stateChanged, [](int state){
        settings.setValue("rightStickABXY", state == Qt::Checked);
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
    // Load settings from file every time the window is shown
    loadKeysFromSettings();
    txtStickVal->setText(settings.value("StickBound", 1488).toString());
    txtCppVal->setText(settings.value("CppBound", 127).toString());
    invertYCheckbox->setChecked(settings.value("invertY", false).toBool());
    invertYCppCheckbox->setChecked(settings.value("invertCPPY", false).toBool());
    swapSticksCheckbox->setChecked(settings.value("swapSticks", false).toBool());
    mhCameraCheckbox->setChecked(settings.value("monsterHunterCamera", false).toBool());
    rsSmashCheckbox->setChecked(settings.value("rightStickSmash", false).toBool());
    disableCStickCheckbox->setChecked(settings.value("cStickDisable", false).toBool());
    rsFaceButtonsCheckbox->setChecked(settings.value("rightStickABXY", false).toBool());
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
}

void ConfigWindow::loadKeysFromSettings() {
    setKeyMapping("ButtonA", settings.value("ButtonA", Qt::Key_J).toInt(), false);
    setKeyMapping("ButtonB", settings.value("ButtonB", Qt::Key_K).toInt(), false);
    setKeyMapping("ButtonX", settings.value("ButtonX", Qt::Key_I).toInt(), false);
    setKeyMapping("ButtonY", settings.value("ButtonY", Qt::Key_L).toInt(), false);
    setKeyMapping("ButtonUp", settings.value("ButtonUp", Qt::Key_W).toInt(), false);
    setKeyMapping("ButtonDown", settings.value("ButtonDown", Qt::Key_S).toInt(), false);
    setKeyMapping("ButtonLeft", settings.value("ButtonLeft", Qt::Key_A).toInt(), false);
    setKeyMapping("ButtonRight", settings.value("ButtonRight", Qt::Key_D).toInt(), false);
    setKeyMapping("ButtonL", settings.value("ButtonL", Qt::Key_Q).toInt(), false);
    setKeyMapping("ButtonR", settings.value("ButtonR", Qt::Key_E).toInt(), false);
    setKeyMapping("ButtonZL", settings.value("ButtonZL", Qt::Key_1).toInt(), false);
    setKeyMapping("ButtonZR", settings.value("ButtonZR", Qt::Key_3).toInt(), false);
    setKeyMapping("ButtonStart", settings.value("ButtonStart", Qt::Key_Return).toInt(), false);
    setKeyMapping("ButtonSelect", settings.value("ButtonSelect", Qt::Key_Shift).toInt(), false);
    setKeyMapping("ButtonHome", settings.value("ButtonHome", Qt::Key_Home).toInt(), false);
    setKeyMapping("ButtonPower", settings.value("ButtonPower", 0).toInt(), false);
    setKeyMapping("ButtonPowerLong", settings.value("ButtonPowerLong", 0).toInt(), false);
}