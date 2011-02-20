/***************************************************************************
 *  This file is part of Qthid.
 * 
 *  Copyright (C) 2010  Howard Long, G6LVB
 * 
 *  Qthid is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Qthid is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Qthid.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***************************************************************************/

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
