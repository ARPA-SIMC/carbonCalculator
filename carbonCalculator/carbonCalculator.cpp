#include "carbonCalculator.h"
#include <math.h>
#include <stdio.h>



void cropResidueManagement::setParameters()
{

}

void cropResidueManagement::computeEquivalentCO2(double residueWeight)
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

void energyManagement::setInput(TkindOfEnergy inputFromUser, double myRenewablesPercentage,int myCountry)
{
    if (inputFromUser.fromElectricityGrid == NODATA) input.fromElectricityGrid = 0;
    else input.fromElectricityGrid = inputFromUser.fromElectricityGrid;
    if (inputFromUser.fromElectricityOwnHydropower ==  NODATA) input.fromElectricityOwnHydropower = 0;
    else input.fromElectricityOwnHydropower = inputFromUser.fromElectricityOwnHydropower;
    if (inputFromUser.fromElectricityOwnPhotovoltaic ==  NODATA) input.fromElectricityOwnPhotovoltaic = 0;
    else input.fromElectricityOwnPhotovoltaic = inputFromUser.fromElectricityOwnPhotovoltaic;
    if (inputFromUser.fromElectricityOwnPhotovoltaic ==  NODATA) input.fromElectricityOwnWind = 0;
    else input.fromElectricityOwnPhotovoltaic = inputFromUser.fromElectricityOwnWind;
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
    percentageRenewablesInGrid = myRenewablesPercentage;


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
    parameter = parameterElectricity.wind;
    emissions.fromElectricityOwnWind = energyManagement::electricityEmissionComputation(input.fromElectricityOwnWind ,parameter);
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
    emissions.fromFuelHighDensityBiomass = 0; // ???????????
    emissions.fromFuelWood = 0; // ????????????????
    emissions.fromFuelCoal = input.fromFuelCoal*parameterFuel.coalEmissionPerEnergyUnit*parameterFuel.coalHeatPower;
    emissions.fromFuelOil = input.fromFuelOil * 0.8* parameterFuel.oilPerEnergyUnit*parameterFuel.highDensityBiomassHeatPower;
    emissions.fromFuelLiquidPropane = input.fromFuelOil * 0.582 *parameterFuel.lpgPerEnergyUnit * parameterFuel.propaneHeatPower;
}

/*void energyManagement::getEmissions()
{

}*/
