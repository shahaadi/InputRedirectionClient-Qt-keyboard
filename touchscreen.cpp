#include "touchscreen.h"
#include "global.h"
#include <QMenu>
#include "configwindow.h" // NEW: Include the header for the ConfigWindow class

// --- Keyboard Event Handlers for this window ---

void TouchScreen::keyPressEvent(QKeyEvent *event) {
    // Don't process game input if a config window is open
    if (settingsConfig->isVisible() || gpConfigurator->isVisible()) {
        QWidget::keyPressEvent(event);
        return;
    }

    if (!event->isAutoRepeat()) {
        pressedKeys.insert(event->key()); //

        // Handle special interface buttons
        if (event->key() == keyHome) interfaceButtons |= 1;
        if (event->key() == keyPower) interfaceButtons |= 2;
        if (event->key() == keyPowerLong) interfaceButtons |= 4;

        // Handle touchscreen shortcuts
        for (const auto& curShort : listShortcuts) {
            if (curShort.button == event->key()) {
                touchScreenPressed = true;
                touchScreenPosition = curShort.pos * tsRatio;
            }
        }
    }
}

void TouchScreen::keyReleaseEvent(QKeyEvent *event) {
    if (!event->isAutoRepeat()) {
        pressedKeys.remove(event->key()); //

        // Handle special interface buttons
        if (event->key() == keyHome) interfaceButtons &= ~1;
        if (event->key() == keyPower) interfaceButtons &= ~2;
        if (event->key() == keyPowerLong) interfaceButtons &= ~4;

        // Handle touchscreen shortcuts
        for (const auto& curShort : listShortcuts) {
            if (curShort.button == event->key()) {
                touchScreenPressed = false;
            }
        }
    }
}


// --- Original TouchScreen Class Implementation ---

void TouchScreen::setTouchScreenPressed(bool b)
{
    touchScreenPressed = b;
}

bool TouchScreen::isTouchScreenPressed()
{
   return touchScreenPressed;
}

QSize TouchScreen::getTouchScreenSize()
{
    return touchScreenSize;
}

QPoint TouchScreen::getTouchScreenPosition()
{
    return touchScreenPosition;
}

TouchScreen::TouchScreen(QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);
    this->setWindowTitle(tr("InputRedirectionClient-Qt - Touch screen"));
    this->setContextMenuPolicy(Qt::CustomContextMenu);

    // This is crucial: allows the window to receive keyboard focus when clicked
    this->setFocusPolicy(Qt::StrongFocus);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(ShowContextMenu(const QPoint&)));

    bgLabel = new QLabel(this);

    updatePixmap();
    setMinimumWidth(TOUCH_SCREEN_WIDTH);
    setMinimumHeight(TOUCH_SCREEN_HEIGHT);

    bgLabel->setScaledContents(true);
    bgLabel->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );

    ellipNeedDraw = true;
}

void TouchScreen::ShowContextMenu(const QPoint& pos)
{
    QMenu* myMenu = new QMenu();
    QString strOpenOverlay = "Open Overlay Image...";
    QString clearOverlayBtn = "Clear Overlay";
    QString strPtToBtn = "Set point to button...";
    QPoint globalPos = this->mapToGlobal(pos);

    myMenu->addAction(strOpenOverlay);
    myMenu->addAction(clearOverlayBtn);
    myMenu->addSeparator();
    myMenu->addAction(strPtToBtn);

    myMenu->popup(globalPos);
    QAction* selectedItem = myMenu->exec(globalPos);

    QPoint newPos;
    newPos.setX(pos.x());
    newPos.setY(pos.y());
    //If custom size, scale the point to orig 3ds touchscreen size
    if(this->width() != TOUCH_SCREEN_WIDTH ||
       this->height() != TOUCH_SCREEN_HEIGHT)
    {
        newPos.setX((TOUCH_SCREEN_WIDTH*pos.x())/this->width());
        newPos.setY((TOUCH_SCREEN_HEIGHT*pos.y())/this->height());
    }

    //Update the shortcut gui position title text
    tsShortcutGui.setCurrentPos(newPos);

    if(!selectedItem)
    {
        myMenu->close();
        return;
    }

    if(selectedItem->text() == strOpenOverlay)
    {
        QString strPic = QFileDialog::getOpenFileName(this,
                       tr("Open Touchscreen Image (320x240)"), "MyDocuments",
                       tr("Image Files (*.jpg *.jpeg *.png *.bmp *.gif *.pbm *.pgm *.ppm *.xbm *.xpm)"));

         if(!strPic.isNull())
         {
             settings.setValue("tsBackgroundImage", strPic);
             updatePixmap();
         }
    }

    if(selectedItem->text() == strPtToBtn )
    {
        if(tsShortcutGui.isVisible())
        {
            tsShortcutGui.updateTitleText();
        }

        tsShortcutGui.setVisible(true);

    }

    if(selectedItem->text() == clearOverlayBtn)
    {
        clearImage();
    }

    myMenu->deleteLater();
}

void TouchScreen::resizeEvent(QResizeEvent* e)
{
        QSize newWinSize = e->size();
        QSize curWinSize = e->oldSize();
        QSize propWinSize = e->size();

        if(curWinSize.height() != newWinSize.height())
        {
            propWinSize.setWidth((TOUCH_SCREEN_WIDTH*newWinSize.height())/TOUCH_SCREEN_HEIGHT);
           propWinSize.setHeight(newWinSize.height());
        }

        if(curWinSize.width() != newWinSize.width())
        {
            propWinSize.setWidth(newWinSize.width());
            propWinSize.setHeight((TOUCH_SCREEN_HEIGHT*newWinSize.width())/TOUCH_SCREEN_WIDTH);
        }

        touchScreenSize = propWinSize;
        this->resize(propWinSize);
        bgLabel->setFixedHeight(this->height());
        bgLabel->setFixedWidth(this->width());

        tsRatio = (double)this->width() / (double)TOUCH_SCREEN_WIDTH;
}

void TouchScreen::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        touchScreenPressed = true;
        touchScreenPosition = ev->pos();
    }
}

void TouchScreen::mouseMoveEvent(QMouseEvent *ev)
{
    if(touchScreenPressed && (ev->buttons() & Qt::LeftButton))
    {
        touchScreenPosition = ev->pos();
    }
}

void TouchScreen::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        touchScreenPressed = false;
    }
}

void TouchScreen::closeEvent(QCloseEvent *ev)
{
    touchScreenPressed = false;
    ev->accept();
}

void TouchScreen::updatePixmap(void)
{
    QString strPic = settings.value("tsBackgroundImage", "").toString();
    QPixmap newPic(strPic);

    if (newPic.isNull())
    {
        newPic = QPixmap(TOUCH_SCREEN_WIDTH, TOUCH_SCREEN_HEIGHT);
        newPic.fill(Qt::transparent);
    } else {
        bgLabel->setPixmap(newPic.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}


void TouchScreen::paintEvent(QPaintEvent* e)
{
    QPainter painter(this);
    QString strPic = settings.value("tsBackgroundImage", "").toString();

    if (!strPic.isEmpty()) {
        QPixmap bg(strPic);
        painter.drawPixmap(this->rect(), bg);
    } else {
        painter.fillRect(this->rect(), Qt::black);
    }

    // Draw shortcut ellipses
    for (const auto& curShort : listShortcuts) {
        painter.setBrush(QBrush(curShort.color));
        painter.setPen(Qt::NoPen);

        // Scale the position from 3DS coordinates to current window coordinates
        double scaleX = (double)this->width() / TOUCH_SCREEN_WIDTH;
        double scaleY = (double)this->height() / TOUCH_SCREEN_HEIGHT;
        QPointF center(curShort.pos.x() * scaleX, curShort.pos.y() * scaleY);
        painter.drawEllipse(center, 5, 5);
    }

    QWidget::paintEvent(e);
}

void TouchScreen::clearImage(void)
{
    settings.setValue("tsBackgroundImage", "");
    update(); // Use update() to trigger a repaint
}