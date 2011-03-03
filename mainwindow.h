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
#include <QComboBox>

namespace Ui {
    class MainWindow;
}
typedef struct
{
    const char *pszDesc;
    qint8 u8Val;
} COMBOITEMSTRUCT;

typedef struct
{
    qint8 u8CommandSet;
    qint8 u8CommandGet;
    qint8 nIdxDefault;
    QComboBox *pComboBox;
    const COMBOITEMSTRUCT *pacis;
} COMBOSTRUCT;

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
    void PopulateCombo(QComboBox *box, int nIdxDefault, const COMBOITEMSTRUCT *pcis);
    void PopulateCombos();
    void ReadDevice();
    void BandChange();


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
    void on_comboBoxLNAGain_activated(int index);
    void on_comboBoxRfFilter_activated(int index);
    void on_comboBoxMixerGain_activated(int index);
    void on_comboBoxMixerFilter_activated(int index);
    void on_comboBoxIFGain1_activated(int index);
    void on_comboBoxIFRCFilter_activated(int index);
    void on_comboBoxIFGain2_activated(int index);
    void on_comboBoxIFGain3_activated(int index);
    void on_comboBoxIFFilter_activated(int index);
    void on_comboBoxIFGain4_activated(int index);
    void on_comboBoxIFGain5_activated(int index);
    void on_comboBoxIFGain6_activated(int index);
    void on_comboBoxLNAEnhance_activated(int index);
    void on_comboBoxBiasCurrent_activated(int index);
    void on_comboBoxIFGainMode_activated(int index);
    void on_doubleSpinBoxDCI_valueChanged(double value);
    void on_doubleSpinBoxDCQ_valueChanged(double value);
    void on_doubleSpinBoxPhase_valueChanged(double value);
    void on_doubleSpinBoxGain_valueChanged(double value);

    void on_pushButtonDefaults_clicked();


    void EnableControls();

    /* actions */
    void on_actionAbout_triggered();
    void on_actionAboutQt_triggered();
};

#endif // MAINWINDOW_H
