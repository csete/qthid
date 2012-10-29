/***************************************************************************
 *  This file is part of Qthid.
 *
 *  Copyright (C) 2010       Howard Long, G6LVB
 *  Copyright (C) 2011       Mario Lorenz, DL5MLO
 *  Copyright (C) 2011-2012  Alexandru Csete, OZ9AEC
 *
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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "freqctrl.h"
#include "iqbalance.h"
#include "firmware.h"
#include "hidapi.h"
#include "fcd.h"
//#include "fcdhidcmd.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    prevMode(FCD_MODE_NONE)
{
    QSettings settings;

    ui->setupUi(this);

    setWindowTitle(tr("Qthid %1").arg(VERSION));

    /* frequency correction */
    ui->spinBoxCorr->setValue(settings.value("Correction","0").toInt());

    /* LNB ofset */
    lnbOffset = settings.value("LnbOffset","0").toInt(); // Stored as Hz
    ui->spinBoxLnb->setValue(lnbOffset/1.0e6);  // Shown as MHz

    /* frequency controller */
    if (lnbOffset < -150e3)
        ui->freqCtrl->Setup(11, 0, 2050e6+lnbOffset, 1, UNITS_MHZ);
    else
        ui->freqCtrl->Setup(11, 150e3+lnbOffset, 2050e6+lnbOffset, 1, UNITS_MHZ);
    ui->freqCtrl->SetFrequency(settings.value("Frequency", 144800000).toInt()+lnbOffset);

    /* FCD status label */
    fcdStatus = new QLabel(tr("FCD status..."), this);
    fcdStatus->setAlignment(Qt::AlignRight);
    fcdStatus->setToolTip(tr("Funcube Dongle status can be No FCD, Application or Bootloader"));
    ui->statusBar->addPermanentWidget(fcdStatus);

    fwDialog = new CFirmware(this);
    connect(fwDialog, SIGNAL(finished(int)), this, SLOT(fwDialogFinished(int)));

    //ReadDevice(); /* disabled until it can properly set default values in case of error */

    enableControls();


    setUnifiedTitleAndToolBarOnMac(true);

    /* connect signals & slots */
    connect(ui->freqCtrl, SIGNAL(NewFrequency(qint64)), this, SLOT(setNewFrequency(qint64)));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(enableControls()));
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    QSettings settings;

    timer->stop();
    delete timer;

    settings.setValue("Frequency",ui->freqCtrl->GetFrequency()-lnbOffset);
    settings.setValue("Correction",ui->spinBoxCorr->value());
    settings.setValue("LnbOffset", lnbOffset);
    /** TODO: LNA **/
    /** TODO: MIXER **/
    /** TODO: others? **/

    delete ui;
}

/** \brief Enable or disable combo boxes
  * \param enabled Flag indicating whether combo boxes should be enabled or disabled
  *
  */
void MainWindow::enableCombos(bool enabled)
{
    qDebug() << "FIXME: Implement" << __func__;
}

/** \brief Read all parameters from FCD.
  * \note "All" refers to the combo box settings and the bias tee button.
  */
void MainWindow::readDevice()
{
    quint8 u8;
    FCD_MODE_ENUM fme;
    bool error = false;

    qDebug() << "FIXME: Implement" << __func__;

    /* push a message to the status bar */
    if (error) {
        qDebug() << "There were errors while reading settings from FCD";
        ui->statusBar->showMessage(tr("There were errors while reading settings from FCD"), 4000);
    }
    else {
        qDebug() << "Successfully read settings from FCD";
        ui->statusBar->showMessage(tr("Successfully read settings from FCD"), 4000);
    }
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
    FCD_MODE_ENUM fme = FCD_MODE_NONE;
    quint8 u8;
    char fwVerStr[6];
    bool convOk = false;
    float fwVer = 0.0;

    fme = fcdGetMode();

    switch (fme)
    {
        case FCD_MODE_APP:
            fcdGetFwVerStr(fwVerStr);
            fcdStatus->setText(tr("FCD is active (%1)").arg(QString(fwVerStr)));

            /* convert version string to float */
            fwVer = QString(fwVerStr).toFloat(&convOk);

            /** FIXME: PLL lock **/
            u8=0;
            //fcdAppGetParam(FCD_CMD_APP_GET_PLL_LOCK, &u8, 1);
            ui->checkBoxPLLLock->setChecked(u8==1);

            break;

        case FCD_MODE_BL:
            fcdStatus->setText(tr("FCD bootloader"));
            break;

        case FCD_MODE_NONE:
            fcdStatus->setText(tr("No FCD detected"));
            break;
    }

    ui->freqCtrl->setEnabled(fme==FCD_MODE_APP);

    /** FIXME bias T functionality available since FW 18h */
    ui->pushButtonBiasT->setEnabled(false);

    ui->spinBoxLnb->setEnabled(fme==FCD_MODE_APP);
    ui->spinBoxCorr->setEnabled(fme==FCD_MODE_APP);

    //ui->actionBalance->setEnabled(fme==FCD_MODE_APP);
    //ui->actionFirmware->setEnabled(fme==FCD_MODE_APP);
    ui->actionDefault->setEnabled(fme==FCD_MODE_APP);

    /* manage FCD mode transitions */
    if (fme != prevMode) {
        qDebug() << "FCD mode change:" << prevMode << "->" << fme;
        ui->statusBar->showMessage(tr("FCD mode change detected"), 2000);

        if (fme == FCD_MODE_APP) {
            /* if previous mode was different read settings from device */
            readDevice();

            /* Set frequency since FCD does not remember anything */
            setNewFrequency(ui->freqCtrl->GetFrequency());
        }
    }

    prevMode = fme;
}


/*! \brief Slot for receiving frequency change signals.
 *  \param[in] freq The new frequency.
 *
 * This slot is connected to the CFreqCtrl::NewFrequency() signal and is used
 * to set new RF frequency.
 */
void MainWindow::setNewFrequency(qint64 freq)
{
    FCD_MODE_ENUM fme;
    unsigned int uFreq, rFreq;
    double d = (double) (freq-lnbOffset);

    d *= 1.0 + ui->spinBoxCorr->value()/1000000.0;
    uFreq = (unsigned int) d;

    fme = fcdAppSetFreq(uFreq, &rFreq);

    qDebug() << "Set new frequency";
    qDebug() << "    Display:" << freq << "Hz";
    qDebug() << " LNB offset:" << lnbOffset << "Hz";
    qDebug() << "    FCD set:" << uFreq << "Hz";
    qDebug() << "    FCD get:" << rFreq << "Hz";

    if ((fme != FCD_MODE_APP) || (uFreq != rFreq))
    {
        ui->statusBar->showMessage(tr("Failed to set frequency"), 2000);
        qDebug() << "Error in" << __func__ << "set:" << uFreq << "read:" << rFreq;
    }

    /** TODO: Check for RF filter settings **/

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

    qDebug() << "FIXME: Implement" << __func__;
}


/** \brief Frequency correction changed.
  * \param n New correction value in ppm.
  *
  * This slot is called when the value of the frequency correction spin button
  * is changed.
  */
void MainWindow::on_spinBoxCorr_valueChanged(int n)
{
    FCD_MODE_ENUM fme;
    unsigned int uFreq, rFreq;
    double d = (double) (ui->freqCtrl->GetFrequency()-lnbOffset);

    d *= 1.0 + n/1000000.0;
    uFreq = (unsigned int) d;

    fme = fcdAppSetFreq(uFreq, &rFreq);

    if ((fme != FCD_MODE_APP) || (uFreq != rFreq))
    {
        ui->statusBar->showMessage(tr("Failed to set frequency"), 2000);
        qDebug() << "Error in" << __func__ << "set:" << uFreq << "read:" << rFreq;
    }
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
    timer->stop();
    fcdAppReset();
    timer->start(1000);

    fwDialog->show();
}


/*! \brief Slot: Firmware dialog finished. */
void MainWindow::fwDialogFinished(int result)
{

    qDebug() << "FIXME: Implement" << __func__;

 #if 0
    qDebug() << "FW dialog finished. Result:" << result;



    //disconnect(fwDialog, SIGNAL(finished(int)), this, SLOT(fwDialogFinished(int)));
    //delete fwDialog;
    fwDialog->hide();

    /* set FCD back to application mode */
    timer->stop();
    fcdBlReset();
    timer->start(1000);
#endif
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

    qDebug() << "FIXME: Implement" << __func__;

    //readDevice();

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
                          "<p>Qthid is a simple controller application for the Funcube Dongle Pro+"
                          "</p>"
                          "<p>Qthid can be used to set and read the various such as frequency, bias T, "
                          "LNA, and whatever else is supported by the firmware.</p>"
                          "<p>Qthid is written using the Qt toolkit (see About Qt) and is available "
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
