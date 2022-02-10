#include "colorpair.h"


ColorPair::ColorPair() : m_id()
{

}
ColorPair::ColorPair(const ColorPair &color) : m_id()
{

}
ColorPair::ColorPair(const QString &id, const QColor &color1, const QColor color2) : m_id()
{

}
ColorPair::ColorPair(const QString &id) : m_id()
{

}
QString ColorPair::toRGBA(const QColor &color)
{
    return color.name(QColor::HexArgb);
}
QColor ColorPair::fromRGBA(const QString &colorStr)
{
    return QColor(colorStr);
}

QColor ColorPair::getColorSource()
{
    return colorSource;
}

QColor ColorPair::getColorTarget()
{
    return colorTarget;
}

QString ColorPair::getId()
{
    return m_id;
}

QString ColorPair::test()
{
    return QString("testttt");
}

