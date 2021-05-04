#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>

#include "dbUtilities.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qApp->setStyleSheet("QMessageBox { messagebox-text-interaction-flags: 5; }");
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
    QString dataPath = "";
    searchDataPath(dataPath);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Choose Sellers file"), dataPath, tr("Comma separated values (*.csv)"));
    if (fileName != "")
    {
        ui->sellerBox->setEnabled(true);
        ui->sellerBox->setText(fileName);
    }
}


void MainWindow::on_actionChoose_buyers_triggered()
{
    QString dataPath = "";
    searchDataPath(dataPath);
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
    //int numberOfExperiments = 0;

    //std::vector<TinputData> inputData;
    //readCsvFile(csvFileName, inputData, numberOfExperiments);
}
