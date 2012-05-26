/***************************************************************************
 *  This file is part of Qthid.
 *
 *  CopyRight (C) 2011-2012  Alexandru Csete, OZ9AEC
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
#include <QSettings>
#include <QDebug>
#include <fcd.h>
#include <fcd_tuner.h>
#include "iqbalance.h"
#include "ui_iqbalance.h"


CIqBalance::CIqBalance(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CIqBalance)
{
    ui->setupUi(this);

    /* load stored values */
    QSettings settings;
    ui->doubleSpinBoxDCI->setValue(settings.value("DCICorr","0.0").toDouble());
    ui->doubleSpinBoxDCQ->setValue(settings.value("DCQCorr","0.0").toDouble());
    ui->doubleSpinBoxPhase->setValue(settings.value("PhaseCorr","0.0").toDouble());
    ui->doubleSpinBoxGain->setValue(settings.value("GainCorr","1.0").toDouble());

}

CIqBalance::~CIqBalance()
{
    /* store values */
    QSettings settings;
    settings.setValue("DCICorr",ui->doubleSpinBoxDCI->value());
    settings.setValue("DCQCorr",ui->doubleSpinBoxDCQ->value());
    settings.setValue("PhaseCorr",ui->doubleSpinBoxPhase->value());
    settings.setValue("GainCorr",ui->doubleSpinBoxGain->value());

    delete ui;

}


/*! \brief In-phase DC offset correction changed. */
void CIqBalance::on_doubleSpinBoxDCI_valueChanged(double value)
{
    union {
        unsigned char auc[4];
        struct {
            qint16 dci;
            qint16 dcq;
        };
    } dcinfo;

    dcinfo.dci = static_cast<signed short>(value*32768.0);
    dcinfo.dcq = static_cast<signed short>(ui->doubleSpinBoxDCQ->value()*32768.0);

    //fcdAppSetParam(FCD_CMD_APP_SET_DC_CORR, dcinfo.auc, 4);
    qDebug() << "FIXME: Not implemented!";
}


/*! \brief Quadrature DC offset correction changed. */
void CIqBalance::on_doubleSpinBoxDCQ_valueChanged(double value)
{
    union {
        unsigned char auc[4];
        struct {
            qint16 dci;
            qint16 dcq;
        };
    } dcinfo;

    dcinfo.dci = static_cast<signed short>(ui->doubleSpinBoxDCI->value()*32768.0);
    dcinfo.dcq = static_cast<signed short>(value*32768.0);

    //fcdAppSetParam(FCD_CMD_APP_SET_DC_CORR, dcinfo.auc, 4);
    qDebug() << "FIXME: Not implemented!";
}


/*! \brief IQ correction phase changed. */
void CIqBalance::on_doubleSpinBoxPhase_valueChanged(double value)
{
    union {
        unsigned char auc[4];
        struct {
            qint16 phase;
            qint16 gain;
        };
    } iqinfo;

    iqinfo.phase = static_cast<signed short>(value*32768.0);
    iqinfo.gain = static_cast<signed short>(ui->doubleSpinBoxGain->value()*32768.0);

    //fcdAppSetParam(FCD_CMD_APP_SET_IQ_CORR, iqinfo.auc, 4);
    qDebug() << "FIXME: Not implemented!";
}


/*! \brief IQ correction gain changed. */
void CIqBalance::on_doubleSpinBoxGain_valueChanged(double value)
{
    union {
        unsigned char auc[4];
        struct {
            qint16 phase;
            qint16 gain;
        };
    } iqinfo;

    iqinfo.phase = static_cast<signed short>(ui->doubleSpinBoxPhase->value()*32768.0);
    iqinfo.gain = static_cast<signed short>(value*32768.0);

    //fcdAppSetParam(FCD_CMD_APP_SET_IQ_CORR, iqinfo.auc, 4);
    qDebug() << "FIXME: Not implemented!";
}


/*! \brief Revert button has been clicked.
 *
 * Revert I/Q correction values to the previously stored values.
 */
void CIqBalance::on_revertButton_clicked()
{
    QSettings settings;
    ui->doubleSpinBoxDCI->setValue(settings.value("DCICorr","0.0").toDouble());
    ui->doubleSpinBoxDCQ->setValue(settings.value("DCQCorr","0.0").toDouble());
    ui->doubleSpinBoxPhase->setValue(settings.value("PhaseCorr","0.0").toDouble());
    ui->doubleSpinBoxGain->setValue(settings.value("GainCorr","1.0").toDouble());
}


/*! \brief Reset button has been clicked. */
void CIqBalance::on_resetButton_clicked()
{
    ui->doubleSpinBoxDCI->setValue(0.0);
    ui->doubleSpinBoxDCQ->setValue(0.0);
    ui->doubleSpinBoxPhase->setValue(0.0);
    ui->doubleSpinBoxGain->setValue(1.0);
}
