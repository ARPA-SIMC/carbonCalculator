#include "stdio.h"
#include "math.h"
#include "biomasstrees.h"


double BiomassTree::getVolume(double height,double diameter)
{
    return turnellParameter*height*diameter*diameter;
}

double BiomassTree::getBiomass(double height,double diameter)
{
    timberDensity = 0.6; //kg/m3
    return timberDensity*getVolume(height,diameter)/1000;
}

double BiomassTree::getBiomassOrchard(double height,double diameter)
{
    return treeDensity*getBiomass(height,diameter);
}

double BiomassTree::getBiomassOrchardLastYear(double height,double diameter)
{
    diameter -= annualGrowthDiameter();
    height -= annualGrowthHeight();
    return (treeDensity+deadTreeDensity)*getBiomass(height,diameter);
}

double BiomassTree::annualGrowthDiameter()
{
    return 1.5;
}

double BiomassTree::annualGrowthHeight()
{
    return 0;
}

double BiomassTree::annualCarbonGain(double height, double diameter, int isOrchard, int nrPlants,double woodyResidues)
{
    double carbon;
    double carbonCO2Eq=0;
    treeDensity = nrPlants;
    if (isOrchard == 1)
    {
        carbon = 0.5 * BiomassTree::getBiomassOrchard(height,diameter); // [g]
        carbon -= 0.5* BiomassTree::getBiomassOrchardLastYear(height,diameter); //[g]
    }
    else
    {
        carbon = 0.5 * 85 * timberDensity * treeDensity;  // from Nader et al. 2019
    }

    carbon -= 1000*woodyResidues*0.5;
    carbonCO2Eq = FROM_C_TO_CO2 * carbon; // stored in trunk
    return -carbonCO2Eq;
}

