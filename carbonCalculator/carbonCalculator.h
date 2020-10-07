#ifndef CARBONCALCULATOR_H
#define CARBONCALCULATOR_H

#define EQUIVALENTCH4TOCO2 25
#define EQUIVALENTN2OTOCO2 296

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
