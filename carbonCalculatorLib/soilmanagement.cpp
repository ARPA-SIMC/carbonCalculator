#include "stdio.h"
#include "math.h"
#include "soilManagement.h"
#include "basicStructures.h"


double SoilManagement::computeEmissions(double carbonInSoil,int myIdClimate)
{
    double emissions;
    setMatrix();
    emissions = 0;
    emissions += FROM_C_TO_CO2 * carbonInSoil * (0.006)*percentage.forest*0.01; // 0.006 from Hockaday et al. 2015
    emissions += FROM_C_TO_CO2 * carbonInSoil * (0.006)*44./33.*percentage.permanentGrass*0.01;
    emissions += FROM_C_TO_CO2 * carbonInSoil * (0.006)*44./33.*percentage.coverCropping*0.01;
    emissions += FROM_C_TO_CO2 * carbonInSoil * (0.006)*44./30.*percentage.noTillage*0.01*(percentage.arable-percentage.coverCropping)*0.01;
    emissions += FROM_C_TO_CO2 * carbonInSoil * (0.006)*44./27.*percentage.minimumTillage*0.01*(percentage.arable-percentage.coverCropping)*0.01;
    emissions += FROM_C_TO_CO2 * carbonInSoil * (0.006)*44./24.*percentage.conventionalTillage*0.01*(percentage.arable-percentage.coverCropping)*0.01;
    /*
    double weightArable = 0.01*(percentage.conventionalTillage*soilTillage[myIdClimate].matrix[2][0] + percentage.minimumTillage*soilTillage[myIdClimate].matrix[1][0]+percentage.noTillage);
    emissions += FROM_C_TO_CO2 * carbonInSoil * (0.006)*40./30.*percentage.arable*0.01*weightArable;
    weightArable = soilTillage[myIdClimate].matrix[2][0];
    */
    emissionsConventionalManagement = FROM_C_TO_CO2 * carbonInSoil * (0.006)*44./24.; // Cools et al. 2014 the number 44, for 24 https://webcache.googleusercontent.com/search?q=cache:gFf4CmDDsT0J:https://www.nrcs.usda.gov/wps/PA_NRCSConsumption/download%3Fcid%3Dnrcs142p2_052823%26ext%3Dpdf+&cd=15&hl=it&ct=clnk&gl=it

    return emissions;
}

double SoilManagement::computeSequestrationRootBiomass(int myIdClimate)
{

    double biomassRootPerHectare;
    double biomassRootCrop=0;
    double biomassRootCoverCrop=0;
    double biomassRootPermanentGrass=0;
    double biomassRootForest=0;
    if (!isOrganic) biomassRootPerHectare = 370;
    else biomassRootPerHectare = 695; // computation for weeds 370 kg/ha in conventional and 695 kg/ha for organic of carbon from Hu et al. 2018
    biomassRootCrop = - biomassRootPerHectare * FROM_C_TO_CO2*exp(-1.27)*(100 - percentage.forest)*0.01;//*0.01*(percentage.conventionalTillage + percentage.noTillage * ((soilTillage[myIdClimate].matrix[2][0]-1)/20 + 1) + percentage.minimumTillage * ((soilTillage[myIdClimate].matrix[1][0]-1)/20 +1));
    biomassRootCoverCrop = - biomassRootPerHectare * FROM_C_TO_CO2*exp(-1.27)*percentage.coverCropping*0.01*0.5;//*0.01*percentage.coverCropping*((soilCoverCropping[myIdClimate].matrix[0][1]-1)/20 + 1);
    biomassRootPermanentGrass = - biomassRootPerHectare * FROM_C_TO_CO2*exp(-1.27)*percentage.permanentGrass*0.01;// *((soilLandUse[myIdClimate].matrix[2][1]-1)/20 + 1);
    biomassRootForest = - 695 * FROM_C_TO_CO2*exp(-1.27)*percentage.forest*0.01;//*((soilLandUse[myIdClimate].matrix[2][0]-1)/20 + 1); // we supposed forest is organic by default
    return biomassRootCrop + biomassRootCoverCrop + biomassRootPermanentGrass + biomassRootForest;
}

double SoilManagement::computeSequestrationUnstableCarbonDueToRoots(int myIdClimate)
{
    double biomassRootPerHectare;
    double biomassRootCrop;
    double biomassRootCoverCrop;
    double biomassRootPermanentGrass;
    double biomassRootForest;
    if (!isOrganic) biomassRootPerHectare = 370;
    else biomassRootPerHectare = 695; // computation for weeds 370 kg/ha in conventional and 695 kg/ha for organic of carbon from Hu et al. 2018
    biomassRootCrop = biomassRootPerHectare*percentage.arable*0.01*(1 - exp(-rootDecayParameter));//*0.01*(percentage.conventionalTillage + percentage.noTillage * ((soilTillage[myIdClimate].matrix[2][0]-1)/20 + 1) + percentage.minimumTillage * ((soilTillage[myIdClimate].matrix[1][0]-1)/20 +1));
    biomassRootCoverCrop = biomassRootPerHectare*(1 - exp(-1.27))*percentage.coverCropping*0.01*0.5;//*0.01*percentage.coverCropping*((soilCoverCropping[myIdClimate].matrix[0][1]-1)/20 + 1);
    biomassRootPermanentGrass = biomassRootPerHectare*(1 - exp(-1.27))*percentage.permanentGrass*0.01;// *((soilLandUse[myIdClimate].matrix[2][1]-1)/20 + 1);
    biomassRootForest = 695*(1 - exp(-0.71))*percentage.forest*0.01;//*((soilLandUse[myIdClimate].matrix[2][0]-1)/20 + 1); // we supposed forest is organic by default
    return biomassRootCrop + biomassRootCoverCrop + biomassRootCoverCrop + biomassRootForest;

}

double SoilManagement::computeSequestrationDueToOrganicMamagement()
{
   if (!isOrganic)
   {
       return -270*FROM_C_TO_CO2*(percentage.forest)*0.01;
   }
   else
   {
       return -270*FROM_C_TO_CO2; // from Gattinger et al. 2012
   }
}

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
    // the first index 0 means TEMPERATE_MOIST; 1 TEMPERATE_DRY; 2 TROPICAL_MOIST ; 3 TROPICAL_DRY
    // in the matrix 0 forest 1 grass 2 arable
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
    // 0 notill 1 minimumTillage 2 conventionalTillage
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

void SoilManagement::computeSequestration(double carbonInSoil, int myIdClimate, double* quantityOfAmendment, double* recalcitrantIndex, double* incrementalParameterAmendment,double* residues , double* dryMatterResidues,bool* isIncorporatedResidue, double sequestrationFromRecalcitrantAmendment, double* dryMatter)
{
    double sequestrationOfCarbon;
    double incrementTillage=1;
    //double incrementResidue=1;
    double incrementOrganicAmendment=1;
    double incrementCoverCrop=1;
    double incrementLandUse = 1;
    double incrementTotal=1;

    setMatrix();
    myIdClimate--; // in order to move from 1-4 notation to 0-3 notation
    incrementTotal *= incrementTillage = (percentage.arable * computeSequestrationTillage(myIdClimate) + 1*(100 - percentage.arable))/100.;
    sequestrationOfCarbon = -1*carbonInSoil*(incrementTillage-1);
    sequestrationCarbonCO2EqTillage = sequestrationOfCarbon * FROM_C_TO_CO2;
    incrementTotal *= incrementCoverCrop = computeSequestrationCoverCropping(myIdClimate);
    sequestrationOfCarbon = -1*carbonInSoil*(incrementCoverCrop-1);
    sequestrationCarbonCO2EqCropCover = sequestrationOfCarbon * FROM_C_TO_CO2;
    incrementTotal *= incrementLandUse = computeSequestrationLandUse(myIdClimate);
    sequestrationOfCarbon = -1*carbonInSoil*(incrementLandUse-1);
    sequestrationCarbonCO2EqLandUse = sequestrationOfCarbon * FROM_C_TO_CO2;
    sequestrationAmendment = 0;

    for (int i=0; i<8; i++)
    {
        double amend;

        if (dryMatter[i] < 0.5)
            amend = 0;
        else
            amend = quantityOfAmendment[i] * 100.0 / dryMatter[i];
        incrementTotal *= incrementOrganicAmendment *= computeSequestrationOrganicAmendments(amend,incrementalParameterAmendment[i],recalcitrantIndex[i]);
        sequestrationOfCarbon = -1*carbonInSoil*(computeSequestrationOrganicAmendments(amend,incrementalParameterAmendment[i],recalcitrantIndex[i])-1);
        sequestrationCarbonCO2EqFertilizerAmendment[i] = sequestrationOfCarbon * FROM_C_TO_CO2;
        sequestrationAmendment += sequestrationCarbonCO2EqFertilizerAmendment[i];
    }
    sequestrationAmendment += sequestrationFromRecalcitrantAmendment;
    incrementTotal *= computeSequestrationOrganicAmendments(2*computeSequestrationUnstableCarbonDueToRoots(myIdClimate),0.001545064,0);
    double percentageUnstable[4] = {1-0.36,1-0.36,1-0.14,1-0.14};
    for (int i=0;i<4;i++)
    {
        if (isIncorporatedResidue[i])
            incrementTotal *= computeSequestrationOrganicAmendments(2*residues[i]*percentageUnstable[i]*1000,0.001545064,0);
    }

    sequestrationOfCarbon = -1*carbonInSoil*(incrementTotal-1);
    //printf("valori residui legno %f verde %f \n", residues[0],residues[1]);
    sequestrationCarbonCO2Eq = sequestrationOfCarbon * FROM_C_TO_CO2;
    sequestrationRoot = SoilManagement::computeSequestrationRootBiomass(myIdClimate);
    sequestrationCarbonCO2Eq += sequestrationRoot;
    sequestrationCarbonCO2EqResidue[0] = SoilManagement::computeSequestrationResidueIncorporation2(residues[0],dryMatterResidues[0],isIncorporatedResidue[0],0)* FROM_C_TO_CO2;
    sequestrationCarbonCO2EqResidue[1] = SoilManagement::computeSequestrationResidueIncorporation2(residues[1],dryMatterResidues[1],isIncorporatedResidue[1],0)* FROM_C_TO_CO2;
    sequestrationCarbonCO2EqResidue[2] = SoilManagement::computeSequestrationResidueIncorporation2(residues[2],dryMatterResidues[2],isIncorporatedResidue[2],1)* FROM_C_TO_CO2;
    sequestrationCarbonCO2EqResidue[3] = SoilManagement::computeSequestrationResidueIncorporation2(residues[3],dryMatterResidues[3],isIncorporatedResidue[3],1)* FROM_C_TO_CO2;
    sequestrationCarbonCO2EqResidueWood = sequestrationCarbonCO2EqResidue[0] + sequestrationCarbonCO2EqResidue[1];
    sequestrationCarbonCO2EqResidueGreen = sequestrationCarbonCO2EqResidue[2] + sequestrationCarbonCO2EqResidue[3];
    sequestrationCarbonCO2Eq += SoilManagement::computeSequestrationResidueIncorporation2(residues[0],dryMatterResidues[0],isIncorporatedResidue[0],0)* FROM_C_TO_CO2;
    sequestrationCarbonCO2Eq += SoilManagement::computeSequestrationResidueIncorporation2(residues[1],dryMatterResidues[1],isIncorporatedResidue[1],0)* FROM_C_TO_CO2;
    sequestrationCarbonCO2Eq += SoilManagement::computeSequestrationResidueIncorporation2(residues[2],dryMatterResidues[2],isIncorporatedResidue[2],1)* FROM_C_TO_CO2;
    sequestrationCarbonCO2Eq += SoilManagement::computeSequestrationResidueIncorporation2(residues[3],dryMatterResidues[3],isIncorporatedResidue[3],1)* FROM_C_TO_CO2;
    //sequestrationCarbonCO2Eq += SoilManagement::computeSequestrationDueToOrganicMamagement();
}

double SoilManagement::computeSequestrationTillage(int myIdClimate)
{
    double matrixElement;
    double incrementNoTillage;
    double incrementMinimumTillage;
    double incrementTillage;
    // no Tillage
    matrixElement = soilTillage[myIdClimate].matrix[2][0];
    incrementNoTillage = 1 + 1./20.*(matrixElement - 1);
    // minimum Tillage
    matrixElement = soilTillage[myIdClimate].matrix[2][1];
    incrementMinimumTillage =  1 + 1./20.*(matrixElement - 1);
    return incrementTillage = incrementNoTillage*percentage.noTillage/100. + incrementMinimumTillage*percentage.minimumTillage/100. + (100 - percentage.noTillage - percentage.minimumTillage)/100.;
}

double SoilManagement::computeSequestrationLandUse(int myIdClimate)
{
    double matrixElement;
    double incrementForest;
    double incrementGrass;
    double incrementLandUse;
    // area dedicated to forestry
    matrixElement = soilLandUse[myIdClimate].matrix[2][0];
    incrementForest = 1 + 1./20.*(matrixElement - 1);

    matrixElement = soilLandUse[myIdClimate].matrix[2][1];
    incrementGrass =  1 + 1./20.*(matrixElement - 1);

    return incrementLandUse = incrementForest*percentage.forest/100. + incrementGrass*percentage.permanentGrass/100. + (100 - percentage.forest - percentage.permanentGrass)/100.;


}


double SoilManagement::computeSequestrationCoverCropping(int myIdClimate)
{
    double matrixElement;
    double increment;
    double incrementCoverCropping;

    matrixElement = soilCoverCropping[myIdClimate].matrix[0][1];
    increment = 1 + 1./20.*(matrixElement - 1);

    return incrementCoverCropping = increment*percentage.coverCropping /100. + (100 - percentage.coverCropping)/100.;
}

double SoilManagement::computeSequestrationOrganicAmendments(double amountOfAmendments, double incrementalParameterAmendment,double recalcitrantIndex)
{
    double increment;
    increment = 1 + incrementalParameterAmendment* amountOfAmendments*(1-recalcitrantIndex)/1000;
    return increment;

}

double SoilManagement::computeSequestrationResidueIncorporation(double residueIncorporated, double percentageDryMatter, bool isIncorporatedResidue,int indexRecalcitrant)
{
    if (!isIncorporatedResidue) return 1;
    double increment;
    double slopeFreshWeight;
    if (indexRecalcitrant == 0)
    slopeFreshWeight = 0.00001 *(1 + 57.7/14.7) * percentageDryMatter + 0.0015; // an average with 32.6% decay with 25% lignin and 75% cellulose
    else slopeFreshWeight = 0.00002 * percentageDryMatter + 0.0015;
    increment =1 + slopeFreshWeight*residueIncorporated;
    return increment;
}

double SoilManagement::computeSequestrationResidueIncorporation2(double residueIncorporated, double percentageDryMatter, bool isIncorporatedResidue,int isHerbaceous)
{
    double sequestration = 0 ;
    if (!isIncorporatedResidue) return 0;
    if (isHerbaceous == 0)
    {
        residueIncorporated += 1*0.01*percentage.forest;//1 t/ha of wood incorporated
        sequestration = -0.5*residueIncorporated *1000*(0.577+0.147)*0.5; //  supposing  lignin = cellulose (content)values from Ye et al. 2012
        if (sequestration < 0) return sequestration;
        else return 0;
    }
    else
    {
        // 0.2 t/ha of forest leaves, 0.5 t/ha of permanent grass, 0.5 t/ha of cover cropping
        residueIncorporated += 0.01*(0.2*percentage.forest+0.5*percentage.permanentGrass + 0.5*percentage.arable*0.01*percentage.coverCropping);
        sequestration = -0.45*residueIncorporated*1000*0.147;
        if (sequestration < 0) return sequestration;
        else return 0;
    }
}
