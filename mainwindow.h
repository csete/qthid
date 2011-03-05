/***************************************************************************
 *  This file is part of Qthid.
 *
 *  Copyright (C) 2010  Howard Long, G6LVB
 *  CopyRight (C) 2011  Alexandru Csete, OZ9AEC
 *                      Mario Lorenz, DL5MLO
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


/** \brief Data definition for combo box items.
  * This data structure represents an item in a combo box used for
  * gain and filter parameters. Each combo box has an array of these
  * data defining the set of options.
  */
typedef struct {
    const char *pszDesc;  /*!< The display name of the item. */
    qint8 u8Val;          /*!< The numerical value of the item. */
} COMBO_ITEM_STRUCT;


/** \brief Data definition for a combo box.
  * This data structure represents a combo box that is used for gain and
  * filter parameters. Each such parameter has a 'set' and 'get' command
  * as well as a list of options (COMBO_ITEM_STRUCT[]).
  * \sa _acs
  */
typedef struct {
    qint8 u8CommandSet;   /*!< The command for setting the parameter in the FCD. */
    qint8 u8CommandGet;   /*!< The command for retrieveing the parameter from the FCD. */
    qint8 nIdxDefault;    /*!< Index pointing to the default value for this parameter in the pacis array. */
    QComboBox *pComboBox; /*!< Pointer to the combo box. */
    const COMBO_ITEM_STRUCT *pacis; /*!< Pointer to the array if items. */
} COMBO_STRUCT;


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

    void populateCombo(QComboBox *box, int nIdxDefault, const COMBO_ITEM_STRUCT *pcis);
    void populateCombos();
    void enableCombos(bool enabled);
    void readDevice();
    void bandChange();


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


    void enableControls();

    /* actions */
    void on_actionAbout_triggered();
    void on_actionAboutQt_triggered();
};

#endif // MAINWINDOW_H
