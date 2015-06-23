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

#ifndef HEXADECIMALGEOMETRYMODEL_HPP
#define HEXADECIMALGEOMETRYMODEL_HPP

#include "model.hpp"

class HexagonalGeometryModel : public GeometryModel
{
public:
    class NearestModel : public NeighboursModel
    {
    public:
        NearestModel(GeometryModel & geometry);
        ModelIndex::List neighbours( ModelIndex index ) const;
    };

    HexagonalGeometryModel();
    HexagonalGeometryModel( Grid * grid );
    ~HexagonalGeometryModel();

    QString description() const { return m_description; }

    QPoint cellCoordinates( ModelIndex index ) const;
    ModelIndex cellIndex(QPoint position) const;

private:

    static QString m_description;
    NearestModel *m_nearest;
};

#endif // HEXADECIMALGEOMETRYMODEL_HPP
