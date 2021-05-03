#include "inputOutput.h"
#include "dbOutput.h"

// uncomment to compute test
//#define TEST

static CarbonCalculator calculatorCO2;
static BuyerCalculator buyerCalculatorCO2;

int main(int argc, char *argv[])
{

    printf("__________________________________\n");
    printf("|                                |\n");
    printf("|       Carbon Calculator        |\n");
    printf("|       IT-HR GECO2 project      |\n") ;
    printf("|________________________________|\n\n\n\n");
    bool printOutputOnScreen = false;
    QCoreApplication a(argc, argv);

    QString dataPath;
    if (! searchDataPath(&dataPath))
    {
        std::cout << "Error: missing DATA directory" << std::endl;
        return -1;
    }

    // read arguments carbon calculator
    QString csvFileName, csvFileNameBuyer;
    if (argc < 2)
    {
        #ifdef TEST
            csvFileName = dataPath + "inputDataTrainingCroatiaBiochar.csv";
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
            csvFileNameBuyer = dataPath + "inputFileBuyerDebug.csv";
        #else
            usage();
            return 1;
        #endif
    }
    else
    {
        csvFileNameBuyer = dataPath + argv[2];
    }

    // read CSV seller
    if (! QFile(csvFileName).exists())
    {
        std::cout << "Error!\nMissing csv file: " << csvFileName.toStdString() << std::endl;
        return -1;
    }
    int numberOfExperiments = 0;
    std::vector<TinputData> inputData;
    readCsvFile(csvFileName, inputData, numberOfExperiments);

    // read CSV buyer
    if (! QFile(csvFileNameBuyer).exists())
    {
        std::cout << "Error!\nMissing csv file (buyer): " << csvFileNameBuyer.toStdString() << std::endl;
        return -1;
    }
    int numberOfExperimentsBuyer = 0;
    std::vector<TinputDataBuyer> inputDataBuyer;
    readCsvFileBuyer(csvFileNameBuyer, inputDataBuyer, numberOfExperimentsBuyer);

    // open db parameters
    QSqlDatabase dbParameters;
    if (! openDataBase(dbParameters, dataPath))
        return -1;

    // choose db output name
    QString dbName;
    char answer;
    printf("Do you want to use the default DB name (output.db)? [y/n] ");
    scanf("%c",&answer);
    fflush(stdin);
    if (answer == 'y')
    {
        dbName = dataPath + "output.db";
    }
    else
    {
        printf("please insert the output DB name (file extension .db) and press return:\n");
        char outputName[100];
        for (int i=0;i<100;i++)
            outputName[i] = '\0';
        scanf("%s",outputName);
        dbName = dataPath + outputName;
    }

    // create output DB
    std::cout << "create db... " << dbName.toStdString() << std::endl ;
    QSqlDatabase dbOutput;
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

    std::cout << "buyer simulation:" << std::endl ;
    for (int iExp=0; iExp<numberOfExperimentsBuyer; iExp++)
    {
        double debits;
        bool isSetVarOk = false;
        isSetVarOk = buyerCalculatorCO2.setInputBuyer(inputDataBuyer,iExp,calculatorCO2);
        if (!isSetVarOk) return -1;
        debits = buyerCalculatorCO2.computeDebitsBuyer();
        QString idBuyer = QString::number(inputDataBuyer[iExp].generalBuyer.year) +
                "_" + inputDataBuyer[iExp].generalBuyer.enterpriseName
                + "_" + inputDataBuyer[iExp].generalBuyer.chainName
                + "_" + inputDataBuyer[iExp].generalBuyer.productName;
        std::cout << "ID: " << idBuyer.toStdString() << "\t" << iExp+1 << " of " << numberOfExperimentsBuyer << std::endl;

        // save db output
        if (! saveOutputBuyer(idBuyer, dbOutput, inputDataBuyer[iExp],buyerCalculatorCO2,debits))
            return -1;
    }


    return 0;
}
