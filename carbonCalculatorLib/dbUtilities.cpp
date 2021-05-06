#include "dbUtilities.h"
#include <QVariant>
#include <QDir>

bool getValue(QVariant myRs, int* myValue)
{
    if (! myRs.isValid() || myRs.isNull())
    {
        *myValue = NODATA;
    }
    else
    {
        if (myRs == "" || myRs == "NULL")
            *myValue = NODATA;
        else
            *myValue = myRs.toInt();
    }
    return (*myValue != NODATA);
}


bool getValue(QVariant myRs, float* myValue)
{
    if (myRs.isNull())
        *myValue = NODATA;
    else
    {
        if (myRs == "")
            *myValue = NODATA;
        else
            *myValue = myRs.toFloat();
    }
    return (int(*myValue) != int(NODATA));
}


bool getValue(QVariant myRs, double* myValue)
{
    if (myRs.isNull())
        *myValue = NODATA;
    else
    {
        if (myRs == "")
            *myValue = NODATA;
        else
            *myValue = myRs.toDouble();
    }
    return (int(*myValue) != int(NODATA));
}


bool getValue(QVariant myRs, QString* myValue)
{
    if (myRs.isNull())
        *myValue = "";
    else
        *myValue = myRs.toString();

    return (*myValue != "");
}


bool searchDataPath(QString  &dataPath)
{
    QString myPath = QDir::currentPath();
    QString myRoot = QDir::rootPath();
    // win only: it can run on drive different from C:
    QString winRoot = myPath.left(3);

    bool isFound = false;
    while (! isFound)
    {
        if (QDir(myPath + "/DATA").exists())
        {
            isFound = true;
            break;
        }

        if (QDir::cleanPath(myPath) == myRoot || QDir::cleanPath(myPath) == winRoot)
            break;

        myPath = QFileInfo(myPath).dir().absolutePath();
    }
    if (! isFound) return false;

    dataPath = QDir::cleanPath(myPath) + "/DATA/";
    return true;
}


QString getFileName(QString fileNameComplete)
{
    QString c;
    QString fileName = "";
    for (int i = fileNameComplete.length()-1; i >= 0; i--)
    {
        c = fileNameComplete.mid(i,1);
        if ((c != "\\") && (c != "/"))
            fileName = c + fileName;
        else
            return fileName;
    }
    return fileName;
}

