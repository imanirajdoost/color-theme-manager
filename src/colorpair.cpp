#include "colorpair.h"


ColorPair::ColorPair() : m_id()
{

}
ColorPair::ColorPair(const ColorPair &color) : m_id()
{

}
ColorPair::ColorPair(const QString &id, const QColor &color1, const QColor &color2) : m_id(id), colorSource(color1), colorTarget(color2)
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

QColor ColorPair::getColorSource() const
{
    return colorSource;
}

void ColorPair::setColorSource(const QColor &color)
{
    colorSource = color;
}

QColor ColorPair::getColorTarget() const
{
    return colorTarget;
}

void ColorPair::setColorTarget(const QColor &color)
{
    colorTarget = color;
}

QString ColorPair::getId() const
{
    return m_id;
}

void ColorPair::setId(const QString _id)
{
    m_id = _id;
}

bool CompareColorPair::operator ()(const ColorPair& col1, const ColorPair &col2) const
{
    if(QString::compare(col1.getId(), col2.getId(), Qt::CaseInsensitive) < 0)
        return true;
    else
        return false;
}

