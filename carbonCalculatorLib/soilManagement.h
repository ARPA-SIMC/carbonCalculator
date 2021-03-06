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
    double emissionsConventionalManagement;
    double sequestrationCarbonCO2Eq;
    double sequestrationCarbonCO2EqResidue[4];
    double sequestrationCarbonCO2EqResidueWood;
    double sequestrationCarbonCO2EqResidueGreen;
    double sequestrationCarbonCO2EqTillage;
    double sequestrationCarbonCO2EqCropCover;
    double sequestrationCarbonCO2EqLandUse;
    double sequestrationCarbonCO2EqFertilizerAmendment[8];
    double sequestrationAmendment;
    double sequestrationRoot;
    bool isOrganic;
    double rootDecayParameter;
    double yield;
    double fieldSize;
    void computeSequestration(double carbonInSoil, int myIdClimate, double* quantityAmendment , double *recalcitrantIndex, double* incrementalParameterAmendment, double *residues, bool* isIncorporatedResidue, double sequestrationFromRecalcitrantAmendment, double *dryMatter);
    double computeEmissions(double carbonInSoil);
    double computeSequestrationRootBiomass();
    double computeSequestrationUnstableCarbonDueToRoots();
    double computeSequestrationTillage(int myIdClimate);
    double computeSequestrationLandUse(int myIdClimate);
    double computeSequestrationCoverCropping(int myIdClimate);
    double computeSequestrationOrganicAmendments(double amountOfAmendments, double incrementalParameterAmendment, double recalcitrantIndex);
    double computeSequestrationResidueIncorporation(double residueIncorporated,double percentageDryMatter,bool isIncorporatedResidue,int indexRecalitrant);
    double computeSequestrationResidueIncorporation2(double residueIncorporated, bool isIncorporatedResidue, int isHerbaceous);
    double computeSequestrationDueToOrganicMamagement();
};

#endif // SOILMANAGEMENT_H
