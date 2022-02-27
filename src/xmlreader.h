#ifndef XMLREADER_H
#define XMLREADER_H

#include <QFile>
#include <set>
#include <QtXml>
#include <QTextStream>
#include "colorpair.h"

class XMLReader
{
public:
    XMLReader();
    void read(QFile& file);
private:
    std::set<ColorPair, CompareColorPair> m_set;
};

#endif // XMLREADER_H
