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
#include "csvUtilities.h"


//cropResidueManagement cropRes;
static carbonCalculator calculatorCO2;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    // input
    // int cropType = 1;
    // int residueType = 1;
    double residueWeight = 5;
    //calculatorCO2.cropResidue.computeEquivalentCO2(residueWeight);

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

    // csv file
    QString csvFileName = dataPath + "geco2ModuloRisposte.csv";
    if (! QFile(csvFileName).exists())
    {
        std::cout << "Error! csv file is missing: " << csvFileName.toStdString() << std::endl;
        return -1;
    }
    FILE *fp;
    fp = fopen(csvFileName.toStdString().c_str(),"r");
    int numberOfFields = 1;
    char dummyComma;
    do
    {
        dummyComma = getc(fp);
        if (dummyComma == ',') numberOfFields++;
    } while (dummyComma != '\n' && dummyComma != EOF);
    fclose(fp);


    // read csv

    std::vector<QStringList> data;
    QString error;
    if (! importCsvData(csvFileName, numberOfFields, true, data, error))
    {
        std::cout << "Error: " << error.toStdString() << std::endl;
    }
    // read values (remove quotes)
    QString idCountry = data[0].value(2).remove("\"");
    float avgTemperature = data[0].value(3).remove("\"").toFloat();
    //...
    // *****************************************************************
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
    // *********************************************************************************
    // query energy_country table
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

    //********************************************************************************
    //read residue_treatment table

    QString idResidue = "biochar";
    queryString = "SELECT * FROM residue_treatment WHERE id_treatment_residue='" + idResidue + "'";
    query = db.exec(queryString);
    query.last();
    if (! query.isValid())
    {
        std::cout << "Error reading data: " + query.lastError().text().toStdString() << std::endl;
        return -1;
    }
    double emissionMethane, emissionN2O, dryMatterToCO2Percentage;
    if (! getValue(query.value("emission_methane"), &emissionMethane))
    {
        std::cout << "Error: missing emission of Methane data" << std::endl;
        return -1;
    }
    query.clear();
    queryString = "SELECT * FROM residue_treatment WHERE id_treatment_residue='" + idResidue + "'";
    query = db.exec(queryString);
    query.last();
    if (! query.isValid())
    {
        std::cout << "Error reading data: " + query.lastError().text().toStdString() << std::endl;
        return -1;
    }

    if (! getValue(query.value("emission_n2o"), &emissionN2O))
    {
        std::cout << "Error: missing emission of N2O data" << std::endl;
        return -1;
    }
    query.clear();

    queryString = "SELECT * FROM residue_treatment WHERE id_treatment_residue='" + idResidue + "'";
    query = db.exec(queryString);
    query.last();
    if (! query.isValid())
    {
        std::cout << "Error reading data: " + query.lastError().text().toStdString() << std::endl;
        return -1;
    }
    if (! getValue(query.value("dry_matter_to_co2"), &dryMatterToCO2Percentage))
    {
        std::cout << "Error: missing emission of dry matter to CO2 data" << std::endl;
        return -1;
    }
    query.clear();

    // **************************************************************************
    // read crop_parameters table
    QString idCrop = "BARLEY";
    double abovegroundNitrogen, belowAboveRatio, dryMatterFraction;

    queryString = "SELECT * FROM crop_parameters WHERE id_fine_classification='" + idCrop + "'";
    query = db.exec(queryString);
    query.last();
    if (! query.isValid())
    {
        std::cout << "Error reading data: " + query.lastError().text().toStdString() << std::endl;
        return -1;
    }
    if (! getValue(query.value("drymatter_fraction_harvested"), &dryMatterFraction))
    {
        std::cout << "Error: missing emission of dry matter fraction data" << std::endl;
        return -1;
    }
    query.clear();

    queryString = "SELECT * FROM crop_parameters WHERE id_fine_classification='" + idCrop + "'";
    query = db.exec(queryString);
    query.last();
    if (! query.isValid())
    {
        std::cout << "Error reading data: " + query.lastError().text().toStdString() << std::endl;
        return -1;
    }
    if (! getValue(query.value("nitrogen_content_aboveground"), &abovegroundNitrogen))
    {
        std::cout << "Error: missing emission of dry matter fraction data" << std::endl;
        return -1;
    }
    query.clear();

    queryString = "SELECT * FROM crop_parameters WHERE id_fine_classification='" + idCrop + "'";
    query = db.exec(queryString);
    query.last();
    if (! query.isValid())
    {
        std::cout << "Error reading data: " + query.lastError().text().toStdString() << std::endl;
        return -1;
    }
    if (! getValue(query.value("below_above_ratio"), &belowAboveRatio))
    {
        std::cout << "Error: missing emission of above/below ratio data" << std::endl;
        return -1;
    }
    query.clear();



    // *****************************************************************************
    std::cout << "Country: " << idCountry.toStdString() << std::endl;
    std::cout << "Avg temperature: " << avgTemperature << std::endl;
    std::cout << "Renewables percentage: " << renewablesPercentage << std::endl;

    std::cout << "CH4 emission conversion: " << emissionMethane << std::endl;
    std::cout << "N2O emission conversion: " << emissionN2O << std::endl;
    std::cout << "dry matter to CO2: " << dryMatterToCO2Percentage << std::endl;



    calculatorCO2.energy.setInput(kindOfEnergy, renewablesPercentage, idCountry);
    calculatorCO2.energy.computeEmissions();
    calculatorCO2.cropResidue.setInput(emissionMethane,emissionN2O,dryMatterToCO2Percentage);
    calculatorCO2.cropResidue.computeEmissions(residueWeight);


    return 0;
}
