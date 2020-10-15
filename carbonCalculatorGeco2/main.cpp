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
#include "dbQueries.h"
#include "csvUtilities.h"

// uncomment to compute test
#define TEST

static CarbonCalculator calculatorCO2;


void usage()
{
    std::cout << "USAGE:\ncarbonCalculatorTest <csv data file>\n";
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString error;

    // search data path
    QString dataPath;
    if (! searchDataPath(&dataPath))
    {
        std::cout << "Error: missing DATA directory" << std::endl;
        return -1;
    }

    // read argument
    QString csvFileName;
    if (argc < 2)
    {
        #ifdef TEST
            csvFileName = dataPath + "geco2ModuloRisposte.csv";
        #else
            usage();
            return 1;
        #endif
    }
    else
    {
        csvFileName = argv[1];
    }

    // ****************************************************************
    // read CSV
    if (! QFile(csvFileName).exists())
    {
        std::cout << "Error!\nMissing csv file: " << csvFileName.toStdString() << std::endl;
        return -1;
    }

    // check numberOfFields
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

    // read data
    std::vector<QStringList> data;
    if (! importCsvData(csvFileName, numberOfFields, true, data, error))
    {
        std::cout << "Error: " << error.toStdString() << std::endl;
    }

    // read values (remove quotes)
    QString idCountry = data[0].value(2).remove("\"");
    float avgTemperature = data[0].value(3).remove("\"").toFloat();
    // ... read other values


    // ****************************************************************
    // read DB
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

    // read renewables
    int year = 2020;
    if (! readRenewables(idCountry, year, db, calculatorCO2, error))
    {
        std::cout << "ERROR: " + error.toStdString() << std::endl;
        return -1;
    }

    // read fertilizer
    QString idFertiliser = "Ammonium_nitrate";
    if (! readFertilizer(idFertiliser, db, calculatorCO2, error))
    {
        std::cout << "ERROR: " + error.toStdString() << std::endl;
        return -1;
    }
    calculatorCO2.fertiliser.amountFertiliser = 50; // kg/ha

    // ****************************************************************
    // TODO: create functions in dbQueries and remove setInput
    //read residue_treatment table
    QString idResidue = "biochar";
    QString queryString = "SELECT * FROM residue_treatment WHERE id_treatment_residue='" + idResidue + "'";
    QSqlQuery query = db.exec(queryString);
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
    if (! getValue(query.value("emission_n2o"), &emissionN2O))
    {
        std::cout << "Error: missing emission of N2O data" << std::endl;
        return -1;
    }
    if (! getValue(query.value("dry_matter_to_co2"), &dryMatterToCO2Percentage))
    {
        std::cout << "Error: missing emission of dry matter to CO2 data" << std::endl;
        return -1;
    }
    query.clear();

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
    if (! getValue(query.value("nitrogen_content_aboveground"), &abovegroundNitrogen))
    {
        std::cout << "Error: missing emission of dry matter fraction data" << std::endl;
        return -1;
    }
    if (! getValue(query.value("below_above_ratio"), &belowAboveRatio))
    {
        std::cout << "Error: missing emission of above/below ratio data" << std::endl;
        return -1;
    }
    query.clear();


    // *********************************************************************
    double residueWeight = 5;
    calculatorCO2.energy.input.fromElectricityGrid = 50; // kWh
    calculatorCO2.energy.input.fromElectricityOwnHydropower = 50; // kWh
    calculatorCO2.energy.input.fromElectricityOwnPhotovoltaic = 50; // kWh
    calculatorCO2.energy.input.fromElectricityOwnWind = 50; // kWh
    calculatorCO2.energy.input.fromFuelBiodiesel = 1; // l
    calculatorCO2.energy.input.fromFuelBioethanol = 1; // l
    calculatorCO2.energy.input.fromFuelCoal = 1; // kg
    calculatorCO2.energy.input.fromFuelDiesel = 1; // l
    calculatorCO2.energy.input.fromFuelHighDensityBiomass = 1; // kg
    calculatorCO2.energy.input.fromFuelLiquidPropane = 1; // l
    calculatorCO2.energy.input.fromFuelOil = 1; // l
    calculatorCO2.energy.input.fromFuelPetrol = 1; // l
    calculatorCO2.energy.input.fromFuelWood = 1; // kg


    // **********************************************************************
    // print parameters
    std::cout << "Country: " << calculatorCO2.energy.country.toStdString() << std::endl;
    std::cout << "Avg temperature: " << avgTemperature << std::endl;
    std::cout << "Renewables percentage: " << calculatorCO2.energy.percentageRenewablesInGrid << std::endl;
    std::cout << "CH4 emission conversion: " << emissionMethane << std::endl;
    std::cout << "N2O emission conversion: " << emissionN2O << std::endl;
    std::cout << "dry matter to CO2: " << dryMatterToCO2Percentage << std::endl;

    calculatorCO2.energy.computeEmissions();

    calculatorCO2.cropResidue.setInput(emissionMethane,emissionN2O,dryMatterToCO2Percentage);
    calculatorCO2.cropResidue.computeEmissions(residueWeight);

    calculatorCO2.pesticide.setInput(15.4, calculatorCO2.energy.percentageRenewablesInGrid);
    calculatorCO2.pesticide.computeEmissions();

    calculatorCO2.fertiliser.computeEmissions();


    return 0;
}
