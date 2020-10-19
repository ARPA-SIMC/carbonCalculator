
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
        renewablesPercentage = 24.;
    }
    query.clear();

    calculator.energy.country = idCountry;
    calculator.energy.percentageRenewablesInGrid = renewablesPercentage;

    //check
    if (calculator.energy.percentageRenewablesInGrid == NODATA)
    {
        calculator.energy.percentageRenewablesInGrid = 24 + (year-2016);
    }

    if (calculator.energy.percentageRenewablesInGrid > 100)
    {
        calculator.energy.percentageRenewablesInGrid = 100;
    }

    return true;
}


bool readFertilizer(QString idFertiliser, QSqlDatabase &db, CarbonCalculator &calculator, QString &error)
{
    QString queryString = "SELECT * FROM fertiliser WHERE name='" + idFertiliser + "'";
    QSqlQuery query = db.exec(queryString);
    query.last();
    if (! query.isValid())
    {
        error = query.lastError().text();
        return false;
    }

    double value;
    if (! getValue(query.value("bouwman_n2o"), &value))
    {
        error = "missing emission of Bouwman index for N2O";
        return false;
    }
    calculator.fertiliser.fertInput.bouwmanN2O = value;

    if (! getValue(query.value("bouwman_no"), &value))
    {
        error = "missing emission of Bouwman index for NO";
        return false;
    }
    calculator.fertiliser.fertInput.bouwmanNO = value;

    if (! getValue(query.value("bouwman_nh3"), &value))
    {
        error = "missing emission of Bouwman index for NH3";
        return false;
    }
    calculator.fertiliser.fertInput.bouwmanNH3 = value;

    if (! getValue(query.value("N"), &value))
    {
        error = "missing emission of nitrogen content";
        return false;
    }
    calculator.fertiliser.fertInput.contentElement.nitrogen = value;

    if (! getValue(query.value("P"), &value))
    {
        error = "missing emission of phosphorus content";
        return false;
    }
    calculator.fertiliser.fertInput.contentElement.phosphorus = value;

    if (! getValue(query.value("K"), &value))
    {
        error = "missing emission of potassium content";
        return false;
    }
    calculator.fertiliser.fertInput.contentElement.potassium = value;

    if (! getValue(query.value("C"), &value))
    {
        error = "missing emission of carbon content";
        return false;
    }
    calculator.fertiliser.fertInput.contentElement.carbon = value;

    if (! getValue(query.value("current_tech"), &value))
    {
        error = "missing emission per kg of product";
        return false;
    }
    calculator.fertiliser.fertInput.emissionPerKgOfProduct = value;

    query.clear();
    return true;
}


bool readResidue(QString idResidue, QSqlDatabase &db, CarbonCalculator &calculator, QString &error)
{
    QString queryString = "SELECT * FROM residue_treatment WHERE id_treatment_residue='" + idResidue + "'";
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
    calculator.cropResidue.cropResidueParameter.emissionCH4 = value;

    if (! getValue(query.value("emission_n2o"), &value))
    {
        error = "Error: missing emission of N2O data";
        return false;
    }
    calculator.cropResidue.cropResidueParameter.emissionN2O = value;

    if (! getValue(query.value("dry_matter_to_co2"), &value))
    {
        error =  "missing emission of dry matter to CO2 data";
        return false;
    }
    calculator.cropResidue.cropResidueParameter.residueReconvertedToCO2 = value;

    query.clear();
    return true;
}

bool readCropParameters(QString idCrop, QSqlDatabase &db, CarbonCalculator &calculator, QString &error)
{
    double value;
    QString queryString = "SELECT * FROM crop_parameters WHERE id_fine_classification='" + idCrop + "'";
    QSqlQuery query = db.exec(queryString);
    query.last();
    if (! query.isValid())
    {
        error = query.lastError().text();
        return false;
    }

    if (! getValue(query.value("drymatter_fraction_harvested"), &value))
    {
        error =  "missing emission of dry matter fraction data";
        return false;
    }
    calculator.cropResidue.cropResidueParameter.dryMatterFraction  = value;

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


    QString stringValue;
    if (! getValue(query.value("bouwman_equivalent"), &stringValue))
    {
        error = "Error: missing emission of Bouwman equivalent macrotype ratio data";
        return false;
    }
    if (stringValue == "grass" )
        calculator.bouwmanEquivalentTag = 0;
    else if (stringValue == "grass_clover")
        calculator.bouwmanEquivalentTag = 1;
    else if (stringValue == "legume")
        calculator.bouwmanEquivalentTag = 2;
    else if (stringValue == "other_crop")
        calculator.bouwmanEquivalentTag = 3;
    else if (stringValue == "wetland_rice")
        calculator.bouwmanEquivalentTag = 4;
    else
    {
        error = "Error: wrong tag for crop";
        return false;
    }
    //calculator.fertiliser.bouwmanParameterN2O.cropType = calculator.bouwmanTableN2O[calculator.bouwmanEquivalentTag];
    //calculator.fertiliser.bouwmanParameterNO.cropType = calculator.bouwmanTableNO[calculator.bouwmanEquivalentTag];
    //calculator.fertiliser.bouwmanParameterNH4.cropType = calculator.bouwmanTableNH4[calculator.bouwmanEquivalentTag];


    query.clear();

    return true;
}


bool readBouwmanNH4(QString idFeature, QSqlDatabase &db, CarbonCalculator &calculator, QString &error)
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

    if (! getValue(query.value(idFeature), &value))
    {
        error = "missing emission of above/below ratio data";
        return false;
    }
    calculator.fertiliser.bouwmanParameterNH4.applicationMethod = value;


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
    /*if (! getValue(query.value("climate_code"), valueInt))
    {
        error = "Error: missing climate data";
        return false;
    }*/
    //calculator.fertiliser.bouwmanParameterN2O.climate = calculator.bouwmanTableN2O.climate[valueInt-1];
    //calculator.fertiliser.bouwmanParameterNO.climate = calculator.bouwmanTableNO.climate[valueInt-1];
    //calculator.fertiliser.bouwmanParameterNH4.climate = calculator.bouwmanTableNH4.climate[valueInt-1];

    query.clear();
    return true;
}
