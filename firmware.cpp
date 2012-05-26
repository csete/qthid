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
#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QFile>
#include <QSettings>
#include <QMessageBox>
#include <fcd.h>
#include "firmware.h"
#include "ui_firmware.h"


CFirmware::CFirmware(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CFirmware)
{
    ui->setupUi(this);

    // disable buttons if lineEdit is empty
    checkFirmwareSelection(ui->lineEdit->text());
}

CFirmware::~CFirmware()
{
    qDebug() << "Firmware dialog destroyed";
    delete ui;
}

void CFirmware::closeEvent(QCloseEvent *event)
{
    qDebug() << "fwDialog closeEvent detected";
    event->ignore();
    finished(42);
}

/*! \brief Select a firmware file on disk. */
void CFirmware::on_selectButton_clicked()
{
    // retrieve last used folder
    QSettings settings;
    QString path = settings.value("LastFwFolder", QDir::currentPath()).toString();

    // execute modal file selector and get FW file name
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open FCD firmware"),
                                                    path,
                                                    tr("FCD firmware files (*.bin)"));

    if (!fileName.isNull())
    {
        // store selected folder
        QFileInfo fileInfo(fileName);
        qDebug() << "FW folder:" << fileInfo.absolutePath();
        settings.setValue("LastFwFolder", fileInfo.absolutePath());

        // show firmware file path
        ui->lineEdit->setText(fileName);
    }

}

void CFirmware::on_uploadButton_clicked()
{
    QFile qf(ui->lineEdit->text());
    qint64 qn64size = qf.size();
    char *buf=new char[qn64size];

    if (buf==NULL)
    {
        QMessageBox::critical(this, tr("FCD"), tr("Unable to allocate memory for firmware image"));
        return;
    }

    if (!qf.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this, tr("FCD"), tr("Unable to open firmware file:\n").arg(ui->lineEdit->text()));
        delete buf;

        return;
    }
    else
    {
        if (qf.read(buf,qn64size) != qn64size)
        {
            QMessageBox::critical(this, tr("FCD"), tr("Unable to read firmware file:\n").arg(ui->lineEdit->text()));
            delete buf;
            qf.close();

            return;
        }
    }

    qf.close();

    ui->statusLabel->setText(tr("Erasing FCD flash..."));
    ui->lineEdit->setEnabled(false);
    ui->selectButton->setEnabled(false);
    ui->uploadButton->setEnabled(false);
    ui->verifyButton->setEnabled(false);

    // process pending GUI events before we commit the block loop
    //qApp->processEvents();
    QCoreApplication::processEvents();  // FIXME: neither works

    // FIXME: progress bar
#if 0
    // Erase old firmware then write the new one
    if (fcdBlErase() != FCD_MODE_BL)
    {
        ui->statusLabel->setText(tr("Flash erase failed"));
    }
    else
    {
        ui->statusLabel->setText(tr("Uploading new firmware..."));
        if (fcdBlWriteFirmware(buf,(int64_t)qn64size) != FCD_MODE_BL)
        {
            ui->statusLabel->setText(tr("Write firmware failed"));
        }
        else {
            ui->statusLabel->setText(tr("Firmware upload successful"));
        }
    }
#endif
    qDebug() << "FIXME: Not implemented!";

    ui->lineEdit->setEnabled(true);
    ui->selectButton->setEnabled(true);
    checkFirmwareSelection(ui->lineEdit->text());

    delete buf;
}


/*! \brief Verify firmware in FCD. */
void CFirmware::on_verifyButton_clicked()
{    
    QFile qf(ui->lineEdit->text());
    qint64 qn64size = qf.size();
    char *buf=new char[qn64size];

    if (buf==NULL)
    {
        QMessageBox::critical(this, tr("FCD"), tr("Unable to allocate memory for firmware image"));
        return;
    }

    if (!qf.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this, tr("FCD"), tr("Unable to open firmware file:\n").arg(ui->lineEdit->text()));
        delete buf;

        return;
    }
    else
    {
        if (qf.read(buf,qn64size) != qn64size)
        {
            QMessageBox::critical(this, tr("FCD"), tr("Unable to read firmware file:\n").arg(ui->lineEdit->text()));
            delete buf;
            qf.close();

            return;
        }
    }

    qf.close();

    ui->statusLabel->setText(tr("Verifying firmware in FCD..."));
    ui->lineEdit->setEnabled(false);
    ui->selectButton->setEnabled(false);
    ui->uploadButton->setEnabled(false);
    ui->verifyButton->setEnabled(false);

    // process pending GUI events before we commit the block loop
    //qApp->processEvents();
    QCoreApplication::processEvents();  // FIXME: neither works
#if 0
    // execute verification
    // FIXME: progress bar
    if (fcdBlVerifyFirmware(buf,(int64_t)qn64size) != FCD_MODE_BL)
    {
        ui->statusLabel->setText(tr("Firmware verification failed"));
    }
    else
    {
        ui->statusLabel->setText(tr("Firmware successfully verified"));
    }
#endif
    qDebug() << "FIXME: Not implemented!";

    ui->lineEdit->setEnabled(true);
    ui->selectButton->setEnabled(true);
    checkFirmwareSelection(ui->lineEdit->text());

    delete buf;
}


/*! \brief The text in the lineEdit has changed (via setText). */
void CFirmware::on_lineEdit_textChanged(const QString & text)
{
    checkFirmwareSelection(text);
}


/*! \brief The text in the lineEdit has been edited by the user. */
void CFirmware::on_lineEdit_textEdited(const QString & text)
{
    checkFirmwareSelection(text);
}


/*! \brief Check whether fwFile exists and enable/disable Upload and
 *         Verify buttons accordingly. */
void CFirmware::checkFirmwareSelection(const QString &fwFile)
{
    if (QFile::exists(fwFile))
    {
        ui->uploadButton->setEnabled(true);
        ui->verifyButton->setEnabled(true);
    }
    else
    {
        ui->uploadButton->setEnabled(false);
        ui->verifyButton->setEnabled(false);
    }
}
