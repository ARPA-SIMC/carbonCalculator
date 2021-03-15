
#include "dbUtilities.h"
#include "dbQueries.h"
#include "carbonCalculator.h"
#include <QtSql>


bool readRenewables(QString idCountry, int year, QSqlDatabase &db, CarbonCalculator &calculator, QString &error)
{
    QString queryString = "SELECT * FROM percentage_renewables_land WHERE id_country='" + idCountry + "'";
    QSqlQuery query = db.exec(queryString);
    query.last();
    if (! query.isValid())
    {
        error = query.lastError().text();
        return false;
    }

    double renewablesPercentage;
    if (! getValue(query.value("percentage"), &renewablesPercentage))
    {
        //error = "missing renewables percentage data";
        //return false;
        //renewablesPercentage = 24.;
    }
    query.clear();

    calculator.energy.country = idCountry;
    calculator.energy.percentageRenewablesInGrid = renewablesPercentage;// + (year-2016);

    //check
    if (calculator.energy.percentageRenewablesInGrid == NODATA)
    {
        calculator.energy.percentageRenewablesInGrid = 24 + (year-2016);
    }
    else
    {
        calculator.energy.percentageRenewablesInGrid += (year-2016);
    }

    if (calculator.energy.percentageRenewablesInGrid > 100)
    {
        calculator.energy.percentageRenewablesInGrid = 100;
    }
    calculator.pesticide.renewablesInCountry = calculator.energy.percentageRenewablesInGrid;
    return true;
}


bool readFertilizer(QString* idFertiliser, QSqlDatabase &db, CarbonCalculator &calculator, QString &error, int nrFertiliserApplied)
{
    for (int i=0 ; i<nrFertiliserApplied; i++)
    {
        if (idFertiliser[i] != "None")
        {
            QString queryString = "SELECT * FROM fertiliser WHERE name='" + idFertiliser[i] + "'";
            QSqlQuery query = db.exec(queryString);
            query.last();
            if (! query.isValid())
            {
                error = query.lastError().text();
                return false;
            }

            double value;

            if (! getValue(query.value("dry_matter"), &value))
            {
                error = "missing dry_matter fraction";
                return false;
            }
            calculator.fertiliser.drymatter[i] = value;

            if (! getValue(query.value("bouwman_n2o"), &value))
            {
                error = "missing emission of Bouwman index for N2O";
                return false;
            }
            calculator.fertiliser.fertInput[i].bouwmanN2O = value;

            if (! getValue(query.value("bouwman_no"), &value))
            {
                error = "missing emission of Bouwman index for NO";
                return false;
            }
            calculator.fertiliser.fertInput[i].bouwmanNO = value;

            if (! getValue(query.value("bouwman_nh3"), &value))
            {
                error = "missing emission of Bouwman index for NH3";
                return false;
            }
            calculator.fertiliser.fertInput[i].bouwmanNH3 = value;

            if (! getValue(query.value("N"), &value))
            {
                error = "missing emission of nitrogen content";
                return false;
            }
            calculator.fertiliser.fertInput[i].contentElement.nitrogen = value;

            if (! getValue(query.value("P"), &value))
            {
                error = "missing emission of phosphorus content";
                return false;
            }
            calculator.fertiliser.fertInput[i].contentElement.phosphorus = value;

            if (! getValue(query.value("K"), &value))
            {
                error = "missing emission of potassium content";
                return false;
            }
            calculator.fertiliser.fertInput[i].contentElement.potassium = value;

            if (! getValue(query.value("C"), &value))
            {
                error = "missing emission of carbon content";
                return false;
            }
            calculator.fertiliser.fertInput[i].contentElement.carbon = value;

            if (! getValue(query.value("current_tech"), &value))
            {
                error = "missing emission per kg of product";
                return false;
            }
            calculator.fertiliser.fertInput[i].emissionPerKgOfProduct = value;


            if (! getValue(query.value("incremental_index_som"), &value))
            {
                error = "missing incremental factor";
                return false;
            }
            calculator.fertiliser.incrementalParameter[i] = value;

            if (! getValue(query.value("recalcitrant_carbon_index"), &value))
            {
                error = "missing recalcitrant carbon index";
                return false;
            }
            calculator.fertiliser.recalcitrantCarbonIndex[i] = value;

            if (! getValue(query.value("volatilization_fraction"), &value))
            {
                error = "missing volatilization fraction";
                return false;
            }
            calculator.fertiliser.volatilizationFraction[i] = value;



            query.clear();
        }
        else
        {
            int value=0;
            calculator.fertiliser.fertInput[i].bouwmanN2O = value;
            calculator.fertiliser.fertInput[i].bouwmanNO = value;
            calculator.fertiliser.fertInput[i].bouwmanNH3 = value;
            calculator.fertiliser.fertInput[i].contentElement.nitrogen = value;
            calculator.fertiliser.fertInput[i].contentElement.phosphorus = value;
            calculator.fertiliser.fertInput[i].contentElement.potassium = value;
            calculator.fertiliser.fertInput[i].contentElement.carbon = value;
            calculator.fertiliser.fertInput[i].emissionPerKgOfProduct = value;
            calculator.fertiliser.incrementalParameter[i] = value;
            calculator.fertiliser.recalcitrantCarbonIndex[i] = value;
            calculator.fertiliser.drymatter[i] = 100.;
        }
    }
    return true;
}


bool readResidue(QString* idResidue, QSqlDatabase &db, CarbonCalculator &calculator, QString &error)
{
    for (int i=0;i<4;i++)
    {
        QString queryString = "SELECT * FROM residue_treatment WHERE id_treatment_residue='" + idResidue[i] + "'";
        QSqlQuery query = db.exec(queryString);
        query.last();
        if (! query.isValid())
        {
            error = query.lastError().text();
            return false;
        }
        double value;

        if (! getValue(query.value("emission_methane"), &value))
        {
            error =  "missing emission of Methane data";
            return false;
        }
        calculator.cropResidue.cropResidueParameter.emissionCH4[i] = value;

        if (! getValue(query.value("emission_n2o"), &value))
        {
            error = "Error: missing emission of N2O data";
            return false;
        }
        calculator.cropResidue.cropResidueParameter.emissionN2O[i] = value;

        if (! getValue(query.value("dry_matter_to_co2"), &value))
        {
            error =  "missing emission of dry matter to CO2 data";
            return false;
        }
        calculator.cropResidue.cropResidueParameter.residueReconvertedToCO2[i] = value;

        query.clear();
    }
    return true;
}

bool readCropParameters(QString idCrop, QSqlDatabase &db, CarbonCalculator &calculator, QString &error)
{
    double value;
    QString valueString;
    QString queryString = "SELECT * FROM crop_parameters WHERE id_fine_classification='" + idCrop + "'";
    QSqlQuery query = db.exec(queryString);
    query.last();
    if (! query.isValid())
    {
        error = query.lastError().text();
        return false;
    }

    if (! getValue(query.value("gross_classification"), &valueString))
    {
        error =  "missing emission of dry matter fraction data";
        return false;
    }
    calculator.biomassInTree.isOrchard = 1;
    if (valueString == "grass")
    {
        calculator.soilManage.rootDecayParameter  = 1.27; // The decomposition of fine and coarse roots: their global patterns and controlling factors Xinyue Zhang & Wei Wang NATURE Scientific Reports volume 5, Article number: 9940 (2015)
        calculator.biomassInTree.isOrchard = 0;
    }
    else if (valueString == "shrub")
    {
        calculator.soilManage.rootDecayParameter  = 1.02;
        calculator.biomassInTree.isOrchard = 0;
    }
    else if (valueString == "tree_broadleaf")
    {
        calculator.soilManage.rootDecayParameter  = 0.71;
    }
    else if (valueString == "tree_conifer")
    {
        calculator.soilManage.rootDecayParameter  = 0.41;
    }
    else
    {
        calculator.soilManage.rootDecayParameter  = 1.02;
    }


    if (! getValue(query.value("nitrogen_content_aboveground"), &value))
    {
        error = "Error: missing emission of dry matter fraction data";
        return false;
    }
    calculator.cropResidue.aboveGroundNitrogen = value;

    if (! getValue(query.value("below_above_ratio"), &value))
    {
        error = "Error: missing emission of above/below ratio data";
        return false;
    }
    calculator.cropResidue.cropResidueParameter.belowAboveRatio = value;


    if (! getValue(query.value("growth_diameter"), &value))
    {
        error = "Error: missing diameter growth data";
        return false;
    }
    calculator.biomassInTree.incrementDiameter = value;

    if (! getValue(query.value("growth_height"), &value))
    {
        error = "Error: missing height growth data";
        return false;
    }
    calculator.biomassInTree.incrementHeight = value;

    if (! getValue(query.value("wood_density"), &value))
    {
        error = "Error: missing wood density data";
        return false;
    }
    calculator.biomassInTree.timberDensity = value;

    if (! getValue(query.value("annual_biomass_increment"), &value))
    {
        error = "Error: missing annual biomass increment data";
        return false;
    }
    calculator.biomassInTree.annualCarbonWoodyDryMatter = value/FROM_C_TO_CO2*2;

    if (! getValue(query.value("woody_crop"), &value))
    {
        error = "Error: missing woody crop data";
        return false;
    }
    if (value > 0.001)
        calculator.woodyCrop = true;
    else
        calculator.woodyCrop = false;

    QString stringValue;
    if (! getValue(query.value("bouwman_equivalent"), &stringValue))
    {
        error = "Error: missing emission of Bouwman equivalent macrotype ratio data";
        return false;
    }

    if (stringValue == "grass" )
    {
        calculator.bouwmanEquivalentTag = 0;
        calculator.fertiliser.inhibitorClass = calculator.bouwmanEquivalentTag;
    }
    else if (stringValue == "grass_clover")
        calculator.bouwmanEquivalentTag = 1;
    else if (stringValue == "legume")
        calculator.bouwmanEquivalentTag = 2;
    else if (stringValue == "other_crop")
        calculator.bouwmanEquivalentTag = 3;
    else if (stringValue == "wetland_rice")
        calculator.bouwmanEquivalentTag = 4;
    /*
Grass	1
Grass-clover	2
Legume	3
Wetland rice	5
Other crop	4

     * */

    else
    {
        error = "Error: wrong tag for crop";
        return false;
    }
    calculator.fertiliser.bouwmanParameterN2O.cropType = calculator.bouwmanTableN2O.croptype[calculator.bouwmanEquivalentTag];
    calculator.fertiliser.bouwmanParameterNO.cropType = calculator.bouwmanTableNO.croptype[calculator.bouwmanEquivalentTag];
    calculator.fertiliser.bouwmanParameterNH4.cropType = calculator.bouwmanTableNH4.croptype[calculator.bouwmanEquivalentTag];


    query.clear();

    return true;
}


bool readBouwmanNH4(QString* idFeature, QSqlDatabase &db, CarbonCalculator &calculator, QString &error,int nrFertiliserApplied)
{
    double value;
    QString queryString = "SELECT * FROM bouwman_nh4_soil_emissions WHERE field1='application_method'";
    QSqlQuery query = db.exec(queryString);
    query.last();
    if (! query.isValid())
    {
        error = query.lastError().text();
        return false;
    }
    for (int i=0; i<nrFertiliserApplied;i++)
    {
        if (! getValue(query.value(idFeature[i]), &value))
        {
            error = "missing parameter for application method data";
            return false;
        }
        calculator.fertiliser.bouwmanParameterNH4ApplicationMethod[i] = value;
    }
    query.clear();
    return true;
}

bool readClimate(QString idClimate, QSqlDatabase &db, CarbonCalculator &calculator, QString &error)
{
    double value;
    QString queryString = "SELECT * FROM climate_type WHERE id_climate='" + idClimate + "'";
    QSqlQuery query = db.exec(queryString);
    query.last();
    if (! query.isValid())
    {
        error = query.lastError().text();
        return false;
    }
    int valueInt;
    if (! getValue(query.value("climate_code"), &valueInt))
    {
        error = "Error: missing climate data";
        return false;
    }
    calculator.fertiliser.bouwmanParameterN2O.climate = calculator.bouwmanTableN2O.climate[valueInt-1];
    calculator.fertiliser.bouwmanParameterNO.climate = calculator.bouwmanTableNO.climate[valueInt-1];
    calculator.fertiliser.bouwmanParameterNH4.climate = calculator.bouwmanTableNH4.climate[valueInt-1];
    if (! getValue(query.value("climate_parameter_leaching"), &value))
    {
        error = "Error: missing climate data";
        return false;
    }
    calculator.fertiliser.leachingParameterDueToClimate = value;

    if (! getValue(query.value("climate_tag"), &valueInt))
    {
        error = "Error: missing climate data";
        return false;
    }
    calculator.idClimate = valueInt;

    query.clear();
    return true;
}
