/****************************************************************************
    This file is part of Casim.

    Copyright (C) 2014,2015 by Andrey V. Skvortsov

    Casim is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    Casim is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with Casim; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
****************************************************************************/

#include "fsmmodelinfo.hpp"
#include "ui_cellinfo.h"

#include "modelgraph.hpp"

#include <QLabel>

VarSpinBox::VarSpinBox(int varIndex, QWidget *parent) :
    QDoubleSpinBox(parent),
    m_varIndex(varIndex)
{

}

FSMModelInfo::FSMModelInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CellInfo),
    m_model(NULL),
    m_track(NULL)
{
    ui->setupUi(this);
    ui->stateField->setLayout(new QVBoxLayout);
}

FSMModelInfo::~FSMModelInfo()
{
    delete ui;
}

void FSMModelInfo::setModel(FSMModel *model)
{
    m_model = model;

    FSMModel::VarNames varNames =
            m_model->varNames();
    QObject::connect( m_model, SIGNAL(updated()),
                      this, SLOT(cellUpdated()) );
    for (int i=0; i<varNames.size(); ++i)
    {
        VarSpinBox * field = new VarSpinBox(i);
	field->setMaximum(4000000000.f);
        field->setValue( m_model->variables()[i] );
        m_fields.push_back(field);
        this->connect( field,
                       SIGNAL(valueChanged( double )),
                       SLOT(valueChanged(double)) );
        QLabel * label = new QLabel;
        QHBoxLayout * layout = new QHBoxLayout;
        label->setText(varNames[i]);
        qobject_cast<QVBoxLayout*>(ui->stateField->layout())->
                addLayout(layout);
        layout->addWidget(label);
        layout->addWidget(field);
    }
}

void FSMModelInfo::valueChanged(double value)
{
    VarSpinBox * sender = qobject_cast<VarSpinBox*>(QObject::sender());
    m_model->variables()[sender->varIndex()] = value;
}

void FSMModelInfo::cellUpdated()
{
    for (int i=0; i<m_model->variables().size(); ++i)
        m_fields[i]->setValue(m_model->variables()[i]);
}

void FSMModelInfo::on_createTrack_clicked()
{
    m_track = new FSMModelTrack(*m_model);
}

void FSMModelInfo::on_deleteTrack_clicked()
{
    delete m_track;
}

void FSMModelInfo::on_showGraph_clicked()
{
    ModelGraph * graphWidget = new ModelGraph(*m_track, this);
    graphWidget->show();
}
