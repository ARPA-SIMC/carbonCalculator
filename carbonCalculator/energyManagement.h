#ifndef ENERGYMANAGEMENT_H
#define ENERGYMANAGEMENT_H

#include "basicStructures.h"
#include <QString>

    class EnergyManagement{

    public:
        // variables
        QString country;
        double percentageRenewablesInGrid;

        TkindOfEnergy emissions;
        TkindOfEnergy input;
        TparameterElectricity parameterElectricity;
        TparameterFuel parameterFuel;

        // constructor
        EnergyManagement();

        // functions
        void setInput(TkindOfEnergy input, double renewablesPercentage, QString myCountry,int year);
        void computeEmissions();
        double electricityEmissionComputation(double input,double parameter);
        double fuelEmissionComputation(double input);
        void parametersEmissionElectricity();

        //void getEmissions(TkindOfEnergy output);

    };


#endif // ENERGYMANAGEMENT_H
