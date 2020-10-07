#include "carbonCalculator.h"
#include <math.h>
#include <stdio.h>



void cropResidueManagement::setParameters()
{

}

void cropResidueManagement::computeEquivalentCO2(double residueWeight)
{
    aboveGroundNitrogen = cropResidueParameter.aboveGroundContentN  *residueWeight;
    belowGroundResidue = cropResidueParameter.belowAboveRatio*residueWeight;
    emissionCH4inCH4Units = cropResidueParameter.emissionCH4*residueWeight*1000;
    emissionN2OinN2OUnits = cropResidueParameter.emissionN2O*residueWeight*1000;
    kgCO2Equivalent.fromCH4 = emissionCH4inCH4Units * EQUIVALENTCH4TOCO2;
    kgCO2Equivalent.fromN2O = emissionN2OinN2OUnits * EQUIVALENTN2OTOCO2;
    kgCO2Equivalent.fromCO2 = 1000*(cropResidueParameter.dryMatterFraction*residueWeight*cropResidueParameter.residueReconvertedToCO2/100.);
    kgCO2Equivalent.total = kgCO2Equivalent.fromCH4 + kgCO2Equivalent.fromN2O + kgCO2Equivalent.fromCO2;
}

void cropResidueManagement::getEquivalentCO2()
{

}


