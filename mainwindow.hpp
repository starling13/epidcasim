/*
    This file is part of Casim.

    Casim is free software: you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version.

    Posixcpp library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU LesserGeneral Public
    License along with Posixcpp library. If not, see
    <http://www.gnu.org/licenses/>.
*/

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QButtonGroup>

#include "model.hpp"
#include "modelproperties.h"
#include "fsmmodel.hpp"
#include "fsmmodelinfo.hpp"

namespace Ui {
class MainWindow;
}

/**
 * @brief The main application window class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    /**
     * @brief set grid aggregate object
     * @param grid new grid object
     */
    void setGrid(Grid * grid);
    
private slots:
   void on_NewAction_triggered();
   void on_stepButton_clicked();
   void on_maxDisplayField_valueChanged(double);
private:
    /**
     * @brief qt-designer ui object
     */
    Ui::MainWindow *ui;
    /**
     * @brief Model properties dialog object
     */
    ModelPropertiesDialog *m_modelProperties;
    /**
     * @brief Group of layers selection group
     */
    QButtonGroup *m_layersGroup;

    Grid * m_grid;
    FSMModelInfo * m_modelInfo;
};

#endif // MAINWINDOW_HPP
