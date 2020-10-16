#ifndef DBQUERIES_H
#define DBQUERIES_H

    class QSqlDatabase;
    class QString;
    class CarbonCalculator;

    bool readRenewables(QString idCountry, int year, QSqlDatabase &db, CarbonCalculator &carbonCalculator, QString &error);
    bool readFertilizer(QString idFertiliser, QSqlDatabase &db, CarbonCalculator &calculator, QString &error);
    bool readResidue(QString idResidue, QSqlDatabase &db, CarbonCalculator &calculator, QString &error);
    bool readCropParameters(QString idCrop, QSqlDatabase &db, CarbonCalculator &calculator, QString &error);
    bool readBouwmanNH4(QString idFeature, QSqlDatabase &db, CarbonCalculator &calculator, QString &error);

#endif // DBQUERIES_H
