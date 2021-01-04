#include "inputOutput.h"

// uncomment to compute test
#define TEST

static CarbonCalculator calculatorCO2;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString dataPath;
    if (! searchDataPath(&dataPath))
    {
        std::cout << "Error: missing DATA directory" << std::endl;
        return -1;
    }

    // read arguments
    QString csvFileName;
    if (argc < 2)
    {
        #ifdef TEST
            csvFileName = dataPath + "inputFile.csv";
        #else
            usage();
            return 1;
        #endif
    }
    else
    {
        csvFileName = argv[1];
    }

    // read CSV
    if (! QFile(csvFileName).exists())
    {
        std::cout << "Error!\nMissing csv file: " << csvFileName.toStdString() << std::endl;
        return -1;
    }
    int numberOfExperiments = 0;
    std::vector<TinputData> inputData;
    readCsvFile(csvFileName,inputData,numberOfExperiments);

    // open DB
    QSqlDatabase db;
    if (! openDataBase(db,dataPath))
        return -1;

    // compute balance
    for (int iExp=0; iExp<numberOfExperiments; iExp++)
    {
        bool isSetVarOk = false;
        isSetVarOk = setCarbonCalculatorVariables(db,calculatorCO2,inputData,iExp);
        if (!isSetVarOk) return -1;
        calculatorCO2.computeBalance();
        printOutput(calculatorCO2);
    }

    // TODO save db output

    return 0;
}
