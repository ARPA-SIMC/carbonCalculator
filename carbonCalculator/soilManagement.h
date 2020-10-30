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

};

class SoilManagement{
private:
    Tpercentage percentage;
    TsoilManagement soilTillage[4];
    TsoilManagement soilLandUse[4];
    TsoilManagement soilCoverCropping[4];
    double carbonFromAmendmentManagement[4][3];

    void setMatrix();

public:

    void computeSequestration(Tpercentage myPercentage, double carbonInSoil);
    double computeSequestrationTillage();
    double computeSequestrationLandUse();
    double computeSequestrationCoverCropping();

};

#endif // SOILMANAGEMENT_H
