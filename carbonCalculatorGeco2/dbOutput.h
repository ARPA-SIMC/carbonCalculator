#ifndef DBOUTPUT_H
#define DBOUTPUT_H

    class QSqlDatabase;
    class QString;
    struct TinputData;

    bool createOutputDB(QSqlDatabase &dbOutput, QString dbName);
    bool createTableGeneral(QSqlDatabase &dbOutput);

    bool saveTableGeneral(QString id, QSqlDatabase &dbOutput, TinputData &inputData, QString tableName);
    bool saveOutput(QString id, QSqlDatabase &dbOutput, TinputData &inputData);

    bool createTableClimate(QSqlDatabase &dbOutput);
    bool saveTableClimate(QSqlDatabase &dbOutput, TinputData &inputData, QString tableName);

    bool createTableSoil(QSqlDatabase &dbOutput);
    bool saveTableSoil(QSqlDatabase &dbOutput, TinputData &inputData, QString tableName);



#endif // DBOUTPUT_H
