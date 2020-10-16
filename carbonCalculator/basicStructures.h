#ifndef BASICSTRUCTURES_H
#define BASICSTRUCTURES_H

    #define EQUIVALENTCH4TOCO2 25
    #define EQUIVALENTN2OTOCO2 296
    #define NODATA -9999

    struct TcropResidueParameter{

        //int residueWeight;
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
        double fromElectricityGrid = 0;
        double fromElectricityOwnPhotovoltaic = 0;
        double fromElectricityOwnWind = 0;
        double fromElectricityOwnHydropower = 0;

        double fromFuelDiesel = 0;
        double fromFuelPetrol = 0;
        double fromFuelBiodiesel = 0;
        double fromFuelBioethanol = 0;
        double fromFuelHighDensityBiomass = 0;
        double fromFuelWood = 0;
        double fromFuelCoal = 0;
        double fromFuelOil = 0;
        double fromFuelLPG = 0;
    };


    struct TnitrogenInhibitors{
        double none[2];
        double nitrificationInhibitor[2];
        double polymerCoated[2];
    };

    struct TcontentElements{
        double nitrogen;
        double potassium;
        double phosphorus;
        double carbon;
    };

    struct TfertInput{

        double bouwmanN2O;
        double bouwmanNO;
        double bouwmanNH3;
        double emissionPerKgOfProduct;
        TcontentElements contentElement;
    };

    struct TBouwmanIndex{
        double modelParameter;
        double cropType;
        double soilTexture;
        double soilOrganicCarbon;
        double pH;
        double cationicExchangeCapacity;
        double drainage;
        double climate;
        double applicationMethod;

    };


#endif // BASICSTRUCTURES_H
