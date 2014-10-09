#include "modelgraph.hpp"
#include "ui_modelgraph.h"

#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_legend.h>
#include <qwt/qwt_plot_grid.h>
#include <qwt_plot_picker.h>
#include <qwt_picker_machine.h>

ModelGraph::ModelGraph(FSMModelTrack &track, QWidget *parent) :
    QDialog(parent),
    m_track(track),
    ui(new Ui::modelgraph)
{
    ui->setupUi(this);

    m_xSeries.resize(m_track.varsTrack()[0].size());
    for (int i=0; i<m_xSeries.size(); ++i)
        m_xSeries[i] = i;
    QwtLegend * legend = new QwtLegend(ui->plot);
    int i = 0;
    Qt::GlobalColor color = Qt::red;
    for (const FSMModelTrack::Var_t var : m_track.varsTrack())
    {
        QwtPlotCurve * curve =
                new QwtPlotCurve(m_track.model().varNames()[i]);
        curve->setSamples(m_xSeries, m_track.varsTrack()[i]);
        curve->setPen( QPen( color ) );
        color = Qt::GlobalColor(color+1);
        curve->attach(ui->plot);
        ++i;
    }
    ui->plot->insertLegend(legend);

    ui->plot->setAxisMaxMajor(QwtPlot::yLeft, 5);
    ui->plot->setAxisMaxMinor(QwtPlot::yLeft, 10);

    ui->plot->setAxisMaxMajor(QwtPlot::xBottom, 5);
    ui->plot->setAxisMaxMinor(QwtPlot::xBottom, 10);

    QwtPlotGrid *grid = new QwtPlotGrid;

#if (QWT_VERSION < 0x060100)
    grid->setMajPen( QPen( Qt::darkGray, 1, Qt::SolidLine ) );
    grid->setMinPen( QPen( Qt::lightGray, 1, Qt::SolidLine ) );
#else
    grid->setMajorPen( QPen( Qt::darkGray, 1, Qt::SolidLine ) );
    grid->setMinorPen( QPen( Qt::lightGray, 1, Qt::SolidLine ) );
#endif
    grid->enableYMin(true);
    grid->enableXMin(true);
    grid->attach(ui->plot);

    ui->plot->setCanvasBackground( QBrush( Qt::white ) );

    QwtPlotPicker * picker = new QwtPlotPicker( QwtPlot::xBottom, QwtPlot::yLeft,
                                       ui->plot->canvas() );
    picker->setRubberBand( QwtPicker::CrossRubberBand );
    picker->setTrackerMode( QwtPicker::AlwaysOn);
    picker->setRubberBandPen( QPen(Qt::red, 1, Qt::SolidLine) );
    picker->setStateMachine(new QwtPickerTrackerMachine );

    ui->plot->replot();
    ui->plot->setAutoReplot(true);
}

ModelGraph::~ModelGraph()
{
    delete ui;
}
