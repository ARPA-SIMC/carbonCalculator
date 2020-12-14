#include "carbonCalculator.h"

#include <math.h>
#include <stdio.h>


/* class CropResidueManagement */

void CropResidueManagement::computeEmissions()
{
    double woodParameterReduction =  0.1;
    aboveGroundNitrogen = cropResidueParameter.aboveGroundContentN  * residueWeight[0];
    aboveGroundNitrogen += cropResidueParameter.aboveGroundContentN  * residueWeight[1];
    belowGroundResidue = cropResidueParameter.belowAboveRatio*residueWeight[0];
    belowGroundResidue += cropResidueParameter.belowAboveRatio*residueWeight[1];
    emissionCH4inCH4Units = cropResidueParameter.emissionCH4[0]*residueWeight[0]*1000*woodParameterReduction;
    emissionCH4inCH4Units += cropResidueParameter.emissionCH4[1]*residueWeight[1]*1000*woodParameterReduction;
    emissionN2OinN2OUnits = cropResidueParameter.emissionN2O[0]*residueWeight[0]*1000*woodParameterReduction;
    emissionN2OinN2OUnits += cropResidueParameter.emissionN2O[1]*residueWeight[1]*1000*woodParameterReduction;
    kgCO2Equivalent.fromCH4 = emissionCH4inCH4Units * EQUIVALENTCH4TOCO2;
    kgCO2Equivalent.fromN2O = emissionN2OinN2OUnits * EQUIVALENTN2OTOCO2;
    kgCO2Equivalent.fromCO2 = 1000*(cropResidueParameter.dryMatterFraction[0]*residueWeight[0]*cropResidueParameter.residueReconvertedToCO2[0]/100.);
    kgCO2Equivalent.fromCO2 += 1000*(cropResidueParameter.dryMatterFraction[1]*residueWeight[1]*cropResidueParameter.residueReconvertedToCO2[1]/100.);
    kgCO2Equivalent.total = kgCO2Equivalent.fromCH4 + kgCO2Equivalent.fromN2O + kgCO2Equivalent.fromCO2;

    aboveGroundNitrogen = cropResidueParameter.aboveGroundContentN  * residueWeight[2];
    aboveGroundNitrogen += cropResidueParameter.aboveGroundContentN  * residueWeight[3];
    belowGroundResidue = cropResidueParameter.belowAboveRatio*residueWeight[2];
    belowGroundResidue += cropResidueParameter.belowAboveRatio*residueWeight[3];
    emissionCH4inCH4Units = cropResidueParameter.emissionCH4[2]*residueWeight[2]*1000;
    emissionCH4inCH4Units += cropResidueParameter.emissionCH4[3]*residueWeight[3]*1000;
    emissionN2OinN2OUnits = cropResidueParameter.emissionN2O[2]*residueWeight[2]*1000;
    emissionN2OinN2OUnits += cropResidueParameter.emissionN2O[3]*residueWeight[3]*1000;
    kgCO2Equivalent.fromCH4 = emissionCH4inCH4Units * EQUIVALENTCH4TOCO2;
    kgCO2Equivalent.fromN2O = emissionN2OinN2OUnits * EQUIVALENTN2OTOCO2;
    kgCO2Equivalent.fromCO2 = 1000*(cropResidueParameter.dryMatterFraction[2]*residueWeight[2]*cropResidueParameter.residueReconvertedToCO2[2]/100.);
    kgCO2Equivalent.fromCO2 += 1000*(cropResidueParameter.dryMatterFraction[3]*residueWeight[3]*cropResidueParameter.residueReconvertedToCO2[3]/100.);
    kgCO2Equivalent.total += kgCO2Equivalent.fromCH4 + kgCO2Equivalent.fromN2O + kgCO2Equivalent.fromCO2;
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
    double electricalEnergyRequired = weightOfActivePrinciple * electricalEnergyPerMassOfActivePrinciple * 0.01 * (100. - renewablesInCountry); // requirement in MJ
    emissionDueToProduction = 0.069 * (nonElectricalEnergyRequired + electricalEnergyRequired); // kgCO2eq
}

double SoilErosion::computeSoilLoss()
{
    double R,K,LS,C,P;
    R = 38.5 + 0.35 * erosionFactor.rainfall; // tonnes/ha Merritt et al. (2004)
    K = erosionFactor.texture;
    LS = 0.1 + 0.21 * pow(erosionFactor.slope, 1.33); // David 1988
    C = erosionFactor.cover;
    P = erosionFactor.soilManagement;
    double loss = 1000*R*K*LS*C*P;
    return loss;
}

void SoilErosion::computeCarbonLoss()
{
    lostCO2 = FROM_C_TO_CO2 * 0.01*computeSoilLoss(); // assuming 1% of organic matter in soil
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

void CarbonCalculator::computeBalance()
{
    energy.computeEmissions();
    pesticide.computeEmissions();
    cropResidue.computeEmissions();
    fertiliser.computeEmissions();
    erosion.computeCarbonLoss();
    soilManage.computeEmissions(carbonInTop30CmSoil,idClimate);
    soilManage.computeSequestration(carbonInTop30CmSoil,idClimate,fertiliser.amountFertiliser,fertiliser.recalcitrantCarbonIndex,fertiliser.incrementalParameter ,cropResidue.residueWeight,cropResidue.cropResidueParameter.dryMatterFraction,cropResidue.residueLeftOnField);
    biomassInTree.annualCarbonGain(300,15,1,2000,cropResidue.residueWeight[0]+cropResidue.residueWeight[1]);
    carbonBudgetPerHectareSoil = energy.emissions.total + pesticide.emissionDueToProduction + cropResidue.kgCO2Equivalent.total + fertiliser.emissionDueToSoil
            + soilManage.computeEmissions(carbonInTop30CmSoil,idClimate) + fertiliser.emissionDueToFertiliserProduction + fertiliser.emissionDueToFertiliserApplication
            + erosion.lostCO2 + soilManage.sequestrationCarbonCO2Eq + fertiliser.sequestrationDueToFertiliserApplication ;
    carbonBudgetWholeFieldSoil = carbonBudgetPerHectareSoil*soilManage.fieldSize;
    carbonFootprintPerKgOfProduceSoil = carbonBudgetPerHectareSoil/soilManage.yield;
    carbonBudgetPerHectare = carbonBudgetPerHectareSoil + biomassInTree.annualCarbonGain(200,15,1,3000,cropResidue.residueWeight[0]);
    carbonBudgetWholeField = carbonBudgetPerHectare*soilManage.fieldSize;
    carbonFootprintPerKgOfProduce = carbonBudgetPerHectare/soilManage.yield;

}

bool CarbonCalculator::initialiazeVariables(QString idDrainage,double pH,QString idSoilTexture,QString idSoilOrganicCarbon,QString* idInhibitor)
{
    // bouwman model initialization
    initializeBouwmanTables();
    fertiliser.bouwmanParameterN2O.modelParameter = bouwmanTableN2O.elementParameter;
    fertiliser.bouwmanParameterNO.modelParameter = bouwmanTableNO.elementParameter;
    fertiliser.bouwmanParameterNH4.modelParameter = bouwmanTableNH4.elementParameter;
    // ***********************************************************************
    // drainage
    if (idDrainage == "Poor")
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

    double somParameterForCec = 30;
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
    double bulkDensity;
    if (idSoilTexture == "Fine")
    {
        index = 0;
        textureParameterForCec = 0.6;
        bulkDensity = 1.5;
    }
    else if (idSoilTexture == "Medium")
    {
        index = 1;
        textureParameterForCec = 0.3;
        bulkDensity = 1.3;
    }
    else if (idSoilTexture == "Coarse")
    {
        index = 2;
        textureParameterForCec = 0.15;
        bulkDensity = 1.7;
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
        carbonInTop30CmSoil = somParameterForCec*1000*bulkDensity;
        carbonInTop30CmSoil *= (100.0 - skeleton)*0.01; // reduction due to presence of skeleton
        if (soilDepth < 30)
        {
            carbonInTop30CmSoil *= (soilDepth/30.);
        }
        double cec;
        cec = (59 - 51*pH)*(carbonInTop30CmSoil)/3000000/bulkDensity + (30 + 4.4*pH)*textureParameterForCec;
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


        fertiliser.setNitrogenInhibitorsTable();
        // default initialization

        for (int i=0;i<8;i++)
        {
            if (idInhibitor[i] == "Polymer_coated")
            {
                fertiliser.inhibitorN2O[i] = fertiliser.nitrogenInhibitorN2O.polymerCoated[fertiliser.inhibitorClass];
                fertiliser.inhibitorNO[i] = fertiliser.nitrogenInhibitorNO.polymerCoated[fertiliser.inhibitorClass];
            }
            else if (idInhibitor[i] == "Nitrification_inhibitor")
            {
                fertiliser.inhibitorN2O[i] = fertiliser.nitrogenInhibitorN2O.nitrificationInhibitor[fertiliser.inhibitorClass];
                fertiliser.inhibitorNO[i] = fertiliser.nitrogenInhibitorNO.nitrificationInhibitor[fertiliser.inhibitorClass];
            }
            else
            {
                fertiliser.inhibitorN2O[i] = fertiliser.nitrogenInhibitorN2O.none[fertiliser.inhibitorClass];
                fertiliser.inhibitorNO[i] = fertiliser.nitrogenInhibitorNO.none[fertiliser.inhibitorClass];
            }
        }



    return true;
}

