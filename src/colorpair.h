#ifndef COLORPAIR_H
#define COLORPAIR_H

#include <QString>
#include <QColor>
#include <iostream>

class ColorPair
{
public:
    ColorPair(); //Empty constructor
    ColorPair(const ColorPair &color); //Constructor by copy
    ColorPair(const QString &id, const QColor &color1, const QColor &color2);
    ColorPair(const QString &id);       //Constructor that initializes id
    static QString toRGBA(const QColor &color);
    static QColor fromRGBA(const QString &colorStr);

    QColor getColorSource() const;
    QColor getColorTarget() const;
    QString getId() const;
    void setColorSource(const QColor &color);
    void setColorTarget(const QColor &color);
private:
    QString m_id;
    QColor colorSource;
    QColor colorTarget;
};

class CompareColorPair
{
public:
    CompareColorPair() {}
    bool operator()(const ColorPair& col1, const ColorPair& col2) const;
};

#endif // COLORPAIR_H
