#ifndef CELLVIEWER_H
#define CELLVIEWER_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>

#include "model.hpp"

class CellViewer : public QWidget
{
    Q_OBJECT
public:
    explicit CellViewer(QWidget *parent = 0);
    ~CellViewer();

    void setModel(Grid *);

    qreal scale() const { return m_scale; }
    void setScale( qreal newVal ) { m_scale = newVal; }

public slots:
    void visibleVarChanged( int index );
    void setCellMaxValue(qreal newVal) { m_cellMaxValue = newVal; }
protected:
    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);
private:
    void drawGrid();

    QPainter m_painter;
    Grid * m_grid;
    qreal m_scale;
    ModelIndex m_selectedIndex;
    QColor m_cellColor;
    qreal m_cellMaxValue;
    int m_visibleVariable;
};

#endif // CELLVIEWER_H
