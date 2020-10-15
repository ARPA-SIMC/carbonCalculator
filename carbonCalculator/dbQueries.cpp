
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
    else if (calculator.energy.percentageRenewablesInGrid > 100)
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

