#ifndef MODELGRAPH_HPP
#define MODELGRAPH_HPP

#include <QDialog>
#include <QVector>

#include "fsmmodel.hpp"

namespace Ui {
class modelgraph;
}

class ModelGraph : public QDialog
{
    Q_OBJECT

public:
    explicit ModelGraph(FSMModelTrack & track, QWidget *parent = 0);
    ~ModelGraph();

private:
    QVector<qreal> m_xSeries;
    Ui::modelgraph *ui;
    FSMModelTrack & m_track;
};

#endif // MODELGRAPH_HPP
