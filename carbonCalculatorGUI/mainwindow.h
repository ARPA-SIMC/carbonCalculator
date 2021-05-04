#ifndef MAINWINDOW_H
#define MAINWINDOW_H

    #include <QMainWindow>
    #include <QSqlDatabase>

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

    private:
        QString dataPath;
        QString error;
        QSqlDatabase dbParameters;

        Ui::MainWindow *ui;
    };

#endif // MAINWINDOW_H
