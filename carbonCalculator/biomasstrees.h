#ifndef BIOMASSTREES_H
#define BIOMASSTREES_H

#include "basicStructures.h"
#include <QString>

    class BiomassTree{

    // refererences for allometric data:
    // Nader et al. 2019 (grapevine)
    // Quinones Oliver & Font 2013 (citrus tree)
    // https://homeguides.sfgate.com/size-apple-tree-rate-growth-57015.html (apple tree)

    public:

    //; // cm questo va reso variabile
    double turrellParameter = 0.5248; // -
    double timberDensity = 0.6;
    double treeDensity;
    double deadTreeDensity;
    double orchardAge;
    double incrementHeight;
    double incrementDiameter;
    double maxDiameter;
    double currentHeight;
    double agePerMaxHeight;
    double agePerMaxDiameter;
    double currentDiameter;
    int isOrchard;
    double annualCarbonWoodyDryMatter;

      //io farei 15 anni 21 cm 5 metri in 5 anni per citrus
      //sentirei vincenzo per gli altri alberi e per la vite

    double annualGrowthDiameter();
    double annualGrowthHeight();
    double getVolume(double height,double diameter);
    double getBiomass(double height,double diameter);
    double getBiomassOrchard(double height,double diameter);
    double getBiomassOrchardLastYear(double height,double diameter);
    double annualCarbonGain(double height, double diameter, int nrPlants, double woodyResidues);
    double annualCarbonGain2(double woodyBiomass, double percentageAccounted,double pruningWeight);
    double woodyCarbonInCO2Eq(double height, double diameter, int nrPlants, double woodyResidues);
    double woodyCarbonInCO2Eq2(double age, double annualWoodyProduction, double percentageAccounted, double pruningWeight);
    double woodyCarbonFromForestInCO2Eq2(double age, double annualCO2Credits, double percentage);
    double annualFromForestCarbonGain2(double annualCO2Credits, double percentage);

    };




#endif // BIOMASSTREES_H
