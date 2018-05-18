#ifndef FMZIPINFO_H
#define FMZIPINFO_H

#include <QMap>
#include <QFile>

#ifdef __linux__
#include <quazip5/quazip.h>
#include <quazip5/quazipfile.h>
#else
#include <quazip/quazip.h>
#include <quazip/quazipfile.h>
#endif

class FMZipInfo
{
public:
    FMZipInfo();
    FMZipInfo(QString);
    QString &operator[](QString i);
    QMap<QString, QString> data;
};

#endif // FMZIPINFO_H
