#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QFile>

#include <stdio.h>
#include <math.h>
#include <iostream>

#include "carbonCalculator.h"
#include "dbUtilities.h"


//cropResidueManagement cropRes;
static carbonCalculator calculatorCO2;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    // input
    // int cropType = 1;
    // int residueType = 1;
    double residueWeight = 5;
    calculatorCO2.cropResidue.computeEquivalentCO2(residueWeight);

    TkindOfEnergy kindOfEnergy;
    kindOfEnergy.fromElectricityGrid = 50; // kWh
    kindOfEnergy.fromElectricityOwnHydropower = 50; // kWh
    kindOfEnergy.fromElectricityOwnPhotovoltaic = 50; // kWh
    kindOfEnergy.fromElectricityOwnWind = 50; // kWh
    kindOfEnergy.fromFuelBiodiesel = NODATA;
    kindOfEnergy.fromFuelBioethanol = NODATA;
    kindOfEnergy.fromFuelCoal = NODATA;
    kindOfEnergy.fromFuelDiesel = NODATA;
    kindOfEnergy.fromFuelHighDensityBiomass = NODATA;
    kindOfEnergy.fromFuelLiquidPropane = NODATA;
    kindOfEnergy.fromFuelOil = NODATA;
    kindOfEnergy.fromFuelPetrol = NODATA;
    kindOfEnergy.fromFuelWood = NODATA;

    // search data path
    QString dataPath;
    if (! searchDataPath(&dataPath))
    {
        std::cout << "Error: missing DATA directory" << std::endl;
        return -1;
    }

    // open database
    QString dbName = dataPath + "carbonCalculatorDataBase.db";
    if (! QFile(dbName).exists())
    {
        std::cout << "Error! db file is missing: " << dbName.toStdString() << std::endl;
        return -1;
    }
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "carbon");
    db.setDatabaseName(dbName);
    if (! db.open())
    {
        std::cout << "Error opening db:" << db.lastError().text().toStdString() << std::endl;
        return -1;
    }

    // query table
    QString idCountry = "ITALY";
    QString queryString = "SELECT * FROM renewable_energy_land WHERE id_country='" + idCountry + "'";
    QSqlQuery query = db.exec(queryString);
    query.last();
    if (! query.isValid())
    {
        std::cout << "Error reading data: " + query.lastError().text().toStdString() << std::endl;
        return -1;
    }

    // read values
    double renewablesPercentage;
    if (! getValue(query.value("percentage"), &renewablesPercentage))
    {
        std::cout << "Error: missing renewables percentage data" << std::endl;
        return -1;
    }
    query.clear();

    // TODO: other queries

    std::cout << "Country: " << idCountry.toStdString() << std::endl;
    std::cout << "Renewables percentage: " << renewablesPercentage << std::endl;

    calculatorCO2.energy.setInput(kindOfEnergy, renewablesPercentage, idCountry);
    calculatorCO2.energy.computeEmissions();


    // output
    /*
    double kgCO2EqDueToN2O = NODATA;
    double kgCO2EqDueToCO2 = NODATA;
    double kgCO2EqDueToCH4 = NODATA;
    double kgCO2EqTotal = NODATA;

    double parameterEquivalentCH4 = 25;
    double parameterEquivalentN2O = 296;
    // parameters to be read from DB
    double parameterDryMatterFraction = 0.9;
    double parameterSlope = 0.29;
    double parameterIntercept = 0;
    double parameterAboveGroundContentN = 0.027;
    double parameterBelowAboveRatio = 0.4;
    double parameterBelowGroundContentN = 0.019;
    int isTreeCrop = 1;

    double parameterEmissionCH4 = 0.065333333;
    double parameterEmissionN2O = 	0.00050675;
    double parameterResidueReconvertedToCO2 = 10;

    double aboveGroundNitrogen;
    aboveGroundNitrogen = parameterAboveGroundContentN*residueWeight;
    double belowGroundResidue;
    belowGroundResidue = parameterBelowAboveRatio*residueWeight;
    double belowGroundNitrogen = belowGroundResidue*parameterBelowGroundContentN;

    double emissionCH4;
    emissionCH4 = parameterEmissionCH4*residueWeight*1000;
    double emissionN2O;
    emissionN2O = parameterEmissionN2O*residueWeight*1000;

    kgCO2EqDueToCH4 = emissionCH4 * parameterEquivalentCH4;
    kgCO2EqDueToN2O = emissionN2O * parameterEquivalentN2O;



    kgCO2EqDueToCO2 = 1000*(parameterDryMatterFraction*residueWeight*parameterResidueReconvertedToCO2/100.);
    printf("%f %f %f\n",kgCO2EqDueToCH4,kgCO2EqDueToN2O,kgCO2EqDueToCO2);
    kgCO2EqTotal = kgCO2EqDueToCO2 + kgCO2EqDueToCH4 + kgCO2EqDueToN2O;
    printf("%f\n",kgCO2EqTotal);*/
    return 0;
}
