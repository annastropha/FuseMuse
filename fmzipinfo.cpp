#include "fmzipinfo.h"

#include <iostream>

FMZipInfo::FMZipInfo()
{
}

FMZipInfo::FMZipInfo(QString filePath)
{
    data.insert("ZipFilePath",filePath);

    QuaZip zip(filePath);
    zip.open(QuaZip::mdUnzip);
    zip.setCurrentFile("packetdata");
    QuaZipFile qzFile(&zip);
    qzFile.open(QIODevice::ReadOnly);

    while(!qzFile.atEnd()) {
        QString line = qzFile.readLine();
        line.remove(QChar('\n'));
        QStringList parts = line.split("=");
        QString propName = parts[0].trimmed();
        parts.removeFirst();
        QString propVal = parts.join("=").trimmed();

        //std::cout << propName.toStdString() << "===" << propVal.toStdString() << std::endl;

        data.insert(propName, propVal);
    }

    qzFile.close();
}

QString& FMZipInfo::operator[](QString key) {
    return data[key];
}
