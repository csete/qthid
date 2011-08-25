/***************************************************************************
 *  This file is part of Qthid.
 *
 *  CopyRight (C) 2011  Alexandru Csete, OZ9AEC
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
#include <QDebug>
#include "fcddiagram.h"
#include "ui_fcddiagram.h"

FcdDiagram::FcdDiagram(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FcdDiagram)
{
    ui->setupUi(this);
}

FcdDiagram::~FcdDiagram()
{
    qDebug() << "Diagram destroyed";
    delete ui;
}
