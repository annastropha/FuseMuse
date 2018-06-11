#include "fmzipinfo.h"

#include <iostream>

FMZipInfo::FMZipInfo()
{
}

FMZipInfo::FMZipInfo(QString filePath)
{
    data.insert("ZipFilePath",filePath);

    QFile packetdata(filePath + "/packetdata");
    packetdata.open(QIODevice::ReadOnly | QIODevice::Text);


    while(!packetdata.atEnd()) {
        QString line = packetdata.readLine();
        std::cout << line.toStdString() << std::endl;
        line.remove(QChar('\n'));
        QStringList parts = line.split("=");
        QString propName = parts[0].trimmed();
        parts.removeFirst();
        QString propVal = parts.join("=").trimmed();

        //std::cout << propName.toStdString() << "===" << propVal.toStdString() << std::endl;

        data.insert(propName, propVal);
    }

    packetdata.close();
}

QString& FMZipInfo::operator[](QString key) {
    return data[key];
}
