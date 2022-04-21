#ifndef THEME_H
#define THEME_H

#include "colorpair.h"

class Theme
{
public:
    Theme();
    void addColorPair(const ColorPair& _colorToAdd);
    QString themeName;
private:
    QList<ColorPair> colorpairList;
};

#endif // THEME_H