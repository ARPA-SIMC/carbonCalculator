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

    // check file
    ui->logBrowser->append("\nRead csv file: " + csvFileName);
    int numberOfExperiments = 0;
    std::vector<TinputData> inputData;
    readCsvFile(csvFileName, inputData, numberOfExperiments, error);
    if (error != "")
        ui->logBrowser->append(error);
    else
        ui->logBrowser->append("File is correct!\n");
}


void MainWindow::on_actionChoose_buyers_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Choose Buyers file"), dataPath, tr("Comma separated values (*.csv)"));
    if (fileName != "")
    {
        ui->buyerBox->setEnabled(true);
        ui->buyerBox->setText(fileName);
    }
}


void MainWindow::on_actionCompute_Sellers_triggered()
{
    QString csvFileName = ui->sellerBox->text();
    if (csvFileName == "")
    {
        ui->logBrowser->append("Choose Sellers file before!");
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
    if (! readCsvFile(csvFileName, inputData, numberOfExperiments, error))
    {
        ui->logBrowser->append(error);
        return;
    }

    // create output DB
    ui->logBrowser->append("Choose output database...");
    QString dbName = QFileDialog::getSaveFileName(this, tr("Save output DB"), dataPath, tr("SQLite database (*.db)"));
    ui->logBrowser->append("Create output db: " + dbName);
    qApp->processEvents();
    QSqlDatabase dbOutput;
    if (! createOutputDB(dbOutput, dbName))
    {
        ui->logBrowser->append("Error in creating db!");
        return;
    }

    // compute
    ui->logBrowser->append("Sellers simulation:");
    for (int iExp = 0; iExp < numberOfExperiments; iExp++)
    {
        QString text = QString::number(iExp+1) + " of " + QString::number(numberOfExperiments);
        ui->logBrowser->append(text);

        bool isSetVarOk = false;
        isSetVarOk = setCarbonCalculatorVariables(dbParameters, calculatorCO2, inputData, iExp, error);
        if (!isSetVarOk)
        {
            ui->logBrowser->append("ERROR! " + error);
            continue;
        }

        calculatorCO2.computeBalance();

        int isAccepted;
        double credits = computeCredits(calculatorCO2, &isAccepted);

        QString idField = QString::number(inputData[iExp].general.year) + "_" + inputData[iExp].general.enterpriseName
                    + "_Field" + QString::number(inputData[iExp].general.nrField);

        QString yesOrNo = (isAccepted? "YES" : "NO");
        text = idField + " --- isAccepted: " + yesOrNo;
        text += " --- credits: " + QString::number(credits);
        ui->logBrowser->append(text);
        qApp->processEvents();

        if (! saveOutput(idField, dbOutput, inputData[iExp], calculatorCO2, credits, &isAccepted))
        {
            ui->logBrowser->append("Error in saving id: " + idField);
        }
    }
}
