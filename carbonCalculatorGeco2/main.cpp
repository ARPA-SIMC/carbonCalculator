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
struct Tgeneral{
    QString enterpriseName;
    QString idCountry;
    int year;
    float latitude;
    float longitude;
    float fieldSize;
    float fieldSlope;
};
struct Tclimate{
    float meanTemperature;
    float annualRainfall;
};
struct Tsoil{
    float depth;
    QString drainage;
    float pH;
    QString texture;
    float organicMatter;
    float skeleton;
};
struct TcropFieldManagement{
    QString cropName;
    QString isOrganic;
    float yield;
    float noTillage;
    float minTillage;
    float coverCrop;
    float permanentGrass;
    float forest;
    float sparseVegetation;
    QString woodyResidueTreatment;
    float woodyResidueWeight;
    QString greenResidueTreatment;
    float greenResidueWeight;
};

struct TagronomicInput{
    float pesticideWeight;
    QString fertilizerName[4];
    float fertilizerAmount[4];
    QString fertilizerApplicationMethod[4];
    QString fertilizerInhibitor[4];
    QString amendmentName[4];
    float amendmentAmount[4];
    QString amendmentApplicationMethod[4];
    QString amendmentInhibitor[4];
};

struct Tenergy{
    float biodiesel;
    float bioethanol;
    float diesel;
    float oil;
    float petrol;
    float LPG;
    float coal;
    float highEnergyDensityBiomass;
    float wood;
    float electricityGridAmount;
    float electricityGridPercentageRenewables;
    float electricitySolar;
    float electricityEolic;
    float electricityHydro;
};

struct TinputData{
    Tgeneral general;
    Tclimate climate;
    Tsoil soil;
    TcropFieldManagement cropFieldManagement;
    TagronomicInput agronomicInput;
    Tenergy energy;
};

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
    /*QString csvFileName;
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
    QString idCountry = data[iExp].value(2).remove("\"");
    float avgTemperature = data[iExp].value(3).remove("\"").toFloat();
    // ... read other values

    */
    // read the .csv file final

    // read argument
    QString csvFileName;

    if (argc < 2)
    {
        #ifdef TEST
            csvFileName = dataPath + "prova1.csv";
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
    // first do cycle in order to avoid the first line, i.e. the header
    do
    {
        dummyComma = char(getc(fp));
    } while (dummyComma != '\n' && dummyComma != EOF);

    do
    {
        dummyComma = char(getc(fp));
        if (dummyComma == ',') numberOfFields++;
    } while (dummyComma != '\n' && dummyComma != EOF);
    fclose(fp);

    // check numberOfExperiments
    //FILE *fp;
    fp = fopen(csvFileName.toStdString().c_str(),"r");
    int numberOfExperiments = 0;
    char dummyLine;
    // first do cycle in order to avoid the first line, i.e. the header
    do
    {
        dummyLine = char(getc(fp));
    } while (dummyLine != '\n' && dummyLine != EOF);
    do
    {
        dummyLine = char(getc(fp));
        if (dummyLine == '\n' || dummyLine == EOF) numberOfExperiments++;
    } while (dummyLine != EOF);
    fclose(fp);

    // read data
    std::vector<QStringList> data;
    if (! importCsvData(csvFileName, numberOfFields, true, data, error))
    {
        std::cout << "Error: " << error.toStdString() << std::endl;
    }
    //std::cout << data[1].value(2).remove("\"").toStdString << std::endl;
    TinputData* inputData;
    TinputData inputData2;
    inputData = (TinputData*)calloc(numberOfExperiments, sizeof(TinputData));
    // read values (remove quotes)
    for (int iExp=0;iExp<numberOfExperiments;iExp++)
    {
        int label=1;
        inputData2.general.enterpriseName = data[iExp].value(label++).remove("\"");
        inputData2.general.idCountry = data[iExp].value(label++).remove("\"");
        inputData2.general.year = (int)(data[iExp].value(label++).remove("\"").toFloat());
        inputData2.general.latitude = data[iExp].value(label++).remove("\"").toFloat();
        inputData2.general.longitude = data[iExp].value(label++).remove("\"").toFloat();
        inputData2.general.fieldSize = data[iExp].value(label++).remove("\"").toFloat();
        inputData2.general.fieldSlope = data[iExp].value(label++).remove("\"").toFloat();

        inputData2.climate.meanTemperature = data[iExp].value(label++).remove("\"").toFloat();
        inputData2.climate.annualRainfall = data[iExp].value(label++).remove("\"").toFloat();

        inputData2.soil.depth = data[iExp].value(label++).remove("\"").toFloat();
        inputData2.soil.drainage = data[iExp].value(label++).remove("\"");
        inputData2.soil.pH = data[iExp].value(label++).remove("\"").toFloat();
        inputData2.soil.texture = data[iExp].value(label++).remove("\"");
        inputData2.soil.organicMatter = data[iExp].value(label++).remove("\"").toFloat();
        inputData2.soil.skeleton = data[iExp].value(label++).remove("\"").toFloat();

        inputData2.cropFieldManagement.cropName = data[iExp].value(label++).remove("\"");
        inputData2.cropFieldManagement.isOrganic = data[iExp].value(label++).remove("\"");
        inputData2.cropFieldManagement.yield = data[iExp].value(label++).remove("\"").toFloat();
        inputData2.cropFieldManagement.noTillage = data[iExp].value(label++).remove("\"").toFloat();
        inputData2.cropFieldManagement.minTillage = data[iExp].value(label++).remove("\"").toFloat();
        inputData2.cropFieldManagement.coverCrop = data[iExp].value(label++).remove("\"").toFloat();
        inputData2.cropFieldManagement.permanentGrass = data[iExp].value(label++).remove("\"").toFloat();
        inputData2.cropFieldManagement.forest = data[iExp].value(label++).remove("\"").toFloat();
        inputData2.cropFieldManagement.sparseVegetation = data[iExp].value(label++).remove("\"").toFloat();
        inputData2.cropFieldManagement.woodyResidueWeight = data[iExp].value(label++).remove("\"").toFloat();
        inputData2.cropFieldManagement.woodyResidueTreatment = data[iExp].value(label++).remove("\"");
        inputData2.cropFieldManagement.greenResidueWeight = data[iExp].value(label++).remove("\"").toFloat();
        inputData2.cropFieldManagement.greenResidueTreatment = data[iExp].value(label++).remove("\"");

        inputData2.agronomicInput.pesticideWeight = data[iExp].value(label++).remove("\"").toFloat();
        for (int i=0;i<4;i++)
        {
            inputData2.agronomicInput.fertilizerName[i] = data[iExp].value(label++).remove("\"");
        }
        for (int i=0;i<4;i++)
        {
             inputData2.agronomicInput.fertilizerAmount[i] = data[iExp].value(label++).remove("\"").toFloat();
        }
        for (int i=0;i<4;i++)
        {
            inputData2.agronomicInput.fertilizerApplicationMethod[i] = data[iExp].value(label++).remove("\"");
        }
        for (int i=0;i<4;i++)
        {
            inputData2.agronomicInput.fertilizerInhibitor[i] = data[iExp].value(label++).remove("\"");
        }

        for (int i=0;i<4;i++)
        {
            inputData2.agronomicInput.amendmentName[i] = data[iExp].value(label++).remove("\"");
        }
        for (int i=0;i<4;i++)
        {
             inputData2.agronomicInput.amendmentAmount[i] = data[iExp].value(label++).remove("\"").toFloat();
        }
        for (int i=0;i<4;i++)
        {
            inputData2.agronomicInput.amendmentApplicationMethod[i] = data[iExp].value(label++).remove("\"");
        }
        for (int i=0;i<4;i++)
        {
            inputData2.agronomicInput.amendmentInhibitor[i] = data[iExp].value(label++).remove("\"");
        }

        inputData2.energy.biodiesel = data[iExp].value(label++).remove("\"").toFloat();
        inputData2.energy.bioethanol = data[iExp].value(label++).remove("\"").toFloat();
        inputData2.energy.diesel = data[iExp].value(label++).remove("\"").toFloat();
        inputData2.energy.oil = data[iExp].value(label++).remove("\"").toFloat();
        inputData2.energy.petrol = data[iExp].value(label++).remove("\"").toFloat();
        inputData2.energy.LPG = data[iExp].value(label++).remove("\"").toFloat();
        inputData2.energy.coal = data[iExp].value(label++).remove("\"").toFloat();
        inputData2.energy.highEnergyDensityBiomass = data[iExp].value(label++).remove("\"").toFloat();
        inputData2.energy.wood = data[iExp].value(label++).remove("\"").toFloat();
        inputData2.energy.electricityGridAmount = data[iExp].value(label++).remove("\"").toFloat();
        inputData2.energy.electricityGridPercentageRenewables = data[iExp].value(label++).remove("\"").toFloat();
        inputData2.energy.electricityHydro = data[iExp].value(label++).remove("\"").toFloat();
        inputData2.energy.electricitySolar = data[iExp].value(label++).remove("\"").toFloat();
        inputData2.energy.electricityEolic = data[iExp].value(label++).remove("\"").toFloat();


        std::cout << inputData2.general.idCountry.toStdString() << std::endl;
    }




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


    float avgRainfall = 700; // input from .csv
    float avgTemperature = 12; // to quit // input from .csv

    // read renewables

    if (! readRenewables(inputData2.general.idCountry,inputData2.general.year, db, calculatorCO2, error))
    {
        std::cout << "ERROR: " + error.toStdString() << std::endl;
        return -1;
    }
    bool isOrganic = true; // input from .csv
    calculatorCO2.soilManage.isOrganic = isOrganic;
    double cropYield = 5 ; // t/ha input from .csv
    double fieldExtension = 1; // ha input from .csv
    double surfaceSparseTreesSchrubsHedgeFallow = 3.456; // m2 of sparse vegetation input from csv
    double ratioFallowExtension;
    ratioFallowExtension = surfaceSparseTreesSchrubsHedgeFallow / (fieldExtension * 10000);

    // read soil drainage
    QString idDrainage = "POOR"; // input from .csv
    // read pH
    double pHSoil = 7.9; // input from .csv
    //double CEC = 15; // to be computed somehow from .csv data
    QString idSoilTexture = "MEDIUM"; // input from .csv
    QString idSoilOrganicCarbon = "SOM<=1.72"; // input from .csv
    QString idClimate;
    double yearETP = 1000 + 100*(avgTemperature - 13);
    if (avgTemperature > 20 && avgRainfall > yearETP)
        idClimate = "TROPICAL_MOIST";
    else if (avgTemperature > 20 && avgRainfall <= yearETP)
        idClimate = "TROPICAL_DRY";
    else if (avgTemperature <= 20 && avgRainfall > yearETP)
        idClimate = "TEMPERATE_MOIST";
    else
        idClimate  = "TEMPERATE_DRY";

    int nrFertilizers = 8;
    QString idFertiliser[8];
    idFertiliser[0] = "Urea_46_4N"; // input from .csv
    idFertiliser[1] = "Compost_fully_aerated_production_1N"; // input from .csv
    idFertiliser[2] = "Biochar"; // input from .csv
    idFertiliser[3] = "Digestate_6percent_drymatter"; // input from .csv
    idFertiliser[4] = "Straw"; // input from .csv
    idFertiliser[5] = "Wood_chips"; // input from .csv
    idFertiliser[6] = "Volcanic_rock_dust"; // input from .csv
    idFertiliser[7] = "NONE"; // input from .csv

    //idFertiliser[4] = "NONE"; // input from .csv
    //idFertiliser[5] = "NONE"; // input from .csv
    //idFertiliser[6] = "NONE"; // input from .csv

    QString inhibitor[8];
    inhibitor[0] = "nitrification_inhibitor"; // input from .csv
    inhibitor[1] = "nitrification_inhibitor";
    inhibitor[2] = "none";
    inhibitor[3] = "none";
    inhibitor[4] = "none";
    inhibitor[5] = "none";
    inhibitor[6] = "none";
    inhibitor[7] = "none";

    QString idFeature[8];
    for (int i=0;i<nrFertilizers;i++)
        idFeature[i] = "field2"; // initialization - default value

    idFeature[0] = "field2"; //input from .csv
    idFeature[1] = "field2"; //input from .csv
    idFeature[2] = "field2"; //input from .csv
    idFeature[3] = "field2"; //input from .csv
    idFeature[4] = "field2"; //input from .csv
    idFeature[5] = "field2"; //input from .csv
    idFeature[6] = "field2"; //input from .csv
    idFeature[7] = "field2"; //input from .csv



    QString idCrop = "APPLE_TREE"; //input from .csv
    calculatorCO2.soilDepth = 28; // [cm] input from .csv
    calculatorCO2.skeleton = 3; // [%] input from .csv

    calculatorCO2.initialiazeVariables(idDrainage,pHSoil,idSoilTexture,idSoilOrganicCarbon,inhibitor);


    // read climate

    if (! readClimate(idClimate, db, calculatorCO2, error))
    {
        std::cout << "ERROR: " + error.toStdString() << std::endl;
        return -1;
    }


    // read fertilizer
    for (int i=0; i<nrFertilizers;i++)
        calculatorCO2.fertiliser.amountFertiliser[i] = 0; // initialization - default value

    calculatorCO2.fertiliser.amountFertiliser[0] = 200; // kg/ha input from .csv
    calculatorCO2.fertiliser.amountFertiliser[1] = 5; // kg/ha input from .csv
    calculatorCO2.fertiliser.amountFertiliser[2] = 5; // kg/ha input from .csv
    calculatorCO2.fertiliser.amountFertiliser[3] = 1; // kg/ha input from .csv
    calculatorCO2.fertiliser.amountFertiliser[4] = 5; // kg/ha input from .csv
    calculatorCO2.fertiliser.amountFertiliser[5] = 5; // kg/ha input from .csv
    calculatorCO2.fertiliser.amountFertiliser[6] = 1; // kg/ha input from .csv



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
    QString idResidue[2]; //input from .csv
    idResidue[0] = "left_on_field_incorporated_or_mulch"; //input from .csv
    idResidue[1] = "left_on_field_incorporated_or_mulch"; //input from .csv

    if (idResidue[0] == "left_on_field_incorporated_or_mulch")
    {
        calculatorCO2.cropResidue.residueLeftOnField[0] = true;
    }
    else
    {
        calculatorCO2.cropResidue.residueLeftOnField[0] = false;
    }
    if (idResidue[1] == "left_on_field_incorporated_or_mulch")
    {
        calculatorCO2.cropResidue.residueLeftOnField[1] = true;
    }
    else
    {
        calculatorCO2.cropResidue.residueLeftOnField[1] = false;
    }

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
    calculatorCO2.cropResidue.residueWeight[0] = 10; //(t/ha) dry weight of woody residue input from .csv
    calculatorCO2.cropResidue.residueWeight[1] = 2; //(t/ha) dry weight of green residue input from .csv
    if (calculatorCO2.cropResidue.residueWeight[0] == NODATA) calculatorCO2.cropResidue.residueWeight[0] = 3; // t/ha
    if (calculatorCO2.cropResidue.residueWeight[1] == NODATA) calculatorCO2.cropResidue.residueWeight[1] = 3; // t/ha

    // **********************************************************************
    double idPercentageEnergyInGrid = 38.4;  // % input from .csv
    if (idPercentageEnergyInGrid != NODATA)
    {
        calculatorCO2.energy.percentageRenewablesInGrid = idPercentageEnergyInGrid;
    }
    calculatorCO2.energy.input.fromElectricityGrid = 3; // kWh input from .csv
    calculatorCO2.energy.input.fromElectricityOwnHydropower = 5; // kWh input from .csv
    calculatorCO2.energy.input.fromElectricityOwnPhotovoltaic = 5; // kWh input from .csv
    calculatorCO2.energy.input.fromElectricityOwnWind = 5; // kWh input from .csv
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
    calculatorCO2.pesticide.weightOfActivePrinciple = 15.4; // input from .csv kg active principle
    // **********************************************************************

    calculatorCO2.soilManage.percentage.noTillage = 100; // input from .csv
    calculatorCO2.soilManage.percentage.minimumTillage = 0; // input from .csv
    calculatorCO2.soilManage.percentage.conventionalTillage = 100 - calculatorCO2.soilManage.percentage.noTillage - calculatorCO2.soilManage.percentage.minimumTillage;

    calculatorCO2.soilManage.percentage.coverCropping = 1; // input from .csv

    calculatorCO2.soilManage.percentage.forest = 0 ; // input from .csv
    calculatorCO2.soilManage.percentage.forest += ratioFallowExtension*100/2. ;
    calculatorCO2.soilManage.percentage.permanentGrass = 0; // input from .csv
    calculatorCO2.soilManage.percentage.permanentGrass += ratioFallowExtension*100/2.; //assuming that sparse vegetation is intermediate between forest and permanetn grass

    calculatorCO2.soilManage.percentage.arable = 100 - calculatorCO2.soilManage.percentage.forest - calculatorCO2.soilManage.percentage.permanentGrass;


    // *********************************************************************
    // erosion
    calculatorCO2.averageTemperature = avgTemperature;
    calculatorCO2.annualRainfall = avgRainfall; // input from .csv
    calculatorCO2.erosion.erosionFactor.rainfall = calculatorCO2.annualRainfall;
    if (idSoilTexture == "MEDIUM")
        calculatorCO2.erosion.erosionFactor.texture = 0.3;
    else if (idSoilTexture == "FINE")
        calculatorCO2.erosion.erosionFactor.texture = 0.2;
    else
        calculatorCO2.erosion.erosionFactor.texture = 0.05;

    double slope = 0.3; // input from .csv
    calculatorCO2.erosion.erosionFactor.slope = slope;
    // cover factor
    calculatorCO2.erosion.erosionFactor.cover = 0.01* (calculatorCO2.soilManage.percentage.forest* 0.005 + calculatorCO2.soilManage.percentage.permanentGrass* 0.01 + calculatorCO2.soilManage.percentage.arable* 0.128);

    calculatorCO2.erosion.erosionFactor.soilManagement = 0.01*calculatorCO2.soilManage.percentage.coverCropping*0.26;
    calculatorCO2.erosion.erosionFactor.soilManagement += (100. - calculatorCO2.soilManage.percentage.coverCropping)*0.01 *(0.01*(calculatorCO2.soilManage.percentage.conventionalTillage + calculatorCO2.soilManage.percentage.minimumTillage*0.52 + calculatorCO2.soilManage.percentage.noTillage*0.26));

    // **********************************************************************


    calculatorCO2.computeBalance();
    std::cout << "values are in kgCO2Eq " << std::endl;
    std::cout << "emissions due to energy: " << calculatorCO2.energy.emissions.total << std::endl;
    std::cout << "emissions due to pesticide production: " << calculatorCO2.pesticide.emissionDueToProduction << std::endl;
    std::cout << "emissions due to residue management: " << calculatorCO2.cropResidue.kgCO2Equivalent.total << std::endl;
    std::cout << "emissions due to type of soil due to Nitrogen: " << calculatorCO2.fertiliser.emissionDueToSoil << std::endl;
    std::cout << "emissions due to type of soil due to Carbon Oxydation: " << calculatorCO2.soilManage.computeEmissions() << std::endl;
    std::cout << "emissions due to fertiliser production: " << calculatorCO2.fertiliser.emissionDueToFertiliserProduction << std::endl;
    std::cout << "emissions due to fertiliser application: " << calculatorCO2.fertiliser.emissionDueToFertiliserApplication << std::endl;
    std::cout << "loss due to erosion: " << calculatorCO2.erosion.lostCO2 << std::endl;
    std::cout << "sequestration due to minimum tillage and crop covering and land use: " << calculatorCO2.soilManage.sequestrationCarbonCO2Eq << std::endl;
    std::cout << "sequestration due to carbon incorporation: " <<calculatorCO2.fertiliser.sequestrationDueToFertiliserApplication << std::endl;
    std::cout << "sequestration due to carbon of roots: " <<-calculatorCO2.soilManage.computeSequestrationRootBiomass() << std::endl;

    // ***************************************************************************************
    // print results



    return 0;
}
