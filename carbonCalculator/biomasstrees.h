#ifndef BIOMASSTREES_H
#define BIOMASSTREES_H

#include "basicStructures.h"
#include <QString>

    class BiomassTree{



    public:

    //; // cm questo va reso variabile
    double turnellParameter = 0.5248; // -
    double timberDensity;
    double treeDensity;
    double deadTreeDensity;
    double orchardAge;
    double incrementHeight;
    double maxDiameter;
    double maxHeight;
    double agePerMaxHeight;
    double agePerMaxDiameter;

      //io farei 15 anni 21 cm 5 metri in 5 anni per citrus
      //sentirei vincenzo per gli altri alberi e per la vite

    double annualGrowthDiameter();
    double annualGrowthHeight();
    double getVolume(double height,double diameter);
    double getBiomass(double height,double diameter);
    double getBiomassOrchard(double height,double diameter);
    double getBiomassOrchardLastYear(double height,double diameter);
    };




#endif // BIOMASSTREES_H
