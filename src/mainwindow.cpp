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

#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QThread>
#include <QRadioButton>
#include <QProgressDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_grid(NULL),
    m_modelInfo(NULL),
    m_layersGroup(NULL)
{
    qDebug() << __PRETTY_FUNCTION__;

    ui->setupUi(this);
    m_modelProperties = new ModelPropertiesDialog(this);
    m_layersGroup = new QButtonGroup(this);
    ui->viewer->connect( m_layersGroup, SIGNAL(buttonClicked(int)),
                        SLOT(visibleVarChanged(int)) );
}

MainWindow::~MainWindow()
{
    qDebug() << __PRETTY_FUNCTION__;

    delete ui;
}

void MainWindow::setGrid(Grid *grid)
{
    qDebug() << __PRETTY_FUNCTION__;

    if (m_grid != NULL)
    {
        delete m_grid;
        delete m_modelInfo;
    }
    m_grid = grid;

    ui->viewer->setModel(m_grid);
    m_modelInfo = new FSMModelInfo;
    m_modelInfo->setModel(&m_grid->fsmModel());
    ui->modelPropsLayout->addWidget(m_modelInfo);

    for (QObject *child : m_layersGroup->buttons())
        delete child;

    int index = 0;
    for (QString name : m_grid->fsmModel().varNames())
    {
        QRadioButton * pb = new QRadioButton;
        pb->setText(name);
        ui->modelPropsLayout->addWidget(pb);
        m_layersGroup->addButton(pb, index++);
    }
}

void MainWindow::on_NewAction_triggered()
{
    qDebug() << __PRETTY_FUNCTION__;

    m_modelProperties->show();
}

void MainWindow::on_stepButton_clicked()
{
    QProgressDialog dialog(QString::fromUtf8("??????????????????????????"),
                           QString::fromUtf8("????????????????????"),
                           0,ui->stepsCount->value() );
    dialog.show();
    for (int i=0; i<ui->stepsCount->value(); ++i)
    {
        dialog.setValue(i);
        m_grid->step();

        if (dialog.wasCanceled())
            break;

        ui->viewer->update();

        QApplication::instance()->processEvents();
    }
}

void MainWindow::on_maxDisplayField_valueChanged(double newVal)
{
    ui->viewer->setCellMaxValue(newVal);
    ui->viewer->update();
}
