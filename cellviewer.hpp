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
