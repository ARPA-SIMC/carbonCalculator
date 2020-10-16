#include "carbonCalculator.h"
#include <math.h>
#include <stdio.h>


/* class CropResidueManagement */
void CropResidueManagement::setInput(double emissionCH4, double emissionN2O, double dryMatterToCO2)
{
    cropResidueParameter.residueReconvertedToCO2 = dryMatterToCO2;
    cropResidueParameter.emissionCH4 = emissionCH4;
    cropResidueParameter.emissionN2O = emissionN2O;

}

void CropResidueManagement::computeEmissions()
{
    aboveGroundNitrogen = cropResidueParameter.aboveGroundContentN  * residueWeight;
    belowGroundResidue = cropResidueParameter.belowAboveRatio*residueWeight;
    emissionCH4inCH4Units = cropResidueParameter.emissionCH4*residueWeight*1000;
    emissionN2OinN2OUnits = cropResidueParameter.emissionN2O*residueWeight*1000;
    kgCO2Equivalent.fromCH4 = emissionCH4inCH4Units * EQUIVALENTCH4TOCO2;
    kgCO2Equivalent.fromN2O = emissionN2OinN2OUnits * EQUIVALENTN2OTOCO2;
    kgCO2Equivalent.fromCO2 = 1000*(cropResidueParameter.dryMatterFraction*residueWeight*cropResidueParameter.residueReconvertedToCO2/100.);
    kgCO2Equivalent.total = kgCO2Equivalent.fromCH4 + kgCO2Equivalent.fromN2O + kgCO2Equivalent.fromCO2;
}


/* class AppliedPesticides */
// constructor
AppliedPesticides::AppliedPesticides()
{
    renewablesInCountry = NODATA;
    emissionDueToProduction = NODATA;
    weightOfActivePrinciple = NODATA;
    energyPerMassOfActivePrinciple = 217.3;             // [MJ/kg]
    electricalEnergyPerMassOfActivePrinciple = 70.83;   // [MJ/kg]
}


void AppliedPesticides::setInput(double myWeightOfActivePrinciple,double renewablePercentageInOwnCountry)
{
    weightOfActivePrinciple = myWeightOfActivePrinciple;
    renewablesInCountry = renewablePercentageInOwnCountry;
}

void AppliedPesticides::computeEmissions()
{
    double nonElectricalEnergyRequired = weightOfActivePrinciple * (energyPerMassOfActivePrinciple - electricalEnergyPerMassOfActivePrinciple); // requirement in MJ
    double electricalEnergyRequired = weightOfActivePrinciple * electricalEnergyPerMassOfActivePrinciple*(100. - renewablesInCountry)/100.; // requirement in MJ
    emissionDueToProduction = 0.069 * (nonElectricalEnergyRequired + electricalEnergyRequired); // kgCO2eq
}


/* class FertiliserApplication */
void FertiliserApplication::setNitrogenInhibitorsTable()
{
    nitrogenInhibitorN2O.none[0] = 1;
    nitrogenInhibitorN2O.none[1] = 1;
    nitrogenInhibitorN2O.nitrificationInhibitor[0] = 0.4;
    nitrogenInhibitorN2O.nitrificationInhibitor[1] = 0.63;
    nitrogenInhibitorN2O.polymerCoated[0] = 0.23;
    nitrogenInhibitorN2O.polymerCoated[1] = 1;

    nitrogenInhibitorNO.none[0] = 1;
    nitrogenInhibitorNO.none[1] = 1;
    nitrogenInhibitorNO.nitrificationInhibitor[0] = 0.54;
    nitrogenInhibitorNO.nitrificationInhibitor[1] = 0.54;
    nitrogenInhibitorNO.polymerCoated[0] = 0.6;
    nitrogenInhibitorNO.polymerCoated[1] = 0.6;
}

void FertiliserApplication::computeEmissions()
{

    double amountNitrogen = amountFertiliser*fertInput.contentElement.nitrogen;
    double amountOtherElements = amountFertiliser * (fertInput.contentElement.phosphorus + fertInput.contentElement.potassium);
    double amountCarbon = amountFertiliser*fertInput.contentElement.carbon;
    double producedN2O = fertInput.bouwmanN2O*amountNitrogen;
    double producedNO = fertInput.bouwmanNO*amountNitrogen;

    // qui ne manca un pezzo ancora!!!!!
    double producedNH3 = exp(bouwmanParameterNH4.applicationMethod + fertInput.bouwmanNH3)*amountNitrogen;

}


void CarbonCalculator::computeEmissions()
{
    energy.computeEmissions();
    pesticide.computeEmissions();
    cropResidue.computeEmissions();
    fertiliser.computeEmissions();
}
