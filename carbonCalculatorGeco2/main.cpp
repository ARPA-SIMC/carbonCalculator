#include "inputOutput.h"
#include "dbOutput.h"

// uncomment to compute test
#define TEST

static CarbonCalculator calculatorCO2;

int main(int argc, char *argv[])
{
    bool printOutputOnScreen = false;
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

    // open parameters DB
    QSqlDatabase dbParameters;
    if (! openDataBase(dbParameters, dataPath))
        return -1;

    // create output DB
    QSqlDatabase dbOutput;
    QString dbName = dataPath + "output.db";
    if (! createOutputDB(dbOutput, dbName))
        return -1;

    // compute balance
    std::cout << "field simulation:" << std::endl ;
    for (int iExp=0; iExp<numberOfExperiments; iExp++)
    {
        bool isSetVarOk = false;
        isSetVarOk = setCarbonCalculatorVariables(dbParameters,calculatorCO2,inputData,iExp);
        if (!isSetVarOk) return -1;
        calculatorCO2.computeBalance();

        QString id = QString::number(inputData[iExp].general.year) +
                "_" + inputData[iExp].general.enterpriseName
                + "_Field" + QString::number(inputData[iExp].general.nrField);
        std::cout << "ID: " << id.toStdString() << "\t" << iExp+1 << " of " << numberOfExperiments << std::endl;
        if (printOutputOnScreen) printOutput(calculatorCO2);

        // save db output
        if (! saveOutput(id, dbOutput, inputData[iExp],calculatorCO2))
            return -1;
    }

    return 0;
}
