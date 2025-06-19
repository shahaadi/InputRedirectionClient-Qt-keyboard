#ifndef TSSHORTCUT_H
#define TSSHORTCUT_H
#include "global.h"
#include <QListView>
#include <QList>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QComboBox>
#include <QColorDialog>
#include <QKeyEvent>
#include <QKeySequence>

class TsShortcut : public QWidget
{
    Q_OBJECT

private:
    QString wTitle;
    QVBoxLayout  *layout;
    QListWidget    *lstWidget;
    QLabel       *lblDirections;
    QPushButton  *btnColorDialog, *btnCreateShort,
                 *btnDelShort, *btnHelp, *btnPressNow, *btnSetKey;
    QLineEdit* txtShortName;
    QPoint curPos;
    QColor curColor;

    QLabel* lblMappedKey;
    int mappedKeyForNewShortcut = 0;
    bool isWaitingForKey = false;

protected:
    void keyPressEvent(QKeyEvent *event) override {
        if (isWaitingForKey) {
            mappedKeyForNewShortcut = event->key();
            lblMappedKey->setText(QKeySequence(mappedKeyForNewShortcut).toString());
            isWaitingForKey = false;
            lblDirections->setText("Enter a name for the new shortcut.");
        } else {
            QWidget::keyPressEvent(event);
        }
    }

public:
    TsShortcut(QWidget *parent = nullptr) : QWidget(parent)
    {
        layout = new QVBoxLayout(this);

        lblDirections = new QLabel(this);
        lstWidget = new QListWidget(this);
        btnColorDialog = new QPushButton(this);
        btnCreateShort = new QPushButton(this);
        btnPressNow = new QPushButton(this);
        btnDelShort = new QPushButton(this);
        btnHelp = new QPushButton(this);
        txtShortName = new QLineEdit(this);
        btnSetKey = new QPushButton("Set Key", this);
        lblMappedKey = new QLabel("None", this);

        lblDirections->setText("Right-click the touchscreen to set a location.");
        btnColorDialog->setText("Choose &Color");
        btnPressNow->setText("Press Selected &Shortcut");
        btnCreateShort->setText("&Create");
        btnDelShort->setText("&Delete");
        btnHelp->setText("&Help");
        txtShortName->setPlaceholderText("Enter shortcut name...");


        layout->addWidget(lblDirections);
        layout->addWidget(lstWidget);

        QFormLayout* form = new QFormLayout();
        form->addRow("Name:", txtShortName);
        form->addRow(btnSetKey, lblMappedKey);
        form->addRow("Color:", btnColorDialog);
        layout->addLayout(form);

        layout->addWidget(btnCreateShort);
        layout->addSpacing(10);
        layout->addWidget(btnPressNow);
        layout->addWidget(btnDelShort);
        layout->addWidget(btnHelp);

        this->setMinimumSize(300, 200);
        this->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
        this->setVisible(false);

        connect(btnSetKey, &QPushButton::clicked, this, [this](){
            isWaitingForKey = true;
            this->setFocus();
            lblDirections->setText("Press any key to map to this shortcut...");
        });

        connect(btnColorDialog, &QPushButton::released, this, [this]() {
            QColor color = QColorDialog::getColor(Qt::yellow, this );
                if( color.isValid() )
                {
                    curColor = color;
                    QString qss = QString("background-color: %1").arg(color.name());
                    btnColorDialog->setStyleSheet(qss);
                }
         });

        connect(btnCreateShort, &QPushButton::released, this, [this]() {
            QString newShortName = txtShortName->text();
            if(newShortName.isEmpty()) {
                QMessageBox::warning(this, "Error", "Cannot create a shortcut without a name.");
                return;
            }
            if(mappedKeyForNewShortcut == 0) {
                QMessageBox::warning(this, "Error", "No key has been set for this shortcut.");
                return;
            }

            ShortCut newShortCut;
            newShortCut.name = newShortName;
            newShortCut.button = mappedKeyForNewShortcut;
            newShortCut.pos = curPos;
            newShortCut.color = curColor.isValid() ? curColor : Qt::blue;

            lstWidget->addItem(newShortName);
            listShortcuts.push_back(newShortCut);

            // Reset for next shortcut
            txtShortName->clear();
            btnColorDialog->setStyleSheet("");
            curColor = nullptr;
            mappedKeyForNewShortcut = 0;
            lblMappedKey->setText("None");
            lblDirections->setText("Shortcut created successfully.");
         });

        connect(btnDelShort, &QPushButton::released, this, [this]() {
            if(lstWidget->selectedItems().size() != 0)
            {
              listShortcuts.erase(listShortcuts.begin()+(lstWidget->currentRow()));
              qDeleteAll(lstWidget->selectedItems());
            }
         });

        connect(btnPressNow, &QPushButton::pressed, this, [this]() {
            if(lstWidget->selectedItems().size() != 0)
            {
               touchScreenPressed = true;
               touchScreenPosition = listShortcuts[lstWidget->currentRow()].pos*tsRatio;
            }
         });

         connect(btnPressNow, &QPushButton::released, []() {
                touchScreenPressed = false;
          });


        connect(btnHelp, &QPushButton::released, this, []() {
            QMessageBox *msgBox = new QMessageBox(0);
            msgBox->setText("How to Create a Touchscreen Shortcut");
            msgBox->setInformativeText(tr("1. Right-click the position on the touchscreen window.\n"
                                          "2. Open this menu.\n"
                                          "3. Type a name for your shortcut.\n"
                                          "4. Click 'Set Key' and press the keyboard key you want to trigger this shortcut.\n"
                                          "5. Choose a color for the shortcut's circle on the touchpad.\n"
                                          "6. Press 'Create'."));
            msgBox->show();
         });
    }

    void setCurrentPos(QPoint pos)
    {
        curPos = pos;
    }

    void updateTitleText()
    {
        wTitle = QString("New Shortcut at X: %1 Y: %2").arg(QString::number(curPos.x())).arg(QString::number(curPos.y()));
        this->setWindowTitle(wTitle);
    }

    void showEvent(QShowEvent * event) override
    {
        lstWidget->clear();
        for (const auto& shortcut : listShortcuts)
        {
            lstWidget->addItem(shortcut.name);
        }

        updateTitleText();
        QWidget::showEvent(event);
    }
};

#endif // TSSHORTCUT_H