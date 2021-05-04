#ifndef DBOUTPUT_H
#define DBOUTPUT_H

//#include "carbonCalculator.h"
#include "inputOutput.h"

    class QSqlDatabase;
    class QString;
    struct TinputData;

    bool createOutputDB(QSqlDatabase &dbOutput, QString dbName);
    bool createTableGeneral(QSqlDatabase &dbOutput);

    bool saveTableGeneral(QString id, QSqlDatabase &dbOutput, TinputData &inputData, QString tableName);
    bool saveOutput(QString id, QSqlDatabase &dbOutput, TinputData &inputData, CarbonCalculator calculatorCO2,double credits, int *isAccepted);

    bool createTableClimate(QSqlDatabase &dbOutput);
    bool saveTableClimate(QString id, QSqlDatabase &dbOutput, TinputData &inputData, QString tableName);

    bool createTableSoil(QSqlDatabase &dbOutput);
    bool saveTableSoil(QString id, QSqlDatabase &dbOutput, TinputData &inputData, QString tableName);

    bool createTableCropField(QSqlDatabase &dbOutput);
    bool saveTableCropField(QString id, QSqlDatabase &dbOutput, TinputData &inputData, QString tableName);

    bool createTableAgronomicInputs(QSqlDatabase &dbOutput);
    bool saveTableAgronomicInputs(QString id, QSqlDatabase &dbOutput, TinputData &inputData, QString tableName);

    bool createTableEnergy(QSqlDatabase &dbOutput);
    bool saveTableEnergy(QString id, QSqlDatabase &dbOutput, TinputData &inputData, QString tableName);

    bool createTableCarbonBudget(QSqlDatabase &dbOutput);
    bool saveTableCarbonBudget(QString id, QSqlDatabase &dbOutput, CarbonCalculator calculatorCO2, QString tableName,double creditsPerHectare, double credits, int *isAccepted);

    bool createTableCarbonDynamics(QSqlDatabase &dbOutput);
    bool saveTableCarbonDynamics(QString id, QSqlDatabase &dbOutput, CarbonCalculator calculatorCO2, QString tableName);


    bool saveOutputBuyer(QString id, QSqlDatabase &dbOutput, TinputDataBuyer &inputData, BuyerCalculator buyerCalculatorCO2, double debits);
    bool createTableBuyer(QSqlDatabase &dbOutput);
    bool saveTableBuyer(QString id_buyer, QSqlDatabase &dbOutput, BuyerCalculator buyerCalculatorCO2, TinputDataBuyer &inputData, QString tableName, double debits);


#endif // DBOUTPUT_H
