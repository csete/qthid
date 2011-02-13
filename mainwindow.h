#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    double StrToDouble(QString s);

private slots:
    void on_spinBoxCorr_valueChanged(int );
    void on_pushButtonDown_clicked();
    void on_lineEditStep_textChanged(QString );
    void on_pushButtonUp_clicked();
    void on_lineEditFreq_textChanged(QString );
    void on_pushButtonVerifyFirmware_clicked();
    void on_pushButtonUpdateFirmware_clicked();
    void on_pushButtonBLReset_clicked();
    void on_pushButtonAppReset_clicked();
    void EnableControls();
};

#endif // MAINWINDOW_H
