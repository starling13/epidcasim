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
    ModelIndex(int col, int row );

    int row() const { return m_row; }
    void setRow( int row ) { m_row = row; }

    int column() const { return m_column; }
    void setColumn( int column ) { m_column = column; }

    bool isValid() const { return ( (m_row >=0) && (m_column >= 0) ); }

private:
    int m_row;
    int m_column;
};

class NeighboursModel
{
public:
    NeighboursModel(GeometryModel & geometry);
    virtual ModelIndex::List neighbours( ModelIndex index ) const = 0;
    virtual QString description() const = 0;
protected:
    GeometryModel & m_geometryModel;
};

class GeometryModel
{
public:
    virtual ~GeometryModel();

    virtual QPoint cellCoordinates( ModelIndex index ) const = 0;
    virtual ModelIndex cellIndex( QPoint position ) const = 0;

    virtual QString description() const = 0;
    QVector<NeighboursModel*> & neighbours() { return m_neighboursModels; }

    qreal cellRadius() const { return m_cellRadius; }
    void setCellRadius( qreal radius ) { m_cellRadius = radius; }

    const NeighboursModel & neighboursModel() const;
    void setNeigboursModel( NeighboursModel & model )
    { m_neighboursModel = &model; }

    Grid * grid() { return m_grid; }
    void setGrid(Grid * grid) { m_grid = grid; }

protected:
    GeometryModel();
    GeometryModel(Grid *grid );

    Grid * m_grid;
    qreal m_cellRadius;
    NeighboursModel * m_neighboursModel;
    QVector<NeighboursModel*> m_neighboursModels;
};

class RectangularGeometryModel : public GeometryModel
{
public:
    class VNNeighboursModel : public NeighboursModel
    {
    public:
        VNNeighboursModel( GeometryModel & geometryModel );
        ModelIndex::List neighbours( ModelIndex indec ) const;
        QString description() const { return QObject::tr("Von Neumann"); }
    };

    RectangularGeometryModel();
    RectangularGeometryModel( Grid * grid );
    ~RectangularGeometryModel();

    QString description() const { return m_description; }

    QPoint cellCoordinates( ModelIndex index ) const;
    ModelIndex cellIndex(QPoint position) const;

private:
    static QString m_description;
    VNNeighboursModel * m_vn;
};

class Cell
{
public:
    Cell( Grid * parentGrid,
          ModelIndex index,
          const FSMModel & fsModel );
    ~Cell();
    FSMModel & fsModel() { return * m_fsModel; }
    Grid * grid() { return m_grid; }
    ModelIndex index() { return m_index; }
private:
    FSMModel * m_fsModel;
    Grid * m_grid;
    const ModelIndex m_index;
};

class Grid
{
public:
    Grid(QSize size,
          GeometryModel &geometryModel,
          FSMModel &fsmModel );
    ~Grid();

    const NeighboursModel & neighModel() const { return m_neighboursModel; }
    GeometryModel & geometryModel() { return m_geometryModel; }
    const GeometryModel & geometryModel() const { return m_geometryModel; }
    FSMModel & fsmModel() { return *m_fsmEtalonModel; }
    const FSMModel & fsmModel() const { return *m_fsmEtalonModel; }

    void step( int count = 1 );

    Cell & cell( ModelIndex index );

    const QSize & size() const { return m_size; }

private:
    Cell *** m_cells;
    QSize m_size;
    const NeighboursModel & m_neighboursModel;
    GeometryModel & m_geometryModel;
    FSMModel * m_fsmEtalonModel;
};

#endif // MODEL_HPP
