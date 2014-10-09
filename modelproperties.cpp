#include "modelproperties.h"
#include "mainwindow.hpp"
#include <QButtonGroup>
#include <QRadioButton>
#include <QDebug>

HexagonalGeometryModel ModelPropertiesDialog::m_hexModel;
RectangularGeometryModel ModelPropertiesDialog::m_rectModel;

RectangularGeometryModel::
VNNeighboursModel ModelPropertiesDialog::m_vnRectModel(m_rectModel);

HexagonalGeometryModel::
NearestModel ModelPropertiesDialog::m_nearHexModel(m_hexModel);

QVector<GeometryModel*> ModelPropertiesDialog::m_geometries;

ModelPropertiesDialog::ModelPropertiesDialog(MainWindow *parent) :
    QDialog(parent),
    m_geometryModel(NULL),
    m_neighboursGroup(NULL)
{
    widget.setupUi(this);
    m_rectModel.setCellRadius(6);
    m_hexModel.setCellRadius(6);

    QButtonGroup *bg = new QButtonGroup(this);
    this->connect(bg, SIGNAL(buttonClicked(int)),
                  SLOT(geometrySelected(int)));

    m_geometries.push_back(&m_hexModel);
    m_geometries.push_back(&m_rectModel);

    int index = 0;
    for ( GeometryModel * model : m_geometries )
    {
        QRadioButton *rb = new QRadioButton;
        rb->setText(model->description());
        widget.shapeLayout->addWidget(rb);
        bg->addButton(rb, index++);
    }
    widget.shapeLayout->addSpacerItem( new QSpacerItem(10,10,
                                           QSizePolicy::Minimum,
                                           QSizePolicy::Expanding) );
    m_neighboursGroup = new QButtonGroup(this);
    this->connect(m_neighboursGroup, SIGNAL(buttonClicked(int)),
                 SLOT(neighbourSelected(int)) );
}

ModelPropertiesDialog::~ModelPropertiesDialog()
{

}

void ModelPropertiesDialog::on_buttonBox_accepted()
{
    QSize gridSize(widget.widthBox->value(),
                   widget.heightBox->value());

    FSMModel * fsmModel = NULL;
    if (widget.sirButton->isChecked())
        fsmModel = new SIRModel(NULL);
    else if (widget.choleraButton->isChecked())
        fsmModel = new CholeraModel(NULL);

    Grid *result = new Grid(gridSize,
                            *m_geometryModel,
                            *fsmModel);
    m_geometryModel->setGrid( result );

    qobject_cast<MainWindow*>(this->parent())->setGrid(result);
}

void ModelPropertiesDialog::geometrySelected(int index)
{
    qDebug() << __PRETTY_FUNCTION__ << "index:" << index;

    m_geometryModel = m_geometries[index];

    for (QObject *child : m_neighboursGroup->buttons())
        delete child;
    int ind = 0;
    for(NeighboursModel * model : m_geometryModel->neighbours())
    {
        QRadioButton * rb = new QRadioButton;
        rb->setText(model->description());
        widget.neighboursGroup->layout()->addWidget(rb);
        m_neighboursGroup->addButton(rb, ind++);
    }
}

void ModelPropertiesDialog::neighbourSelected(int index)
{
    qDebug() << __PRETTY_FUNCTION__ << "index:" << index;

    m_geometryModel->setNeigboursModel(
                *m_geometryModel->neighbours()[index]);
}
