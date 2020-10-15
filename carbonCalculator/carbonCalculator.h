#ifndef CARBONCALCULATOR_H
#define CARBONCALCULATOR_H

    #include "basicStructures.h"
    #include "energyManagement.h"
    #include <QString>


    class AppliedPesticides{
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


    class CropResidueManagement {
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


    class FertiliserApplication{
    private:
        double applicationRate; // kg/ha
        int kindOfFertiliser;
        int applicationMethod;
        int kindOfInhibitor;
        int kindOfFertiliserProduction;
        TnitrogenInhibitors nitrogenInhibitorNO;
        TnitrogenInhibitors nitrogenInhibitorN2O;

        double indexBouwmanN2O;
        double indexBouwmanNO;
        double indexBouwmanNH3;

        double emissionsByProduction;

        void setNitrogenInhibitorsTable();

    public:
        TfertInput fertInput;
        double amountFertiliser;

        void computeEmissions();

    };


    class CarbonCalculator{
    private:
        // classes

        // functions

        //variables

    public:
        // classes
            CropResidueManagement cropResidue;
            EnergyManagement energy;
            AppliedPesticides pesticide;
            FertiliserApplication fertiliser;

        // functions

        //variables

    };

#endif // CARBONCALCULATOR_H
