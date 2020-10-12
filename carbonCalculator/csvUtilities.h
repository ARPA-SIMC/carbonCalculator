#ifndef CSVUTILITIES_H
#define CSVUTILITIES_H

    #include <QStringList>

    bool importCsvData(QString csvFileName, int nrValues, bool headerExist, std::vector<QStringList> &data, QString &error);


#endif // CSVUTILITIES_H
