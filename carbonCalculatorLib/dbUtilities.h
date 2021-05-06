#ifndef DBUTILITIES_H
#define DBUTILITIES_H

    #ifndef NODATA
        #define NODATA -9999
    #endif

    class QVariant;
    class QString;

    bool getValue(QVariant myRs, int* myValue);
    bool getValue(QVariant myRs, float* myValue);
    bool getValue(QVariant myRs, double* myValue);
    bool getValue(QVariant myRs, QString* myValue);

    bool searchDataPath(QString &dataPath);
    QString getFileName(QString fileNameComplete);


#endif // DBUTILITIES_H
