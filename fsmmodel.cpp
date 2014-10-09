#include "fsmmodel.hpp"

#include <QDebug>

FSMModel::FSMModel(Cell * parentCell, QObject *parent) :
    QObject(parent),
    m_parentCell(parentCell)
{
}

FSMModel::~FSMModel()
{

}

void FSMModel::modelStepComplete()
{
    m_variables.swap(m_nextStepVars);
    emit updated();
}

void FSMModel::update()
{
    emit updated();
}

FSMModelTrack::FSMModelTrack(FSMModel &model) :
    QObject(&model),
    m_model(model)
{
    qDebug() << __PRETTY_FUNCTION__;

    m_varsTrack.resize(model.variables().size());
    this->connect( &m_model, SIGNAL(updated()), SLOT(modelUpdated()) );
}

FSMModelTrack::~FSMModelTrack()
{
    qDebug() << __PRETTY_FUNCTION__;
}

void FSMModelTrack::modelUpdated()
{
    int index = 0;
    for (qreal var : m_model.variables())
    {
        m_varsTrack[index++].push_back(var);
    }
}
