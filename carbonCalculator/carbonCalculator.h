#ifndef CARBONCALCULATOR_H
#define CARBONCALCULATOR_H

    #include "basicStructures.h"
    #include "energyManagement.h"
    #include <QString>


    class AppliedPesticides{
    public:
        double renewablesInCountry;
        double emissionDueToProduction;
        double weightOfActivePrinciple;
        double energyPerMassOfActivePrinciple; // [MJ/kg]
        double electricalEnergyPerMassOfActivePrinciple; // [MJ/kg]

    public:
        // constructor
        AppliedPesticides();

        void setInput(double myWeigthOfActivePrinciple, double myRenewablePercentage);
        void computeEmissions();
    };


    class CropResidueManagement {
    public:
        // functions

        // variables
        TkgCO2Equivalent kgCO2Equivalent;

        double aboveGroundNitrogen;
        double belowGroundResidue;
        double emissionCH4inCH4Units;
        double emissionN2OinN2OUnits;
        double residueWeight;

    public:
        // functions
        void setInput(double emissionCH4, double emissionN2O, double dryMatterToCO2);
        //void getEquivalentCO2();
        void computeEmissions();
        // variables
        TcropResidueParameter cropResidueParameter;
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



    public:
        TfertInput fertInput;
        double amountFertiliser;
        TBouwmanIndex bouwmanParameterNH4;
        TBouwmanIndex bouwmanParameterNO;
        TBouwmanIndex bouwmanParameterN2O;


        void computeEmissions();
        void setNitrogenInhibitorsTable();
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
            void computeEmissions();
            void initializeBouwmanTables();
            bool initialiazeVariables(QString idDrainage, double pH, double CEC, QString idSoilTexture,QString idSoilOrganicCarbon);
        //variables
            TbouwmanTable bouwmanTableN2O;
            TbouwmanTable bouwmanTableNO;
            TbouwmanTable bouwmanTableNH4;
            int bouwmanEquivalentTag;
            TsoilTexture soilTexture;




    };

#endif // CARBONCALCULATOR_H
