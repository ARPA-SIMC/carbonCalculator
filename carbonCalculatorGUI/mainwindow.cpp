#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>

#include "dbUtilities.h"
#include "dbOutput.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qApp->setStyleSheet("QMessageBox { messagebox-text-interaction-flags: 5; }");

    searchDataPath(dataPath);

    if (! openDBParameters(dbParameters, dataPath, error))
    {
        ui->logBrowser->append(error);
    }
    else
    {
        ui->logBrowser->append("DB parameters = " + dbParameters.databaseName());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::resizeEvent(QResizeEvent * event)
{
    ui->logBrowser->resize(ui->logBrowser->width(), this->height() - ui->logBrowser->y() - 50);
}


void MainWindow::on_actionShow_Info_triggered()
{
    QMessageBox::information(this, "Carbon Calculator V1.1", "https://github.com/ARPA-SIMC/carbonCalculator");
}


void MainWindow::on_actionChoose_sellers_triggered()
{
    QString csvFileName = QFileDialog::getOpenFileName(this, tr("Choose Sellers file"), dataPath, tr("Comma separated values (*.csv)"));
    if (csvFileName != "")
    {
        ui->sellerBox->setEnabled(true);
        ui->sellerBox->setText(csvFileName);
    }
    else return;

    // check file
    ui->logBrowser->append("\nRead csv file: " + csvFileName);
    int numberOfExperiments = 0;
    std::vector<TinputData> inputData;
    error = "";
    readCsvFileSeller(csvFileName, inputData, numberOfExperiments, error);
    if (error != "")
        ui->logBrowser->append(error);
    else
        ui->logBrowser->append("File is correct!\n");
}


void MainWindow::on_actionChoose_buyers_triggered()
{
    QString csvFileName = QFileDialog::getOpenFileName(this, tr("Choose Buyers file"), dataPath, tr("Comma separated values (*.csv)"));
    if (csvFileName != "")
    {
        ui->buyerBox->setEnabled(true);
        ui->buyerBox->setText(csvFileName);
    }
    else return;

    // check file
    ui->logBrowser->append("\nRead csv file: " + csvFileName);
    int numberOfBuyers = 0;
    std::vector<TinputDataBuyer> inputDataBuyer;
    error = "";
    readCsvFileBuyer(csvFileName, inputDataBuyer, numberOfBuyers, error);
    if (error != "")
        ui->logBrowser->append(error);
    else
        ui->logBrowser->append("File is correct!\n");
}


void MainWindow::on_actionCompute_Sellers_triggered()
{
    QString csvFileName = ui->sellerBox->text();
    if (csvFileName == "")
    {
        ui->logBrowser->append("Choose Credits file before!");
        return;
    }

    if (! QFile(csvFileName).exists())
    {
        ui->logBrowser->append("Missing csv file: " + csvFileName);
        return;
    }

    // read csv file
    int numberOfExperiments = 0;
    std::vector<TinputData> inputData;
    if (! readCsvFileSeller(csvFileName, inputData, numberOfExperiments, error))
    {
        ui->logBrowser->append(error);
        ui->logBrowser->append("Computation failed.");
        return;
    }

    // choose output DB
    ui->logBrowser->append("Choose output database...");
    QString dbNameWithPath = QFileDialog::getSaveFileName(this, tr("Save output DB"), dataPath, tr("SQLite database (*.db)"));
    QString fileName = getFileName(dbNameWithPath);
    if (fileName == "carbonCalculatorParameters.db")
    {
        QMessageBox::information(this, fileName, "You are not allowed to modify this database.");
        ui->logBrowser->append("Error: Choose another output database");
        return;
    }

    ui->logBrowser->append("Create output db: " + dbNameWithPath);
    qApp->processEvents();
    QSqlDatabase dbOutput;
    if (! createOutputDB(dbOutput, dbNameWithPath))
    {
        ui->logBrowser->append("Error in creating db!");
        ui->logBrowser->append("Computation failed.");
        return;
    }

    ui->logBrowser->append("\nCredits computation:");
    for (int iExp = 0; iExp < numberOfExperiments; iExp++)
    {
        QString text = QString::number(iExp+1) + " of " + QString::number(numberOfExperiments);
        ui->logBrowser->append(text);

        bool isSetVarOk = false;
        isSetVarOk = setCarbonCalculatorVariables(dbParameters, calculatorCO2, inputData, iExp, error);
        if (!isSetVarOk)
        {
            ui->logBrowser->append("ERROR in setCarbonCalculatorVariables " + error);
            continue;
        }

        calculatorCO2.computeBalance();

        int isAccepted;
        double credits = computeCredits(calculatorCO2, &isAccepted);

        QString idField = QString::number(inputData[iExp].general.year) + "_" + inputData[iExp].general.enterpriseName
                    + "_Field" + QString::number(inputData[iExp].general.nrField);

        QString yesOrNo = (isAccepted? "YES" : "NO");
        text = idField + " --- isAccepted: " + yesOrNo;
        text += " --- Budget: " + QString::number(credits) + " kg CO2eq";
        ui->logBrowser->append(text);
        qApp->processEvents();

        if (! saveOutput(idField, dbOutput, inputData[iExp], calculatorCO2, credits, &isAccepted))
        {
            ui->logBrowser->append("Error in saving id: " + idField);
        }
    }
    ui->logBrowser->append("\nComputation ended.");
}


void MainWindow::on_actionCompute_Buyers_triggered()
{
    QString csvFileName = ui->buyerBox->text();
    if (csvFileName == "")
    {
        ui->logBrowser->append("Choose Debits file before!");
        return;
    }

    if (! QFile(csvFileName).exists())
    {
        ui->logBrowser->append("Missing csv file: " + csvFileName);
        return;
    }

    // read csv file
    int numberOfBuyers = 0;
    std::vector<TinputDataBuyer> inputDataBuyer;
    if (! readCsvFileBuyer(csvFileName, inputDataBuyer, numberOfBuyers, error))
    {
        ui->logBrowser->append(error);
        ui->logBrowser->append("Computation failed.");
        return;
    }

    ui->logBrowser->append("Choose output database...");
    QString dbNameWithPath = QFileDialog::getSaveFileName(this, tr("Save output DB"), dataPath, tr("SQLite database (*.db)"));
    QString fileName = getFileName(dbNameWithPath);
    if (fileName == "carbonCalculatorParameters.db")
    {
        QMessageBox::information(this, fileName, "You are not allowed to modify this database.");
        ui->logBrowser->append("Error: Choose another output database");
        return;
    }

    ui->logBrowser->append("Create output db: " + dbNameWithPath);
    qApp->processEvents();
    QSqlDatabase dbOutput;
    if (! createOutputDB(dbOutput, dbNameWithPath))
    {
        ui->logBrowser->append("Error in creating db!");
        return;
    }

    ui->logBrowser->append("\nDebits computation:");
    for (int i=0; i < numberOfBuyers; i++)
    {
        QString text = QString::number(i+1) + " of " + QString::number(numberOfBuyers);
        ui->logBrowser->append(text);

        bool isSetVarOk = false;
        isSetVarOk = buyerCalculatorCO2.setInputBuyer(inputDataBuyer,i, calculatorCO2);
        if (!isSetVarOk)
        {
            ui->logBrowser->append("ERROR in setInputBuyer!");
            continue;
        }

        double debits = buyerCalculatorCO2.computeDebitsBuyer();
        QString idBuyer = QString::number(inputDataBuyer[i].generalBuyer.year) +
                "_" + inputDataBuyer[i].generalBuyer.enterpriseName
                + "_" + inputDataBuyer[i].generalBuyer.chainName
                + "_" + inputDataBuyer[i].generalBuyer.productName;

        text = idBuyer + " --- Debits: " + QString::number(debits) + " kg CO2eq";
        ui->logBrowser->append(text);
        qApp->processEvents();

        // save db output
        if (! saveOutputBuyer(idBuyer, dbOutput, inputDataBuyer[i],buyerCalculatorCO2,debits))
        {
            ui->logBrowser->append("Error in saving id: " + idBuyer);
        }
    }
    ui->logBrowser->append("\nComputation ended.");

}
