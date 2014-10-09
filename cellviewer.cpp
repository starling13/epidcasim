#include "cellviewer.hpp"

#include <QDebug>
#include <QMouseEvent>
#include <QWheelEvent>

#include "fsmmodelinfo.hpp"

CellViewer::CellViewer(QWidget *parent) :
    QWidget(parent),
    m_grid(NULL),
    m_scale(2.0),
    m_selectedIndex(-1,-1),
    m_visibleVariable(0),
    m_cellColor( Qt::green ),
    m_cellMaxValue( 1000.0 )
{
    qDebug() << __PRETTY_FUNCTION__;
}

CellViewer::~CellViewer()
{
    qDebug() << __PRETTY_FUNCTION__;
}

void CellViewer::paintEvent(QPaintEvent*)
{
    m_painter.begin(this);
    m_painter.setBrush(QBrush(Qt::gray));

    m_painter.fillRect(this->rect(), Qt::gray);
    if (m_grid != NULL)
        drawGrid();

    m_painter.end();
}

void CellViewer::mouseReleaseEvent(QMouseEvent * event)
{
    qDebug() << __PRETTY_FUNCTION__;
    qDebug() << event->posF();
    if (m_grid == NULL)
        return;

    QPoint position = event->pos();
    position.setX( position.x() - 10 );
    position.setY( position.y() - 10 );

    m_selectedIndex =
        m_grid->geometryModel().cellIndex(position);

    qDebug() << m_selectedIndex.column() << m_selectedIndex.row();
    this->update();
    if (m_selectedIndex.isValid())
    {
        FSMModelInfo * ci =
                new FSMModelInfo;
        ci->setAttribute(Qt::WA_DeleteOnClose);
        ci->setWindowTitle(QString::number(m_selectedIndex.column())+
                           QString(" : ")+
                           QString::number(m_selectedIndex.row()) );
        ci->setModel( &m_grid->cell( m_selectedIndex ).fsModel() );
        ci->show();
    }
}

void CellViewer::setModel(Grid * model)
{
    qDebug() << __PRETTY_FUNCTION__;

    this->m_grid = model;
    m_grid->geometryModel().setCellRadius(m_scale);
    this->setMinimumSize(m_grid->size().width() * m_grid->geometryModel().cellRadius() * 2 + 10 +10,
                         m_grid->size().height() * m_grid->geometryModel().cellRadius() * 2 + 10 +10);
    this->repaint();
}

void CellViewer::visibleVarChanged(int index)
{
    m_visibleVariable = index;
    this->update();
}

void CellViewer::drawGrid()
{
    qreal var = 0.0;

    for(int i=0; i<m_grid->size().height(); ++i)
        for(int j=0; j<m_grid->size().width(); ++j)
        {
            ModelIndex index(j, i);
            QPoint coord =
                m_grid->geometryModel().cellCoordinates( index );

            var = m_grid->cell(index).fsModel().variables()[m_visibleVariable];

            int red = var * m_cellColor.red() / m_cellMaxValue;
            if (red > 255) red = 255;
            int green = var * m_cellColor.green() / m_cellMaxValue;
            if (green > 255) green = 255;
            int blue = var * m_cellColor.blue() / m_cellMaxValue;
            if (blue > 255) blue = 255;

            QColor color( red, green, blue );
            m_painter.setPen(QPen(QBrush(color), m_scale*1.8));

            coord.setX( coord.x() + 10 );
            coord.setY( coord.y() + 10 );
            m_painter.drawPoint( coord );
        }
    if (m_selectedIndex.isValid())
    {
        m_painter.setPen(QPen(QBrush(Qt::yellow), 1));
        QPoint selected =
                m_grid->geometryModel().cellCoordinates(m_selectedIndex);
        QPoint selectedPoly[5] =
        {
            QPoint(selected.x() - (m_scale/2) + 10, selected.y() - (m_scale/2) + 10),
            QPoint(selected.x() + (m_scale/2) + 10 + 1, selected.y() - (m_scale/2) + 10),
            QPoint(selected.x() + (m_scale/2) + 10 + 1, selected.y() + (m_scale/2) + 10 + 1),
            QPoint(selected.x() - (m_scale/2) + 10, selected.y() + (m_scale/2) + 10 + 1),
            QPoint(selected.x() - (m_scale/2) + 10, selected.y() - (m_scale/2) + 10),
        };
        m_painter.drawPolyline( selectedPoly, 5 );

        m_painter.setPen(QPen(QBrush(Qt::darkYellow), 1));
        ModelIndex::List list=
                m_grid->geometryModel().neighboursModel().neighbours(
                    m_selectedIndex);
        for (ModelIndex index : list)
        {
            selected =
                m_grid->geometryModel().cellCoordinates(index);
            selectedPoly[0] = QPoint(selected.x() - (m_scale/2) + 10, selected.y() - (m_scale/2) + 10),
            selectedPoly[1] = QPoint(selected.x() + (m_scale/2) + 10 + 1, selected.y() - (m_scale/2) + 10),
            selectedPoly[2] = QPoint(selected.x() + (m_scale/2) + 10 + 1, selected.y() + (m_scale/2) + 10 + 1),
            selectedPoly[3] = QPoint(selected.x() - (m_scale/2) + 10, selected.y() + (m_scale/2) + 10 + 1),
            selectedPoly[4] = QPoint(selected.x() - (m_scale/2) + 10, selected.y() - (m_scale/2) + 10),
            m_painter.drawPolyline( selectedPoly, 5 );
        }
    }
}

void CellViewer::wheelEvent(QWheelEvent * event)
{
    m_scale += event->delta() / 100;
    m_grid->geometryModel().setCellRadius( m_scale );
    this->update();
}
