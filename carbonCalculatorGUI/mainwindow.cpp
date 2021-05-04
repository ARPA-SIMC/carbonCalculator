#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>

#include "dbUtilities.h"
#include "inputOutput.h"
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
    QString fileName = QFileDialog::getOpenFileName(this, tr("Choose Sellers file"), dataPath, tr("Comma separated values (*.csv)"));
    if (fileName != "")
    {
        ui->sellerBox->setEnabled(true);
        ui->sellerBox->setText(fileName);
    }
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

    ui->logBrowser->append("Read csv file: " + csvFileName);
    int numberOfExperiments = 0;
    std::vector<TinputData> inputData;
    readCsvFile(csvFileName, inputData, numberOfExperiments);

    // create output DB
    QString dbName = QFileDialog::getSaveFileName(this, tr("Save output DB"), dataPath, tr("SQLite database (*.db)"));
    ui->logBrowser->append("Create output db: " + dbName);
    QSqlDatabase dbOutput;
    if (! createOutputDB(dbOutput, dbName))
    {
        ui->logBrowser->append("Error in creating db!");
        return;
    }

    // compute budget
    ui->logBrowser->append("field simulation:");
    for (int iExp=0; iExp<numberOfExperiments; iExp++)
    {
        bool isSetVarOk = false;
        isSetVarOk = setCarbonCalculatorVariables(dbParameters, calculatorCO2, inputData, iExp);
        if (!isSetVarOk)
            return;

        calculatorCO2.computeBalance();

        double credits;
        int isAccepted;
        credits = computeCredits(calculatorCO2,&isAccepted);
        QString id = QString::number(inputData[iExp].general.year) +
                "_" + inputData[iExp].general.enterpriseName
                + "_Field" + QString::number(inputData[iExp].general.nrField);
        std::cout << "ID: " << id.toStdString() << "\t" << iExp+1 << " of " << numberOfExperiments << std::endl;
        if (printOutputOnScreen) printOutput(calculatorCO2);

        if (! saveOutput(id, dbOutput, inputData[iExp],calculatorCO2,credits,&isAccepted))
            return;
    }
}
