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
        dummyComma = char(getc(fp));
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
    avgTemperature = 12; // to quit
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
    // read soil drainage
    QString idDrainage = "POOR"; // input from .csv
    // read pH
    double pHSoil = 7.9; // input from .csv
    //double CEC = 15; // to be computed somehow from .csv data
    QString idSoilTexture = "MEDIUM"; // input from .csv
    QString idSoilOrganicCarbon = "SOM<=1.72"; // input from .csv
    QString idClimate = "TEMPERATE_MOIST"; // input from .csv
    int nrFertilizers = 4;
    QString idFertiliser[4];
    idFertiliser[0] = "Ammonium_nitrate"; // input from .csv
    idFertiliser[1] = "Ammonium_Bicarbonate"; // input from .csv
    idFertiliser[2] = "Lime_52CaO"; // input from .csv
    idFertiliser[3] = "NONE"; // input from .csv
    QString inhibitor[4];
    inhibitor[0] = "nitrification_inhibitor"; // input from .csv
    inhibitor[1] = "nitrification_inhibitor";
    inhibitor[2] = "none";
    inhibitor[3] = "none";
    QString idFeature[4];
    for (int i=0;i<nrFertilizers;i++)
        idFeature[i] = "field2"; // initialization - default value

    idFeature[0] = "field2"; //input from .csv
    idFeature[1] = "field2"; //input from .csv
    idFeature[2] = "field2"; //input from .csv
    idFeature[3] = "field2"; //input from .csv
    QString idCrop = "BARLEY"; //input from .csv

    calculatorCO2.initialiazeVariables(idDrainage,pHSoil,idSoilTexture,idSoilOrganicCarbon,inhibitor);


    // read climate

    if (! readClimate(idClimate, db, calculatorCO2, error))
    {
        std::cout << "ERROR: " + error.toStdString() << std::endl;
        return -1;
    }


    // read fertilizer
    for (int i=0; i<4;i++)
        calculatorCO2.fertiliser.amountFertiliser[i] = 0; // initialization - default value

    calculatorCO2.fertiliser.amountFertiliser[0] = 100; // kg/ha input from .csv
    calculatorCO2.fertiliser.amountFertiliser[1] = 100; // kg/ha input from .csv
    calculatorCO2.fertiliser.amountFertiliser[2] = 100; // kg/ha input from .csv
    //calculatorCO2.fertiliser.amountFertiliser[3] = 80; // kg/ha input from .csv




    if (! readFertilizer(idFertiliser, db, calculatorCO2, error,nrFertilizers))
    {
        std::cout << "ERROR: " + error.toStdString() << std::endl;
        return -1;
    }

    // read bouwmanNH4 table


    if (! readBouwmanNH4(idFeature, db, calculatorCO2, error,nrFertilizers))
    {
        std::cout << "ERROR: " + error.toStdString() << std::endl;
        return -1;
    }

    //read residue_treatment table
    QString idResidue = "biochar"; //input from .csv
    if (! readResidue(idResidue, db, calculatorCO2, error))
    {
        std::cout << "ERROR: " + error.toStdString() << std::endl;
        return -1;
    }

    // read crop_parameters table
    if (! readCropParameters(idCrop, db, calculatorCO2, error))
    {
        std::cout << "ERROR: " + error.toStdString() << std::endl;
        return -1;
    }




    // *********************************************************************
    calculatorCO2.cropResidue.residueWeight = 5; //(t/ha) fresh weight of residue input from .csv
    calculatorCO2.energy.input.fromElectricityGrid = 50; // kWh input from .csv
    calculatorCO2.energy.input.fromElectricityOwnHydropower = 50; // kWh input from .csv
    calculatorCO2.energy.input.fromElectricityOwnPhotovoltaic = 50; // kWh input from .csv
    calculatorCO2.energy.input.fromElectricityOwnWind = 50; // kWh input from .csv
    calculatorCO2.energy.input.fromFuelBiodiesel = 1; // l input from .csv
    calculatorCO2.energy.input.fromFuelBioethanol = 1; // l input from .csv
    calculatorCO2.energy.input.fromFuelCoal = 1; // kg input from .csv
    calculatorCO2.energy.input.fromFuelDiesel = 1; // l input from .csv
    calculatorCO2.energy.input.fromFuelHighDensityBiomass = 1; // kg input from .csv
    calculatorCO2.energy.input.fromFuelLPG = 1; // l input from .csv
    calculatorCO2.energy.input.fromFuelOil = 1; // l input from .csv
    calculatorCO2.energy.input.fromFuelPetrol = 1; // l input from .csv
    calculatorCO2.energy.input.fromFuelWood = 1; // kg input from .csv

    // **********************************************************************
    calculatorCO2.pesticide.weightOfActivePrinciple = 15.4; // input from .csv
    // **********************************************************************


    // print parameters
    //std::cout << "Country: " << calculatorCO2.energy.country.toStdString() << std::endl;
    //std::cout << "Avg temperature: " << avgTemperature << std::endl;
    //std::cout << "Renewables percentage: " << calculatorCO2.energy.percentageRenewablesInGrid << std::endl;
    //std::cout << "CH4 emission conversion: " << emissionMethane << std::endl;
    //std::cout << "N2O emission conversion: " << emissionN2O << std::endl;
    //std::cout << "dry matter to CO2: " << dryMatterToCO2Percentage << std::endl;

    //calculatorCO2.energy.computeEmissions();

    //calculatorCO2.cropResidue.setInput(emissionMethane,emissionN2O,dryMatterToCO2Percentage);
    //calculatorCO2.cropResidue.computeEmissions();

    //calculatorCO2.pesticide.setInput(calculatorCO2.pesticide.weightOfActivePrinciple, calculatorCO2.energy.percentageRenewablesInGrid);
    //calculatorCO2.pesticide.computeEmissions();

    //calculatorCO2.fertiliser.computeEmissions();

    calculatorCO2.computeEmissions();
    return 0;
}
