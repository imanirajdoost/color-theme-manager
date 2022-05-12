#ifndef XMLREADER_H
#define XMLREADER_H

#include <QFile>
#include <set>
#include <QtXml>
#include <QTextStream>
#include "colorpair.h"
#include "systemstat.h"
#include "theme.h"

class XMLReader
{
public:
    XMLReader();
    void read(QFile& file, QList<Theme*>* listOfThemes);
    void update(Theme* _theme, QString content);
    void write(SystemStat* stat);
    SystemStat* getLastStat();
private:
    std::set<ColorPair, CompareColorPair> m_set;
};

#endif // XMLREADER_H
