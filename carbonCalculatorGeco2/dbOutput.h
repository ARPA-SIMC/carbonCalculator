#ifndef DBOUTPUT_H
#define DBOUTPUT_H

    class QSqlDatabase;
    class QString;
    struct TinputData;

    bool createOutputDB(QSqlDatabase &dbOutput, QString dbName);
    bool createTableGeneral(QSqlDatabase &dbOutput);
    bool saveTableGeneral(QString id, QSqlDatabase &dbOutput, TinputData &inputData);
    bool saveOutput(QString id, QSqlDatabase &dbOutput, TinputData &inputData);


#endif // DBOUTPUT_H
