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

#include <QtGui>
#include <QtDebug>
#include <QMessageBox>
#include <fcd.h>
#include <fcd_tuner.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "freqctrl.h"
#include "iqbalance.h"
#include "firmware.h"


/** \brief LNA gain options */
static const COMBO_ITEM_STRUCT _cisLNAGain[]=
{
    {QT_TR_NOOP("-5.0dB"), FCD_TLGE_N5_0DB},
    {QT_TR_NOOP("-2.5dB"), FCD_TLGE_N2_5DB},
    {QT_TR_NOOP("+0.0dB"), FCD_TLGE_P0_0DB},
    {QT_TR_NOOP("+2.5dB"), FCD_TLGE_P2_5DB},
    {QT_TR_NOOP("+5,0dB"), FCD_TLGE_P5_0DB},
    {QT_TR_NOOP("+7.5dB"), FCD_TLGE_P7_5DB},
    {QT_TR_NOOP("+10.0dB"), FCD_TLGE_P10_0DB},
    {QT_TR_NOOP("+12.5dB"), FCD_TLGE_P12_5DB},
    {QT_TR_NOOP("+15.0dB"), FCD_TLGE_P15_0DB},
    {QT_TR_NOOP("+17.5dB"), FCD_TLGE_P17_5DB},
    {QT_TR_NOOP("+20.0dB"), FCD_TLGE_P20_0DB},
    {QT_TR_NOOP("+25.0dB"), FCD_TLGE_P25_0DB},
    {QT_TR_NOOP("+30.0dB"), FCD_TLGE_P30_0DB},
    {NULL,0}
};

/** \brief LNA enhance options */
static const COMBO_ITEM_STRUCT _cisLNAEnhance[]=
{
        {QT_TR_NOOP("Off"), FCD_TLEE_OFF},
        {QT_TR_NOOP("0"), FCD_TLEE_0},
        {QT_TR_NOOP("1"), FCD_TLEE_1},
        {QT_TR_NOOP("2"), FCD_TLEE_2},
        {QT_TR_NOOP("3"), FCD_TLEE_3},
        {NULL,0}
};

/** \brief Band selector options */
static const COMBO_ITEM_STRUCT _cisBand[]=
{
        {QT_TR_NOOP("VHF II"), FCD_TBE_VHF2},
        {QT_TR_NOOP("VHF III"), FCD_TBE_VHF3},
        {QT_TR_NOOP("UHF"), FCD_TBE_UHF},
        {QT_TR_NOOP("L band"), FCD_TBE_LBAND},
        {NULL,0}
};

/** \brief RF filter band 0 options */
static const COMBO_ITEM_STRUCT _cisRFFilter0[]=
{
        {QT_TR_NOOP("268MHz LPF"), FCD_TRFE_LPF268MHZ},
        {QT_TR_NOOP("299MHz LPF"), FCD_TRFE_LPF299MHZ},
        {NULL,0}
};

/** \brief RF filter band 1 options */
static const COMBO_ITEM_STRUCT _cisRFFilter1[]=
{
        {QT_TR_NOOP("509MHz LPF"), FCD_TRFE_LPF509MHZ},
        {QT_TR_NOOP("656MHz LPF"), FCD_TRFE_LPF656MHZ},
        {NULL,0}
};

/** \brief RF filter band 2 options */
static const COMBO_ITEM_STRUCT _cisRFFilter2[]=
{
        {QT_TR_NOOP("360MHz BPF"), FCD_TRFE_BPF360MHZ},
        {QT_TR_NOOP("380MHz BPF"), FCD_TRFE_BPF380MHZ},
        {QT_TR_NOOP("405MHz BPF"), FCD_TRFE_BPF405MHZ},
        {QT_TR_NOOP("425MHz BPF"), FCD_TRFE_BPF425MHZ},
        {QT_TR_NOOP("450MHz BPF"), FCD_TRFE_BPF450MHZ},
        {QT_TR_NOOP("475MHz BPF"), FCD_TRFE_BPF475MHZ},
        {QT_TR_NOOP("505MHz BPF"), FCD_TRFE_BPF505MHZ},
        {QT_TR_NOOP("540MHz BPF"), FCD_TRFE_BPF540MHZ},
        {QT_TR_NOOP("575MHz BPF"), FCD_TRFE_BPF575MHZ},
        {QT_TR_NOOP("615MHz BPF"), FCD_TRFE_BPF615MHZ},
        {QT_TR_NOOP("670MHz BPF"), FCD_TRFE_BPF670MHZ},
        {QT_TR_NOOP("720MHz BPF"), FCD_TRFE_BPF720MHZ},
        {QT_TR_NOOP("760MHz BPF"), FCD_TRFE_BPF760MHZ},
        {QT_TR_NOOP("840MHz BPF"), FCD_TRFE_BPF840MHZ},
        {QT_TR_NOOP("890MHz BPF"), FCD_TRFE_BPF890MHZ},
        {QT_TR_NOOP("970MHz BPF"), FCD_TRFE_BPF970MHZ},
        {NULL,0}
};

/** \brief RF filter band 3 options */
static const COMBO_ITEM_STRUCT _cisRFFilter3[]=
{
        {QT_TR_NOOP("1300MHz BPF"), FCD_TRFE_BPF1300MHZ},
        {QT_TR_NOOP("1320MHz BPF"), FCD_TRFE_BPF1320MHZ},
        {QT_TR_NOOP("1360MHz BPF"), FCD_TRFE_BPF1360MHZ},
        {QT_TR_NOOP("1410MHz BPF"), FCD_TRFE_BPF1410MHZ},
        {QT_TR_NOOP("1445MHz BPF"), FCD_TRFE_BPF1445MHZ},
        {QT_TR_NOOP("1460MHz BPF"), FCD_TRFE_BPF1460MHZ},
        {QT_TR_NOOP("1490MHz BPF"), FCD_TRFE_BPF1490MHZ},
        {QT_TR_NOOP("1530MHz BPF"), FCD_TRFE_BPF1530MHZ},
        {QT_TR_NOOP("1560MHz BPF"), FCD_TRFE_BPF1560MHZ},
        {QT_TR_NOOP("1590MHz BPF"), FCD_TRFE_BPF1590MHZ},
        {QT_TR_NOOP("1640MHz BPF"), FCD_TRFE_BPF1640MHZ},
        {QT_TR_NOOP("1660MHz BPF"), FCD_TRFE_BPF1660MHZ},
        {QT_TR_NOOP("1680MHz BPF"), FCD_TRFE_BPF1680MHZ},
        {QT_TR_NOOP("1700MHz BPF"), FCD_TRFE_BPF1700MHZ},
        {QT_TR_NOOP("1720MHz BPF"), FCD_TRFE_BPF1720MHZ},
        {QT_TR_NOOP("1750MHz BPF"), FCD_TRFE_BPF1750MHZ},
        {NULL,0}
};

/** \brief Mixer gain options */
static const COMBO_ITEM_STRUCT _cisMixerGain[]=
{
        {QT_TR_NOOP("4dB"), FCD_TMGE_P4_0DB},
        {QT_TR_NOOP("12dB"), FCD_TMGE_P12_0DB},
        {NULL,0}
};

/** \brief Bias options */
static const COMBO_ITEM_STRUCT _cisBiasCurrent[]=
{
        {QT_TR_NOOP("00 L band"), FCD_TBCE_LBAND},
        {QT_TR_NOOP("01"), FCD_TBCE_1},
        {QT_TR_NOOP("10"), FCD_TBCE_2},
        {QT_TR_NOOP("11 V/U band"), FCD_TBCE_VUBAND},
        {NULL,0}
};

/** \brief Mixer filter options */
static const COMBO_ITEM_STRUCT _cisMixerFilter[]=
{
        {QT_TR_NOOP("1.9MHz"), FCD_TMFE_1_9MHZ},
        {QT_TR_NOOP("2.3MHz"), FCD_TMFE_2_3MHZ},
        {QT_TR_NOOP("2.7MHz"), FCD_TMFE_2_7MHZ},
        {QT_TR_NOOP("3.0MHz"), FCD_TMFE_3_0MHZ},
        {QT_TR_NOOP("3.4MHz"), FCD_TMFE_3_4MHZ},
        {QT_TR_NOOP("3.8MHz"), FCD_TMFE_3_8MHZ},
        {QT_TR_NOOP("4.2MHz"), FCD_TMFE_4_2MHZ},
        {QT_TR_NOOP("4.6MHz"), FCD_TMFE_4_6MHZ},
        {QT_TR_NOOP("27MHz"), FCD_TMFE_27_0MHZ},
        {NULL,0}
};

/** \brief IF gain 1 options */
static const COMBO_ITEM_STRUCT _cisIFGain1[]=
{
        {QT_TR_NOOP("-3dB"), FCD_TIG1E_N3_0DB},
        {QT_TR_NOOP("+6dB"), FCD_TIG1E_P6_0DB},
        {NULL,0}
};

/** \brief IF gain mode options */
static const COMBO_ITEM_STRUCT _cisIFGainMode[]=
{
        {QT_TR_NOOP("Linearity"), FCD_TIGME_LINEARITY},
        {QT_TR_NOOP("Sensitivity"), FCD_TIGME_SENSITIVITY},
        {NULL,0}
};

/** \brief IF RC filter options */
static const COMBO_ITEM_STRUCT _cisIFRCFilter[]=
{
        {QT_TR_NOOP("1.0MHz"), FCD_TIRFE_1_0MHZ},
        {QT_TR_NOOP("1.2MHz"), FCD_TIRFE_1_2MHZ},
        {QT_TR_NOOP("1.8MHz"), FCD_TIRFE_1_8MHZ},
        {QT_TR_NOOP("2.6MHz"), FCD_TIRFE_2_6MHZ},
        {QT_TR_NOOP("3.4MHz"), FCD_TIRFE_3_4MHZ},
        {QT_TR_NOOP("4.4MHz"), FCD_TIRFE_4_4MHZ},
        {QT_TR_NOOP("5.3MHz"), FCD_TIRFE_5_3MHZ},
        {QT_TR_NOOP("6.4MHz"), FCD_TIRFE_6_4MHZ},
        {QT_TR_NOOP("7.7MHz"), FCD_TIRFE_7_7MHZ},
        {QT_TR_NOOP("9.0MHz"), FCD_TIRFE_9_0MHZ},
        {QT_TR_NOOP("10.6MHz"), FCD_TIRFE_10_6MHZ},
        {QT_TR_NOOP("12.4MHz"), FCD_TIRFE_12_4MHZ},
        {QT_TR_NOOP("14.7MHz"), FCD_TIRFE_14_7MHZ},
        {QT_TR_NOOP("17.6MHz"), FCD_TIRFE_17_6MHZ},
        {QT_TR_NOOP("21.0MHz"), FCD_TIRFE_21_0MHZ},
        {QT_TR_NOOP("21.4MHz"), FCD_TIRFE_21_4MHZ},
        {NULL,0}
};

/** \brief IF gain 2 options */
static const COMBO_ITEM_STRUCT _cisIFGain2[]=
{
        {QT_TR_NOOP("0dB"), FCD_TIG2E_P0_0DB},
        {QT_TR_NOOP("+3dB"), FCD_TIG2E_P3_0DB},
        {QT_TR_NOOP("+6dB"), FCD_TIG2E_P6_0DB},
        {QT_TR_NOOP("+9dB"), FCD_TIG2E_P9_0DB},
        {NULL,0}
};

/** \brief IF gain 3 options */
static const COMBO_ITEM_STRUCT _cisIFGain3[]=
{
        {QT_TR_NOOP("0dB"), FCD_TIG3E_P0_0DB},
        {QT_TR_NOOP("+3dB"), FCD_TIG3E_P3_0DB},
        {QT_TR_NOOP("+6dB"), FCD_TIG3E_P6_0DB},
        {QT_TR_NOOP("+9dB"), FCD_TIG3E_P9_0DB},
        {NULL,0}
};

/** \brief IF gain 4 options */
static const COMBO_ITEM_STRUCT _cisIFGain4[]=
{
        {QT_TR_NOOP("0dB"), FCD_TIG4E_P0_0DB},
        {QT_TR_NOOP("+1dB"), FCD_TIG4E_P1_0DB},
        {QT_TR_NOOP("+2dB"), FCD_TIG4E_P2_0DB},
        {NULL,0}
};

/** \brief IF filter options */
static const COMBO_ITEM_STRUCT _cisIFFilter[]=
{
        {QT_TR_NOOP("2.15MHz"), FCD_TIFE_2_15MHZ},
        {QT_TR_NOOP("2.20MHz"), FCD_TIFE_2_20MHZ},
        {QT_TR_NOOP("2.24MHz"), FCD_TIFE_2_24MHZ},
        {QT_TR_NOOP("2.28MHz"), FCD_TIFE_2_28MHZ},
        {QT_TR_NOOP("2.30MHz"), FCD_TIFE_2_30MHZ},
        {QT_TR_NOOP("2.40MHz"), FCD_TIFE_2_40MHZ},
        {QT_TR_NOOP("2.45MHz"), FCD_TIFE_2_45MHZ},
        {QT_TR_NOOP("2.50MHz"), FCD_TIFE_2_50MHZ},
        {QT_TR_NOOP("2.55MHz"), FCD_TIFE_2_55MHZ},
        {QT_TR_NOOP("2.60MHz"), FCD_TIFE_2_60MHZ},
        {QT_TR_NOOP("2.70MHz"), FCD_TIFE_2_70MHZ},
        {QT_TR_NOOP("2.75MHz"), FCD_TIFE_2_75MHZ},
        {QT_TR_NOOP("2.80MHz"), FCD_TIFE_2_80MHZ},
        {QT_TR_NOOP("2.90MHz"), FCD_TIFE_2_90MHZ},
        {QT_TR_NOOP("2.95MHz"), FCD_TIFE_2_95MHZ},
        {QT_TR_NOOP("3.00MHz"), FCD_TIFE_3_00MHZ},
        {QT_TR_NOOP("3.10MHz"), FCD_TIFE_3_10MHZ},
        {QT_TR_NOOP("3.20MHz"), FCD_TIFE_3_20MHZ},
        {QT_TR_NOOP("3.30MHz"), FCD_TIFE_3_30MHZ},
        {QT_TR_NOOP("3.40MHz"), FCD_TIFE_3_40MHZ},
        {QT_TR_NOOP("3.60MHz"), FCD_TIFE_3_60MHZ},
        {QT_TR_NOOP("3.70MHz"), FCD_TIFE_3_70MHZ},
        {QT_TR_NOOP("3.80MHz"), FCD_TIFE_3_80MHZ},
        {QT_TR_NOOP("3.90MHz"), FCD_TIFE_3_90MHZ},
        {QT_TR_NOOP("4.10MHz"), FCD_TIFE_4_10MHZ},
        {QT_TR_NOOP("4.30MHz"), FCD_TIFE_4_30MHZ},
        {QT_TR_NOOP("4.40MHz"), FCD_TIFE_4_40MHZ},
        {QT_TR_NOOP("4.60MHz"), FCD_TIFE_4_60MHZ},
        {QT_TR_NOOP("4.80MHz"), FCD_TIFE_4_80MHZ},
        {QT_TR_NOOP("5.00MHz"), FCD_TIFE_5_00MHZ},
        {QT_TR_NOOP("5.30MHz"), FCD_TIFE_5_30MHZ},
        {QT_TR_NOOP("5.50MHz"), FCD_TIFE_5_50MHZ},
        {NULL,0}
};

/** \brief IF gain 5 options */
static const COMBO_ITEM_STRUCT _cisIFGain5[]=
{
        {QT_TR_NOOP("+3dB"), FCD_TIG5E_P3_0DB},
        {QT_TR_NOOP("+6dB"), FCD_TIG5E_P6_0DB},
        {QT_TR_NOOP("+9dB"), FCD_TIG5E_P9_0DB},
        {QT_TR_NOOP("+12dB"), FCD_TIG5E_P12_0DB},
        {QT_TR_NOOP("+15dB"), FCD_TIG5E_P15_0DB},
        {NULL,0}
};

/** \brief IF gain 6 options */
static const COMBO_ITEM_STRUCT _cisIFGain6[]=
{
        {QT_TR_NOOP("+3dB"), FCD_TIG6E_P3_0DB},
        {QT_TR_NOOP("+6dB"), FCD_TIG6E_P6_0DB},
        {QT_TR_NOOP("+9dB"), FCD_TIG6E_P9_0DB},
        {QT_TR_NOOP("+12dB"), FCD_TIG6E_P12_0DB},
        {QT_TR_NOOP("+15dB"), FCD_TIG6E_P15_0DB},
        {NULL,0}
};


/** \brief List of all combo boxes.
 *
 * FIXME: Used to have separate SET/GET but with libfcd we only need one.
 */
static COMBO_STRUCT _acs[] =
{
    {FCD_VALUE_LNA_GAIN,     FCD_VALUE_LNA_GAIN,    10, NULL, _cisLNAGain},
    {FCD_VALUE_LNA_ENHANCE,  FCD_VALUE_LNA_ENHANCE,  0, NULL, _cisLNAEnhance},
    {FCD_VALUE_BAND,         FCD_VALUE_BAND,         0, NULL, _cisBand},
    {FCD_VALUE_RF_FILTER,    FCD_VALUE_RF_FILTER,    0, NULL, _cisRFFilter1},
    {FCD_VALUE_MIXER_GAIN,   FCD_VALUE_MIXER_GAIN,   1, NULL, _cisMixerGain},
    {FCD_VALUE_BIAS_CURRENT, FCD_VALUE_BIAS_CURRENT, 3, NULL, _cisBiasCurrent},
    {FCD_VALUE_MIXER_FILTER, FCD_VALUE_MIXER_FILTER, 0, NULL, _cisMixerFilter},
    {FCD_VALUE_IF_GAIN1,     FCD_VALUE_IF_GAIN1,     1, NULL, _cisIFGain1},
    {FCD_VALUE_IF_GAIN_MODE, FCD_VALUE_IF_GAIN_MODE, 0, NULL, _cisIFGainMode},
    {FCD_VALUE_IF_RC_FILTER, FCD_VALUE_IF_RC_FILTER, 0, NULL, _cisIFRCFilter},
    {FCD_VALUE_IF_GAIN2,     FCD_VALUE_IF_GAIN2,     0, NULL, _cisIFGain2},
    {FCD_VALUE_IF_GAIN3,     FCD_VALUE_IF_GAIN3,     0, NULL, _cisIFGain3},
    {FCD_VALUE_IF_GAIN4,     FCD_VALUE_IF_GAIN4,     0, NULL, _cisIFGain4},
    {FCD_VALUE_IF_FILTER,    FCD_VALUE_IF_FILTER,    0, NULL, _cisIFFilter},
    {FCD_VALUE_IF_GAIN5,     FCD_VALUE_IF_GAIN5,     0, NULL, _cisIFGain5},
    {FCD_VALUE_IF_GAIN6,     FCD_VALUE_IF_GAIN6,     0, NULL, _cisIFGain6},
    {0, 0, 0, NULL, NULL}
};



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    prevMode(false),
    ui(new Ui::MainWindow),
    diagramDialog(0)
{
    QSettings settings;

    ui->setupUi(this);

    /* create dock widgets */
    uiDockIfGain = new DockIfGain();

    /* Add dock widgets to main window. This should be done even for
       dock widgets that are going to be hidden, otherwise they will
       end up floating in their own top-level window and can not be
       docked to the mainwindow.
    */
    addDockWidget(Qt::RightDockWidgetArea, uiDockIfGain);

    /* hide dock widgets we don't want to show by default */
    uiDockIfGain->hide();

    /* Add dock widget actions to View menu. This way all signal/slot
       connections will be established automagially.
    */
    ui->menu_View->addAction(uiDockIfGain->toggleViewAction());


    /* Populate Combo Box list structure */
    _acs[0].pComboBox=ui->comboBoxLNAGain;
    _acs[1].pComboBox=ui->comboBoxLNAEnhance;
    _acs[2].pComboBox=ui->comboBoxBand;
    _acs[3].pComboBox=ui->comboBoxRfFilter;
    _acs[4].pComboBox=ui->comboBoxMixerGain;
    _acs[5].pComboBox=ui->comboBoxBiasCurrent;
    _acs[6].pComboBox=ui->comboBoxMixerFilter;
    _acs[7].pComboBox=uiDockIfGain->ui->comboBoxIFGain1;
    _acs[8].pComboBox=uiDockIfGain->ui->comboBoxIFGainMode;
    _acs[9].pComboBox=uiDockIfGain->ui->comboBoxIFRCFilter;
    _acs[10].pComboBox=uiDockIfGain->ui->comboBoxIFGain2;
    _acs[11].pComboBox=uiDockIfGain->ui->comboBoxIFGain3;
    _acs[12].pComboBox=uiDockIfGain->ui->comboBoxIFGain4;
    _acs[13].pComboBox=uiDockIfGain->ui->comboBoxIFFilter;
    _acs[14].pComboBox=uiDockIfGain->ui->comboBoxIFGain5;
    _acs[15].pComboBox=uiDockIfGain->ui->comboBoxIFGain6;

    populateCombos();

    /* frequency correction */
    ui->spinBoxCorr->setValue(settings.value("Correction","-120").toInt());

    /* LNB ofset */
    lnbOffset = settings.value("LnbOffset","0").toInt(); // Stored as Hz
    ui->spinBoxLnb->setValue(lnbOffset/1.0e6);  // Shown as MHz

    /* frequency controller */
    if (lnbOffset < -50e6)
        ui->freqCtrl->Setup(11, 0, 2200e6+lnbOffset, 1, UNITS_MHZ);
    else
        ui->freqCtrl->Setup(11, 50e6+lnbOffset, 2200e6+lnbOffset, 1, UNITS_MHZ);
    ui->freqCtrl->SetFrequency(settings.value("Frequency", 144800000).toInt()+lnbOffset);

    /* FCD status label */
    fcdStatus = new QLabel(tr("FCD status..."), this);
    fcdStatus->setAlignment(Qt::AlignRight);
    fcdStatus->setToolTip(tr("Funcube Dongle status can be No FCD, Application or Bootloader"));
    ui->statusBar->addPermanentWidget(fcdStatus);

    fwDialog = new CFirmware(this);
    connect(fwDialog, SIGNAL(finished(int)), this, SLOT(fwDialogFinished(int)));

    //ReadDevice(); /* disabled until it can properly set default values in case of error */

    /* create FCDE device */
    dongle = fcd_open(NULL);
    if (!dongle)
    {
        qDebug() << "FCD not found.";
    }

    enableControls();

    setUnifiedTitleAndToolBarOnMac(true);

    /* connect signals & slots */
    connect(ui->freqCtrl, SIGNAL(NewFrequency(qint64)), this, SLOT(setNewFrequency(qint64)));

    /* if gains */
    connect(uiDockIfGain->ui->comboBoxIFGainMode, SIGNAL(activated(int)), this, SLOT(setIfGainMode(int)));
    connect(uiDockIfGain->ui->comboBoxIFRCFilter, SIGNAL(activated(int)), this, SLOT(setIfRcFilter(int)));
    connect(uiDockIfGain->ui->comboBoxIFFilter, SIGNAL(activated(int)), this, SLOT(setIfFilter(int)));
    connect(uiDockIfGain->ui->comboBoxIFGain1, SIGNAL(activated(int)), this, SLOT(setIfGain1(int)));
    connect(uiDockIfGain->ui->comboBoxIFGain2, SIGNAL(activated(int)), this, SLOT(setIfGain2(int)));
    connect(uiDockIfGain->ui->comboBoxIFGain3, SIGNAL(activated(int)), this, SLOT(setIfGain3(int)));
    connect(uiDockIfGain->ui->comboBoxIFGain4, SIGNAL(activated(int)), this, SLOT(setIfGain4(int)));
    connect(uiDockIfGain->ui->comboBoxIFGain5, SIGNAL(activated(int)), this, SLOT(setIfGain5(int)));
    connect(uiDockIfGain->ui->comboBoxIFGain6, SIGNAL(activated(int)), this, SLOT(setIfGain6(int)));

}

MainWindow::~MainWindow()
{
    QSettings settings;

    if (dongle)
        fcd_close(dongle);

    delete uiDockIfGain;

    settings.setValue("Frequency",ui->freqCtrl->GetFrequency()-lnbOffset);
    settings.setValue("Correction",ui->spinBoxCorr->value());
    settings.setValue("LnbOffset", lnbOffset);

    delete ui;
}


/** \brief Populates a combo box with all its items and selects the default item
  * \param box Pointer to the QComboBox to populate.
  * \param nIdxDefault Index of the default item.
  * \param pcis Pointer to the array containing the data for the combo box items
  */
void MainWindow::populateCombo(QComboBox *box, int nIdxDefault, const COMBO_ITEM_STRUCT *pcis)
{
    box->clear();
    while(pcis->pszDesc != NULL){
        box->addItem(QString(pcis->pszDesc));
        pcis++;
    }
    box->setCurrentIndex(nIdxDefault);
}


/** \brief Populate combo boxes
  *
  */
void MainWindow::populateCombos()
{
    COMBO_STRUCT *pcs = _acs;

    /* iterate trough all combo boxes */
    while (pcs->pacis!=NULL)
    {
        const COMBO_ITEM_STRUCT *pcis = pcs->pacis;
        populateCombo(pcs->pComboBox, pcs->nIdxDefault, pcis);
        pcs++;
    }
}


/** \brief Enable or disable combo boxes
  * \param enabled Flag indicating whether combo boxes should be enabled or disabled
  *
  */
void MainWindow::enableCombos(bool enabled)
{
    COMBO_STRUCT *pcs = _acs;

    /* iterate trough all combo boxes */
    while (pcs->pacis!=NULL)
    {
        pcs->pComboBox->setEnabled(enabled);
        pcs++;
    }

    /* Band selection happens automatically; ensure band selector is disabled */
    /** Leave combo enabled to allow forcing band selection as a workaround
        for band selection bug in FW 18f: http://uk.groups.yahoo.com/group/funcube/message/1139
    **/
    //if (enabled)
    //    ui->comboBoxBand->setEnabled(false);
}


/** \brief Read all parameters from FCD.
  * \note "All" refers to the combo box settings and the bias tee button.
  */
void MainWindow::readDevice()
{
    COMBO_STRUCT *pcs=_acs;
    quint8 u8;
    bool error = false;

#if 0
    /* bias tee button */
    /** FIXME: FCD_CMD_APP_GET_BIAS_TEE doesn't work? **/
    /*fme = fcdAppGetParam(FCD_CMD_APP_GET_BIAS_TEE, &u8, 1);
    if (fme == FCD_MODE_APP) {
        ui->pushButtonBiasT->setChecked((u8==1));
    }
    else {
        qDebug() << "ERROR reading bias tee";
        error = true;
    }*/

    /* iterate through all combo boxes */
    while (pcs->pacis!=NULL)
    {
        const COMBO_ITEM_STRUCT *pcis = pcs->pacis;
        int nIdx=0;

        /* read FCD value for this combo box */
        fme = fcdAppGetParam(pcs->u8CommandGet, &u8, 1);

        /* if read was successful try to use read value */
        if (fme == FCD_MODE_APP) {

            /* Try to find the index to the register field value */
            while (pcis->pszDesc!=NULL && pcis->u8Val!=u8)
            {
                nIdx++;
                pcis++;
            }

            if (pcis->pszDesc != NULL) {
                pcs->pComboBox->setCurrentIndex(nIdx);
            } else {
                pcs->pComboBox->setCurrentIndex(pcs->nIdxDefault);
            }
        } else {
            /* use default value to display */
            pcs->pComboBox->setCurrentIndex(pcs->nIdxDefault);

            error = true;
        }

        bandChange();
        pcs++;
    }

    /* push a message to the status bar */
    if (error) {
        qDebug() << "There were errors while reading settings from FCD";
        ui->statusBar->showMessage(tr("There were errors while reading settings from FCD"), 4000);
    }
    else {
        qDebug() << "Successfully read settings from FCD";
        ui->statusBar->showMessage(tr("Successfully read settings from FCD"), 4000);
    }
#endif
    qDebug() << "FIXME: Not implemenbted!";
}


/** \brief Switch to new frequency band.
  *
  * This function will update the RF filter combo according to the active selection in the
  * Band combo box (different bands have different set of filters). Band selection happens
  * automatically by the FCD when we change the frequency adn we must update the UI when
  * such events occur.
  */
void MainWindow::bandChange()
{
    static const COMBO_ITEM_STRUCT *apcis[4] = {_cisRFFilter0,_cisRFFilter1,_cisRFFilter2,_cisRFFilter3};
    int nIdx = ui->comboBoxBand->currentIndex();
    int nIdxOrg = ui->comboBoxRfFilter->currentIndex();

    populateCombo(ui->comboBoxRfFilter, nIdxOrg, apcis[nIdx]);

}


/** \brief Convert a string to double */
double MainWindow::StrToDouble(QString s)
{
    int i;
    QString s2="";

    for (i=0;i<s.length();i++)
    {
        QChar c=s[i];
        if (c>='0' and c<='9')
        {
            s2+=c;
        }
    }
    s2=s2.left(10);

    return s2.toDouble();
}

/** \brief Eanble/disable controls depending on FCD mode.
  *
  * This function reads the FCD mode and enables or disables the UI controls accordingly.
  * \todo Combo boxes.
  */
void MainWindow::enableControls()
{
    char buffer[65];
    bool hasBiasT = false;
    bool isAppMode = false;

    if (!dongle)
    {
        fcdStatus->setText(tr("No FCD detected"));
        return;
    }

    if (!fcd_query(dongle, &buffer[0], 65))
    {
        qDebug() << __FUNCTION__ << "FCD query failed";
        return;
    }

    QString fcdString(buffer);
    qDebug() << "FCD query returned:" << fcdString;

    if (fcdString.contains("FCDBL"))
    {
        fcdStatus->setText(tr("FCD bootloader"));
    }
    else if (fcdString.contains("FCDAPP"))
    {
        /*
         * When the FCD is in application mode, the string returned by the query command is:
         *   FCDAPP 18.08 Brd 1.0 No blk
         * 1.0 means no bias tee, 1.1 means there is a bias tee
         * 'No blk' means it is not cellular blocked.
         *
         * Ref: http://uk.groups.yahoo.com/group/FCDevelopment/message/303
         */
        QStringList list = fcdString.split(" ");
        fcdStatus->setText(tr("FCD is active (FW: %1)").arg(list[1]));
        isAppMode = true;
        hasBiasT = (list[3] == "1.0");

        // check PLL lock  FIXME: not avaialble in libfcd
        //u8=0;
        //fcdAppGetParam(FCD_CMD_APP_GET_PLL_LOCK, &u8, 1);
        //ui->checkBoxPLLLock->setChecked(u8==1);
        qDebug() << __FUNCTION__ << "FIXME: check PLL lock not implemented";
    }

    ui->freqCtrl->setEnabled(isAppMode);

    /* bias T functionality available since FW 18h */
    ui->pushButtonBiasT->setEnabled((isAppMode) && (hasBiasT));

    ui->spinBoxCorr->setEnabled(isAppMode);

    ui->actionBalance->setEnabled(isAppMode);
    ui->actionFirmware->setEnabled(isAppMode);
    ui->actionDefault->setEnabled(isAppMode);

    uiDockIfGain->setEnabled(isAppMode);

    /* manage FCD mode transitions */
    if (isAppMode != prevMode) {
        qDebug() << "FCD mode change:" << prevMode << "->" << isAppMode;
        ui->statusBar->showMessage(tr("FCD mode change detected"), 2000);

        if (isAppMode) {
            /* if previous mode was different read settings from device */
            readDevice();

            /* Set frequency since FCD does not remember anything */
            setNewFrequency(ui->freqCtrl->GetFrequency());
        }
    }

    prevMode = isAppMode;
}


/*! \brief Slot for receiving frequency change signals.
 *  \param[in] freq The new frequency.
 *
 * This slot is connected to the CFreqCtrl::NewFrequency() signal and is used
 * to set new RF frequency.
 */
void MainWindow::setNewFrequency(qint64 freq)
{
    double d = (double) (freq-lnbOffset);

    if (!dongle)
    {
        return;
    }

    d *= 1.0 + ui->spinBoxCorr->value()/1000000.0;

    qDebug() << "Set new frequency";
    qDebug() << "    Display:" << freq;
    qDebug() << "    LNB_offset:" << lnbOffset;
    qDebug() << "    FCD set:" << d;

    if (fcd_set_frequency_Hz(dongle, (unsigned int)(d/1000.0)))
    {
        qWarning() << "Failed to set frequency";
        ui->statusBar->showMessage(tr("Failed to set frequency"), 3000);

        return;
    }

    /** TODO: Read back frequency **/


    /* band changes occur automatically in FCD when we change frequency */
    quint8 u8;

    /* read band selection form FCD */
    if (fcd_get_value(dongle, FCD_VALUE_BAND, &u8) == 0)
    {
        if (u8 != ui->comboBoxBand->currentIndex())
        {
            qDebug() << "Band change detected:" << u8;
            ui->comboBoxBand->setCurrentIndex(u8);
            bandChange();
        }
    }
    /* else we ignore it */

    /* filter */
    if (fcd_get_value(dongle, FCD_VALUE_RF_FILTER, &u8) == 0)
    {
        if (u8 != ui->comboBoxRfFilter->currentIndex())
        {
            qDebug() << "RF filter change detected:" << u8;
            ui->comboBoxRfFilter->setCurrentIndex(u8);
        }
    }

    /* bias current */
    if (fcd_get_value(dongle, FCD_VALUE_BIAS_CURRENT, &u8) == 0)
    {
        if (u8 != ui->comboBoxBiasCurrent->currentIndex())
        {
            qDebug() << "Bias current change detected:" << u8;
            ui->comboBoxBiasCurrent->setCurrentIndex(u8);
        }
    }

}


/** \brief Bias T button ON/OFF
  * \param isOn Flag indicating whether the button is ON or OFF
  *
  * This slot is called when the user toggles the Bias T button. It is used
  * to switch the bias tee power ON and OFF.
  * isOn = true => power ON
  * isOn = false => power OFF
  */
void MainWindow::on_pushButtonBiasT_toggled(bool isOn)
{
    quint8 u8Write = isOn ? 1 : 0;
#if 0
    fcdAppSetParam(FCD_CMD_APP_SET_BIAS_TEE, &u8Write, 1);
#endif
    qDebug() << "FIXME: Not implemented!";
}


/** \brief Frequency correction changed.
  * \param n New correction value in ppm.
  *
  * This slot is called when the value of the frequency correction spin button
  * is changed.
  */
void MainWindow::on_spinBoxCorr_valueChanged(int n)
{
    /*** FIXME!! ***/
    double d = (double) (ui->freqCtrl->GetFrequency()-lnbOffset);

    d *= 1.0 + n/1000000.0;
#if 0
    fcdAppSetFreqkHz((int)(d/1000.0));
#endif
    qDebug() << "FIXME: Not implemented!";
}

/** \brief LNB frequency offset changed.
  * \param value The new frequewncy offset in MHz.
  *
  * This slot is called when the value of the LNB frequency offset is changed
  * by the user. The frequency shown on the display is calculated as the FCD
  * frequency + the LNB offset. This means that we use positive offset for
  * downconverters and negative offset for upconverters.
  *
  * When the LNB frequewncy offset is changed, the FCD frequency is kept and
  * the displayed frequency is adjusted.
  */
void MainWindow::on_spinBoxLnb_valueChanged(double value)
{
    qint64 currDispFreq = ui->freqCtrl->GetFrequency();
    qint64 currFcdFreq = currDispFreq - lnbOffset;

    /* convert to Hz */
    lnbOffset = (qint64)(value*1.0e6);

    qDebug() << "New LNB offset:" << lnbOffset;

    /* update controller limits */
    if (lnbOffset < -50e6)
        ui->freqCtrl->Setup(11, 0, 2200e6+lnbOffset, 1, UNITS_MHZ);
    else
        ui->freqCtrl->Setup(11, 50e6+lnbOffset, 2200e6+lnbOffset, 1, UNITS_MHZ);

    /* update display frequency (changes with LNB offset) */
    ui->freqCtrl->SetFrequency(currFcdFreq + lnbOffset);
#if 0
    /* set new FCD frequency */
    setNewFrequency(ui->freqCtrl->GetFrequency());
#endif
    qDebug() << "FIXME: Not implemented!";
}

void MainWindow::on_comboBoxLNAGain_activated(int index)
{
    quint8 u8Write = _acs[0].pacis[index].u8Val;
    //fcdAppSetParam(_acs[0].u8CommandSet, &u8Write, 1);
    qDebug() << "FIXME: Not implemented!";
}

void MainWindow::on_comboBoxLNAEnhance_activated(int index)
{
    quint8 u8Write = _acs[1].pacis[index].u8Val;
    //fcdAppSetParam(_acs[1].u8CommandSet, &u8Write, 1);
    qDebug() << "FIXME: Not implemented!";
}

void MainWindow::on_comboBoxBand_activated(int index)
{
#if 0
    quint8 u8Write = _acs[2].pacis[index].u8Val;
    fcdAppSetParam(_acs[2].u8CommandSet, &u8Write, 1);

    /* trigger bandChange() to update RF filter combo */
    bandChange();

    /* select filter chosen by FCD */
    FCD_MODE_ENUM fme;
    quint8 u8;

    fme = fcdAppGetParam(FCD_CMD_APP_GET_RF_FILTER, &u8, 1);
    if (fme == FCD_MODE_APP) {
        if (u8 != ui->comboBoxRfFilter->currentIndex()) {
            ui->statusBar->showMessage(tr("RF filter change detected (%1)").arg(u8), 4000);
            qDebug() << "RF filter change detected:" << u8;
            ui->comboBoxRfFilter->setCurrentIndex(u8);
        }
    }
#endif
    qDebug() << "FIXME: Not implemented!";
}

void MainWindow::on_comboBoxRfFilter_activated(int index)
{
    quint8 u8Write = _acs[3].pacis[index].u8Val;
    //fcdAppSetParam(_acs[3].u8CommandSet, &u8Write, 1);
    qDebug() << "FIXME: Not implemented!";
}

void MainWindow::on_comboBoxMixerGain_activated(int index)
{
    quint8 u8Write = _acs[4].pacis[index].u8Val;
    //fcdAppSetParam(_acs[4].u8CommandSet, &u8Write, 1);
    qDebug() << "FIXME: Not implemented!";
}

void MainWindow::on_comboBoxBiasCurrent_activated(int index)
{
    quint8 u8Write = _acs[5].pacis[index].u8Val;
    //fcdAppSetParam(_acs[5].u8CommandSet, &u8Write, 1);
    qDebug() << "FIXME: Not implemented!";
}

void MainWindow::on_comboBoxMixerFilter_activated(int index)
{
    quint8 u8Write = _acs[6].pacis[index].u8Val;
    //fcdAppSetParam(_acs[6].u8CommandSet, &u8Write, 1);
    qDebug() << "FIXME: Not implemented!";
}

void MainWindow::setIfGainMode(int index)
{
    quint8 u8Write = _acs[8].pacis[index].u8Val;
    //fcdAppSetParam(_acs[8].u8CommandSet, &u8Write, 1);
    qDebug() << "FIXME: Not implemented!";
}

void MainWindow::setIfGain1(int index)
{
    quint8 u8Write = _acs[7].pacis[index].u8Val;
    //fcdAppSetParam(_acs[7].u8CommandSet, &u8Write, 1);
    qDebug() << "FIXME: Not implemented!";
}

void MainWindow::setIfRcFilter(int index)
{
    quint8 u8Write = _acs[9].pacis[index].u8Val;
    //fcdAppSetParam(_acs[9].u8CommandSet, &u8Write, 1);
    qDebug() << "FIXME: Not implemented!";
}

void MainWindow::setIfGain2(int index)
{
    quint8 u8Write = _acs[10].pacis[index].u8Val;
    //fcdAppSetParam(_acs[10].u8CommandSet, &u8Write, 1);
    qDebug() << "FIXME: Not implemented!";
}

void MainWindow::setIfGain3(int index)
{
    quint8 u8Write = _acs[11].pacis[index].u8Val;
    //fcdAppSetParam(_acs[11].u8CommandSet, &u8Write, 1);
    qDebug() << "FIXME: Not implemented!";
}

void MainWindow::setIfGain4(int index)
{
    quint8 u8Write = _acs[12].pacis[index].u8Val;
    //fcdAppSetParam(_acs[12].u8CommandSet, &u8Write, 1);
    qDebug() << "FIXME: Not implemented!";
}

void MainWindow::setIfFilter(int index)
{
    quint8 u8Write = _acs[13].pacis[index].u8Val;
    //fcdAppSetParam(_acs[13].u8CommandSet, &u8Write, 1);
    qDebug() << "FIXME: Not implemented!";
}

void MainWindow::setIfGain5(int index)
{
    quint8 u8Write = _acs[14].pacis[index].u8Val;
    //fcdAppSetParam(_acs[14].u8CommandSet, &u8Write, 1);
    qDebug() << "FIXME: Not implemented!";
}

void MainWindow::setIfGain6(int index)
{
    quint8 u8Write = _acs[15].pacis[index].u8Val;
    //fcdAppSetParam(_acs[15].u8CommandSet,&u8Write,1);
    qDebug() << "FIXME: Not implemented!";
}


/** \brief Action: Load FCD settigns from file. */
void MainWindow::on_actionLoad_triggered()
{
    qDebug() << "MainWindow::on_actionLoad_triggered() not implemented";
}


/** \brief Action: Save FCD settings to file. */
void MainWindow::on_actionSave_triggered()
{
    qDebug() << "MainWindow::on_actionSave_triggered() not implemented";
}


/*! \brief Action: Open I/Q correction settings. */
void MainWindow::on_actionBalance_triggered()
{
    CIqBalance *iqDialog = new CIqBalance(this);

    iqDialog->exec();

    delete iqDialog;
}


/** \brief Action: Open firmware tools. */
void MainWindow::on_actionFirmware_triggered()
{
    //fwDialog = new CFirmware(this);
    //connect(fwDialog, SIGNAL(finished(int)), this, SLOT(fwDialogFinished(int)));

    /* set FCD in bootloader mode */
    //timer->stop();
    //fcdAppReset();
    //timer->start(1000);
    qDebug() << "FIXME: Not implemented!";

    //fwDialog->show();
}


/*! \brief Slot: Firmware dialog finished. */
void MainWindow::fwDialogFinished(int result)
{
    qDebug() << "FW dialog finished. Result:" << result;

    //disconnect(fwDialog, SIGNAL(finished(int)), this, SLOT(fwDialogFinished(int)));
    //delete fwDialog;
    //fwDialog->hide();

    /* set FCD back to application mode */
    //timer->stop();
    //fcdBlReset();
    //timer->start(1000);
    qDebug() << "FIXME: Not implemented!";
}


/*! \brief Action: Reset to defaulrts. */
void MainWindow::on_actionDefault_triggered()
{
     QMessageBox::StandardButton but = QMessageBox::question(this,
                                                             tr("Reset FCD"),
                                                             tr("Reset FCD to default settings?"),
                                                             QMessageBox::Yes | QMessageBox::No,
                                                             QMessageBox::No);
    if (but == QMessageBox::No)
        return;

    // perform reset
    COMBO_STRUCT *pcs=_acs;
#if 0
    while (pcs->pacis!=NULL)
    {
        quint8 u8Write = pcs->pacis[pcs->nIdxDefault].u8Val;
        fcdAppSetParam(pcs->u8CommandSet, &u8Write, 1);
        pcs++;
    }

    readDevice();
#endif
    qDebug() << "FIXME: Not implemented!";
    ui->statusBar->showMessage(tr("FCD has been reset"), 5000);
}

/** \brief Action: About Qthid
  *
  * This slot is called when the user activates the
  *   Help|About menu item (or Qthid|About on Mac)
  */
void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About Qthid"),
                       tr("<p>This is qthid %1</p>"
                          "<p>Qthid is a simple controller application for the Funcube Dongle "
                          "software defined radio receiver for 64 MHz - 1.7 GHz.</p>"
                          "<p>Qthid can "
                          "be used to upload new firmware and to set various parameters "
                          "such as frequency, gain, filters, etc.</p>"
                          "<p>Qthid is written using the Qt toolkit (see About Qt) and is avaialble "
                          "for Linux, Mac and Windows. You can download the latest version from the "
                          "<a href='http://www.oz9aec.net/index.php/funcube-dongle/qthid-fcd-controller'>Qthid website</a>."
                          "</p>"
                          "<p>"
                          "<a href='http://funcubedongle.com/'>Funcube Dongle website</a><br/>"
                          "<a href='http://funcube.org.uk/'>Information about Funcube</a>"
                          "</p>").arg(VERSION));
}

/** \brief Action: About Qt
  *
  * This slot is called when the user activates the
  *   Help|About Qt menu item (or Qthid|About Qt on Mac)
  */
void MainWindow::on_actionAboutQt_triggered()
{
    QMessageBox::aboutQt(this, tr("About Qt"));
}


/*! \brief Action: Show FCD diagram. */
void MainWindow::on_actionDiagram_triggered()
{
    if (!diagramDialog) {
        diagramDialog = new FcdDiagram(this);
    }

    // FIXME: should we connect close signal and destroy dialog every time it's closed?

    diagramDialog->show();
    diagramDialog->raise();
    diagramDialog->activateWindow();
}
