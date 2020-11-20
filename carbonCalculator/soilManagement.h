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
    bool isOrganic;
    double rootDecayParameter;

    void computeSequestration(double carbonInSoil, int myIdClimate, double* quantityAmendment , double* incrementalParameterAmendment, double *residues, double *dryMatterResidues, bool* isIncorporatedResidue);
    double computeEmissions(double carbonInSoil, int myIdClimate);
    double computeSequestrationRootBiomass();
    double computeSequestrationTillage(int myIdClimate);
    double computeSequestrationLandUse(int myIdClimate);
    double computeSequestrationCoverCropping(int myIdClimate);
    double computeSequestrationOrganicAmendments(double amountOfAmendments, double incrementalParameterAmendment);
    double computeSequestrationResidueIncorporation(double residueIncorporated,double percentageDryMatter,bool isIncorporatedResidue);

};

#endif // SOILMANAGEMENT_H
