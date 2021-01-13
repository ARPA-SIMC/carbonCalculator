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
    if (! createTableClimate(dbOutput))
        return false;
    if (! createTableSoil(dbOutput))
        return false;
    if (! createTableCropField(dbOutput))
        return false;
    if (! createTableAgronomicInputs(dbOutput))
        return false;
    if (! createTableEnergy(dbOutput))
        return false;

    // TODO other tables

    return true;
}

bool saveOutput(QString id, QSqlDatabase &dbOutput, TinputData &inputData)
{
    if (! saveTableGeneral(id, dbOutput, inputData, "general"))
        return false;
    if (! saveTableClimate(id, dbOutput, inputData, "climate"))
        return false;
    if (! saveTableSoil(id, dbOutput, inputData, "soil"))
        return false;
    if (! saveTableCropField(id, dbOutput, inputData, "crop_field"))
        return false;
    if (! saveTableAgronomicInputs(id, dbOutput, inputData, "agronomic_inputs"))
        return false;
    if (! saveTableEnergy(id, dbOutput, inputData, "energy"))
        return false;
    // TODO save other tables

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
                 + "," + QString::number(inputData.general.latitude)
                 + "," + QString::number(inputData.general.longitude)
                 + "," + QString::number(inputData.general.fieldSize)
                 + "," + QString::number(inputData.general.fieldSlope)
                 + ")";

    QSqlQuery myQuery = dbOutput.exec(queryOutput);
    if (myQuery.lastError().isValid())
    {
        std::cout << "Error in saving table " + tableName.toStdString() + ": " << myQuery.lastError().text().toStdString();
        return false;
    }

    return true;
}

bool createTableClimate(QSqlDatabase &dbOutput)
{
    QString queryString = "DROP TABLE climate";
    QSqlQuery myQuery = dbOutput.exec(queryString);

    queryString = "CREATE TABLE climate "
                  " (id TEXT, annual_mean_temperature REAL, cumulated_annual_precipitation REAL, "
                  " reference_evapotranspiration REAL, climatic_water_balance REAL )";


    myQuery = dbOutput.exec(queryString);

    if (myQuery.lastError().isValid())
    {
        std::cout << "Error in creating table 'climate': " << myQuery.lastError().text().toStdString();
        return false;
    }

    return true;
}


bool saveTableClimate(QString id, QSqlDatabase &dbOutput, TinputData &inputData, QString tableName)
{
    QString queryOutput = "INSERT INTO " + tableName
                       + " (id, annual_mean_temperature, cumulated_annual_precipitation, "
                         " reference_evapotranspiration , climatic_water_balance)"
                       " VALUES ";

    queryOutput += "('" + id + "'"
                 + "," + QString::number(inputData.climate.meanTemperature)
                 + "," + QString::number(inputData.climate.annualRainfall)
                 + "," + QString::number(inputData.climate.referenceEvapotranspiration)
                 + "," + QString::number(inputData.climate.climaticWaterBalance)
                 + ")";

    QSqlQuery myQuery = dbOutput.exec(queryOutput);
    if (myQuery.lastError().isValid())
    {
        std::cout << "Error in saving table " + tableName.toStdString() + ": " << myQuery.lastError().text().toStdString();
        return false;
    }

    return true;
}

bool createTableSoil(QSqlDatabase &dbOutput)
{
    QString queryString = "DROP TABLE soil";
    QSqlQuery myQuery = dbOutput.exec(queryString);

    queryString = "CREATE TABLE soil "
                  " (id TEXT, soil_depth REAL, drainage TEXT, pH REAL, texture TEXT, "
                  " organic_matter REAL, skeleton REAL, available_water_capacity REAL, "
                  " total_nitrogen REAL, C_N_ratio REAL )";

    myQuery = dbOutput.exec(queryString);

    if (myQuery.lastError().isValid())
    {
        std::cout << "Error in creating table 'soil': " << myQuery.lastError().text().toStdString();
        return false;
    }

    return true;
}


bool saveTableSoil(QString id, QSqlDatabase &dbOutput, TinputData &inputData, QString tableName)
{
    QString queryOutput = "INSERT INTO " + tableName
                       + " (id, soil_depth, drainage, pH, texture, "
                         " organic_matter, skeleton, available_water_capacity, "
                         " total_nitrogen, C_N_ratio) "
                       " VALUES ";

    queryOutput += "('" + id + "'"
                 + "," + QString::number(inputData.soil.depth)
                 + ",'" + inputData.soil.drainage + "'"
                 + "," + QString::number(inputData.soil.pH)
                 + ",'" + inputData.soil.texture + "'"
                 + "," + QString::number(inputData.soil.organicMatter)
                 + "," + QString::number(inputData.soil.skeleton)
                 + "," + QString::number(inputData.soil.availableWaterCapacity)
                 + "," + QString::number(inputData.soil.totalNitrogen)
                 + "," + QString::number(inputData.soil.carbonNitrogenRatio)
                 + ")";

    QSqlQuery myQuery = dbOutput.exec(queryOutput);
    if (myQuery.lastError().isValid())
    {
        std::cout << "Error in saving table " + tableName.toStdString() + ": " << myQuery.lastError().text().toStdString();
        return false;
    }

    return true;
}

bool createTableCropField(QSqlDatabase &dbOutput)
{
    QString queryString = "DROP TABLE crop_field";
    QSqlQuery myQuery = dbOutput.exec(queryString);

    queryString = "CREATE TABLE crop_field "
                  " (id TEXT, crop_type TEXT, density INTEGER, delta_trees INTEGER,"
                  " orchard_age INTEGER, DBH REAL, "
                  " tree_height REAL, organic_management TEXT, yield REAL, "
                  " no_tillage_area REAL, minimum_tillage_area REAL,"
                  " cover_crop_area REAL, forestry_area REAL, sparse_vegetation_area REAL,"
                  " woody_residue_weight_1 REAL, woody_residue_weight_2 REAL,"
                  " woody_residue_treatment_1 TEXT, woody_residue_treatment_2 TEXT,"
                  " green_residue_weight_1 REAL, green_residue_weight_2 REAL,"
                  " green_residue_treatment_1 TEXT, green_residue_treatment_2 TEXT"
                  ")";

    myQuery = dbOutput.exec(queryString);

    if (myQuery.lastError().isValid())
    {
        std::cout << "Error in creating table 'crop_field': " << myQuery.lastError().text().toStdString();
        return false;
    }

    return true;
}


bool saveTableCropField(QString id, QSqlDatabase &dbOutput, TinputData &inputData, QString tableName)
{
    QString queryOutput = "INSERT INTO " + tableName
                       + " (id, crop_type, density,delta_trees, orchard_age, DBH, "
                         " tree_height, organic_management, yield, "
                         " no_tillage_area, minimum_tillage_area, "
                         " cover_crop_area, forestry_area, sparse_vegetation_area,"
                         " woody_residue_weight_1, woody_residue_weight_2,"
                         " woody_residue_treatment_1, woody_residue_treatment_2,"
                         " green_residue_weight_1, green_residue_weight_2,"
                         " green_residue_treatment_1, green_residue_treatment_2) "
                       " VALUES ";

    queryOutput += "('" + id + "'"
                + ",'" + inputData.cropFieldManagement.cropName + "'"
                + "," + QString::number(inputData.cropFieldManagement.treeDensity)
                + "," + QString::number(inputData.cropFieldManagement.deadTreeDensity)
                + "," + QString::number(inputData.cropFieldManagement.orchardAge)
                + "," + QString::number(inputData.cropFieldManagement.treeDBH)
                + "," + QString::number(inputData.cropFieldManagement.treeHeight)
                + ",'" + inputData.cropFieldManagement.isOrganic + "'"
                + "," + QString::number(inputData.cropFieldManagement.yield)
                + "," + QString::number(inputData.cropFieldManagement.noTillage)
                + "," + QString::number(inputData.cropFieldManagement.minTillage)
                + "," + QString::number(inputData.cropFieldManagement.coverCrop)
                + "," + QString::number(inputData.cropFieldManagement.forest)
                + "," + QString::number(inputData.cropFieldManagement.sparseVegetation)
                + "," + QString::number(inputData.cropFieldManagement.woodyResidueWeight[0])
                + "," + QString::number(inputData.cropFieldManagement.woodyResidueWeight[1])
                + ",'" + inputData.cropFieldManagement.woodyResidueTreatment[0] + "'"
                + ",'" + inputData.cropFieldManagement.woodyResidueTreatment[1] + "'"
                + "," + QString::number(inputData.cropFieldManagement.greenResidueWeight[0])
                + "," + QString::number(inputData.cropFieldManagement.greenResidueWeight[1])
                + ",'" + inputData.cropFieldManagement.greenResidueTreatment[0] + "'"
                + ",'" + inputData.cropFieldManagement.greenResidueTreatment[1] + "'"
                + ")";

    QSqlQuery myQuery = dbOutput.exec(queryOutput);
    if (myQuery.lastError().isValid())
    {
        std::cout << "Error in saving table " + tableName.toStdString() + ": " << myQuery.lastError().text().toStdString();
        return false;
    }

    return true;
}

bool createTableAgronomicInputs(QSqlDatabase &dbOutput)
{
    QString queryString = "DROP TABLE agronomic_inputs";
    QSqlQuery myQuery = dbOutput.exec(queryString);

    queryString = "CREATE TABLE agronomic_inputs"
                  " (id TEXT,pesticide_weight REAL,"
                  "fertilizer_name_1 TEXT,fertilizer_name_2 TEXT,"
                  "fertilizer_name_3 TEXT,fertilizer_name_4 TEXT, "
                  "fertilizer_rate_1 REAL,fertilizer_rate_2 REAL,"
                  "fertilizer_rate_3 REAL,fertilizer_rate_4 REAL, "
                  "fertilizer_application_method_1 TEXT,fertilizer_application_method_2 TEXT,"
                  "fertilizer_application_method_3 TEXT,fertilizer_application_method_4 TEXT, "
                  "fertilizer_inhibitor_1 TEXT,fertilizer_inhibitor_2 TEXT,"
                  "fertilizer_inhibitor_3 TEXT,fertilizer_inhibitor_4 TEXT,"
                  "amendment_name_1 TEXT,amendment_name_2 TEXT,"
                  "amendment_name_3 TEXT,amendment_name_4 TEXT, "
                  "amendment_rate_1 REAL,amendment_rate_2 REAL,"
                  "amendment_rate_3 REAL,amendment_rate_4 REAL, "
                  "amendment_application_method_1 TEXT,amendment_application_method_2 TEXT,"
                  "amendment_application_method_3 TEXT,amendment_application_method_4 TEXT, "
                  "amendment_inhibitor_1 TEXT,amendment_inhibitor_2 TEXT,"
                  "amendment_inhibitor_3 TEXT,amendment_inhibitor_4 TEXT"
                  ")";

    myQuery = dbOutput.exec(queryString);

    if (myQuery.lastError().isValid())
    {
        std::cout << "Error in creating table 'agronomic_inputs': " << myQuery.lastError().text().toStdString();
        return false;
    }

    return true;
}


bool saveTableAgronomicInputs(QString id, QSqlDatabase &dbOutput, TinputData &inputData, QString tableName)
{

    QString queryOutput = "INSERT INTO " + tableName
                       + " (id,pesticide_weight,"
                         "fertilizer_name_1,fertilizer_name_2,"
                         "fertilizer_name_3,fertilizer_name_4, "
                         "fertilizer_rate_1,fertilizer_rate_2,"
                         "fertilizer_rate_3,fertilizer_rate_4, "
                         "fertilizer_application_method_1,fertilizer_application_method_2,"
                         "fertilizer_application_method_3,fertilizer_application_method_4, "
                         "fertilizer_inhibitor_1,fertilizer_inhibitor_2,"
                         "fertilizer_inhibitor_3,fertilizer_inhibitor_4,"
                         "amendment_name_1,amendment_name_2,"
                         "amendment_name_3,amendment_name_4, "
                         "amendment_rate_1,amendment_rate_2,"
                         "amendment_rate_3,amendment_rate_4, "
                         "amendment_application_method_1,amendment_application_method_2,"
                         "amendment_application_method_3,amendment_application_method_4,"
                         "amendment_inhibitor_1,amendment_inhibitor_2,"
                         "amendment_inhibitor_3,amendment_inhibitor_4) "
                       " VALUES ";

    queryOutput += "('" + id + "'"
                + "," + QString::number(inputData.agronomicInput.pesticideWeight)
                + ",'" + inputData.agronomicInput.fertilizerName[0] + "'"
                + ",'" + inputData.agronomicInput.fertilizerName[1] + "'"
                + ",'" + inputData.agronomicInput.fertilizerName[2] + "'"
                + ",'" + inputData.agronomicInput.fertilizerName[3] + "'"
                + "," + QString::number(inputData.agronomicInput.fertilizerAmount[0])
                + "," + QString::number(inputData.agronomicInput.fertilizerAmount[1])
                + "," + QString::number(inputData.agronomicInput.fertilizerAmount[2])
                + "," + QString::number(inputData.agronomicInput.fertilizerAmount[3])
                + ",'" + inputData.agronomicInput.fertilizerApplicationMethod[0] + "'"
                + ",'" + inputData.agronomicInput.fertilizerApplicationMethod[1] + "'"
                + ",'" + inputData.agronomicInput.fertilizerApplicationMethod[2] + "'"
                + ",'" + inputData.agronomicInput.fertilizerApplicationMethod[3] + "'"
                + ",'" + inputData.agronomicInput.fertilizerInhibitor[0] + "'"
                + ",'" + inputData.agronomicInput.fertilizerInhibitor[1] + "'"
                + ",'" + inputData.agronomicInput.fertilizerInhibitor[2] + "'"
                + ",'" + inputData.agronomicInput.fertilizerInhibitor[3] + "'"
                + ",'" + inputData.agronomicInput.amendmentName[0] + "'"
                + ",'" + inputData.agronomicInput.amendmentName[1] + "'"
                + ",'" + inputData.agronomicInput.amendmentName[2] + "'"
                + ",'" + inputData.agronomicInput.amendmentName[3] + "'"
                + "," + QString::number(inputData.agronomicInput.amendmentAmount[0])
                + "," + QString::number(inputData.agronomicInput.amendmentAmount[1])
                + "," + QString::number(inputData.agronomicInput.amendmentAmount[2])
                + "," + QString::number(inputData.agronomicInput.amendmentAmount[3])
                + ",'" + inputData.agronomicInput.amendmentApplicationMethod[0] + "'"
                + ",'" + inputData.agronomicInput.amendmentApplicationMethod[1] + "'"
                + ",'" + inputData.agronomicInput.amendmentApplicationMethod[2] + "'"
                + ",'" + inputData.agronomicInput.amendmentApplicationMethod[3] + "'"
                + ",'" + inputData.agronomicInput.amendmentInhibitor[0] + "'"
                + ",'" + inputData.agronomicInput.amendmentInhibitor[1] + "'"
                + ",'" + inputData.agronomicInput.amendmentInhibitor[2] + "'"
                + ",'" + inputData.agronomicInput.amendmentInhibitor[3] + "'"
                + ")";

    QSqlQuery myQuery = dbOutput.exec(queryOutput);
    if (myQuery.lastError().isValid())
    {
        std::cout << "Error in saving table " + tableName.toStdString() + ": " << myQuery.lastError().text().toStdString();
        return false;
    }

    return true;
}


bool createTableEnergy(QSqlDatabase &dbOutput)
{
    QString queryString = "DROP TABLE energy";
    QSqlQuery myQuery = dbOutput.exec(queryString);

    queryString = "CREATE TABLE energy"
                  " (id TEXT,biodiesel REAL,"
                  "bioethanol REAL,diesel REAL,"
                  "oil REAL,petrol REAL, "
                  "LPG REAL,coal REAL,"
                  "high_density_biomass REAL,wood REAL, "
                  "methane REAL,electricity_grid REAL,"
                  "percentage_renewables_provider REAL,hydropower_electricity REAL, "
                  "photovoltaic_electricity REAL,eolic_electricity REAL"
                  ")";

    myQuery = dbOutput.exec(queryString);

    if (myQuery.lastError().isValid())
    {
        std::cout << "Error in creating table 'energy': " << myQuery.lastError().text().toStdString();
        return false;
    }

    return true;
}


bool saveTableEnergy(QString id, QSqlDatabase &dbOutput, TinputData &inputData, QString tableName)
{
    QString queryOutput = "INSERT INTO " + tableName
                       + " (id,biodiesel,"
                         "bioethanol,diesel,"
                         "oil,petrol, "
                         "LPG,coal,"
                         "high_density_biomass,wood, "
                         "methane,electricity_grid,"
                         "percentage_renewables_provider,hydropower_electricity, "
                         "photovoltaic_electricity,eolic_electricity) "
                       " VALUES ";

    queryOutput += "('" + id + "'"
                + "," + QString::number(inputData.energy.biodiesel)
                + "," + QString::number(inputData.energy.bioethanol)
                + "," + QString::number(inputData.energy.diesel)
                + "," + QString::number(inputData.energy.oil)
                + "," + QString::number(inputData.energy.petrol)
                + "," + QString::number(inputData.energy.LPG)
                + "," + QString::number(inputData.energy.coal)
                + "," + QString::number(inputData.energy.highEnergyDensityBiomass)
                + "," + QString::number(inputData.energy.wood)
                + "," + QString::number(inputData.energy.methane)
                + "," + QString::number(inputData.energy.electricityGridAmount)
                + "," + QString::number(inputData.energy.electricityGridPercentageRenewables)
                + "," + QString::number(inputData.energy.electricityHydro)
                + "," + QString::number(inputData.energy.electricitySolar)
                + "," + QString::number(inputData.energy.electricityEolic)
                + ")";

    QSqlQuery myQuery = dbOutput.exec(queryOutput);
    if (myQuery.lastError().isValid())
    {
        std::cout << "Error in saving table " + tableName.toStdString() + ": " << myQuery.lastError().text().toStdString();
        return false;
    }

    return true;
}
