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

#ifndef SIRMODEL_HPP
#define SIRMODEL_HPP

#include "fsmmodel.hpp"
#include <random>

/**
 * @class SIRModel
 * @brief SIR epidemiological model
 */
class SIRModel : public FSMModel
{
    Q_OBJECT
public:
    /**
     * @brief Variable indexes
     */
    enum class Var_t
    {
        /**
         * @brief Susceptible
         */
        S = 0,
        /**
         * @brief Infectious
         */
        I,
        /**
         * @brief Recovered
         */
        R,
        NUM_VARS
    };

    /**
     * @brief Parameter indexes
     */
    enum class Param_t
    {
        /**
         * @brief Betta
         */
        BETTA = 0,
        /**
         * @brief Mu
         */
        NU,
        M,
        NUM
    };

    explicit SIRModel( Cell *parentCell, QObject * parent = 0 );

    FSMModel * clone(Cell * parentCell) const;
    const FSMModel::VarNames & varNames() const { return m_varNames; }
    FSMModel::Variables & parameters() { return m_parameters; }
    const FSMModel::VarNames & paramNames() const { return m_paramNames; }

    qreal & S() { return m_variables[int(Var_t::S)]; }
    qreal & I() { return m_variables[int(Var_t::I)]; }
    qreal & R() { return m_variables[int(Var_t::R)]; }

    static qreal & P_BETTA() { return m_parameters[int(Param_t::BETTA)]; }
    static qreal & P_NU() { return m_parameters[int(Param_t::NU)]; }
    static qreal & P_M() { return m_parameters[int(Param_t::M)]; }

public slots:
    void step();
private:
    qreal & nextS() { return m_nextStepVars[int(Var_t::S)]; }
    qreal & nextI() { return m_nextStepVars[int(Var_t::I)]; }
    qreal & nextR() { return m_nextStepVars[int(Var_t::R)]; }

    class ClassInitializer
    {
    public:
        ClassInitializer();
        ~ClassInitializer();
    };
    friend class ClassInitializer;
    static FSMModel::VarNames m_varNames;
    static FSMModel::Variables m_parameters;
    static FSMModel::VarNames m_paramNames;
    static ClassInitializer m_initializer;
};

#endif // SIRMODEL_HPP
