#ifndef CARBONCALCULATOR_H
#define CARBONCALCULATOR_H

#define EQUIVALENTCH4TOCO2 25
#define EQUIVALENTN2OTOCO2 296
#define NODATA -9999

struct TcropResidueParameter{

    int residueWeight;
    int cropType = 1;
    int residueType = 1;
    double dryMatterFraction = 0.9; //default parameter
    double slope = 0.29; //default parameter
    double intercept = 0; //default parameter
    double aboveGroundContentN = 0.027; //default parameter
    double belowAboveRatio = 0.4; //default parameter
    double belowGroundContentN = 0.019; //default parameter
    int isTreeCrop = 1; //default parameter

    double emissionCH4 = 0.065333333; //default parameter
    double emissionN2O = 	0.00050675; //default parameter
    double residueReconvertedToCO2 = 10; //default parameter

};

struct TkgCO2Equivalent{
    double fromCH4;
    double fromCO2;
    double fromN2O;
    double total;
};

struct TparameterElectricity{
    double fossil = 0.15; //kg CO2/MJ supposed to be produced mostly from gas
    double wind = 0.003333333; //kg CO2/MJ
    double photovoltaic = 0.019722222; //kg CO2/MJ
    double hydropower = 0.001666667; //kg CO2/MJ
};

struct TparameterFuel{

    double coalEmissionPerEnergyUnit = 0.0927; // kgCO2Eq/MJ
    double lpgPerEnergyUnit = 0.0628; // kgCO2Eq/MJ
    double oilPerEnergyUnit = 0.0773; // kgCO2Eq/MJ

    double coalHeatPower = 31.50; // MJ/kg
    double propaneHeatPower = 47.50; // MJ/kg
    double heavyOilHeatPower = 40.33; // MJ/kg
    double highDensityBiomassHeatPower = 20.08;  // MJ/kg
    double woodHeatPower = 12.99;  // MJ/kg

    double dieselEmissionPerLitre = 2.68 ; // kgCO2/litre
    double ethanolEmissionPerLitre = 2.32; // kgCO2/litre
    double petrolEmissionPerLitre = 2.32; // kgCO2/litre
    double biodieselEmissionPerLitre = 2.68; // kgCO2/litre


};


struct TkindOfEnergy {
    double fromElectricityGrid;
    double fromElectricityOwnPhotovoltaic;
    double fromElectricityOwnWind;
    double fromElectricityOwnHydropower;

    double fromFuelDiesel;
    double fromFuelPetrol;
    double fromFuelBiodiesel;
    double fromFuelBioethanol;
    double fromFuelHighDensityBiomass;
    double fromFuelWood;
    double fromFuelCoal;
    double fromFuelOil;
    double fromFuelLiquidPropane;

};


class energyManagement{
public:
// variables
    TkindOfEnergy emissions;
    TkindOfEnergy input;
    TparameterElectricity parameterElectricity;
    TparameterFuel parameterFuel;
private:
    double percentageRenewablesInGrid;
    double country;

// functions
public:
    void setInput(TkindOfEnergy input, double renewablesPercentage,int country);
    void computeEmissions();
    double electricityEmissionComputation(double input,double parameter);
    double fuelEmissionComputation(double input);
    void parametersEmissionElectricity();
    //void getEmissions(TkindOfEnergy output);

};

class cropResidueManagement {
private:
    // functions

    // variables
    TkgCO2Equivalent kgCO2Equivalent;
    TcropResidueParameter cropResidueParameter;
    double aboveGroundNitrogen;
    double belowGroundResidue;
    double emissionCH4inCH4Units;
    double emissionN2OinN2OUnits;

public:
    // functions
    void setParameters();
    void getEquivalentCO2();
    void computeEquivalentCO2(double residueWeight);
    // variables

};

#endif // CARBONCALCULATOR_H


class carbonCalculator{
private:
    // classes

    // functions

    //variables

public:
    // classes
        cropResidueManagement cropResidue;
        energyManagement energy;
    // functions

    //variables


};
