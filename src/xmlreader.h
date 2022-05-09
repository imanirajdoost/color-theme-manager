#ifndef XMLREADER_H
#define XMLREADER_H

#include <QFile>
#include <set>
#include <QtXml>
#include <QTextStream>
#include "colorpair.h"
#include "theme.h"

class XMLReader
{
public:
    XMLReader();
    void read(QFile& file, QList<Theme*>* listOfThemes);
private:
    std::set<ColorPair, CompareColorPair> m_set;
};

#endif // XMLREADER_H
