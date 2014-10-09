#ifndef CELLINFO_H
#define CELLINFO_H

#include <QDialog>
#include <QDoubleSpinBox>
#include <QEvent>
#include <QVector>

#include "model.hpp"

namespace Ui {
class CellInfo;
}

class VarSpinBox : public QDoubleSpinBox
{
    Q_OBJECT
public:
    VarSpinBox(int varIndex, QWidget * parent = 0);
    int varIndex() const { return m_varIndex; }
private:
    int m_varIndex;
};

class FSMModelInfo : public QWidget
{
    Q_OBJECT
public:
    explicit FSMModelInfo(QWidget *parent = 0);
    ~FSMModelInfo();

public slots:
    void setModel(FSMModel *model);
    
private:
    typedef QVector<VarSpinBox*> Fields_t;

    Ui::CellInfo *ui;
    FSMModel * m_model;
    FSMModelTrack * m_track;
    Fields_t m_fields;
private slots:
    void valueChanged( double value );
    void cellUpdated();
    void on_createTrack_clicked();
    void on_deleteTrack_clicked();
    void on_showGraph_clicked();
};

#endif // CELLINFO_H
