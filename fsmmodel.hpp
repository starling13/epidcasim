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

/**
 * @file fsmmodel.hpp
 * @brief Finite state machine class header
 */

#ifndef FSMMODEL_HPP
#define FSMMODEL_HPP

#include <QObject>
#include <QVector>

class Cell;
class FSMModelTrack;

/**
 * @class FSMModel
 * @brief Finite state machine model class
 */
class FSMModel : public QObject
{
    Q_OBJECT
public:
    enum class ModelType_t
    {
        DETERMINATE,
        STOCHASTIC
    };

    /**
     * @brief Constructor
     * @param parentCell pointer to the cell, containig model
     * @param parent parent QObject
     */
    explicit FSMModel(Cell *parentCell, QObject *parent = 0);
    virtual ~FSMModel();

    /**
     * @brief State variables collection
     */
    typedef QVector<qreal> Variables;
    /**
     * @brief State variables descriptions collection
     */
    typedef QVector<QString> VarNames;

    const Variables & variables() const { return m_variables; }
    Variables & variables() { return m_variables; }

    virtual Variables & parameters() = 0;

    /**
     * @brief Get variables description for the concrete model class
     * @return Collection of descriptions
     */
    virtual const VarNames & varNames() const = 0;
    virtual const VarNames & paramNames() const = 0;
    /**
     * @brief Clone current model class
     * @return new model object pointer
     */
    virtual FSMModel * clone(Cell * parentCell) const = 0;

public slots:
    /**
     * @brief One step for the model
     */
    virtual void step() = 0;
    /**
     * @brief this slot should be called after all cells state
     * for the current step is complete
     */
    void modelStepComplete();
    /**
     *
     */
    void update();
signals:
    /**
     * @brief Emitted after completing step
     */
    void updated();
protected:
    /**
     * @brief Values of state variables, used on current step
     */
    Variables m_variables;
    /**
     * @brief Values of state variables, are being computed
     */
    Variables m_nextStepVars;
    /**
     * @brief parameters
     */
    static Variables m_parameters;
    Cell *m_parentCell;
    ModelType_t m_type = ModelType_t::STOCHASTIC;
private:
    friend class FSMModelTrack;
};

class FSMModelTrack : public QObject
{
    Q_OBJECT;
public:
    typedef QVector<qreal> Var_t;
    typedef QVector<Var_t> Vars_t;

    FSMModelTrack(FSMModel & model);
    ~FSMModelTrack();

    const Vars_t & varsTrack() const { return m_varsTrack; }
    const FSMModel & model() const { return m_model; }
private:
    FSMModel & m_model;
    int m_currentStep;
    Vars_t m_varsTrack;
private slots:
    void modelUpdated();
};

#endif // FSMMODEL_HPP
