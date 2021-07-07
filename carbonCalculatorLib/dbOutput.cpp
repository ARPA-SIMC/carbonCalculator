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
    if (! createTableCarbonBudget(dbOutput))
        return false;
    if (! createTableCarbonDynamics(dbOutput))
        return false;
    if (! createTableBuyer(dbOutput))
        return false;

    // TODO other tables

    return true;
}

bool saveOutput(QString id, QSqlDatabase &dbOutput, TinputData &inputData, CarbonCalculator calculatorCO2, double credits, int*isAccepted)
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
    if (! saveTableCarbonBudget(id, dbOutput,calculatorCO2 , "carbon_budget",1.*credits/inputData.general.fieldSize,credits,isAccepted))
        return false;
    if (! saveTableCarbonDynamics(id, dbOutput,calculatorCO2 , "carbon_dynamics"))
        return false;
    // TODO save other tables

    return true;
}

bool saveOutputBuyer(QString id, QSqlDatabase &dbOutput, TinputDataBuyer &inputData, BuyerCalculator buyerCalculatorCO2, double debts)
{
    if (! saveTableBuyer(id, dbOutput,buyerCalculatorCO2,inputData , "buyer",debts))
        return false;

    // TODO save other tables

    return true;

}

bool createTableGeneral(QSqlDatabase &dbOutput)
{
    QString queryString = "DROP TABLE general";
    QSqlQuery myQuery = dbOutput.exec(queryString);

    queryString = "CREATE TABLE general "
                  " (id TEXT, enterprise_name TEXT, email_address TEXT, vat_number TEXT, fiscal_code TEXT,"
                  " project_manager_name TEXT, project_manager_surname TEXT,"
                  " compiler_name TEXT, compiler_email TEXT,"
                  " nr_field INTEGER, year INTEGER, country TEXT,region TEXT, description TEXT, "
                  " latitude REAL, longitude REAL, field_size REAL, field_slope REAL )";

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
                       + " (id, enterprise_name, email_address, vat_number, fiscal_code, project_manager_name, project_manager_surname,"
                         " compiler_name, compiler_email, nr_field, year, country, region, description, latitude, longitude, field_size, field_slope) "
                       " VALUES ";

    queryOutput += "('" + id + "'"
                + ",'" + inputData.general.enterpriseName + "'"
                + ",'" + inputData.general.emailAddress + "'"
                + ",'" + inputData.general.vatNumber + "'"
                + ",'" + inputData.general.fiscalCode + "'"
                + ",'" + inputData.general.projectManagerName + "'"
                + ",'" + inputData.general.projectManagerSurname + "'"
                + ",'" + inputData.general.compilerName + "'"
                + ",'" + inputData.general.compilerEmail + "'"
                + "," + QString::number(inputData.general.nrField)
                + "," + QString::number(inputData.general.year)
                + ",'" + inputData.general.idCountry + "'"
                + ",'" + inputData.general.idRegion + "'"
                + ",'" + inputData.general.description + "'"
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
                  " no_tillage_area REAL, minimum_tillage_area REAL, conventional_tillage_area REAL, "
                  " cover_crop_area REAL, grass_area REAL, forestry_area REAL, sparse_vegetation_area REAL,"
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
                         " no_tillage_area, minimum_tillage_area, conventional_tillage_area,"
                         " cover_crop_area, grass_area, forestry_area, sparse_vegetation_area,"
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
                + "," + QString::number(inputData.cropFieldManagement.conventionalTillage)
                + "," + QString::number(inputData.cropFieldManagement.coverCrop)
                + "," + QString::number(inputData.cropFieldManagement.permanentGrass)
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

bool createTableCarbonBudget(QSqlDatabase &dbOutput)
{
    QString queryString = "DROP TABLE carbon_budget";
    QSqlQuery myQuery = dbOutput.exec(queryString);

    queryString = "CREATE TABLE carbon_budget"
                  " (id TEXT,soil_carbon_budget_hectare REAL,"
                  "soil_carbon_budget_field REAL,"
                  "soil_carbon_saved_due_to_sustainable_practices_hectare REAL,soil_carbon_saved_due_to_sustainable_practices_field REAL,"
                  "soil_carbon_footprint REAL,"
                  "biomass_carbon_budget_hectare REAL,biomass_carbon_budget_field REAL, "
                  "biomass_carbon_hectare REAL,biomass_carbon_field REAL,"
                  "biomass_carbon_footprint REAL,"
                  "nr_sustainable_practices INTEGER,"
                  "isAccepted INTEGER, credits_per_hectare REAL,"
                  "credits REAL"
                  ")";

    myQuery = dbOutput.exec(queryString);

    if (myQuery.lastError().isValid())
    {
        std::cout << "Error in creating table 'carbon_budget': " << myQuery.lastError().text().toStdString();
        return false;
    }

    return true;
}


bool saveTableCarbonBudget(QString id, QSqlDatabase &dbOutput, CarbonCalculator calculatorCO2, QString tableName, double creditsPerHectare, double credits, int* isAccepted)
{
    QString queryOutput = "INSERT INTO " + tableName
                       + " (id,soil_carbon_budget_hectare,"
                         "soil_carbon_budget_field,"
                         "soil_carbon_saved_due_to_sustainable_practices_hectare,soil_carbon_saved_due_to_sustainable_practices_field,"
                         "soil_carbon_footprint,"
                         "biomass_carbon_budget_hectare,biomass_carbon_budget_field, "
                         "biomass_carbon_hectare,biomass_carbon_field,"
                         "biomass_carbon_footprint,nr_sustainable_practices,"
                         "isAccepted,credits_per_hectare,"
                         "credits) "
                       " VALUES ";

    queryOutput += "('" + id + "'"
                + "," + QString::number(calculatorCO2.carbonBudgetPerHectareSoil)
                + "," + QString::number(calculatorCO2.carbonBudgetWholeFieldSoil)
                + "," + QString::number(calculatorCO2.carbonSavedBySustainablePractices)
                + "," + QString::number(calculatorCO2.carbonSavedBySustainablePracticesWholeField)
                + "," + QString::number(calculatorCO2.carbonFootprintPerKgOfProduceSoil)
                + "," + QString::number(calculatorCO2.carbonBudgetPerHectareBiomass)
                + "," + QString::number(calculatorCO2.carbonBudgetWholeFieldBiomass)
                + "," + QString::number(calculatorCO2.carbonBiomass)
                + "," + QString::number(calculatorCO2.carbonBiomassWholeField)
                + "," + QString::number(calculatorCO2.carbonFootprintPerKgOfProduceBiomass)
                + "," + QString::number(calculatorCO2.nrConservativePracticesAdopted)
                + "," + QString::number(*isAccepted)
                + "," + QString::number(creditsPerHectare)
                + "," + QString::number(credits)
                + ")";

    QSqlQuery myQuery = dbOutput.exec(queryOutput);
    if (myQuery.lastError().isValid())
    {
        std::cout << "Error in saving table " + tableName.toStdString() + ": " << myQuery.lastError().text().toStdString();
        return false;
    }

    return true;
}

bool createTableCarbonDynamics(QSqlDatabase &dbOutput)
{
    QString queryString = "DROP TABLE carbon_dynamics";
    QSqlQuery myQuery = dbOutput.exec(queryString);

    queryString = "CREATE TABLE carbon_dynamics"
                  " (id TEXT,energy_emission REAL,"
                  "pesticide_production_emission REAL,"
                  "residue_management_emission REAL,nitrogen_compounds_emission REAL,"
                  "carbon_oxidation_emission REAL,"
                  "fertilizer_production_emission REAL,fertilizer_application_emission REAL, "
                  "carbon_lost_erosion REAL,conservative_tillage_sequestration REAL,"
                  "crop_cover_sequestration REAL, conservative_landuse_sequestration REAL,"
                  "amendment_application_sequestration REAL,woody_residue_sequestration REAL,"
                  "green_residue_sequestration REAL, fine_root_sequestration REAL"
                  ")";

    myQuery = dbOutput.exec(queryString);

    if (myQuery.lastError().isValid())
    {
        std::cout << "Error in creating table 'carbon_dynamics': " << myQuery.lastError().text().toStdString();
        return false;
    }

    return true;
}


bool saveTableCarbonDynamics(QString id, QSqlDatabase &dbOutput, CarbonCalculator calculatorCO2, QString tableName)
{
    QString queryOutput = "INSERT INTO " + tableName
                       + " (id,energy_emission,"
                         "pesticide_production_emission,"
                         "residue_management_emission,nitrogen_compounds_emission,"
                         "carbon_oxidation_emission,"
                         "fertilizer_production_emission,fertilizer_application_emission, "
                         "carbon_lost_erosion,conservative_tillage_sequestration,"
                         "crop_cover_sequestration, conservative_landuse_sequestration,"
                         "amendment_application_sequestration,woody_residue_sequestration,"
                         "green_residue_sequestration, fine_root_sequestration) "
                       " VALUES ";

    queryOutput += "('" + id + "'"
                + "," + QString::number(calculatorCO2.energy.emissions.total)
                + "," + QString::number(calculatorCO2.pesticide.emissionDueToProduction)
                + "," + QString::number(calculatorCO2.cropResidue.kgCO2Equivalent.total)
                + "," + QString::number(calculatorCO2.fertiliser.emissionDueToSoil)
                + "," + QString::number(calculatorCO2.emissionsDueToOxidation)
                + "," + QString::number(calculatorCO2.fertiliser.emissionDueToFertiliserProduction)
                + "," + QString::number(calculatorCO2.fertiliser.emissionDueToFertiliserApplication)
                + "," + QString::number(calculatorCO2.erosion.lostCO2)
                + "," + QString::number(calculatorCO2.soilManage.sequestrationCarbonCO2EqTillage)
                + "," + QString::number(calculatorCO2.soilManage.sequestrationCarbonCO2EqCropCover)
                + "," + QString::number(calculatorCO2.soilManage.sequestrationCarbonCO2EqLandUse)
                + "," + QString::number(calculatorCO2.soilManage.sequestrationAmendment)
                + "," + QString::number(calculatorCO2.soilManage.sequestrationCarbonCO2EqResidueWood)
                + "," + QString::number(calculatorCO2.soilManage.sequestrationCarbonCO2EqResidueGreen)
                + "," + QString::number(calculatorCO2.soilManage.sequestrationRoot)
                + ")";

    QSqlQuery myQuery = dbOutput.exec(queryOutput);
    if (myQuery.lastError().isValid())
    {
        std::cout << "Error in saving table " + tableName.toStdString() + ": " << myQuery.lastError().text().toStdString();
        return false;
    }

    return true;
}


bool createTableBuyer(QSqlDatabase &dbOutput)
{
    QString queryString = "DROP TABLE buyer";
    QSqlQuery myQuery = dbOutput.exec(queryString);

    queryString = "CREATE TABLE buyer"
                  " (id_buyer TEXT, enterprise_name TEXT, email_address TEXT, vat_number TEXT, fiscal_code TEXT,"
                  " project_manager_name TEXT, project_manager_surname TEXT,"
                  " compiler_name TEXT, compiler_email TEXT,"
                  " name_chain_production TEXT, name_of_product TEXT, year INTEGER, "
                  " country TEXT, region TEXT, description TEXT, latitude REAL, longitude REAL, "
                  "is_LCA_present INTEGER,life_cycle_assessment REAL,"
                  "biodiesel REAL,"
                  "bioethanol REAL,diesel REAL,"
                  "oil REAL,petrol REAL, "
                  "LPG REAL,coal REAL,"
                  "high_density_biomass REAL,wood REAL, "
                  "methane REAL,electricity_grid REAL,"
                  "percentage_renewables_provider REAL,hydropower_electricity REAL, "
                  "photovoltaic_electricity REAL,eolic_electricity REAL,"
                  "total_energetic_emission REAL, debts REAL)";

    myQuery = dbOutput.exec(queryString);

    if (myQuery.lastError().isValid())
    {
        std::cout << "Error in creating table 'buyer': " << myQuery.lastError().text().toStdString();
        return false;
    }

    return true;
}


bool saveTableBuyer(QString id_buyer, QSqlDatabase &dbOutput, BuyerCalculator buyerCalculatorCO2, TinputDataBuyer &inputData, QString tableName,double debts)
{
    QString queryOutput = "INSERT INTO " + tableName
                       + " (id_buyer, enterprise_name, email_address, vat_number, fiscal_code,"
                         "project_manager_name, project_manager_surname,"
                         "compiler_name, compiler_email, name_chain_production,name_of_product, year,"
                         "country, region, description, latitude, longitude, "
                         "is_LCA_present,life_cycle_assessment,"
                         "biodiesel,"
                         "bioethanol,diesel,"
                         "oil,petrol, "
                         "LPG,coal,"
                         "high_density_biomass,wood,"
                         "methane,electricity_grid,"
                         "percentage_renewables_provider,hydropower_electricity,"
                         "photovoltaic_electricity,eolic_electricity,"
                         "total_energetic_emission, debts) "
                       " VALUES ";


    queryOutput += "('" + id_buyer + "'"
                + ",'" + inputData.generalBuyer.enterpriseName + "'"
                + ",'" + inputData.generalBuyer.emailAddress + "'"
                + ",'" + inputData.generalBuyer.vatNumber + "'"
                + ",'" + inputData.generalBuyer.fiscalCode + "'"
                + ",'" + inputData.generalBuyer.projectManagerName + "'"
                + ",'" + inputData.generalBuyer.projectManagerSurname + "'"
                + ",'" + inputData.generalBuyer.compilerName + "'"
                + ",'" + inputData.generalBuyer.compilerEmail + "'"
                + "," + "'" + inputData.generalBuyer.chainName   + "'"
                + "," + "'" + inputData.generalBuyer.productName   + "'"
                + "," + QString::number(inputData.generalBuyer.year)
                + "," + "'" + inputData.generalBuyer.idCountry + "'"
                + "," + "'" + inputData.generalBuyer.idRegion + "'"
                + "," + "'" + inputData.generalBuyer.description   + "'"
                + "," + QString::number(inputData.generalBuyer.latitude)
                + "," + QString::number(inputData.generalBuyer.longitude)
                + "," + QString::number(inputData.generalBuyer.isPresentLCA)
                + "," + QString::number(inputData.generalBuyer.valueLCA)
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
                + "," + QString::number(buyerCalculatorCO2.energy.emissions.total)
                + "," + QString::number(debts)
                + ")";

    QSqlQuery myQuery = dbOutput.exec(queryOutput);
    if (myQuery.lastError().isValid())
    {
        std::cout << "Error in saving table " + tableName.toStdString() + ": " << myQuery.lastError().text().toStdString();
        return false;
    }

    return true;
}
