#include "stdio.h"
#include "math.h"
#include "biomasstrees.h"


double BiomassTree::getVolume(double height,double diameter)
{
    return turnellParameter*height*diameter*diameter;
}

double BiomassTree::getBiomass(double height,double diameter)
{
    timberDensity = 0.6;
    return timberDensity*getVolume(height,diameter);
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
    return 0;
}

double BiomassTree::annualGrowthHeight()
{
    return 0;
}

double BiomassTree::annualCarbonGain(double height, double diameter, int isOrchard, int nrPlants)
{
    double carbon;
    double carbonCO2Eq=0;
    treeDensity = nrPlants;
    if (isOrchard == 1)
    {
        carbon = 0.5 * BiomassTree::getBiomassOrchard(height,diameter);
        carbon -= 0.5* BiomassTree::getBiomassOrchardLastYear(height,diameter);
    }
    else
    {
        carbon = 0.5 * 85 * timberDensity/1000.;  // from Nader et al. 2019
    }
    carbon *= 1.5; //  [g] adding woody roots;
    carbonCO2Eq = FROM_C_TO_CO2 * carbon * nrPlants; // stored in trunk
    return -carbonCO2Eq;
}

