#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "global.h"
#include <QMessageBox>
#include <QVariant>
#include "touchscreen.h"
#include "configwindow.h"
#include "gpconfigurator.h"


class Widget : public QWidget
{
    Q_OBJECT

private slots:
    void ShowContextMenu(const QPoint &pos);
private:
    QVBoxLayout  *layout;
    QFormLayout  *formLayout;
    QPushButton  *homeButton, *powerButton, *longPowerButton, *settingsConfigButton,
                 *clearImageButton, *configGamepadButton;
    QLineEdit    *addrLineEdit;
    QSlider      *touchOpacitySlider;
    // ConfigWindow *settingsConfig; // REMOVED: This is now a global variable

protected:
    void closeEvent(QCloseEvent *ev) override;
    void moveEvent(QMoveEvent *event) override;
    void showEvent(QShowEvent* event) override;

public:
    TouchScreen *touchScreen;

    Widget(QWidget *parent = nullptr);

    void show(void);
    virtual ~Widget(void);
};

#endif // MAINWIDGET_H