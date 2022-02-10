#ifndef COLORPAIR_H
#define COLORPAIR_H

#include <QString>
#include <QColor>

class ColorPair
{
public:
    ColorPair(); //Empty constructor
    ColorPair(const ColorPair &color); //Constructor by copy
    ColorPair(const QString &id, const QColor &color1, const QColor color2);
    ColorPair(const QString &id);       //Constructor that initializes id
    static QString toRGBA(const QColor &color);
    static QColor fromRGBA(const QString &colorStr);
    QString test();

    QColor getColorSource();
    QColor getColorTarget();
    QString getId();
private:
    QString m_id;
    QColor colorSource;
    QColor colorTarget;
};

#endif // COLORPAIR_H
