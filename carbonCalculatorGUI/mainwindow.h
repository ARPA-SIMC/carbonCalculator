#ifndef MAINWINDOW_H
#define MAINWINDOW_H

    #include <QMainWindow>
    #include <QSqlDatabase>
    #include "carbonCalculator.h"
    #include "inputOutput.h"

    QT_BEGIN_NAMESPACE
    namespace Ui { class MainWindow; }
    QT_END_NAMESPACE

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private slots:
        void on_actionShow_Info_triggered();

        void on_actionChoose_sellers_triggered();

        void on_actionChoose_buyers_triggered();

        void on_actionCompute_Sellers_triggered();

        void on_actionCompute_Buyers_triggered();

    protected:
        void resizeEvent(QResizeEvent * event);

    private:
        QString dataPath;
        QString error;
        QSqlDatabase dbParameters;
        CarbonCalculator calculatorCO2;
        BuyerCalculator buyerCalculatorCO2;

        Ui::MainWindow *ui;
    };

#endif // MAINWINDOW_H
