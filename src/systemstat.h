#ifndef SYSTEMSTAT_H
#define SYSTEMSTAT_H

#include <QList>
#include <QString>

class SystemStat
{
public:
    SystemStat();

    QList<QString>* linksToFiles;
};

#endif // SYSTEMSTAT_H
