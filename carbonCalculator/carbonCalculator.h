#ifndef CARBONCALCULATOR_H
#define CARBONCALCULATOR_H

#define EQUIVALENTCH4TOCO2 25
#define EQUIVALENTN2OTOCO2 296
#define NODATA -9999

#include <QString>

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
    double HighDensityBiomassEmissionPerEnergyUnit = 0.106; // kgCO2Eq/MJ from https://www.volker-quaschning.de/datserv/CO2-spez/index_e.php
    double woodEmissionPerEnergyUnit = 0.109; // kgCO2Eq/MJ from https://www.volker-quaschning.de/datserv/CO2-spez/index_e.php
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


struct TnitrogenInhibitors{
    double none[2];
    double nitrificationInhibitor[2];
    double polymerCoated[2];
};

struct TpercentageElements{
    double nitrogen;
    double potassium;
    double phosphorus;
    double carbon;
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
    QString country;

// functions
public:
    void setInput(TkindOfEnergy input, double renewablesPercentage, QString myCountry,int year);
    void computeEmissions();
    double electricityEmissionComputation(double input,double parameter);
    double fuelEmissionComputation(double input);
    void parametersEmissionElectricity();
    //void getEmissions(TkindOfEnergy output);

};

class appliedPesticides{
private:
    double renewablesInCountry;
    double emissionDueToProduction;
    double weightOfActivePrinciple;
    double energyPerMassOfActivePrinciple = 217.3; // [MJ/kg]
    double electricalEnergyPerMassOfActivePrinciple = 70.83; // [MJ/kg]
public:
    void setInput(double myWeigthOfActivePrinciple, double myRenewablePercentage);
    void computeEmissions();

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
    void setInput(double emissionCH4, double emissionN2O, double dryMatterToCO2);
    void getEquivalentCO2();
    void computeEmissions(double residueWeight);
    // variables

};

class fertiliserApplication{
private:
    double applicationRate; // kg/ha
    int kindOfFertiliser;
    int applicationMethod;
    int kindOfInhibitor;
    int kindOfFertiliserProduction;
    TnitrogenInhibitors nitrogenInhibitorNO;
    TnitrogenInhibitors nitrogenInhibitorN2O;
    TpercentageElements percentageElements;

    double amountFertiliser;

    double indexBouwmanN2O;
    double indexBouwmanNO;
    double indexBouwmanNH3;

    double emissionsByProduction;



    void setNitrogenInhibitorsTable();

public:
    void setInput();
    void computeEmissions();


};


class carbonCalculator{
private:
    // classes

    // functions

    //variables

public:
    // classes
        cropResidueManagement cropResidue;
        energyManagement energy;
        appliedPesticides pesticide;
        fertiliserApplication fertiliser;

    // functions

    //variables


};
#endif // CARBONCALCULATOR_H
