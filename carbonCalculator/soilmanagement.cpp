#include "stdio.h"
#include "math.h"
#include "soilManagement.h"
#include "basicStructures.h"


void SoilManagement::setMatrix()
{

    for(int iClimate=0; iClimate<4;iClimate++)
    {
        for(int i=0; i<3;i++)
        {
            for(int j=0; j<3;j++)
            {
                soilLandUse[iClimate].matrix[i][j]=1;
                soilTillage[iClimate].matrix[i][j]=1;
                soilCoverCropping[iClimate].matrix[i][j]=1;
            }
        }
    }

    soilLandUse[0].matrix[0][1]=	0.82;
    soilLandUse[0].matrix[0][2]=	0.71;
    soilLandUse[0].matrix[1][0]=	1.219512195;
    soilLandUse[0].matrix[1][2]=    0.865853659;
    soilLandUse[0].matrix[2][0]=	1.408450704;
    soilLandUse[0].matrix[2][1]=    1.154929577;

    soilLandUse[1].matrix[0][1]=	0.93;
    soilLandUse[1].matrix[0][2]=	0.82;
    soilLandUse[1].matrix[1][0]=	1.075268817;
    soilLandUse[1].matrix[1][2]=    0.88172043;
    soilLandUse[1].matrix[2][0]=	1.219512195;
    soilLandUse[1].matrix[2][1]=    1.134146341;

    soilLandUse[2].matrix[0][1]=	0.663832016;
    soilLandUse[2].matrix[0][2]=	0.58;
    soilLandUse[2].matrix[1][0]=	1.506405198;
    soilLandUse[2].matrix[1][2]=    0.873715015;
    soilLandUse[2].matrix[2][0]=	1.724137931;
    soilLandUse[2].matrix[2][1]=    1.144537959;

    soilLandUse[3].matrix[0][1]=	0.789731192;
    soilLandUse[3].matrix[0][2]=	0.69;
    soilLandUse[3].matrix[1][0]=	1.266253644;
    soilLandUse[3].matrix[1][2]=    0.873715015;
    soilLandUse[3].matrix[2][0]=	1.449275362;
    soilLandUse[3].matrix[2][1]=    1.144537959;

    soilTillage[0].matrix[0][1]=	0.939655172;
    soilTillage[0].matrix[0][2]=	0.862068966;
    soilTillage[0].matrix[1][0]=	1.064220183;
    soilTillage[0].matrix[1][2]=    0.917431193;
    soilTillage[0].matrix[2][0]=	1.16;
    soilTillage[0].matrix[2][1]=	1.09;

    soilTillage[1].matrix[0][1]=	0.936363636;
    soilTillage[1].matrix[0][2]=	0.909090909;
    soilTillage[1].matrix[1][0]=	1.067961165;
    soilTillage[1].matrix[1][2]=    0.970873786;
    soilTillage[1].matrix[2][0]=	1.1;
    soilTillage[1].matrix[2][1]=	1.03;

    soilTillage[2].matrix[0][1]=	0.943089431;
    soilTillage[2].matrix[0][2]=	0.81300813;
    soilTillage[2].matrix[1][0]=	1.060344828;
    soilTillage[2].matrix[1][2]=    0.862068966;
    soilTillage[2].matrix[2][0]=	1.23;
    soilTillage[2].matrix[2][1]=	1.16;

    soilTillage[3].matrix[0][1]=	0.94017094;
    soilTillage[3].matrix[0][2]=	0.854700855;
    soilTillage[3].matrix[1][0]=	1.063636364;
    soilTillage[3].matrix[1][2]=    0.909090909;
    soilTillage[3].matrix[2][0]=	1.17;
    soilTillage[3].matrix[2][1]=	1.1;

    soilCoverCropping[0].matrix[0][1]=	1.098901099;
    soilCoverCropping[0].matrix[0][2]=	1.21978022;
    soilCoverCropping[0].matrix[1][0]=	0.91;
    soilCoverCropping[0].matrix[1][2]=  1.11;
    soilCoverCropping[0].matrix[2][0]=	0.81981982;
    soilCoverCropping[0].matrix[2][1]=	0.900900901;

    soilCoverCropping[1].matrix[0][1]=	1.086956522;
    soilCoverCropping[1].matrix[0][2]=	1.163043478;
    soilCoverCropping[1].matrix[1][0]=	0.92;
    soilCoverCropping[1].matrix[1][2]=  1.07;
    soilCoverCropping[1].matrix[2][0]=	0.859813084;
    soilCoverCropping[1].matrix[2][1]=	0.934579439;

    soilCoverCropping[2].matrix[0][1]=	1.098901099;
    soilCoverCropping[2].matrix[0][2]=	1.21978022;
    soilCoverCropping[2].matrix[1][0]=	0.91;
    soilCoverCropping[2].matrix[1][2]=  1.11;
    soilCoverCropping[2].matrix[2][0]=	0.81981982;
    soilCoverCropping[2].matrix[2][1]=	0.900900901;

    soilCoverCropping[3].matrix[0][1]=	1.086956522;
    soilCoverCropping[3].matrix[0][2]=	1.163043478;
    soilCoverCropping[3].matrix[1][0]=	0.92;
    soilCoverCropping[3].matrix[1][2]=  1.07;
    soilCoverCropping[3].matrix[2][0]=	0.859813084;
    soilCoverCropping[3].matrix[2][1]=	0.934579439;

    carbonFromAmendmentManagement[0][0]= 0.65942029;
    carbonFromAmendmentManagement[0][1]= 0.724637681;
    carbonFromAmendmentManagement[0][2]= 0.804347826;

    carbonFromAmendmentManagement[1][0]= 0.686567164;
    carbonFromAmendmentManagement[1][1]= 0.746268657;
    carbonFromAmendmentManagement[1][2]= 0.798507463;

    carbonFromAmendmentManagement[2][0]= 0.65942029;
    carbonFromAmendmentManagement[2][1]= 0.724637681;
    carbonFromAmendmentManagement[2][2]= 0.804347826;

    carbonFromAmendmentManagement[3][0]= 0.686567164;
    carbonFromAmendmentManagement[3][1]= 0.746268657;
    carbonFromAmendmentManagement[3][2]= 0.798507463;
}

void SoilManagement::computeSequestration(Tpercentage myPercentage, double carbonInSoil)
{
    double sequestration;
    percentage.forest = myPercentage.forest;
    percentage.permanentGrass = myPercentage.permanentGrass;
    percentage.minimumTillage = myPercentage.minimumTillage;
    percentage.noTillage = myPercentage.noTillage;
    double incrementTillage=1;
    double incrementResidue=1;
    double incrementOrganicAmendment=1;
    double incrementCoverCrop=1;
    double incrementTotal;
    //SoilManagement::computeSequestrationLandUse();
    incrementTillage = SoilManagement::computeSequestrationTillage();
    incrementTotal = incrementTillage*incrementCoverCrop*incrementOrganicAmendment*incrementResidue;
    sequestration = -1*carbonInSoil*(incrementTotal-1);
    //SoilManagement::computeSequestrationCoverCropping();
}

double SoilManagement::computeSequestrationTillage()
{
    double matrixElement;
    double incrementNoTillage;
    //double incrementNoTillagePercentage;
    double incrementMinimumTillage;
    double incrementTillage;

    matrixElement = soilTillage[0].matrix[2][1];
    incrementNoTillage = 1 + 1./20.*(matrixElement - 1);
    //incrementNoTillagePercentage = incrementNoTillage*percentage.noTillage/100 + (100 - percentage.noTillage)/100.;


    matrixElement = soilTillage[0].matrix[2][1];
    incrementMinimumTillage =  1 + 1./20.*(matrixElement - 1);
    //incrementMinimumTillagePercentage = incrementMinimumTillage*percentage.minimumTillage/100 + (100 - percentage.minimumTillage)/100.;

    return incrementTillage = incrementNoTillage*percentage.noTillage/100. + incrementMinimumTillage*percentage.minimumTillage/100. + (100 - percentage.noTillage - percentage.minimumTillage)/100.;


}



/*void SoilManagement::computeSequestrationLandUse()
{
    if (percentage.forest > EPSILON)
    {
        int deltaLandUse;


    }


}
*/
