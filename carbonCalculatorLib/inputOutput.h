#ifndef INPUTOUTPUT_H
#define INPUTOUTPUT_H

#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QFile>

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <vector>

#include "carbonCalculator.h"
#include "dbUtilities.h"
#include "dbQueries.h"
#include "csvUtilities.h"

struct TgeneralBuyer{
    QString compilerName;
    QString emailAddress;
    QString enterpriseName;
    QString idCountry;
    QString idRegion;
    QString chainName;
    QString productName;
    QString description;
    int year;
    float latitude;
    float longitude;
    int isPresentLCA;
    float valueLCA;
};

struct Tgeneral{
    QString compilerName;
    QString emailAddress;
    QString enterpriseName;
    QString idCountry;
    QString idRegion;
    QString description;
    int nrField;
    int year;
    float latitude;
    float longitude;
    float fieldSize;
    float fieldSlope;
};
struct Tclimate{
    float meanTemperature;
    float annualRainfall;
    float referenceEvapotranspiration;
    float climaticWaterBalance;
};
struct Tsoil{
    float depth;
    QString drainage;
    float pH;
    QString texture;
    float organicMatter;
    float skeleton;
    float availableWaterCapacity;
    float totalNitrogen;
    float carbonNitrogenRatio;
};
struct TcropFieldManagement{
    QString cropName;
    QString isOrganic;
    float yield;
    float noTillage;
    float minTillage;
    float conventionalTillage;
    float coverCrop;
    float permanentGrass;
    float forest;
    float sparseVegetation;
    QString woodyResidueTreatment[2];
    float woodyResidueWeight[2];
    QString greenResidueTreatment[2];
    float greenResidueWeight[2];
    float treeDensity;
    float deadTreeDensity;
    float orchardAge;
    float treeDBH;
    float treeHeight;

};

struct TagronomicInput{
    float pesticideWeight;
    QString fertilizerName[4];
    float fertilizerAmount[4];
    QString fertilizerApplicationMethod[4];
    QString fertilizerInhibitor[4];
    QString amendmentName[4];
    float amendmentAmount[4];
    QString amendmentApplicationMethod[4];
    QString amendmentInhibitor[4];
};

struct Tenergy{
    float biodiesel;
    float bioethanol;
    float diesel;
    float oil;
    float petrol;
    float LPG;
    float methane;
    float coal;
    float highEnergyDensityBiomass;
    float wood;
    float electricityGridAmount;
    float electricityGridPercentageRenewables;
    float electricitySolar;
    float electricityEolic;
    float electricityHydro;
};

struct TinputDataBuyer{
    TgeneralBuyer generalBuyer;
    Tenergy energy;
};


struct TinputData{
    Tgeneral general;
    Tclimate climate;
    Tsoil soil;
    TcropFieldManagement cropFieldManagement;
    TagronomicInput agronomicInput;
    Tenergy energy;
};

class BuyerCalculator{
public:
    bool setInputBuyer(std::vector<TinputDataBuyer> inputData,int iExp,CarbonCalculator calculatorCO2);
    double computeDebtsBuyer();
    int isPresentLCA;
    double valueLCA;
    EnergyManagement energy;
    double emissionEnergy;
};


bool readCsvFileSeller(QString csvFileName, std::vector<TinputData> &inputData, int &numberOfExperiments, QString &error);
bool readCsvFileBuyer(QString csvFileName, std::vector<TinputDataBuyer> &inputData, int& numberOfExperiments, QString &error);
bool openDBParameters(QSqlDatabase &db, QString dataPath, QString &error);
bool setCarbonCalculatorVariables(QSqlDatabase &db, CarbonCalculator &calculatorCO2, std::vector<TinputData> &inputData, int iExp, QString &error);
void printOutput(CarbonCalculator &calculatorCO2);
void usage();

// credits

double computeCredits(CarbonCalculator calculatorCO2, int *isAccepted);
double computeBufferSoil(int nrPractices);
double computeBufferBiomass(int nrPractices);

#endif // INPUTOUTPUT_H
