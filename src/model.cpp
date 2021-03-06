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

#include "model.hpp"
#include <stdexcept>

#include <QDebug>

ModelIndex::ModelIndex() :
    m_row(0),
    m_column(0)
{
}

ModelIndex::ModelIndex(int col, int row) :
    m_row(row),
    m_column(col)
{
}

NeighbourhoodModel::NeighbourhoodModel(
  const QString &description,
  GeometryModel &geometry) :
m_geometryModel(geometry),
m_description(description)
{
}

GeometryModel::GeometryModel() :
m_grid(),
m_cellRadius(10),
m_neighboursModel(NULL)
{
}

GeometryModel::GeometryModel(
  const QString &description,
  Grid *grid) :
m_grid(grid),
m_cellRadius(10),
m_neighboursModel(NULL),
m_description(description)
{
    qDebug() << __PRETTY_FUNCTION__;
}

GeometryModel::~GeometryModel()
{
}

const NeighbourhoodModel & GeometryModel::
neighboursModel() const
{
    if (this->m_neighboursModel == NULL)
        throw std::runtime_error("No neighbours model");

    return *m_neighboursModel;
}

RectangularGeometryModel::VNNeighboursModel::
VNNeighboursModel(GeometryModel & geometryModel) :
NeighbourhoodModel(QObject::tr("Von Neuman"), geometryModel)
{
}

ModelIndex::List
RectangularGeometryModel::VNNeighboursModel::
neighbours( ModelIndex index ) const
{
    ModelIndex::List result;

    if ( index.row() > 0 )
        result.append( ModelIndex( index.column(), index.row()-1 ) );
    if ( index.column() > 0 )
        result.append( ModelIndex( index.column()-1, index.row() ) );
    if ( index.row() < (m_geometryModel.grid()->size().height()-1) )
        result.append( ModelIndex( index.column(), index.row()+1 ) );
    if ( index.column() < (m_geometryModel.grid()->size().width()-1) )
        result.append( ModelIndex( index.column()+1, index.row() ) );

    return result;
}


RectangularGeometryModel::
RectangularGeometryModel() :
GeometryModel(QObject::tr("Rectangular"), NULL)
{
    m_vn = new VNNeighboursModel(*this);
    m_neighboursModels.push_back(m_vn);
}

RectangularGeometryModel::
RectangularGeometryModel(Grid * grid) :
GeometryModel(QObject::tr("Rectangular"), grid)
{
    m_vn = new VNNeighboursModel(*this);
    m_neighboursModels.push_back(m_vn);
}

RectangularGeometryModel::~RectangularGeometryModel()
{
}

QPoint RectangularGeometryModel::
cellCoordinates(ModelIndex index) const
{
    Q_ASSERT(index .isValid());

    QPoint result;

    if ( index.isValid() )
    {
        result.setX( index.column() * m_cellRadius * 2 );
        result.setY( index.row() * m_cellRadius * 2 );
    }

    return result;
}

ModelIndex RectangularGeometryModel::
cellIndex(QPoint position) const
{
    ModelIndex result(-1, -1);

    int row = position.y() / (2*m_cellRadius);
    int col = 0;

    if (row < m_grid->size().height()) {
        col = position.x() / (2*m_cellRadius);
        if (col < m_grid->size().width())
            result.setColumn(col), result.setRow(row);
    }

    return result;
}

Cell::Cell(
  Grid *parentGrid,
  ModelIndex index,
  const FSMModel &fsModel) :
m_grid(parentGrid),
m_index(index)
{
    m_fsModel = fsModel.clone(this);
}

Cell::~Cell()
{
   delete m_fsModel;
}

Grid::Grid(
  QSize size,
  GeometryModel &geometryModel,
  FSMModel &fsmModel) :
m_size(size),
m_neighboursModel(geometryModel.neighboursModel()),
m_geometryModel(geometryModel),
m_fsmEtalonModel(&fsmModel)
{
    this->m_cells = new Cell**[size.width()];
    for(int i=0; i<size.width(); ++i) {
        this->m_cells[i] = new Cell*[size.height()];
        for(int j=0; j<size.height(); ++j)
            this->m_cells[i][j] =
                new Cell(this, ModelIndex(i,j), *m_fsmEtalonModel);
    }
}

Grid::~Grid()
{
    for(int i=0; i<m_size.width(); ++i)
    {
        for(int j=0; j<m_size.height(); ++j)
            delete this->m_cells[i][j];
        delete [] this->m_cells[i];
    }
    delete [] this->m_cells;
    delete this->m_fsmEtalonModel;
}

void Grid::step(int count)
{
    int step = 0;
    while((step++) < count) {
        // Zeroing full model variables
        for (int k=0;
             k<m_fsmEtalonModel->variables().size();
             ++k)
            m_fsmEtalonModel->variables()[k] = 0;
        for (int i=0; i<this->size().width(); ++i)
            for(int j=0; j<this->size().height(); ++j)
                this->m_cells[i][j]->fsModel().step();
        for (int i=0; i<this->size().width(); ++i)
            for(int j=0; j<this->size().height(); ++j)
                this->m_cells[i][j]->fsModel().modelStepComplete();
        for (int i=0; i<this->size().width(); ++i)
            for(int j=0; j<this->size().height(); ++j)
            {
                for (int k=0;
                     k<m_cells[i][j]->fsModel().variables().size();
                     ++k)
                    m_fsmEtalonModel->variables()[k] +=
                            m_cells[i][j]->fsModel().variables()[k];
            }
        m_fsmEtalonModel->update();
    }
}

Cell & Grid::cell( ModelIndex index )
{
    Q_ASSERT(index.isValid());

    return *m_cells[index.column()][index.row()];
}
