#include "global.h"
#include "gpmanager.h"
#include "tsshortcut.h"
#include <QPainter>
#include <QKeyEvent> // Added for keyboard events

#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H

#include "tsshortcut.h"

struct TouchScreen : public QWidget {
    Q_OBJECT
public slots:
    void ShowContextMenu(const QPoint& pos);

private:
    QLabel *bgLabel;
    TsShortcut tsShortcutGui;

protected:
    // Added keyboard event handlers
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    // Added override specifier for consistency
    void resizeEvent(QResizeEvent* e) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void closeEvent(QCloseEvent *ev) override;
    void paintEvent(QPaintEvent* e) override;

public:
    TouchScreen(QWidget *parent = nullptr);

    bool ellipNeedDraw;

    QPoint getTouchScreenPosition();
    QSize getTouchScreenSize();
    bool isTouchScreenPressed();
    void setTouchScreenPressed(bool b);
    void updatePixmap(void);
    void clearImage(void);

   ~TouchScreen(void)
    {
        // The original code had a qDebug here which requires <QDebug>
        // It's commented out to avoid needing another include.
        // qDebug() << "DECON";
        tsShortcutGui.close();
    }
};

#endif // TOUCHSCREEN_H