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

#ifndef MODEL_HPP
#define MODEL_HPP

#include <cstdlib>

#include <QPoint>
#include <QVector>
#include <QSize>
#include <QList>
#include <QString>

#include <QObject>
#include "fsmmodel.hpp"

class Cell;
class Grid;
class GeometryModel;

enum class GridShape
{
    RECTANGULAR,
    HEXADECIMAL
};

class ModelIndex
{
public:
    typedef QList<ModelIndex> List;

    ModelIndex();

    ModelIndex(int col, int row);

    int row() const
    {
        return m_row;
    }
    void setRow(int row)
    {
        m_row = row;
    }

    int column() const
    {
        return m_column;
    }
    void setColumn(int column)
    {
        m_column = column;
    }

    bool isValid() const
    {
        return ((m_row >=0) && (m_column >= 0));
    }

private:

    int m_row;

    int m_column;
};

class NeighboursModel
{
public:

    virtual ModelIndex::List neighbours(ModelIndex index) const = 0;

    const QString & description() const
    {
        return m_description;
    }

protected:

    NeighboursModel(
      const QString & description,
      GeometryModel & geometry);

    GeometryModel & m_geometryModel;

private:

    QString m_description;
};

class GeometryModel
{
public:

    virtual ~GeometryModel();

    virtual QPoint cellCoordinates(ModelIndex index) const = 0;

    virtual ModelIndex cellIndex(QPoint position) const = 0;

    const QString &description() const
    {
        return m_description;
    }

    QVector<NeighboursModel*> & neighbours()
    {
        return m_neighboursModels;
    }

    qreal cellRadius() const
    {
        return m_cellRadius;
    }

    void setCellRadius( qreal radius )
    {
        m_cellRadius = radius;
    }

    const NeighboursModel & neighboursModel() const;

    void setNeigboursModel(NeighboursModel & model)
    {
        m_neighboursModel = &model;
    }

    Grid * grid()
    {
        return m_grid;
    }

    void setGrid(Grid * grid)
    {
        m_grid = grid;
    }

protected:

    GeometryModel();

    GeometryModel(
      const QString &description,
      Grid *grid);

    Grid * m_grid;

    qreal m_cellRadius;

    NeighboursModel * m_neighboursModel;

    QVector<NeighboursModel*> m_neighboursModels;

private:

    QString m_description;
};

class RectangularGeometryModel : public GeometryModel
{
public:

    class VNNeighboursModel : public NeighboursModel
    {
    public:
        VNNeighboursModel(GeometryModel & geometryModel);

        ModelIndex::List neighbours(ModelIndex indec) const;
    };

    RectangularGeometryModel();

    RectangularGeometryModel(Grid * grid);

    ~RectangularGeometryModel();

    QPoint cellCoordinates(ModelIndex index) const;

    ModelIndex cellIndex(QPoint position) const;

private:

    VNNeighboursModel * m_vn;
};

class Cell
{
public:

    Cell(
      Grid * parentGrid,
      ModelIndex index,
      const FSMModel & fsModel);

    ~Cell();

    FSMModel & fsModel()
    {
        return * m_fsModel;
    }

    Grid * grid()
    {
        return m_grid;
    }

    ModelIndex index()
    {
        return m_index;
    }

private:

    FSMModel * m_fsModel;

    Grid * m_grid;

    const ModelIndex m_index;
};

class Grid
{
public:

    Grid(
      QSize size,
      GeometryModel &geometryModel,
      FSMModel &fsmModel);

    ~Grid();

    const NeighboursModel & neighModel() const
    {
        return m_neighboursModel;
    }

    GeometryModel & geometryModel()
    {
        return m_geometryModel;
    }

    const GeometryModel & geometryModel() const
    {
        return m_geometryModel;
    }

    FSMModel & fsmModel()
    {
        return *m_fsmEtalonModel;
    }

    const FSMModel & fsmModel() const
    {
        return *m_fsmEtalonModel;
    }

    void step( int count = 1 );

    Cell & cell(ModelIndex index);

    const QSize & size() const
    {
        return m_size;
    }

private:
    Cell *** m_cells;
    QSize m_size;
    const NeighboursModel & m_neighboursModel;
    GeometryModel & m_geometryModel;
    FSMModel * m_fsmEtalonModel;
};

#endif // MODEL_HPP
