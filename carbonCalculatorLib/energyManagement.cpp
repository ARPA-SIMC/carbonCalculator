#include "energyManagement.h"


// constructor
EnergyManagement::EnergyManagement()
{
    country = "";
    percentageRenewablesInGrid = NODATA;
}


double EnergyManagement::electricityEmissionComputation(double input,double parameterConversionInput)
{
    double output = input*3.6*parameterConversionInput; // convert from kWh to MJ
    return output;
}


void EnergyManagement::computeEmissions()
{
    double parameter;
    if (fabs(percentageRenewablesInGrid - NODATA) < 0.00001) percentageRenewablesInGrid = 50.;
    parameter = parameterElectricity.fossil * (100. - percentageRenewablesInGrid)/100 + (percentageRenewablesInGrid/100.)*(parameterElectricity.hydropower+parameterElectricity.photovoltaic + parameterElectricity.wind)/3.0;
    emissions.fromElectricityGrid = electricityEmissionComputation(input.fromElectricityGrid,parameter);
    parameter = parameterElectricity.photovoltaic;
    emissions.fromElectricityOwnPhotovoltaic = electricityEmissionComputation(input.fromElectricityOwnPhotovoltaic ,parameter);
    parameter = parameterElectricity.wind;
    emissions.fromElectricityOwnWind = electricityEmissionComputation(input.fromElectricityOwnWind ,parameter);
    parameter = parameterElectricity.hydropower;
    emissions.fromElectricityOwnHydropower = electricityEmissionComputation(input.fromElectricityOwnHydropower ,parameter);

    emissions.fromFuelBiodiesel = input.fromFuelBiodiesel * parameterFuel.biodieselEmissionPerLitre;
    emissions.fromFuelBioethanol = input.fromFuelBioethanol * parameterFuel.ethanolEmissionPerLitre;
    emissions.fromFuelPetrol = input.fromFuelPetrol * parameterFuel.petrolEmissionPerLitre;
    emissions.fromFuelDiesel = input.fromFuelDiesel * parameterFuel.dieselEmissionPerLitre;
    emissions.fromFuelHighDensityBiomass = input.fromFuelHighDensityBiomass*parameterFuel.HighDensityBiomassEmissionPerEnergyUnit*parameterFuel.highDensityBiomassHeatPower; // ???????????
    emissions.fromFuelWood = input.fromFuelWood*parameterFuel.woodEmissionPerEnergyUnit*parameterFuel.woodHeatPower; // ??????????????????????
    emissions.fromFuelCoal = input.fromFuelCoal*parameterFuel.coalEmissionPerEnergyUnit*parameterFuel.coalHeatPower;
    emissions.fromFuelOil = input.fromFuelOil * 0.8* parameterFuel.oilPerEnergyUnit*parameterFuel.heavyOilHeatPower;
    emissions.fromFuelLPG = input.fromFuelLPG * 0.582 *parameterFuel.lpgPerEnergyUnit * parameterFuel.LPGHeatPower;
    emissions.fromFuelMethane = input.fromFuelMethane * parameterFuel.methanePerEnergyUnit * parameterFuel.methaneHeatPower;
    emissions.total = emissions.fromElectricityGrid + emissions.fromElectricityOwnPhotovoltaic
            + emissions.fromElectricityOwnWind + emissions.fromElectricityOwnHydropower
            + emissions.fromFuelBiodiesel + emissions.fromFuelBioethanol
            + emissions.fromFuelPetrol + emissions.fromFuelDiesel
            + emissions.fromFuelHighDensityBiomass + emissions.fromFuelWood
            + emissions.fromFuelCoal + emissions.fromFuelOil
            + emissions.fromFuelLPG + emissions.fromFuelMethane;

}


