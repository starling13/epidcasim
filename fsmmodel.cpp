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

#include "fsmmodel.hpp"

#include <QDebug>

FSMModel::FSMModel(Cell * parentCell, QObject *parent) :
    QObject(parent),
    m_parentCell(parentCell)
{
}

FSMModel::~FSMModel()
{

}

void FSMModel::modelStepComplete()
{
    m_variables.swap(m_nextStepVars);
    emit updated();
}

void FSMModel::update()
{
    emit updated();
}

FSMModelTrack::FSMModelTrack(FSMModel &model) :
    QObject(&model),
    m_model(model)
{
    qDebug() << __PRETTY_FUNCTION__;

    m_varsTrack.resize(model.variables().size());
    this->connect( &m_model, SIGNAL(updated()), SLOT(modelUpdated()) );
}

FSMModelTrack::~FSMModelTrack()
{
    qDebug() << __PRETTY_FUNCTION__;
}

void FSMModelTrack::modelUpdated()
{
    int index = 0;
    for (qreal var : m_model.variables())
    {
        m_varsTrack[index++].push_back(var);
    }
}
