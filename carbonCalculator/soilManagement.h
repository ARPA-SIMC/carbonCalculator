#ifndef SOILMANAGEMENT_H
#define SOILMANAGEMENT_H

struct TsoilManagement {

    double matrix[3][3];

};

struct Tpercentage{
    double forest;
    double permanentGrass;
    double arable;

    double conventionalTillage;
    double minimumTillage;
    double noTillage;

    double coverCropping;

};

class SoilManagement{
private:
    TsoilManagement soilTillage[4];
    TsoilManagement soilLandUse[4];
    TsoilManagement soilCoverCropping[4];
    double carbonFromAmendmentManagement[4][3];


    void setMatrix();

public:

    Tpercentage percentage;
    double sequestrationCarbonCO2Eq;
    double sequestrationCarbonCO2EqResidue[2];
    double sequestrationCarbonCO2EqTillage;
    double sequestrationCarbonCO2EqCropCover;
    double sequestrationCarbonCO2EqLandUse;
    double sequestrationCarbonCO2EqFertilizerAmendment[8];
    bool isOrganic;
    double rootDecayParameter;
    double yield;
    double fieldSize;
    void computeSequestration(double carbonInSoil, int myIdClimate, double* quantityAmendment , double *recalcitrantIndex, double* incrementalParameterAmendment, double *residues, double *dryMatterResidues, bool* isIncorporatedResidue);
    double computeEmissions(double carbonInSoil, int myIdClimate);
    double computeSequestrationRootBiomass(int myIdClimate);
    double computeSequestrationTillage(int myIdClimate);
    double computeSequestrationLandUse(int myIdClimate);
    double computeSequestrationCoverCropping(int myIdClimate);
    double computeSequestrationOrganicAmendments(double amountOfAmendments, double incrementalParameterAmendment, double recalcitrantIndex);
    double computeSequestrationResidueIncorporation(double residueIncorporated,double percentageDryMatter,bool isIncorporatedResidue,int indexRecalitrant);
    double computeSequestrationResidueIncorporation2(double residueIncorporated, double percentageDryMatter, bool isIncorporatedResidue, int isHerbaceous);
    double computeSequestrationDueToOrganicMamagement();
};

#endif // SOILMANAGEMENT_H
