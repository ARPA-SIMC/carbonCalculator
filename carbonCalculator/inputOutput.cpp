#include "inputOutput.h"

void usage()
{
    std::cout << "USAGE:\ncarbonCalculatorTest <csv data file>\n";
}

bool readCsvFileBuyer(QString csvFileName,std::vector<TinputDataBuyer> &inputData,int& numberOfExperiments)
{
    // check numberOfFields
    QString error;
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
    numberOfExperiments = 0;
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

    inputData.resize(numberOfExperiments);

    // read values (remove quotes)
    for (int iExp=0; iExp < numberOfExperiments; iExp++)
    {
        int label=1;
        inputData[iExp].generalBuyer.compilerName = data[iExp].value(label++).remove("\"");
        inputData[iExp].generalBuyer.enterpriseName = data[iExp].value(label++).remove("\"");
        inputData[iExp].generalBuyer.chainName = data[iExp].value(label++).remove("\"");
        inputData[iExp].generalBuyer.productName = data[iExp].value(label++).remove("\"");
        inputData[iExp].generalBuyer.idCountry = data[iExp].value(label++).remove("\"");
        inputData[iExp].generalBuyer.idRegion = data[iExp].value(label++).remove("\"");
        inputData[iExp].generalBuyer.year = (int)(data[iExp].value(label++).remove("\"").toFloat());
        inputData[iExp].generalBuyer.description = data[iExp].value(label++).remove("\"");
        inputData[iExp].generalBuyer.latitude = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].generalBuyer.longitude = data[iExp].value(label++).remove("\"").toFloat();
        if (data[iExp].value(label++).remove("\"") == "yes")
            inputData[iExp].generalBuyer.isPresentLCA = 1;
        else
            inputData[iExp].generalBuyer.isPresentLCA = 0;
        inputData[iExp].generalBuyer.valueLCA = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].energy.biodiesel = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].energy.bioethanol = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].energy.diesel = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].energy.oil = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].energy.petrol = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].energy.LPG = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].energy.coal = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].energy.highEnergyDensityBiomass = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].energy.wood = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].energy.methane = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].energy.electricityGridAmount = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].energy.electricityGridPercentageRenewables = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].energy.electricityHydro = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].energy.electricitySolar = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].energy.electricityEolic = data[iExp].value(label++).remove("\"").toFloat();
        // inputData[iExp] capire perché non c'è generalBuyer
    }
    return true;
}


bool readCsvFile(QString csvFileName,std::vector<TinputData> &inputData,int& numberOfExperiments)
{
    // check numberOfFields
    QString error;
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
    numberOfExperiments = 0;
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

    inputData.resize(numberOfExperiments);

    // read values (remove quotes)
    for (int iExp=0; iExp < numberOfExperiments; iExp++)
    {
        int label=1;
        inputData[iExp].general.compilerName = data[iExp].value(label++).remove("\"");
        inputData[iExp].general.enterpriseName = data[iExp].value(label++).remove("\"");
        inputData[iExp].general.nrField = (int) data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].general.idCountry = data[iExp].value(label++).remove("\"");
        inputData[iExp].general.idRegion = data[iExp].value(label++).remove("\"");
        inputData[iExp].general.year = (int)(data[iExp].value(label++).remove("\"").toFloat());
        inputData[iExp].general.description = data[iExp].value(label++).remove("\"");
        inputData[iExp].general.latitude = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].general.longitude = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].general.fieldSize = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].general.fieldSlope = data[iExp].value(label++).remove("\"").toFloat();

        inputData[iExp].climate.meanTemperature = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].climate.annualRainfall = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].climate.referenceEvapotranspiration = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].climate.climaticWaterBalance = data[iExp].value(label++).remove("\"").toFloat();


        inputData[iExp].soil.depth = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].soil.drainage = data[iExp].value(label++).remove("\"");
        inputData[iExp].soil.pH = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].soil.texture = data[iExp].value(label++).remove("\"");
        inputData[iExp].soil.organicMatter = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].soil.skeleton = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].soil.availableWaterCapacity = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].soil.totalNitrogen = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].soil.carbonNitrogenRatio = data[iExp].value(label++).remove("\"").toFloat();


        inputData[iExp].cropFieldManagement.cropName = data[iExp].value(label++).remove("\"");
        inputData[iExp].cropFieldManagement.treeDensity = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].cropFieldManagement.deadTreeDensity = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].cropFieldManagement.orchardAge = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].cropFieldManagement.treeDBH = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].cropFieldManagement.treeHeight = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].cropFieldManagement.isOrganic = data[iExp].value(label++).remove("\"");
        inputData[iExp].cropFieldManagement.yield = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].cropFieldManagement.noTillage = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].cropFieldManagement.minTillage = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].cropFieldManagement.coverCrop = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].cropFieldManagement.permanentGrass = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].cropFieldManagement.forest = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].cropFieldManagement.sparseVegetation = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].cropFieldManagement.woodyResidueWeight[0] = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].cropFieldManagement.woodyResidueWeight[1] = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].cropFieldManagement.woodyResidueTreatment[0] = data[iExp].value(label++).remove("\"");
        inputData[iExp].cropFieldManagement.woodyResidueTreatment[1] = data[iExp].value(label++).remove("\"");
        inputData[iExp].cropFieldManagement.greenResidueWeight[0] = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].cropFieldManagement.greenResidueWeight[1] = data[iExp].value(label++).remove("\"").toFloat();
        inputData[iExp].cropFieldManagement.greenResidueTreatment[0] = data[iExp].value(label++).remove("\"");
        inputData[iExp].cropFieldManagement.greenResidueTreatment[1] = data[iExp].value(label++).remove("\"");

        inputData[iExp].agronomicInput.pesticideWeight = data[iExp].value(label++).remove("\"").toFloat();
        for (int i=0;i<4;i++)
        {
            inputData[iExp].agronomicInput.fertilizerName[i] = data[iExp].value(label++).remove("\"");
        }
        for (int i=0;i<4;i++)
        {
             inputData[iExp].agronomicInput.fertilizerAmount[i] = data[iExp].value(label++).remove("\"").toFloat();
        }
        for (int i=0;i<4;i++)
        {
            inputData[iExp].agronomicInput.fertilizerApplicationMethod[i] = data[iExp].value(label++).remove("\"");
        }
        for (int i=0;i<4;i++)
        {
            inputData[iExp].agronomicInput.fertilizerInhibitor[i] = data[iExp].value(label++).remove("\"");
        }

        for (int i=0;i<4;i++)
        {
            inputData[iExp].agronomicInput.amendmentName[i] = data[iExp].value(label++).remove("\"");
        }
        for (int i=0;i<4;i++)
        {
             inputData[iExp].agronomicInput.amendmentAmount[i] = data[iExp].value(label++).remove("\"").toFloat();
        }
        for (int i=0;i<4;i++)
        {
            inputData[iExp].agronomicInput.amendmentApplicationMethod[i] = data[iExp].value(label++).remove("\"");
        }
        for (int i=0;i<4;i++)
        {
            inputData[iExp].agronomicInput.amendmentInhibitor[i] = data[iExp].value(label++).remove("\"");
        }
        double valueEnergy;
        valueEnergy = inputData[iExp].energy.biodiesel = data[iExp].value(label++).remove("\"").toFloat();
        if (valueEnergy < 0) inputData[iExp].energy.biodiesel = valueEnergy = 20.;
        valueEnergy = inputData[iExp].energy.bioethanol = data[iExp].value(label++).remove("\"").toFloat();
        if (valueEnergy < 0) inputData[iExp].energy.bioethanol = valueEnergy = 20.;
        valueEnergy = inputData[iExp].energy.diesel = data[iExp].value(label++).remove("\"").toFloat();
        if (valueEnergy < 0) inputData[iExp].energy.diesel = valueEnergy = 20.;
        valueEnergy = inputData[iExp].energy.oil = data[iExp].value(label++).remove("\"").toFloat();
        if (valueEnergy < 0) inputData[iExp].energy.oil = valueEnergy = 20.;
        valueEnergy = inputData[iExp].energy.petrol = data[iExp].value(label++).remove("\"").toFloat();
        if (valueEnergy < 0) inputData[iExp].energy.petrol = valueEnergy = 20.;
        valueEnergy = inputData[iExp].energy.LPG = data[iExp].value(label++).remove("\"").toFloat();
        if (valueEnergy < 0) inputData[iExp].energy.LPG = valueEnergy = 20.;
        valueEnergy = inputData[iExp].energy.coal = data[iExp].value(label++).remove("\"").toFloat();
        if (valueEnergy < 0) inputData[iExp].energy.coal = valueEnergy = 20.;
        valueEnergy = inputData[iExp].energy.highEnergyDensityBiomass = data[iExp].value(label++).remove("\"").toFloat();
        if (valueEnergy < 0) inputData[iExp].energy.highEnergyDensityBiomass = valueEnergy = 20.;
        valueEnergy = inputData[iExp].energy.wood = data[iExp].value(label++).remove("\"").toFloat();
        if (valueEnergy < 0) inputData[iExp].energy.wood = valueEnergy = 20.;
        valueEnergy = inputData[iExp].energy.methane = data[iExp].value(label++).remove("\"").toFloat();
        if (valueEnergy < 0) inputData[iExp].energy.methane = valueEnergy = 20.;
        valueEnergy = inputData[iExp].energy.electricityGridAmount = data[iExp].value(label++).remove("\"").toFloat();
        if (valueEnergy < 0) inputData[iExp].energy.electricityGridAmount = valueEnergy = 20.;
        inputData[iExp].energy.electricityGridPercentageRenewables = data[iExp].value(label++).remove("\"").toFloat();
        valueEnergy = inputData[iExp].energy.electricityHydro = data[iExp].value(label++).remove("\"").toFloat();
        if (valueEnergy < 0) inputData[iExp].energy.electricityHydro = valueEnergy = 20.;
        valueEnergy = inputData[iExp].energy.electricitySolar = data[iExp].value(label++).remove("\"").toFloat();
        if (valueEnergy < 0) inputData[iExp].energy.electricitySolar = valueEnergy = 20.;
        valueEnergy = inputData[iExp].energy.electricityEolic = data[iExp].value(label++).remove("\"").toFloat();
        if (valueEnergy < 0) inputData[iExp].energy.electricityEolic = valueEnergy = 20.;
    }
    return true;
}

bool openDataBase(QSqlDatabase &db, QString dataPath)
{

    QString dbName = dataPath + "carbonCalculatorDataBase.db";
    if (! QFile(dbName).exists())
    {
        std::cout << "Error! db file is missing: " << dbName.toStdString() << std::endl;
        return false;
    }
    db = QSqlDatabase::addDatabase("QSQLITE", "carbon");
    db.setDatabaseName(dbName);
    if (! db.open())
    {
        std::cout << "Error opening db:" << db.lastError().text().toStdString() << std::endl;
        return false;
    }

    return true;
}

bool setCarbonCalculatorVariables(QSqlDatabase &db,CarbonCalculator &calculatorCO2,std::vector<TinputData> &inputData,int iExp)
{

    QString error;
    float avgRainfall = inputData[iExp].climate.annualRainfall ; // input from .csv
    if (avgRainfall == NODATA)
    {
        if (inputData[iExp].general.idRegion == "Emilia-Romagna")
            avgRainfall = 687; // data of Cesena
        else if (inputData[iExp].general.idRegion == "Molise")
            avgRainfall = 583; // data of Campobasso
        else if (inputData[iExp].general.idRegion == "Puglia")
            avgRainfall = 562.6; // data of Bari
        else if (inputData[iExp].general.idRegion == "Marche")
            avgRainfall = 742; // data of Pesaro
        else if (inputData[iExp].general.idRegion == "Zadar")
            avgRainfall = 879.2; // data of Zadar
        else if (inputData[iExp].general.idRegion == "Dubrovnik_Neretva")
            avgRainfall = 1446.6; // data of Dubrovnik
        else if (inputData[iExp].general.idRegion == "Split_Dalmatia")
            avgRainfall = 692.1; // data of Split
        else
            avgRainfall = 687;


    }
    float avgTemperature = inputData[iExp].climate.meanTemperature; // to quit // input from .csv
    if (avgTemperature == NODATA)
    {
        if (inputData[iExp].general.idRegion == "Emilia-Romagna")
            avgTemperature = 13;
        else if (inputData[iExp].general.idRegion == "Molise")
            avgTemperature = 9.4;
        else if (inputData[iExp].general.idRegion == "Puglia")
            avgTemperature = 15.6; // data of Bari
        else if (inputData[iExp].general.idRegion == "Marche")
            avgTemperature = 13.5; // data of Pesaro
        else if (inputData[iExp].general.idRegion == "Zadar")
            avgTemperature = 15; // data of Zadar
        else if (inputData[iExp].general.idRegion == "Dubrovnik_Neretva")
            avgTemperature = 16.5; // data of Dubrovnik
        else if (inputData[iExp].general.idRegion == "Split_Dalmatia")
            avgTemperature = 15.3; // data of Split
        else
            avgTemperature = 13;
    }
    // read renewables

    if (! readRenewables(inputData[iExp].general.idCountry,inputData[iExp].general.year, db, calculatorCO2, error))
    {
        std::cout << "ERROR: " + error.toStdString() << std::endl;
        return false;
    }
    bool isOrganic = false; // input from .csv
    if (inputData[iExp].cropFieldManagement.isOrganic == "Yes")
    {
        isOrganic = true; // input from .csv
    }

    calculatorCO2.soilManage.isOrganic = isOrganic;
    calculatorCO2.soilManage.yield = inputData[iExp].cropFieldManagement.yield ; // t/ha input from .csv
    calculatorCO2.soilManage.fieldSize = inputData[iExp].general.fieldSize; // ha input from .csv
    double surfaceSparseTreesSchrubsHedgeFallow = inputData[iExp].cropFieldManagement.sparseVegetation; // m2 of sparse vegetation input from csv
    double ratioFallowExtension;
    ratioFallowExtension = surfaceSparseTreesSchrubsHedgeFallow / (calculatorCO2.soilManage.fieldSize * 10000);
    //ratioFallowExtension = surfaceSparseTreesSchrubsHedgeFallow / 100.;
    // read soil drainage
    QString idDrainage = inputData[iExp].soil.drainage; // input from .csv
    // read pH
    double pHSoil = inputData[iExp].soil.pH; // input from .csv
    QString idSoilTexture = inputData[iExp].soil.texture; // input from .csv
    QString idSoilOrganicCarbon;
    if (inputData[iExp].soil.organicMatter <= 1.72)
    {
        idSoilOrganicCarbon = "SOM<=1.72";
    }
    else if (inputData[iExp].soil.organicMatter > 1.72 && inputData[iExp].soil.organicMatter <= 5.16)
    {
        idSoilOrganicCarbon = "1.72<SOM<=5.16";
    }
    else if (inputData[iExp].soil.organicMatter < 5.16 && inputData[iExp].soil.organicMatter <= 10.32)
    {
        idSoilOrganicCarbon = "5.16<SOM<=10.32";
    }
    else
    {
        idSoilOrganicCarbon = "SOM>10.32";
    }

    QString idClimate;
    double yearET0 = 1000 + 100*(avgTemperature - 13);
    double yearActualET = yearET0*0.6; // supposing that actual ET is 0.6 of ET0
    if (avgTemperature > 20 && avgRainfall > yearActualET)
        idClimate = "TROPICAL_MOIST";
    else if (avgTemperature > 20 && avgRainfall <= yearActualET)
        idClimate = "TROPICAL_DRY";
    else if (avgTemperature <= 20 && avgRainfall > yearActualET)
        idClimate = "TEMPERATE_MOIST";
    else
        idClimate  = "TEMPERATE_DRY";

    int nrFertilizers = 8;
    QString idFertiliser[8];
    idFertiliser[0] = inputData[iExp].agronomicInput.fertilizerName[0];
    idFertiliser[1] = inputData[iExp].agronomicInput.fertilizerName[1];
    idFertiliser[2] = inputData[iExp].agronomicInput.fertilizerName[2];
    idFertiliser[3] = inputData[iExp].agronomicInput.fertilizerName[3];
    idFertiliser[4] = inputData[iExp].agronomicInput.amendmentName[0];
    idFertiliser[5] = inputData[iExp].agronomicInput.amendmentName[1];
    idFertiliser[6] = inputData[iExp].agronomicInput.amendmentName[2];
    idFertiliser[7] = inputData[iExp].agronomicInput.amendmentName[3];

    QString inhibitor[8];
    inhibitor[0] = inputData[iExp].agronomicInput.fertilizerInhibitor[0];
    inhibitor[1] = inputData[iExp].agronomicInput.fertilizerInhibitor[1];
    inhibitor[2] = inputData[iExp].agronomicInput.fertilizerInhibitor[2];
    inhibitor[3] = inputData[iExp].agronomicInput.fertilizerInhibitor[3];
    inhibitor[4] = inputData[iExp].agronomicInput.amendmentInhibitor[0];
    inhibitor[5] = inputData[iExp].agronomicInput.amendmentInhibitor[1];
    inhibitor[6] = inputData[iExp].agronomicInput.amendmentInhibitor[2];
    inhibitor[7] = inputData[iExp].agronomicInput.amendmentInhibitor[3];

    QString idFeature[8];
    for (int i=0;i<nrFertilizers;i++)
        idFeature[i] = "field2"; // initialization - default value


    for (int i=0;i<nrFertilizers/2;i++)
    {
        if (inputData[iExp].agronomicInput.fertilizerApplicationMethod[i] == "Apply_in_solution")
        {
            idFeature[i] = "field2";
        }
        if (inputData[iExp].agronomicInput.amendmentApplicationMethod[i] == "Apply_in_solution")
        {
            idFeature[i+(nrFertilizers/2)] = "field2";
        }
        if (inputData[iExp].agronomicInput.fertilizerApplicationMethod[i] == "Spread")
        {
            idFeature[i] = "field3";
        }
        if (inputData[iExp].agronomicInput.amendmentApplicationMethod[i] == "Spread")
        {
            idFeature[i+(nrFertilizers/2)] = "field3";
        }
        if (inputData[iExp].agronomicInput.fertilizerApplicationMethod[i] == "Spread_or_incorporate_then_flood")
        {
            idFeature[i] = "field4";
        }
        if (inputData[iExp].agronomicInput.amendmentApplicationMethod[i] == "Spread_or_incorporate_then_flood")
        {
            idFeature[i+(nrFertilizers/2)] = "field4";
        }
        if (inputData[iExp].agronomicInput.fertilizerApplicationMethod[i] == "Spread_to_floodwater_at_panicle_initiation")
        {
            idFeature[i] = "field5";
        }
        if (inputData[iExp].agronomicInput.amendmentApplicationMethod[i] == "Spread_to_floodwater_at_panicle_initiation")
        {
            idFeature[i+(nrFertilizers/2)] = "field5";
        }
        if (inputData[iExp].agronomicInput.fertilizerApplicationMethod[i] == "Incorporate")
        {
            idFeature[i] = "field6";
        }
        if (inputData[iExp].agronomicInput.amendmentApplicationMethod[i] == "Incorporate")
        {
            idFeature[i+(nrFertilizers/2)] = "field6";
        }
        if (inputData[iExp].agronomicInput.fertilizerApplicationMethod[i] == "Subsurface_drip")
        {
            idFeature[i] = "field7";
        }
        if (inputData[iExp].agronomicInput.amendmentApplicationMethod[i] == "Subsurface_drip")
        {
            idFeature[i+(nrFertilizers/2)] = "field7";
        }
        if (inputData[iExp].agronomicInput.fertilizerApplicationMethod[i] == "Not_applied")
        {
            idFeature[i] = "field7";
        }
        if (inputData[iExp].agronomicInput.amendmentApplicationMethod[i] == "Not_apllied")
        {
            idFeature[i+(nrFertilizers/2)] = "field7";
        }

    }


    QString idCrop = inputData[iExp].cropFieldManagement.cropName; //input from .csv
    calculatorCO2.soilDepth = inputData[iExp].soil.depth;
    if (inputData[iExp].soil.depth < 0) calculatorCO2.soilDepth = 30;
    calculatorCO2.skeleton = inputData[iExp].soil.skeleton;

    calculatorCO2.initialiazeVariables(idDrainage,pHSoil,idSoilTexture,idSoilOrganicCarbon,inhibitor);


    // read climate

    if (! readClimate(idClimate, db, calculatorCO2, error))
    {
        std::cout << "ERROR: " + error.toStdString() << std::endl;
        return false;
    }


    // read fertilizer
    for (int i=0; i<nrFertilizers;i++)
        calculatorCO2.fertiliser.amountFertiliser[i] = 0; // initialization - default value

    calculatorCO2.fertiliser.amountFertiliser[0] = inputData[iExp].agronomicInput.fertilizerAmount[0];
    calculatorCO2.fertiliser.amountFertiliser[1] = inputData[iExp].agronomicInput.fertilizerAmount[1];
    calculatorCO2.fertiliser.amountFertiliser[2] = inputData[iExp].agronomicInput.fertilizerAmount[2];
    calculatorCO2.fertiliser.amountFertiliser[3] = inputData[iExp].agronomicInput.fertilizerAmount[3];
    calculatorCO2.fertiliser.amountFertiliser[4] = inputData[iExp].agronomicInput.amendmentAmount[0];
    calculatorCO2.fertiliser.amountFertiliser[5] = inputData[iExp].agronomicInput.amendmentAmount[1];
    calculatorCO2.fertiliser.amountFertiliser[6] = inputData[iExp].agronomicInput.amendmentAmount[2];
    calculatorCO2.fertiliser.amountFertiliser[7] = inputData[iExp].agronomicInput.amendmentAmount[3];


    if (! readFertilizer(idFertiliser, db, calculatorCO2, error,nrFertilizers))
    {
        std::cout << "ERROR: " + error.toStdString() << std::endl;
        return false;
    }

    // read bouwmanNH4 table


    if (! readBouwmanNH4(idFeature, db, calculatorCO2, error,nrFertilizers))
    {
        std::cout << "ERROR: " + error.toStdString() << std::endl;
        return false;
    }

    // biomass data
    calculatorCO2.biomassInTree.orchardAge = inputData[iExp].cropFieldManagement.orchardAge ;
    calculatorCO2.biomassInTree.currentHeight = inputData[iExp].cropFieldManagement.treeHeight;
    calculatorCO2.biomassInTree.treeDensity = inputData[iExp].cropFieldManagement.treeDensity;
    calculatorCO2.biomassInTree.deadTreeDensity = inputData[iExp].cropFieldManagement.deadTreeDensity;
    calculatorCO2.biomassInTree.currentDiameter = inputData[iExp].cropFieldManagement.treeDBH;

    //read residue_treatment table
    QString idResidue[4]; //input from .csv
    idResidue[0] = inputData[iExp].cropFieldManagement.woodyResidueTreatment[0];
    idResidue[2] = inputData[iExp].cropFieldManagement.greenResidueTreatment[0];
    idResidue[1] = inputData[iExp].cropFieldManagement.woodyResidueTreatment[1];
    idResidue[3] = inputData[iExp].cropFieldManagement.greenResidueTreatment[1];
    for (int i=0;i<4;i++)
    {
        if (idResidue[i] == "Left_on_field_incorporated_or_mulch")
        {
            calculatorCO2.cropResidue.residueLeftOnField[i] = true;
        }
        else
        {
            calculatorCO2.cropResidue.residueLeftOnField[i] = false;
        }
    }

    if (! readResidue(idResidue, db, calculatorCO2, error))
    {
        std::cout << "ERROR: " + error.toStdString() << std::endl;
        return false;
    }


    // read crop_parameters table
    if (! readCropParameters(idCrop, db, calculatorCO2, error))
    {
        std::cout << "ERROR: " + error.toStdString() << std::endl;
        return false;
    }




    // *********************************************************************
    calculatorCO2.cropResidue.residueWeight[0] = inputData[iExp].cropFieldManagement.woodyResidueWeight[0]; //(t/ha) dry weight of woody residue input from .csv
    calculatorCO2.cropResidue.residueWeight[1] = inputData[iExp].cropFieldManagement.woodyResidueWeight[1]; //(t/ha) dry weight of woody residue input from .csv
    calculatorCO2.cropResidue.residueWeight[2] = inputData[iExp].cropFieldManagement.greenResidueWeight[0]; //(t/ha) dry weight of green residue input from .csv
    calculatorCO2.cropResidue.residueWeight[3] = inputData[iExp].cropFieldManagement.woodyResidueWeight[1]; //(t/ha) dry weight of woody residue input from .csv

    if (calculatorCO2.cropResidue.residueWeight[0] == NODATA && calculatorCO2.cropResidue.residueWeight[1] == NODATA)
    {
        calculatorCO2.cropResidue.residueWeight[0] = calculatorCO2.biomassInTree.annualCarbonWoodyDryMatter*0.5; // t/ha
        calculatorCO2.cropResidue.residueWeight[1] = 0;
    }
    else if (calculatorCO2.cropResidue.residueWeight[0] != NODATA && calculatorCO2.cropResidue.residueWeight[1] == NODATA)
    {
        calculatorCO2.cropResidue.residueWeight[1] = 0; // t/ha
    }
    if (calculatorCO2.cropResidue.residueWeight[2] == NODATA && calculatorCO2.cropResidue.residueWeight[3] == NODATA)
    {
        calculatorCO2.cropResidue.residueWeight[2] = 0.5; // t/ha
        calculatorCO2.cropResidue.residueWeight[3] = 0; // t/ha
    }
    else if (calculatorCO2.cropResidue.residueWeight[2] != NODATA && calculatorCO2.cropResidue.residueWeight[3] == NODATA)
    {
        calculatorCO2.cropResidue.residueWeight[3] = 0; // t/ha
    }

    calculatorCO2.cropResidue.totalWoodyResidue = calculatorCO2.cropResidue.residueWeight[0];
    calculatorCO2.cropResidue.totalWoodyResidue += calculatorCO2.cropResidue.residueWeight[1];
    /*calculatorCO2.percentageTreeBiomassToAccountFor = 20 * 0.36; // supposing 20% is the wheight of the belowground biomass
    for (int i=0;i<2;i++)
    {
        if (idResidue[i] == "Left_on_field_incorporated_or_mulch")
            calculatorCO2.percentageTreeBiomassToAccountFor += (80 * 0.36)*calculatorCO2.cropResidue.residueWeight[i]/calculatorCO2.cropResidue.totalWoodyResidue;
        else if (idResidue[i] == "Biochar")
            calculatorCO2.percentageTreeBiomassToAccountFor += (80 * 0.5)*calculatorCO2.cropResidue.residueWeight[i]/calculatorCO2.cropResidue.totalWoodyResidue;
        else if (idResidue[i] == "Burned")
            calculatorCO2.percentageTreeBiomassToAccountFor += (80 * 0.05)*calculatorCO2.cropResidue.residueWeight[i]/calculatorCO2.cropResidue.totalWoodyResidue;
    }*/
    // **********************************************************************
    double idPercentageEnergyInGrid = inputData[iExp].energy.electricityGridPercentageRenewables;  // % input from .csv
    if (idPercentageEnergyInGrid != NODATA)
    {
        calculatorCO2.energy.percentageRenewablesInGrid = idPercentageEnergyInGrid;
    }
    calculatorCO2.energy.input.fromElectricityGrid = inputData[iExp].energy.electricityGridAmount; // kWh input from .csv
    calculatorCO2.energy.input.fromElectricityOwnHydropower = inputData[iExp].energy.electricityHydro; // kWh input from .csv
    calculatorCO2.energy.input.fromElectricityOwnPhotovoltaic = inputData[iExp].energy.electricitySolar; // kWh input from .csv
    calculatorCO2.energy.input.fromElectricityOwnWind = inputData[iExp].energy.electricityEolic; // kWh input from .csv
    calculatorCO2.energy.input.fromFuelBiodiesel = inputData[iExp].energy.biodiesel; // l input from .csv
    calculatorCO2.energy.input.fromFuelBioethanol = inputData[iExp].energy.bioethanol; // l input from .csv
    calculatorCO2.energy.input.fromFuelCoal = inputData[iExp].energy.coal; // kg input from .csv
    calculatorCO2.energy.input.fromFuelDiesel = inputData[iExp].energy.diesel; // l input from .csv
    calculatorCO2.energy.input.fromFuelHighDensityBiomass = inputData[iExp].energy.highEnergyDensityBiomass; // kg input from .csv
    calculatorCO2.energy.input.fromFuelLPG = inputData[iExp].energy.highEnergyDensityBiomass; // l input from .csv
    calculatorCO2.energy.input.fromFuelOil = inputData[iExp].energy.oil; // l input from .csv
    calculatorCO2.energy.input.fromFuelPetrol = inputData[iExp].energy.petrol; // l input from .csv
    calculatorCO2.energy.input.fromFuelWood = inputData[iExp].energy.wood; // kg input from .csv
    calculatorCO2.energy.input.fromFuelMethane = inputData[iExp].energy.methane; // kg input from .csv
    // **********************************************************************
    calculatorCO2.pesticide.weightOfActivePrinciple = inputData[iExp].agronomicInput.pesticideWeight; // input from .csv kg active principle
    // **********************************************************************

    calculatorCO2.soilManage.percentage.coverCropping = 100.*inputData[iExp].cropFieldManagement.coverCrop/(calculatorCO2.soilManage.fieldSize*10000); // input from .csv

    calculatorCO2.soilManage.percentage.forest = 100. * inputData[iExp].cropFieldManagement.forest/(calculatorCO2.soilManage.fieldSize*10000) ; // input from .csv
    calculatorCO2.soilManage.percentage.forest += (ratioFallowExtension*100/2.) ;
    calculatorCO2.soilManage.percentage.permanentGrass = 100. * inputData[iExp].cropFieldManagement.permanentGrass/(calculatorCO2.soilManage.fieldSize*10000); // input from .csv
    calculatorCO2.soilManage.percentage.permanentGrass += (ratioFallowExtension*100/2.); //assuming that sparse vegetation is intermediate between forest and permanetn grass

    calculatorCO2.soilManage.percentage.arable = 100 - calculatorCO2.soilManage.percentage.forest - calculatorCO2.soilManage.percentage.permanentGrass;
    if (calculatorCO2.soilManage.percentage.arable > 0.01)
    {
        calculatorCO2.soilManage.percentage.noTillage =100 * inputData[iExp].cropFieldManagement.noTillage/(calculatorCO2.soilManage.fieldSize*10000)*100/calculatorCO2.soilManage.percentage.arable; // input from .csv
        calculatorCO2.soilManage.percentage.minimumTillage =100 * inputData[iExp].cropFieldManagement.minTillage/(calculatorCO2.soilManage.fieldSize*10000)*100/calculatorCO2.soilManage.percentage.arable; // input from .csv
        calculatorCO2.soilManage.percentage.conventionalTillage = 100 - calculatorCO2.soilManage.percentage.noTillage - calculatorCO2.soilManage.percentage.minimumTillage;
    }
    else
    {
        calculatorCO2.soilManage.percentage.noTillage = 100; // input from .csv
        calculatorCO2.soilManage.percentage.minimumTillage = 0; // input from .csv
        calculatorCO2.soilManage.percentage.conventionalTillage = 0;
    }
    // *********************************************************************
    // erosion
    calculatorCO2.averageTemperature = avgTemperature;
    calculatorCO2.annualRainfall = avgRainfall;
    calculatorCO2.erosion.erosionFactor.rainfall = calculatorCO2.annualRainfall;
    if (idSoilTexture == "Medium")
        calculatorCO2.erosion.erosionFactor.texture = 0.3;
    else if (idSoilTexture == "Fine")
        calculatorCO2.erosion.erosionFactor.texture = 0.2;
    else
        calculatorCO2.erosion.erosionFactor.texture = 0.05;


    calculatorCO2.erosion.erosionFactor.slope = inputData[iExp].general.fieldSlope;
    // cover factor
    calculatorCO2.erosion.erosionFactor.cover = 0.01* (calculatorCO2.soilManage.percentage.forest* 0.005 + calculatorCO2.soilManage.percentage.permanentGrass* 0.01 + calculatorCO2.soilManage.percentage.arable* 0.128);

    calculatorCO2.erosion.erosionFactor.soilManagement = 0.01*(calculatorCO2.soilManage.percentage.coverCropping*0.26 + (100 - calculatorCO2.soilManage.percentage.coverCropping)*1);
    calculatorCO2.erosion.erosionFactor.soilManagement *= 0.01*(calculatorCO2.soilManage.percentage.conventionalTillage + calculatorCO2.soilManage.percentage.minimumTillage*0.52 + calculatorCO2.soilManage.percentage.noTillage*0.26);
    calculatorCO2.erosion.erosionFactor.soilManagement *= 0.01 * calculatorCO2.soilManage.percentage.arable;
    calculatorCO2.erosion.erosionFactor.soilManagement += 0.01*(calculatorCO2.soilManage.percentage.permanentGrass + calculatorCO2.soilManage.percentage.forest)*0.26;
    return true;
}

bool  BuyerCalculator::setInputBuyer(std::vector<TinputDataBuyer> inputData, int iExp, CarbonCalculator calculatorCO2)
{
    isPresentLCA = inputData[iExp].generalBuyer.isPresentLCA;
    valueLCA = inputData[iExp].generalBuyer.valueLCA;
    energy.percentageRenewablesInGrid = inputData[iExp].energy.electricityGridPercentageRenewables;
    energy.input.fromElectricityGrid = inputData[iExp].energy.electricityGridAmount; // kWh input from .csv
    energy.input.fromElectricityOwnHydropower = inputData[iExp].energy.electricityHydro; // kWh input from .csv
    energy.input.fromElectricityOwnPhotovoltaic = inputData[iExp].energy.electricitySolar; // kWh input from .csv
    energy.input.fromElectricityOwnWind = inputData[iExp].energy.electricityEolic; // kWh input from .csv
    energy.input.fromFuelBiodiesel = inputData[iExp].energy.biodiesel; // l input from .csv
    energy.input.fromFuelBioethanol = inputData[iExp].energy.bioethanol; // l input from .csv
    energy.input.fromFuelCoal = inputData[iExp].energy.coal; // kg input from .csv
    energy.input.fromFuelDiesel = inputData[iExp].energy.diesel; // l input from .csv
    energy.input.fromFuelHighDensityBiomass = inputData[iExp].energy.highEnergyDensityBiomass; // kg input from .csv
    energy.input.fromFuelLPG = inputData[iExp].energy.highEnergyDensityBiomass; // l input from .csv
    energy.input.fromFuelOil = inputData[iExp].energy.oil; // l input from .csv
    energy.input.fromFuelPetrol = inputData[iExp].energy.petrol; // l input from .csv
    energy.input.fromFuelWood = inputData[iExp].energy.wood; // kg input from .csv
    energy.input.fromFuelMethane = inputData[iExp].energy.methane; // kg input from .csv

    energy.parameterElectricity.fossil = calculatorCO2.energy.parameterElectricity.fossil;
    energy.parameterElectricity.hydropower = calculatorCO2.energy.parameterElectricity.hydropower;
    energy.parameterElectricity.photovoltaic = calculatorCO2.energy.parameterElectricity.photovoltaic;
    energy.parameterElectricity.wind = calculatorCO2.energy.parameterElectricity.wind;
    energy.parameterFuel.biodieselEmissionPerLitre = calculatorCO2.energy.parameterFuel.biodieselEmissionPerLitre;
    energy.parameterFuel.coalEmissionPerEnergyUnit = calculatorCO2.energy.parameterFuel.coalEmissionPerEnergyUnit;
    energy.parameterFuel.coalHeatPower = calculatorCO2.energy.parameterFuel.coalHeatPower;
    energy.parameterFuel.dieselEmissionPerLitre = calculatorCO2.energy.parameterFuel.dieselEmissionPerLitre;
    energy.parameterFuel.ethanolEmissionPerLitre = calculatorCO2.energy.parameterFuel.ethanolEmissionPerLitre;
    energy.parameterFuel.heavyOilHeatPower = calculatorCO2.energy.parameterFuel.heavyOilHeatPower;
    energy.parameterFuel.highDensityBiomassHeatPower = calculatorCO2.energy.parameterFuel.HighDensityBiomassEmissionPerEnergyUnit;
    energy.parameterFuel.HighDensityBiomassEmissionPerEnergyUnit = calculatorCO2.energy.parameterFuel.highDensityBiomassHeatPower;
    energy.parameterFuel.LPGHeatPower = calculatorCO2.energy.parameterFuel.LPGHeatPower;
    energy.parameterFuel.lpgPerEnergyUnit = calculatorCO2.energy.parameterFuel.lpgPerEnergyUnit;
    energy.parameterFuel.methaneHeatPower = calculatorCO2.energy.parameterFuel.methaneHeatPower;
    energy.parameterFuel.methanePerEnergyUnit = calculatorCO2.energy.parameterFuel.methanePerEnergyUnit;
    energy.parameterFuel.oilPerEnergyUnit = calculatorCO2.energy.parameterFuel.oilPerEnergyUnit;
    energy.parameterFuel.petrolEmissionPerLitre = calculatorCO2.energy.parameterFuel.petrolEmissionPerLitre;
    energy.parameterFuel.woodEmissionPerEnergyUnit = calculatorCO2.energy.parameterFuel.woodEmissionPerEnergyUnit;
    energy.parameterFuel.woodHeatPower = calculatorCO2.energy.parameterFuel.woodHeatPower;


    return true;
}

double BuyerCalculator::computeDebitsBuyer()
{
    double debits;

    if (isPresentLCA > 0.1)
    {
        energy.emissions.total = NODATA;
        debits = valueLCA;
    }
    else
    {
        energy.computeEmissions();
        debits = energy.emissions.total;
    }
    return debits;
}

void printOutput(CarbonCalculator &calculatorCO2)
{
    std::cout << "values are in kgCO2Eq " << std::endl;
    std::cout << "emissions due to energy: " << calculatorCO2.energy.emissions.total << std::endl;
    std::cout << "emissions due to pesticide production: " << calculatorCO2.pesticide.emissionDueToProduction << std::endl;
    std::cout << "emissions due to residue management: " << calculatorCO2.cropResidue.kgCO2Equivalent.total << std::endl;
    std::cout << "emissions due to type of soil due to Nitrogen: " << calculatorCO2.fertiliser.emissionDueToSoil << std::endl;
    std::cout << "emissions due to type of soil due to Carbon Oxidation: " << calculatorCO2.soilManage.computeEmissions(calculatorCO2.carbonInTop30CmSoil,calculatorCO2.idClimate) << std::endl;
    std::cout << "emissions due to fertiliser production: " << calculatorCO2.fertiliser.emissionDueToFertiliserProduction << std::endl;
    std::cout << "emissions due to fertiliser application: " << calculatorCO2.fertiliser.emissionDueToFertiliserApplication << std::endl;
    std::cout << "loss due to erosion: " << calculatorCO2.erosion.lostCO2 << std::endl;
    //std::cout << "sequestration due to minimum tillage and crop covering and land use: " << calculatorCO2.soilManage.sequestrationCarbonCO2Eq << std::endl;
    std::cout << "sequestration due to conservative tillage"<< calculatorCO2.soilManage.sequestrationCarbonCO2EqTillage << std::endl;
    std::cout << "sequestration due to cover crop use"<< calculatorCO2.soilManage.sequestrationCarbonCO2EqCropCover << std::endl;
    std::cout << "sequestration due to conservative Land use (forest + permanent grass + sparse vegetation)"<< calculatorCO2.soilManage.sequestrationCarbonCO2EqLandUse << std::endl;
    for (int i=0; i<8; i++)
    {
        std::cout << "sequestration due to amendment"<<calculatorCO2.soilManage.sequestrationCarbonCO2EqFertilizerAmendment[i] << std::endl;;
    }
    std::cout << "sequestration due to woody residues" <<calculatorCO2.soilManage.sequestrationCarbonCO2EqResidue[0] << std::endl;
    std::cout << "sequestration due to woody residues2" <<calculatorCO2.soilManage.sequestrationCarbonCO2EqResidue[1] << std::endl;
    std::cout << "sequestration due to green residues" <<calculatorCO2.soilManage.sequestrationCarbonCO2EqResidue[2] << std::endl;
    std::cout << "sequestration due to green residues2" <<calculatorCO2.soilManage.sequestrationCarbonCO2EqResidue[3] << std::endl;

    std::cout <<"sequestration - recalcitrant carbon stock: " <<calculatorCO2.fertiliser.sequestrationDueToFertiliserApplication << std::endl;
    std::cout << "sequestration due to roots: " <<calculatorCO2.soilManage.computeSequestrationRootBiomass(calculatorCO2.idClimate) << std::endl;

    printf("\n\n");
    std::cout << "___________________________________________________________________________\n" << std::endl;
    std::cout << "carbon budget per hectare (soil): " <<calculatorCO2.carbonBudgetPerHectareSoil << "  "<<std::endl;
    std::cout << "___________________________________________________________________________\n" << std::endl;
    std::cout << "carbon budget whole field (soil): " <<calculatorCO2.carbonBudgetWholeFieldSoil << "  "<<std::endl;
    std::cout << "___________________________________________________________________________\n" << std::endl;
    std::cout << "carbon footprint gCO2Eq per kg of produce (soil): " <<calculatorCO2.carbonFootprintPerKgOfProduceSoil << "  "<<std::endl;
    std::cout << "___________________________________________________________________________\n" << std::endl;

    printf("\n\n");
    std::cout << "___________________________________________________________________________\n" << std::endl;
    std::cout << "carbon budget per hectare soil : " <<calculatorCO2.carbonBudgetPerHectareSoil << "  "<<std::endl;
    std::cout << "___________________________________________________________________________\n" << std::endl;
    std::cout << "carbon budget per Hectare biomass: " <<calculatorCO2.carbonBudgetPerHectareBiomass << "  "<<std::endl;
    std::cout << "___________________________________________________________________________\n" << std::endl;
    std::cout << "total Biomass : " <<calculatorCO2.carbonBiomass << "  "<<std::endl;
    std::cout << "___________________________________________________________________________\n" << std::endl;
    std::cout << "orchard age : " <<calculatorCO2.biomassInTree.orchardAge << "  "<<std::endl;
    std::cout << "___________________________________________________________________________\n" << std::endl;
    std::cout << "carbon saved due to sustainable practices: " <<calculatorCO2.carbonSavedBySustainablePractices << "  "<<std::endl;
    std::cout << "___________________________________________________________________________\n" << std::endl;
    printf("\n\n");
}



