#include "csvUtilities.h"
#include <QFile>
#include <QTextStream>

bool importCsvData(QString csvFileName, int nrValues, bool headerExist, std::vector<QStringList> &data, QString &error)
{
    // check file
    QFile myFile(csvFileName);
    if(! myFile.open (QIODevice::ReadOnly))
    {
        error = myFile.errorString();
        return false;
    }

    QTextStream myStream (&myFile);
    if (myStream.atEnd())
    {
        error = "file is empty.";
        myFile.close();
        return false;
    }
    else if (headerExist)
    {
        // skip header (first row)
        QStringList header = myStream.readLine().split(',');
    }

    // read data
    data.clear();
    while(!myStream.atEnd())
    {
        QStringList line = myStream.readLine().split(',');

        // skip incomplete lines
        if (line.length() < nrValues) continue;

        data.push_back(line);
    }
    myFile.close();

    if (data.size() == 0)
    {
        error = "Error: data are incomplete.";
        return false;
    }

    return true;
}


