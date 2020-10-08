#include <QCoreApplication>
#include <stdio.h>
#include <math.h>
#include "carbonCalculator.h"

#define NODATA -9999

//cropResidueManagement cropRes;
carbonCalculator calculatorCO2;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    // input
    // int cropType = 1;
    // int residueType = 1;
    double residueWeight = 5;
    //cropRes.computeEquivalentCO2(residueWeight);
    calculatorCO2.cropResidue.computeEquivalentCO2(residueWeight);
    // output
    /*
    double kgCO2EqDueToN2O = NODATA;
    double kgCO2EqDueToCO2 = NODATA;
    double kgCO2EqDueToCH4 = NODATA;
    double kgCO2EqTotal = NODATA;

    double parameterEquivalentCH4 = 25;
    double parameterEquivalentN2O = 296;
    // parameters to be read from DB
    double parameterDryMatterFraction = 0.9;
    double parameterSlope = 0.29;
    double parameterIntercept = 0;
    double parameterAboveGroundContentN = 0.027;
    double parameterBelowAboveRatio = 0.4;
    double parameterBelowGroundContentN = 0.019;
    int isTreeCrop = 1;

    double parameterEmissionCH4 = 0.065333333;
    double parameterEmissionN2O = 	0.00050675;
    double parameterResidueReconvertedToCO2 = 10;

    double aboveGroundNitrogen;
    aboveGroundNitrogen = parameterAboveGroundContentN*residueWeight;
    double belowGroundResidue;
    belowGroundResidue = parameterBelowAboveRatio*residueWeight;
    double belowGroundNitrogen = belowGroundResidue*parameterBelowGroundContentN;

    double emissionCH4;
    emissionCH4 = parameterEmissionCH4*residueWeight*1000;
    double emissionN2O;
    emissionN2O = parameterEmissionN2O*residueWeight*1000;

    kgCO2EqDueToCH4 = emissionCH4 * parameterEquivalentCH4;
    kgCO2EqDueToN2O = emissionN2O * parameterEquivalentN2O;



    kgCO2EqDueToCO2 = 1000*(parameterDryMatterFraction*residueWeight*parameterResidueReconvertedToCO2/100.);
    printf("%f %f %f\n",kgCO2EqDueToCH4,kgCO2EqDueToN2O,kgCO2EqDueToCO2);
    kgCO2EqTotal = kgCO2EqDueToCO2 + kgCO2EqDueToCH4 + kgCO2EqDueToN2O;
    printf("%f\n",kgCO2EqTotal);*/
    return 0;
}
