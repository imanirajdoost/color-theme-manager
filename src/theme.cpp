#include "theme.h"


Theme::Theme()
{

}

void Theme::addColorPair(const ColorPair& _colorToAdd)
{
    colorpairList.append(_colorToAdd);
}
