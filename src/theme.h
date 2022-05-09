#ifndef THEME_H
#define THEME_H

#include "colorpair.h"

class Theme
{
public:
    Theme();
    void addColorPair(ColorPair* _colorToAdd);
    void setName(QString _name);
    QList<ColorPair*>* getColorPair();
    QString themeName;
private:
    QList<ColorPair*>* colorpairList;
};

#endif // THEME_H
