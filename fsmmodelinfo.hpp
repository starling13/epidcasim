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

#ifndef CELLINFO_H
#define CELLINFO_H

#include <QDialog>
#include <QDoubleSpinBox>
#include <QEvent>
#include <QVector>

#include "model.hpp"

namespace Ui {
class CellInfo;
}

class VarSpinBox : public QDoubleSpinBox
{
    Q_OBJECT
public:
    VarSpinBox(int varIndex, QWidget * parent = 0);
    int varIndex() const { return m_varIndex; }
private:
    int m_varIndex;
};

class FSMModelInfo : public QWidget
{
    Q_OBJECT
public:
    explicit FSMModelInfo(QWidget *parent = 0);
    ~FSMModelInfo();

public slots:
    void setModel(FSMModel *model);
    
private:
    typedef QVector<VarSpinBox*> Fields_t;

    Ui::CellInfo *ui;
    FSMModel * m_model;
    FSMModelTrack * m_track;
    Fields_t m_fields;
private slots:
    void valueChanged( double value );
    void cellUpdated();
    void on_createTrack_clicked();
    void on_deleteTrack_clicked();
    void on_showGraph_clicked();
};

#endif // CELLINFO_H
