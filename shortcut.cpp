#include "shortcut.h"
#include <QBuffer>

// The old getButtonId and getIdButton functions are no longer needed.

QDataStream &operator<<(QDataStream &out, const ShortCut &obj)
{
    // Stream the int directly
    out << obj.name << obj.pos << obj.color << obj.button;
    return out;
}

QDataStream &operator>>(QDataStream &in, ShortCut &obj)
{
    // Read the int directly
    in >> obj.name >> obj.pos >> obj.color >> obj.button;
    return in;
}