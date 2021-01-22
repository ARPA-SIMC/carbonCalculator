#include "inputOutput.h"



double computeCredits(CarbonCalculator calculatorCO2,int* isAccepted)
{

    double credits;
    double bufferSoil,bufferBiomass;
    bufferSoil = computeBufferSoil(calculatorCO2.nrConservativePracticesAdopted);
    bufferBiomass = computeBufferBiomass(calculatorCO2.nrConservativePracticesAdopted);
    credits = bufferSoil*calculatorCO2.carbonBudgetPerHectareSoil;
    credits += bufferBiomass*calculatorCO2.carbonBudgetPerHectareBiomass;

    if (calculatorCO2.nrConservativePracticesAdopted <3)
    {
        *isAccepted = 0;
    }
    else
    {
        if (credits <= - 500)
            *isAccepted = 1;
        else
            *isAccepted = 0;
    }

    /*
    PROPOSTA per il calcolo dei crediti:

    crediti totali = buffer_soil*(soil_carbon_budget_field)+buffer_biomass*(biomass_carbon_budget_field)

    buffer_soil=0,5*(0,9*log_10(numero di pratiche); che restituisce valori compresi tra 0,68 per tre pratiche e 0,95 per 10 pratiche
    buffer_biomass=0,9 se pratiche se = >5
                      =0,8 se pratiche <5
    buffer: Risk of non-permanence of the project generating credits, expressed as a percentage (%) of credits.
*/
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
