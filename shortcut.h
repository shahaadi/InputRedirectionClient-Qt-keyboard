#ifndef SHORTCUT_H
#define SHORTCUT_H
#include <QMetaType>
#include <QDataStream>
#include <QColor>
#include <QPoint>

struct ShortCut
{
    QString name;
    int button; // Changed from QGamepadManager::GamepadButton to int
    QPoint pos;
    QColor color;

};
Q_DECLARE_METATYPE(ShortCut)

extern QDataStream &operator<<(QDataStream &out, const ShortCut &obj);
extern QDataStream &operator>>(QDataStream &in, ShortCut &obj);


#endif // SHORTCUT_H