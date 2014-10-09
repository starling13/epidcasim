#ifndef MODELPROPERTIES_H
#define MODELPROPERTIES_H

#include <QDialog>
#include "ui_Model.h"
#include "model.hpp"
#include "sirmodel.hpp"
#include "choleramodel.hpp"
#include "hexagonalgeometrymodel.hpp"

class MainWindow;

class ModelPropertiesDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ModelPropertiesDialog(MainWindow *parent = 0);
    ~ModelPropertiesDialog();
    
signals:
    
private slots:
    void on_buttonBox_accepted();
    void geometrySelected( int index );
    void neighbourSelected( int );
private:
    Ui::Dialog widget;
    GeometryModel * m_geometryModel;
    QButtonGroup * m_neighboursGroup;
    static HexagonalGeometryModel m_hexModel;
    static RectangularGeometryModel m_rectModel;
    static RectangularGeometryModel::
           VNNeighboursModel m_vnRectModel;
    static HexagonalGeometryModel::
           NearestModel m_nearHexModel;
    static QVector<GeometryModel*> m_geometries;
};

#endif // MODELPROPERTIES_H
