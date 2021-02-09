#include "inputOutput.h"



double computeCredits(CarbonCalculator calculatorCO2,int* isAccepted)
{

    double credits;
    double bufferSoil,bufferBiomass;
    bufferSoil = computeBufferSoil(calculatorCO2.nrConservativePracticesAdopted);
    bufferBiomass = computeBufferBiomass(calculatorCO2.nrConservativePracticesAdopted);
    credits = bufferSoil*calculatorCO2.carbonBudgetWholeFieldSoil;
    credits += bufferBiomass*calculatorCO2.carbonBudgetWholeFieldBiomass;

    if (calculatorCO2.nrConservativePracticesAdopted <3)
    {
        *isAccepted = 0;
    }
    else
    {
        if ((bufferSoil*calculatorCO2.carbonBudgetPerHectareSoil+bufferBiomass*calculatorCO2.carbonBudgetPerHectareBiomass) <= - 500)
            *isAccepted = 1;
        else
            *isAccepted = 0;
    }

    return credits;
}

double computeBufferSoil(int nrPractices)
{
    static double parameter = 0.25;
    return (1 - (1/(1+exp(parameter*nrPractices))));
}


double computeBufferBiomass(int nrPractices)
{
    static double parameter = 2;
    double function;
    nrPractices -= 4;
    function = 0.1*(1 - (1/(1+exp(parameter*(nrPractices)))));
    function += 0.8;
    return function;
}
