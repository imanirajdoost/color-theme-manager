#include "theme.h"


Theme::Theme()
{
    colorpairList = new QList<ColorPair>();
}

void Theme::addColorPair(ColorPair& _colorToAdd)
{
    colorpairList->append(_colorToAdd);
}

void Theme::setName(QString _name)
{
    themeName = _name;
}
