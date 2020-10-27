#include "carbonCalculator.h"
#include <math.h>
#include <stdio.h>


/* class CropResidueManagement */
/*void CropResidueManagement::setInput(double emissionCH4, double emissionN2O, double dryMatterToCO2)
{
    cropResidueParameter.residueReconvertedToCO2 = dryMatterToCO2;
    cropResidueParameter.emissionCH4 = emissionCH4;
    cropResidueParameter.emissionN2O = emissionN2O;

}*/

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

    double amountNitrogen[4];
    double amountOtherElements[4];
    double amountCarbon[4];
    double producedN2O[4];
    double producedNO[4];

    for (int i=0;i<4;i++)
    {
        amountNitrogen[i] = 0;
        amountOtherElements[i] = 0;
        amountCarbon[i] = 0;
        producedN2O[i] = 0;
        producedNO[i] = 0;

    }

    for (int i=0;i<4;i++)
    {
        amountNitrogen[i] = amountFertiliser[i]*fertInput[i].contentElement.nitrogen;
        amountOtherElements[i] = amountFertiliser[i] * (fertInput[i].contentElement.phosphorus + fertInput[i].contentElement.potassium);
        amountCarbon[i] = amountFertiliser[i]*fertInput[i].contentElement.carbon;
        producedN2O[i] = fertInput[i].bouwmanN2O*amountNitrogen[i];
        producedNO[i] = fertInput[i].bouwmanNO*amountNitrogen[i];
    }
    // qui ne manca un pezzo ancora!!!!!
    double producedNH3[4];
    double sumOfEnvironmentalFactorsToComputeNH3;
    sumOfEnvironmentalFactorsToComputeNH3 = bouwmanParameterNH4.modelParameter + bouwmanParameterNH4.drainage
            + bouwmanParameterNH4.drainage + bouwmanParameterNH4.cationicExchangeCapacity
            + bouwmanParameterNH4.climate + bouwmanParameterNH4.cropType
            + bouwmanParameterNH4.pH + bouwmanParameterNH4.soilOrganicCarbon
            + bouwmanParameterNH4.soilTexture;
    for (int i=0;i<4;i++)
    {
        producedNH3[i] = exp(bouwmanParameterNH4ApplicationMethod[i]
                             + fertInput[i].bouwmanNH3 + sumOfEnvironmentalFactorsToComputeNH3);
        producedNH3[i] *= amountNitrogen[i];
    }
    double subTotalEmissionN2OBackground;
    double subTotalEmissionNOBackground;
    double subTotalEmissionN2OFertilisers;
    double subTotalEmissionNOFertilisers;
    double subTotalEmissionNH4Fertilisers;
    double subTotalEmissionLeachingFertilisers;

    double totalEmissionSoilFertiliser;

    subTotalEmissionN2OBackground = exp(bouwmanParameterN2O.modelParameter + bouwmanParameterN2O.drainage
                                        + bouwmanParameterN2O.drainage + bouwmanParameterN2O.cationicExchangeCapacity
                                        + bouwmanParameterN2O.climate + bouwmanParameterN2O.cropType
                                        + bouwmanParameterN2O.pH + bouwmanParameterN2O.soilOrganicCarbon
                                        + bouwmanParameterN2O.soilTexture);

    subTotalEmissionNOBackground = exp(bouwmanParameterNO.modelParameter + bouwmanParameterNO.drainage
                                        + bouwmanParameterNO.drainage + bouwmanParameterNO.cationicExchangeCapacity
                                        + bouwmanParameterNO.climate + bouwmanParameterNO.cropType
                                        + bouwmanParameterNO.pH + bouwmanParameterNO.soilOrganicCarbon
                                        + bouwmanParameterNO.soilTexture);



}


void CarbonCalculator::initializeBouwmanTables()
{
   bouwmanTableNH4.elementParameter = 0;
   bouwmanTableN2O.elementParameter = -0.4136;
   bouwmanTableNO.elementParameter = -1.527;

   for (int i=0;i<6;i++)
   {
        bouwmanTableN2O.cationicExchangeCapacity[i] = NODATA;
        bouwmanTableNO.cationicExchangeCapacity[i] = NODATA;
        bouwmanTableNH4.cationicExchangeCapacity[i] = NODATA;

        bouwmanTableN2O.climate[i] = NODATA;
        bouwmanTableNO.climate[i] = NODATA;
        bouwmanTableNH4.climate[i] = NODATA;

        bouwmanTableN2O.croptype[i] = NODATA;
        bouwmanTableNO.croptype[i] = NODATA;
        bouwmanTableNH4.croptype[i] = NODATA;

        bouwmanTableN2O.drainage[i] = NODATA;
        bouwmanTableNO.drainage[i] = NODATA;
        bouwmanTableNH4.drainage[i] = NODATA;

        bouwmanTableN2O.pH[i] = NODATA;
        bouwmanTableNO.pH[i] = NODATA;
        bouwmanTableNH4.pH[i] = NODATA;

        bouwmanTableN2O.soilOrganicCarbon[i] = NODATA;
        bouwmanTableNO.soilOrganicCarbon[i] = NODATA;
        bouwmanTableNH4.soilOrganicCarbon[i] = NODATA;

        bouwmanTableN2O.soilTexture[i] = NODATA;
        bouwmanTableNO.soilTexture[i] = NODATA;
        bouwmanTableNH4.soilTexture[i] = NODATA;
   }
   int i = 0;
   bouwmanTableNH4.croptype[i++] = -0.158;
   bouwmanTableNH4.croptype[i++] = -0.158;
   bouwmanTableNH4.croptype[i++] = -0.045;
   bouwmanTableNH4.croptype[i++] = -0.045;
   bouwmanTableNH4.croptype[i++] = 0;
   i = 0;
   bouwmanTableNH4.soilTexture[i++] = 0;
   bouwmanTableNH4.soilTexture[i++] = 0;
   bouwmanTableNH4.soilTexture[i++] = 0;
   i = 0;
   bouwmanTableNH4.soilOrganicCarbon[i++] = 0;
   bouwmanTableNH4.soilOrganicCarbon[i++] = 0;
   bouwmanTableNH4.soilOrganicCarbon[i++] = 0;
   bouwmanTableNH4.soilOrganicCarbon[i++] = 0;
   i = 0;
   bouwmanTableNH4.pH[i++] = -1.072;
   bouwmanTableNH4.pH[i++] = -0.933;
   bouwmanTableNH4.pH[i++] = -0.608;
   bouwmanTableNH4.pH[i++] = 0;
   i = 0;
   bouwmanTableNH4.cationicExchangeCapacity[i++] = 0.088;
   bouwmanTableNH4.cationicExchangeCapacity[i++] = 0.012;
   bouwmanTableNH4.cationicExchangeCapacity[i++] = 0.163;
   bouwmanTableNH4.cationicExchangeCapacity[i++] = 0;
   i = 0;
   bouwmanTableNH4.drainage[i++] = 0;
   bouwmanTableNH4.drainage[i++] = 0;
   i = 0;
   bouwmanTableNH4.climate[i++] = -0.402;
   bouwmanTableNH4.climate[i++] = 0;


   i = 0;
   bouwmanTableN2O.croptype[i++] = -1.268;
   bouwmanTableN2O.croptype[i++] = -1.242;
   bouwmanTableN2O.croptype[i++] = -0.023;
   bouwmanTableN2O.croptype[i++] = 0;
   bouwmanTableN2O.croptype[i++] = -2.536;
   i = 0;
   bouwmanTableN2O.soilTexture[i++] = 0;
   bouwmanTableN2O.soilTexture[i++] = -0.472;
   bouwmanTableN2O.soilTexture[i++] = -0.008;
   i = 0;
   bouwmanTableN2O.soilOrganicCarbon[i++] = 0;
   bouwmanTableN2O.soilOrganicCarbon[i++] = 0.14;
   bouwmanTableN2O.soilOrganicCarbon[i++] = 0.58;
   bouwmanTableN2O.soilOrganicCarbon[i++] = 1.045;
   i = 0;
   bouwmanTableN2O.pH[i++] = 0;
   bouwmanTableN2O.pH[i++] = 0.109;
   bouwmanTableN2O.pH[i++] = -0.352;
   bouwmanTableN2O.pH[i++] = -0.352;
   i = 0;
   bouwmanTableN2O.cationicExchangeCapacity[i++] = 0.;
   bouwmanTableN2O.cationicExchangeCapacity[i++] = 0.;
   bouwmanTableN2O.cationicExchangeCapacity[i++] = 0.;
   bouwmanTableN2O.cationicExchangeCapacity[i++] = 0;
   i = 0;
   bouwmanTableN2O.drainage[i++] = 0;
   bouwmanTableN2O.drainage[i++] = -0.42;
   i = 0;
   bouwmanTableN2O.climate[i++] = 0;
   bouwmanTableN2O.climate[i++] = 0.824;

   i = 0;
   bouwmanTableNO.croptype[i++] = 0;
   bouwmanTableNO.croptype[i++] = 0;
   bouwmanTableNO.croptype[i++] = 0;
   bouwmanTableNO.croptype[i++] = 0;
   bouwmanTableNO.croptype[i++] = 0;
   i = 0;
   bouwmanTableNO.soilTexture[i++] = 0;
   bouwmanTableNO.soilTexture[i++] = 0;
   bouwmanTableNO.soilTexture[i++] = 0;
   i = 0;
   bouwmanTableNO.soilOrganicCarbon[i++] = 0;
   bouwmanTableNO.soilOrganicCarbon[i++] = 0;
   bouwmanTableNO.soilOrganicCarbon[i++] = 2.571;
   bouwmanTableNO.soilOrganicCarbon[i++] = 2.571;
   i = 0;
   bouwmanTableNO.pH[i++] = 0;
   bouwmanTableNO.pH[i++] = 0;
   bouwmanTableNO.pH[i++] = 0;
   bouwmanTableNO.pH[i++] = 0;
   i = 0;
   bouwmanTableNO.cationicExchangeCapacity[i++] = 0.;
   bouwmanTableNO.cationicExchangeCapacity[i++] = 0.;
   bouwmanTableNO.cationicExchangeCapacity[i++] = 0.;
   bouwmanTableNO.cationicExchangeCapacity[i++] = 0;
   i = 0;
   bouwmanTableNO.drainage[i++] = 0;
   bouwmanTableNO.drainage[i++] = 0.946;
   i = 0;
   bouwmanTableNO.climate[i++] = 0;
   bouwmanTableNO.climate[i++] = 0;

}

void CarbonCalculator::computeEmissions()
{
    energy.computeEmissions();
    pesticide.computeEmissions();
    cropResidue.computeEmissions();
    fertiliser.computeEmissions();
}

bool CarbonCalculator::initialiazeVariables(QString idDrainage,double pH,double CEC,QString idSoilTexture,QString idSoilOrganicCarbon)
{
    initializeBouwmanTables();
    fertiliser.setNitrogenInhibitorsTable();
    fertiliser.bouwmanParameterN2O.modelParameter = bouwmanTableN2O.elementParameter;
    fertiliser.bouwmanParameterNO.modelParameter = bouwmanTableNO.elementParameter;
    fertiliser.bouwmanParameterNH4.modelParameter = bouwmanTableNH4.elementParameter;
    // ***********************************************************************
    // drainage
    if (idDrainage == "POOR")
    {
        fertiliser.bouwmanParameterN2O.drainage = bouwmanTableN2O.drainage[0];
        fertiliser.bouwmanParameterNO.drainage = bouwmanTableNO.drainage[0];
        fertiliser.bouwmanParameterNH4.drainage = bouwmanTableNH4.drainage[0];
    }
    else
    {
        fertiliser.bouwmanParameterN2O.drainage = bouwmanTableN2O.drainage[1];
        fertiliser.bouwmanParameterNO.drainage = bouwmanTableNO.drainage[1];
        fertiliser.bouwmanParameterNH4.drainage = bouwmanTableNH4.drainage[1];
    }

    // *************************************************************************
    //SOM

    double somParameterForCec;
    if(idSoilOrganicCarbon == "SOM<=1.72")
    {
        fertiliser.bouwmanParameterN2O.soilOrganicCarbon = bouwmanTableN2O.soilOrganicCarbon[0];
        fertiliser.bouwmanParameterNO.soilOrganicCarbon = bouwmanTableNO.soilOrganicCarbon[0];
        fertiliser.bouwmanParameterNH4.soilOrganicCarbon = bouwmanTableNH4.soilOrganicCarbon[0];
        somParameterForCec = 30;
    }
    else if (idSoilOrganicCarbon == "1.72<SOM<=5.16")
    {
        fertiliser.bouwmanParameterN2O.soilOrganicCarbon = bouwmanTableN2O.soilOrganicCarbon[1];
        fertiliser.bouwmanParameterNO.soilOrganicCarbon = bouwmanTableNO.soilOrganicCarbon[1];
        fertiliser.bouwmanParameterNH4.soilOrganicCarbon = bouwmanTableNH4.soilOrganicCarbon[1];
        somParameterForCec = 60;
    }
    else if (idSoilOrganicCarbon == "5.16<SOM<=10.32")
    {
        fertiliser.bouwmanParameterN2O.soilOrganicCarbon = bouwmanTableN2O.soilOrganicCarbon[2];
        fertiliser.bouwmanParameterNO.soilOrganicCarbon = bouwmanTableNO.soilOrganicCarbon[2];
        fertiliser.bouwmanParameterNH4.soilOrganicCarbon = bouwmanTableNH4.soilOrganicCarbon[2];
        somParameterForCec = 135;
    }
    else if (idSoilOrganicCarbon == "SOM>10.32")
    {
        fertiliser.bouwmanParameterN2O.soilOrganicCarbon = bouwmanTableN2O.soilOrganicCarbon[3];
        fertiliser.bouwmanParameterNO.soilOrganicCarbon = bouwmanTableNO.soilOrganicCarbon[3];
        fertiliser.bouwmanParameterNH4.soilOrganicCarbon = bouwmanTableNH4.soilOrganicCarbon[3];
        somParameterForCec = 180;
    }

    //**************************************************************************
    // pH

    if (pH < 0 ) return false;
    if (pH > 14) return false;

    if(pH < 5.5)
    {
        fertiliser.bouwmanParameterN2O.pH = bouwmanTableN2O.pH[0];
        fertiliser.bouwmanParameterNO.pH = bouwmanTableNO.pH[0];
        fertiliser.bouwmanParameterNH4.pH = bouwmanTableNH4.pH[0];
    }
    else if (pH >= 5.5 && pH < 7.3)
    {
        fertiliser.bouwmanParameterN2O.pH = bouwmanTableN2O.pH[1];
        fertiliser.bouwmanParameterNO.pH = bouwmanTableNO.pH[1];
        fertiliser.bouwmanParameterNH4.pH = bouwmanTableNH4.pH[1];
    }
    else if (pH >= 7.3 && pH < 8.5)
    {
        fertiliser.bouwmanParameterN2O.pH = bouwmanTableN2O.pH[2];
        fertiliser.bouwmanParameterNO.pH = bouwmanTableNO.pH[2];
        fertiliser.bouwmanParameterNH4.pH = bouwmanTableNH4.pH[2];
    }
    else
    {
        fertiliser.bouwmanParameterN2O.pH = bouwmanTableN2O.pH[3];
        fertiliser.bouwmanParameterNO.pH = bouwmanTableNO.pH[3];
        fertiliser.bouwmanParameterNH4.pH = bouwmanTableNH4.pH[3];
    }

    // ******************************************************************************
    // soil Texture
    int index;
    double textureParameterForCec;
    if (idSoilTexture == "FINE")
    {
        index = 0;
        textureParameterForCec = 0.6;
    }
    else if (idSoilTexture == "MEDIUM")
    {
        index = 1;
        textureParameterForCec = 0.3;
    }
    else if (idSoilTexture == "COARSE")
    {
        index = 2;
        textureParameterForCec = 0.15;
    }
    else
    {
        return false;
    }

        fertiliser.bouwmanParameterN2O.soilTexture = bouwmanTableN2O.soilTexture[index];
        fertiliser.bouwmanParameterNO.soilTexture = bouwmanTableNO.soilTexture[index];
        fertiliser.bouwmanParameterNH4.soilTexture = bouwmanTableNH4.soilTexture[index];
// *********************************************************************
        // cationic exchange capacity
        double cec;

        cec = 59 - 51*(somParameterForCec)/3000 + (30 + 4.4)*textureParameterForCec;

        if (cec < 16)
        {
            fertiliser.bouwmanParameterN2O.cationicExchangeCapacity = bouwmanTableN2O.cationicExchangeCapacity[0];
            fertiliser.bouwmanParameterNO.cationicExchangeCapacity = bouwmanTableNO.cationicExchangeCapacity[0];
            fertiliser.bouwmanParameterNH4.cationicExchangeCapacity = bouwmanTableNH4.cationicExchangeCapacity[0];
        }
        else if (cec >= 16 && cec < 24)
        {
            fertiliser.bouwmanParameterN2O.cationicExchangeCapacity = bouwmanTableN2O.cationicExchangeCapacity[1];
            fertiliser.bouwmanParameterNO.cationicExchangeCapacity = bouwmanTableNO.cationicExchangeCapacity[1];
            fertiliser.bouwmanParameterNH4.cationicExchangeCapacity = bouwmanTableNH4.cationicExchangeCapacity[1];
        }
        else if (cec >= 24 && cec < 32)
        {
            fertiliser.bouwmanParameterN2O.cationicExchangeCapacity = bouwmanTableN2O.cationicExchangeCapacity[2];
            fertiliser.bouwmanParameterNO.cationicExchangeCapacity = bouwmanTableNO.cationicExchangeCapacity[2];
            fertiliser.bouwmanParameterNH4.cationicExchangeCapacity = bouwmanTableNH4.cationicExchangeCapacity[2];
        }
        else if (cec >= 32)
        {
            fertiliser.bouwmanParameterN2O.cationicExchangeCapacity = bouwmanTableN2O.cationicExchangeCapacity[3];
            fertiliser.bouwmanParameterNO.cationicExchangeCapacity = bouwmanTableNO.cationicExchangeCapacity[3];
            fertiliser.bouwmanParameterNH4.cationicExchangeCapacity = bouwmanTableNH4.cationicExchangeCapacity[3];
        }
// *********************************************************************
// cropType is filled in dbQueries.cpp


    return true;
}

