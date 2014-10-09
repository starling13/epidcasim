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
        QString description() const { return QObject::tr("Nearest"); }
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
