#include "mainwidget.h"
#include "global.h"
#include <QKeyEvent>

Widget::Widget(QWidget *parent) : QWidget(parent)
{
    layout = new QVBoxLayout(this);

    addrLineEdit = new QLineEdit(this);
    addrLineEdit->setClearButtonEnabled(true);

    formLayout = new QFormLayout();
    formLayout->addRow(tr("IP &address"), addrLineEdit);

    touchOpacitySlider = new QSlider(Qt::Horizontal);
    touchOpacitySlider->setRange(0, 10);
    touchOpacitySlider->setValue(10);
    touchOpacitySlider->setTickInterval(1);
    formLayout->addRow(tr("TS &Opacity"), touchOpacitySlider);

    homeButton = new QPushButton(tr("&Home"), this);
    powerButton = new QPushButton(tr("&Power"), this);
    longPowerButton = new QPushButton(tr("Power (&long)"), this);
    settingsConfigButton = new QPushButton(tr("&Settings"), this);
    clearImageButton = new QPushButton(tr("&Clear Image"), this);
    configGamepadButton = new QPushButton(tr("&Configure Custom Gamepad"));

    setContextMenuPolicy(Qt::CustomContextMenu);
    
    if (QSysInfo::productType() == "windows" || QSysInfo::productType() == "osx") {
        configGamepadButton->setEnabled(false);
        configGamepadButton->setVisible(false);
    }

    layout->addLayout(formLayout);
    layout->addWidget(homeButton);
    layout->addWidget(powerButton);
    layout->addWidget(longPowerButton);
    layout->addWidget(configGamepadButton);
    layout->addWidget(settingsConfigButton);
    layout->addWidget(clearImageButton);

    gpConfigurator = new GamepadConfigurator();

    connect(addrLineEdit, &QLineEdit::textChanged, this,
            [](const QString &text) {
        ipAddress = text;
        settings.setValue("ipAddress", text);
    });
    connect(configGamepadButton, &QPushButton::released, [](){
       gpConfigurator->showGui();
     });
    connect(homeButton, &QPushButton::pressed, [](){
       interfaceButtons |= 1;
    });
    connect(homeButton, &QPushButton::released, [](){
       interfaceButtons &= ~1;
    });
    connect(powerButton, &QPushButton::pressed, [](){
       interfaceButtons |= 2;
    });
    connect(powerButton, &QPushButton::released, [](){
       interfaceButtons &= ~2;
    });
    connect(gpConfigurator->skipButton, &QPushButton::released, [](){
           gpConfigurator->next();
    });
    connect(gpConfigurator->resetConfigButton, &QPushButton::released, [](){
           QMessageBox *msgBox = new QMessageBox(0);
           QGamepadManager::instance()->resetConfiguration(gpConfigurator->getCurDeviceId());
           msgBox->setText("Reset");
           msgBox->setInformativeText("Please restart the program for changes to take affect.");
           msgBox->show();
    });
    connect(longPowerButton, &QPushButton::pressed, [](){
       interfaceButtons |= 4;
    });
    connect(longPowerButton, &QPushButton::released, [](){
       interfaceButtons &= ~4;
    });
    connect(settingsConfigButton, &QPushButton::released, this,
            [this](){
        if (!settingsConfig->isVisible()) {
            settingsConfig->move(this->x() - settingsConfig->width() - 5,this->y());
            settingsConfig->show();
        } else settingsConfig->hide();
    });
    connect(clearImageButton, &QPushButton::released, this,
            [this](){
       touchScreen->clearImage();
    });
    connect(touchOpacitySlider, &QSlider::valueChanged, this,
            [this](int value){
        touchScreen->setWindowOpacity(value / 10.0);
        touchScreen->update();
    });

    touchScreen = new TouchScreen(nullptr);
    // This now initializes the GLOBAL settingsConfig pointer
    settingsConfig = new ConfigWindow(nullptr, touchScreen);
    this->setWindowTitle(tr("InputRedirectionClient-Qt"));
    addrLineEdit->setText(settings.value("ipAddress", "").toString());
}

void Widget::show(void)
{
    QWidget::show();
    touchScreen->move(this->x() + this->width() + 5,this->y());
    touchScreen->show();
    settingsConfig->hide();
}

void Widget::closeEvent(QCloseEvent *ev)
{
    unsigned int i = 0;
    for (i=0; i<listShortcuts.size(); i++) {
        QString valName = tr("tsShortcut%1").arg(i);
        settings.setValue(valName, QVariant::fromValue(listShortcuts[i]));
    }
    for(; settings.contains(tr("tsShortcut%1").arg(i)); i++) {
        settings.remove(tr("tsShortcut%1").arg(i));
    }
    touchScreen->close();
    settingsConfig->close();
    touchScreen->setTouchScreenPressed(false);
    delete touchScreen;
    ev->accept();
}

void Widget::moveEvent(QMoveEvent *event)
{
    touchScreen->move(touchScreen->pos() + (event->pos() - event->oldPos()));
}

void Widget::showEvent(QShowEvent* event)
{
    qRegisterMetaType<ShortCut>("ShortCut");
    qRegisterMetaTypeStreamOperators<ShortCut>("ShortCut");
    QString valName = "tsShortcut0";
    for(int i = 0; settings.contains(valName); i++) {
        valName = tr("tsShortcut%1").arg(i);
        QVariant variant = settings.value(valName);
        ShortCut curShort = variant.value<ShortCut>();
        if(variant.isValid() && curShort.name != "") {
            if(curShort.name != "") {
             listShortcuts.push_back(curShort);
            }
        }
        else {
            settings.remove(tr("tsShortcut%1").arg(i));
        }
    }
    QWidget::showEvent(event);
}

Widget::~Widget(void)
{
    worker.setLeftAxis(0.0, 0.0);
    worker.setRightAxis(0.0, 0.0);
    buttons = QGamepadManager::GamepadButtons();
    interfaceButtons = 0;
    delete settingsConfig;
}

void Widget::ShowContextMenu(const QPoint &pos) {
    (void)pos;
}