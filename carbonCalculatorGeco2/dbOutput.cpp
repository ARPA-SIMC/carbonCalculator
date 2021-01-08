#include "dbOutput.h"
#include "inputOutput.h"

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <iostream>


bool createOutputDB(QSqlDatabase &dbOutput, QString dbName)
{
    dbOutput = QSqlDatabase::addDatabase("QSQLITE", "output");
    dbOutput.setDatabaseName(dbName);

    if (! dbOutput.open())
    {
        std::cout << "Error opening output db:" << dbOutput.lastError().text().toStdString() << std::endl;
        return false;
    }

    if (! createTableGeneral(dbOutput))
        return false;

    // TODO other tables

    return true;
}


bool createTableGeneral(QSqlDatabase &dbOutput)
{
    QString queryString = "DROP TABLE general";
    QSqlQuery myQuery = dbOutput.exec(queryString);

    queryString = "CREATE TABLE general "
                  " (id TEXT, enterprise_name TEXT, nr_field INTEGER, year INTEGER, "
                  " country TEXT, latitude REAL, longitude REAL, "
                  " field_size REAL, field_slope REAL )";

    myQuery = dbOutput.exec(queryString);

    if (myQuery.lastError().isValid())
    {
        std::cout << "Error in creating table 'general': " << myQuery.lastError().text().toStdString();
        return false;
    }

    return true;
}


bool saveTableGeneral(QString id, QSqlDatabase &dbOutput, TinputData &inputData, QString tableName)
{
    QString queryOutput = "INSERT INTO " + tableName
                       + " (id, enterprise_name, nr_field, year, country, latitude, longitude, field_size, field_slope) "
                       " VALUES ";

    queryOutput += "('" + id + "'"
                 + ",'" + inputData.general.enterpriseName + "'"
                 + "," + QString::number(inputData.general.nrField)
                 + "," + QString::number(inputData.general.year)
                 + ",'" + inputData.general.idCountry + "'"
                 + "," + QString::number(inputData.general.latitude, 'g', 3)
                 + "," + QString::number(inputData.general.longitude, 'g', 3)
                 + "," + QString::number(inputData.general.fieldSize, 'g', 3)
                 + "," + QString::number(inputData.general.fieldSlope, 'g', 3)
                 + ")";

    QSqlQuery myQuery = dbOutput.exec(queryOutput);
    if (myQuery.lastError().isValid())
    {
        std::cout << "Error in saving table " + tableName.toStdString() + ": " << myQuery.lastError().text().toStdString();
        return false;
    }

    return true;
}


bool saveOutput(QString id, QSqlDatabase &dbOutput, TinputData &inputData)
{
    if (! saveTableGeneral(id, dbOutput, inputData, "general"))
        return false;

    // TODO save other tables

    return true;
}
