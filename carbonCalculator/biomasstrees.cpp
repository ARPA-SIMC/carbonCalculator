#include "stdio.h"
#include "math.h"
#include "biomasstrees.h"


double BiomassTree::getVolume(double height,double diameter)
{
    return turnellParameter*height*diameter*diameter;
}

double BiomassTree::getBiomass(double height,double diameter)
{
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

}

double BiomassTree::annualGrowthHeight()
{

}

