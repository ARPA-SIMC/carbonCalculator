#include "fertilisationandsoil.h"

#include "stdio.h"
#include "math.h"



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
    double sumProducedN2O = 0;
    double sumProducedNO = 0;

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
        sumProducedN2O += producedN2O[i];
        sumProducedNO += producedNO[i];

    }

    double producedNH3[4];
    double sumProducedNH3=0;
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
        sumProducedNH3 += producedNH3[i];
    }

    double subTotalEmissionN2OBackground;
    double subTotalEmissionNOBackground;
    double subTotalEmissionN2OFertilisers;
    double subTotalEmissionNOFertilisers;
    double subTotalEmissionNH3Fertilisers;
    double subTotalEmissionLeachingFertilisers;
    double totalEmissionSoilNitrogen;

    double sumOfEnvironmentalFactorsToComputeN2O,sumOfEnvironmentalFactorsToComputeNO;
    sumOfEnvironmentalFactorsToComputeN2O = bouwmanParameterN2O.modelParameter + bouwmanParameterN2O.drainage
            + bouwmanParameterN2O.drainage + bouwmanParameterN2O.cationicExchangeCapacity
            + bouwmanParameterN2O.climate + bouwmanParameterN2O.cropType
            + bouwmanParameterN2O.pH + bouwmanParameterN2O.soilOrganicCarbon
            + bouwmanParameterN2O.soilTexture;
    subTotalEmissionN2OBackground = exp(sumOfEnvironmentalFactorsToComputeN2O);
    sumOfEnvironmentalFactorsToComputeNO = bouwmanParameterNO.modelParameter + bouwmanParameterNO.drainage
            + bouwmanParameterNO.drainage + bouwmanParameterNO.cationicExchangeCapacity
            + bouwmanParameterNO.climate + bouwmanParameterNO.cropType
            + bouwmanParameterNO.pH + bouwmanParameterNO.soilOrganicCarbon
            + bouwmanParameterNO.soilTexture;
    subTotalEmissionNOBackground = 0.01*exp(sumOfEnvironmentalFactorsToComputeNO);

    double inhibitorWeightN2O = 1;
    double inhibitorWeightNO = 1;
    double sumAmountNitrogenFromFertilizer = 0;
    sumAmountNitrogenFromFertilizer = (amountNitrogen[0] + amountNitrogen[1] + amountNitrogen[2] + amountNitrogen[3]);
    if (sumAmountNitrogenFromFertilizer > 0.00001)
    {
        inhibitorWeightN2O = 0;
        inhibitorWeightNO = 0;
        for (int i=0;i<4;i++)
        {
            inhibitorWeightN2O += inhibitorN2O[i]*amountNitrogen[i];
            inhibitorWeightNO += inhibitorNO[i]*amountNitrogen[i];
        }
        inhibitorWeightN2O /= (amountNitrogen[0] + amountNitrogen[1] + amountNitrogen[2] + amountNitrogen[3]);
        inhibitorWeightNO /= (amountNitrogen[0] + amountNitrogen[1] + amountNitrogen[2] + amountNitrogen[3]);
    }
    subTotalEmissionN2OBackground *= inhibitorWeightN2O;
    subTotalEmissionNOBackground *= inhibitorWeightNO;
    subTotalEmissionN2OFertilisers = exp(sumOfEnvironmentalFactorsToComputeN2O + sumProducedN2O)*inhibitorWeightN2O - subTotalEmissionN2OBackground;
    subTotalEmissionNOFertilisers = 0.01*exp(sumOfEnvironmentalFactorsToComputeNO + sumProducedNO)*inhibitorWeightNO - subTotalEmissionNOBackground;

    subTotalEmissionNH3Fertilisers = 0.01*sumProducedNH3;
    subTotalEmissionLeachingFertilisers = 0.01*sumAmountNitrogenFromFertilizer*leachingParameterDueToClimate;
    totalEmissionSoilNitrogen = subTotalEmissionLeachingFertilisers + subTotalEmissionNH3Fertilisers
            + subTotalEmissionNOFertilisers + subTotalEmissionN2OFertilisers
            + subTotalEmissionNOBackground + subTotalEmissionN2OBackground;
    totalEmissionSoilNitrogen *= 1.571428571; // conversion into kg of N2O units
    emissionDueToSoil = (subTotalEmissionNOBackground + subTotalEmissionN2OBackground)*1.571428571;
    emissionDueToFertiliserApplication = totalEmissionSoilNitrogen - emissionDueToSoil;
    emissionDueToSoil *= EQUIVALENTN2OTOCO2; // conversion to kg CO2Eq
    emissionDueToFertiliserApplication *= EQUIVALENTN2OTOCO2; // conversion to kg CO2Eq
    // emission due to fertiliser Production

    emissionDueToFertiliserProduction = 0; // initialization
    for (int i=0;i<4;i++)
    {
        emissionDueToFertiliserProduction += amountFertiliser[i]*fertInput[i].emissionPerKgOfProduct;
    }
    sequestrationDueToFertiliserApplication = 0;
    for (int i=0;i<4;i++)
    {
        sequestrationDueToFertiliserApplication += amountCarbon[i];
    }
    sequestrationDueToFertiliserApplication *= FROM_C_TO_CO2; // in CO2 Units
}
