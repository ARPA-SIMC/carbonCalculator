#include "carbonCalculator.h"
#include <math.h>
#include <stdio.h>


void cropResidueManagement::setInput(double emissionCH4, double emissionN2O, double dryMatterToCO2)
{
    cropResidueParameter.residueReconvertedToCO2 = dryMatterToCO2;
    cropResidueParameter.emissionCH4 = emissionCH4;
    cropResidueParameter.emissionN2O = emissionN2O;

}
void cropResidueManagement::computeEmissions(double residueWeight)
{
    aboveGroundNitrogen = cropResidueParameter.aboveGroundContentN  *residueWeight;
    belowGroundResidue = cropResidueParameter.belowAboveRatio*residueWeight;
    emissionCH4inCH4Units = cropResidueParameter.emissionCH4*residueWeight*1000;
    emissionN2OinN2OUnits = cropResidueParameter.emissionN2O*residueWeight*1000;
    kgCO2Equivalent.fromCH4 = emissionCH4inCH4Units * EQUIVALENTCH4TOCO2;
    kgCO2Equivalent.fromN2O = emissionN2OinN2OUnits * EQUIVALENTN2OTOCO2;
    kgCO2Equivalent.fromCO2 = 1000*(cropResidueParameter.dryMatterFraction*residueWeight*cropResidueParameter.residueReconvertedToCO2/100.);
    kgCO2Equivalent.total = kgCO2Equivalent.fromCH4 + kgCO2Equivalent.fromN2O + kgCO2Equivalent.fromCO2;
}

void cropResidueManagement::getEquivalentCO2()
{

}

void energyManagement::setInput(TkindOfEnergy inputFromUser, double myRenewablesPercentage, QString myCountry,int year)
{
    if (inputFromUser.fromElectricityGrid == NODATA) input.fromElectricityGrid = 0;
    else input.fromElectricityGrid = inputFromUser.fromElectricityGrid;
    if (inputFromUser.fromElectricityOwnHydropower ==  NODATA) input.fromElectricityOwnHydropower = 0;
    else input.fromElectricityOwnHydropower = inputFromUser.fromElectricityOwnHydropower;
    if (inputFromUser.fromElectricityOwnPhotovoltaic ==  NODATA) input.fromElectricityOwnPhotovoltaic = 0;
    else input.fromElectricityOwnPhotovoltaic = inputFromUser.fromElectricityOwnPhotovoltaic;
    if (inputFromUser.fromElectricityOwnWind ==  NODATA) input.fromElectricityOwnWind = 0;
    else input.fromElectricityOwnWind = inputFromUser.fromElectricityOwnWind;
    if (inputFromUser.fromFuelBiodiesel ==  NODATA) input.fromFuelBiodiesel = 0;
    else input.fromFuelBiodiesel = inputFromUser.fromFuelBiodiesel;
    if (inputFromUser.fromFuelBioethanol ==  NODATA) input.fromFuelBioethanol = 0;
    else input.fromFuelBioethanol = inputFromUser.fromFuelBioethanol;
    if (inputFromUser.fromFuelCoal ==  NODATA) input.fromFuelCoal = 0;
    else input.fromFuelCoal = inputFromUser.fromFuelCoal;
    if (inputFromUser.fromFuelDiesel ==  NODATA) input.fromFuelDiesel = 0;
    else input.fromFuelDiesel = inputFromUser.fromFuelDiesel;
    if (inputFromUser.fromFuelHighDensityBiomass ==  NODATA) input.fromFuelHighDensityBiomass = 0;
    else input.fromFuelHighDensityBiomass = inputFromUser.fromFuelHighDensityBiomass;
    if (inputFromUser.fromFuelLiquidPropane ==  NODATA) input.fromFuelLiquidPropane = 0;
    else input.fromFuelLiquidPropane = inputFromUser.fromFuelLiquidPropane;
    if (inputFromUser.fromFuelOil ==  NODATA) input.fromFuelOil = 0;
    else input.fromFuelOil = inputFromUser.fromFuelOil;
    if (inputFromUser.fromFuelPetrol ==  NODATA) input.fromFuelPetrol = 0;
    else input.fromFuelPetrol = inputFromUser.fromFuelPetrol;
    if (inputFromUser.fromFuelWood ==  NODATA) input.fromFuelWood = 0;
    else input.fromFuelWood = inputFromUser.fromFuelWood;

    country = myCountry;
    if (myRenewablesPercentage == NODATA && country == "") percentageRenewablesInGrid = 24 + (year-2016); // supposed renewable energy are 50% of the production

    else percentageRenewablesInGrid = myRenewablesPercentage;

    if (percentageRenewablesInGrid > 100) percentageRenewablesInGrid = 100;

}
double energyManagement::electricityEmissionComputation(double input,double parameterConversionInput)
{
    double output = input*3.6*parameterConversionInput; // convert from kWh to MJ
    return output;
}

void energyManagement::computeEmissions()
{
    double parameter;
    if (percentageRenewablesInGrid == NODATA) percentageRenewablesInGrid = 50.;
    parameter = parameterElectricity.fossil * (100. - percentageRenewablesInGrid)/100 + (percentageRenewablesInGrid/100.)*(parameterElectricity.hydropower+parameterElectricity.photovoltaic + parameterElectricity.wind)/3.0;
    emissions.fromElectricityGrid = energyManagement::electricityEmissionComputation(input.fromElectricityGrid,parameter);
    //parameter = parameterElectricity.wind;
    //emissions.fromElectricityOwnWind = energyManagement::electricityEmissionComputation(input.fromElectricityOwnWind ,parameter);
    parameter = parameterElectricity.photovoltaic;
    emissions.fromElectricityOwnPhotovoltaic = energyManagement::electricityEmissionComputation(input.fromElectricityOwnPhotovoltaic ,parameter);
    parameter = parameterElectricity.wind;
    emissions.fromElectricityOwnWind = energyManagement::electricityEmissionComputation(input.fromElectricityOwnWind ,parameter);
    parameter = parameterElectricity.hydropower;
    emissions.fromElectricityOwnHydropower = energyManagement::electricityEmissionComputation(input.fromElectricityOwnHydropower ,parameter);

    emissions.fromFuelBiodiesel = input.fromFuelBiodiesel * parameterFuel.biodieselEmissionPerLitre;
    emissions.fromFuelBioethanol = input.fromFuelBioethanol * parameterFuel.ethanolEmissionPerLitre;
    emissions.fromFuelPetrol = input.fromFuelPetrol * parameterFuel.petrolEmissionPerLitre;
    emissions.fromFuelDiesel = input.fromFuelDiesel * parameterFuel.dieselEmissionPerLitre;
    emissions.fromFuelHighDensityBiomass = input.fromFuelHighDensityBiomass*parameterFuel.HighDensityBiomassEmissionPerEnergyUnit*parameterFuel.highDensityBiomassHeatPower; // ???????????
    emissions.fromFuelWood = input.fromFuelWood*parameterFuel.woodEmissionPerEnergyUnit*parameterFuel.woodHeatPower; // ??????????????????????
    emissions.fromFuelCoal = input.fromFuelCoal*parameterFuel.coalEmissionPerEnergyUnit*parameterFuel.coalHeatPower;
    emissions.fromFuelOil = input.fromFuelOil * 0.8* parameterFuel.oilPerEnergyUnit*parameterFuel.heavyOilHeatPower;
    emissions.fromFuelLiquidPropane = input.fromFuelOil * 0.582 *parameterFuel.lpgPerEnergyUnit * parameterFuel.propaneHeatPower;
}

/*void energyManagement::getEmissions()
{

}*/

void appliedPesticides::setInput(double myWeightOfActivePrinciple,double renewablePercentageInOwnCountry)
{
    weightOfActivePrinciple = myWeightOfActivePrinciple;
    renewablesInCountry = renewablePercentageInOwnCountry;
}

void appliedPesticides::computeEmissions()
{
    double nonElectricalEnergyRequired = weightOfActivePrinciple * (energyPerMassOfActivePrinciple - electricalEnergyPerMassOfActivePrinciple); // requirement in MJ
    double electricalEnergyRequired = weightOfActivePrinciple * electricalEnergyPerMassOfActivePrinciple*(100. - renewablesInCountry)/100.; // requirement in MJ
    emissionDueToProduction = 0.069 * (nonElectricalEnergyRequired + electricalEnergyRequired); // kgCO2eq
}

void fertiliserApplication::setNitrogenInhibitorsTable()
{
    nitrogenInhibitorN2O.none[0] = 1;
    nitrogenInhibitorN2O.none[1] = 1;
    nitrogenInhibitorN2O.nitrificationInhibitor[0] = 0.4;
    nitrogenInhibitorN2O.nitrificationInhibitor[1] = 0.63;
    nitrogenInhibitorN2O.polymerCoated[0] = 0.23;
    nitrogenInhibitorN2O.polymerCoated[1] = 1;

    nitrogenInhibitorNO.none[0] = 1;
    nitrogenInhibitorNO.none[1] = 1;
    nitrogenInhibitorNO.nitrificationInhibitor[0] = 0.54;
    nitrogenInhibitorNO.nitrificationInhibitor[1] = 0.54;
    nitrogenInhibitorNO.polymerCoated[0] = 0.6;
    nitrogenInhibitorNO.polymerCoated[1] = 0.6;
}

void fertiliserApplication::computeEmissions()
{

    double amountNitrogen = amountFertiliser*fertInput.contentElement.nitrogen;
    double amountOtherElements = amountFertiliser * (fertInput.contentElement.phosphorus + fertInput.contentElement.potassium);
    double amountCarbon = amountFertiliser*fertInput.contentElement.carbon;
    double producedN2O = indexBouwmanN2O*amountNitrogen;
    double producedNO = indexBouwmanNO*amountNitrogen;
    double producedNH3 = indexBouwmanNH3*amountNitrogen;

}
