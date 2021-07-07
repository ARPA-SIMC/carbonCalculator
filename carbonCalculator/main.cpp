#include "inputOutput.h"
#include "dbOutput.h"

// uncomment to compute test
#define TEST

static CarbonCalculator calculatorCO2;
static BuyerCalculator buyerCalculatorCO2;


void usage()
{
    std::cout << "USAGE:\ncarbonCalculatorTest [sellerDataFileName.csv] [buyerDataFileName.csv] [outputFileName.db]\n";
}


int main(int argc, char *argv[])
{

    printf("__________________________________\n");
    printf("|                                |\n");
    printf("|       Carbon Calculator        |\n");
    printf("|       IT-HR GECO2 project      |\n") ;
    printf("|________________________________|\n\n");
    bool printOutputOnScreen = false;
    QCoreApplication a(argc, argv);

    QString dataPath;
    if (! searchDataPath(dataPath))
    {
        std::cout << "Error: missing DATA directory" << std::endl;
        return -1;
    }

    // read arguments carbon calculator
    QString csvFileName, csvFileNameBuyer, dbName;

    if (argc < 2)
    {
        #ifdef TEST
            csvFileName = dataPath + "testInputCredits.csv";
        #else
            usage();
            return 1;
        #endif
    }
    else
    {
        csvFileName = dataPath + argv[1];
    }

    if (argc < 3)
    {
        #ifdef TEST
            csvFileNameBuyer = dataPath + "testInputDebts.csv";
        #else
            usage();
            return 1;
        #endif
    }
    else
    {
        csvFileNameBuyer = dataPath + argv[2];
    }

    if (argc < 4)
    {
        #ifdef TEST
            dbName = dataPath + "output.db";
        #else
            usage();
            return 1;
        #endif
    }
    else
    {
        csvFileNameBuyer = dataPath + argv[2];
    }

    std::cout << "Read credits csv..." << std::endl ;
    if (! QFile(csvFileName).exists())
    {
        std::cout << "Error!\nMissing csv file: " << csvFileName.toStdString() << std::endl;
        return -1;
    }
    QString error;
    int numberOfExperiments = 0;
    std::vector<TinputData> inputData;
    if (! readCsvFileSeller(csvFileName, inputData, numberOfExperiments, error))
    {
        std::cout << error.toStdString();
        return -1;
    }
    else if (error != "")
        std::cout << error.toStdString();

    std::cout << "\nRead debts csv..." << std::endl ;
    if (! QFile(csvFileNameBuyer).exists())
    {
        std::cout << "Error!\nMissing csv file (buyer): " << csvFileNameBuyer.toStdString() << std::endl;
        return -1;
    }
    int numberOfExperimentsBuyer = 0;
    std::vector<TinputDataBuyer> inputDataBuyer;
    if (!readCsvFileBuyer(csvFileNameBuyer, inputDataBuyer, numberOfExperimentsBuyer, error))
    {
        std::cout << error.toStdString() << std::endl;
        std::cout << "Wrong buyer file, the simulation will be stopped." << std::endl;
        return 0;
    }

    // open db parameters
    QSqlDatabase dbParameters;

    if (! openDBParameters(dbParameters, dataPath, error))
    {
        std::cout << error.toStdString() << std::endl;
        return -1;
    }

    // create output DB
    std::cout << "\nCreate output db: " << dbName.toStdString() << std::endl ;
    QSqlDatabase dbOutput;
    if (! createOutputDB(dbOutput, dbName))
        return -1;

    // compute balance
    std::cout << "\nCredits computation:" << std::endl ;
    for (int iExp=0; iExp<numberOfExperiments; iExp++)
    {
        bool isSetVarOk = false;
        isSetVarOk = setCarbonCalculatorVariables(dbParameters, calculatorCO2, inputData, iExp, error);
        if (!isSetVarOk)
        {
            std::cout << iExp+1 << " of " << numberOfExperiments << " ERROR! " + error.toStdString() << std::endl;
            continue;
        }
        calculatorCO2.computeBalance();
        double credits;
        int isAccepted;
        credits = computeCredits(calculatorCO2,&isAccepted);
        QString id = QString::number(inputData[iExp].general.year) +
                "_" + inputData[iExp].general.enterpriseName
                + "_Field" + QString::number(inputData[iExp].general.nrField);
        std::cout << "ID: " << id.toStdString() << "\t" << iExp+1 << " of " << numberOfExperiments << std::endl;
        if (printOutputOnScreen) printOutput(calculatorCO2);

        // save db output
        if (! saveOutput(id, dbOutput, inputData[iExp],calculatorCO2,credits,&isAccepted))
            return -1;
    }

    std::cout << "\nDebts computation:" << std::endl ;
    for (int iExp=0; iExp<numberOfExperimentsBuyer; iExp++)
    {
        double debts;
        bool isSetVarOk = false;
        isSetVarOk = buyerCalculatorCO2.setInputBuyer(inputDataBuyer,iExp,calculatorCO2);
        if (!isSetVarOk) return -1;
        debts = buyerCalculatorCO2.computeDebtsBuyer();
        QString idBuyer = QString::number(inputDataBuyer[iExp].generalBuyer.year) +
                "_" + inputDataBuyer[iExp].generalBuyer.enterpriseName
                + "_" + inputDataBuyer[iExp].generalBuyer.chainName
                + "_" + inputDataBuyer[iExp].generalBuyer.productName;
        std::cout << "ID: " << idBuyer.toStdString() << "\t" << iExp+1 << " of " << numberOfExperimentsBuyer << std::endl;

        // save db output
        if (! saveOutputBuyer(idBuyer, dbOutput, inputDataBuyer[iExp],buyerCalculatorCO2,debts))
            return -1;
    }


    return 0;
}
