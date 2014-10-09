#include <QTranslator>
#include "hexagonalgeometrymodel.hpp"

QString HexagonalGeometryModel::m_description =
    QObject::tr("Hexagonal");

HexagonalGeometryModel::
HexagonalGeometryModel()
{
    m_nearest = new NearestModel(*this);
    m_neighboursModels.push_back(m_nearest);
}

HexagonalGeometryModel::
HexagonalGeometryModel(Grid *grid) :
GeometryModel(grid)
{
    m_nearest = new NearestModel(*this);
    m_neighboursModels.push_back(m_nearest);
}

HexagonalGeometryModel::~HexagonalGeometryModel()
{

}

HexagonalGeometryModel::
NearestModel::NearestModel(GeometryModel &geometry) :
    NeighboursModel(geometry)
{

}

ModelIndex::List HexagonalGeometryModel::
NearestModel::neighbours(ModelIndex index) const
{
    ModelIndex::List result;

    if ( index.column() > 0 )
        result.append( ModelIndex( index.column()-1, index.row() ) );
    if ( index.column() < (m_geometryModel.grid()->size().width()-1) )
        result.append( ModelIndex( index.column()+1, index.row() ) );

    if ( index.row() & 1 )
    {
        if (index.row() > 0)
        {
            result.append( ModelIndex( index.column(), index.row()-1 ) );
            if ( index.column() > 0 )
                result.append( ModelIndex( index.column()-1, index.row()-1) );
        }
        if (index.row() < m_geometryModel.grid()->size().height()-1)
        {
            result.append( ModelIndex( index.column(), index.row()+1 ) );
            if ( index.column() > 0 )
                result.append( ModelIndex( index.column()-1, index.row()+1) );
        }
    }
    else
    {
        if (index.row() > 0)
        {
            result.append( ModelIndex( index.column(), index.row()-1 ) );
            if ( index.column() < (m_geometryModel.grid()->size().width()-1) )
                result.append( ModelIndex( index.column()+1, index.row()-1) );
        }
        if (index.row() < m_geometryModel.grid()->size().height()-1)
        {
            result.append( ModelIndex( index.column(), index.row()+1 ) );
            if ( index.column() < (m_geometryModel.grid()->size().width()-1) )
                result.append( ModelIndex( index.column()+1, index.row()+1) );
        }
    }

    return result;
}

QPoint HexagonalGeometryModel::
cellCoordinates(ModelIndex index) const
{
    Q_ASSERT(index .isValid());

    QPoint result;

    if ( index.isValid() )
    {
        if ( index.row() & 1 )
            result.setX( index.column() * m_cellRadius * 2 );
        else
            result.setX( index.column() * m_cellRadius * 2 + m_cellRadius );
        result.setY( index.row() * m_cellRadius * 2 );
    }

    return result;
}

ModelIndex HexagonalGeometryModel::
HexagonalGeometryModel::cellIndex(QPoint position) const
{
    ModelIndex result(-1, -1);

    int row = position.y() / (2*m_cellRadius);
    int col = 0;

    if (row < m_grid->size().height() )
    {
        if ( row & 1 )
            col = position.x() / (2*m_cellRadius);
        else
            col = (position.x() - m_cellRadius) / (2*m_cellRadius);
        if (col < m_grid->size().width())
            result.setColumn(col), result.setRow(row);
    }

    return result;
}
