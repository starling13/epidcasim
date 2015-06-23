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

#ifndef CHOLERAMODEL_HPP
#define CHOLERAMODEL_HPP

#include "fsmmodel.hpp"
#include <random>

class CholeraModel : public FSMModel
{
    Q_OBJECT
public:
    enum class Var_t
    {
        /** @brief Susceptible */
        S = 0,
        /** @brief Infectious */
        Bn,
        /** @brief Recovered */
        Bl,
        Is,
        Ia,
        R,
        NUM_VARS
    };

    enum class Param_t
    {
        ALPHA = 0,
        BETA_N,
        BETA_L,
        K_N,
        K_L,
        X,
        DELTAl,
        MU,
        MUs,
        GAMMA,
        P,
        SIGMAs,
        SIGMAa,
        PI,
        M
    };

    explicit CholeraModel( Cell *parentCell, QObject * parent = 0 );
    FSMModel * clone(Cell * parentCell) const;
    const FSMModel::VarNames & varNames() const { return m_varNames; }
    FSMModel::Variables & parameters() { return m_parameters; }
    const FSMModel::VarNames & paramNames() const { return m_paramNames; }

    qreal & S() { return m_variables[int(Var_t::S)]; }
    qreal & Bn() { return m_variables[int(Var_t::Bn)]; }
    qreal & Bl() { return m_variables[int(Var_t::Bl)]; }
    qreal & Is() { return m_variables[int(Var_t::Is)]; }
    qreal & Ia() { return m_variables[int(Var_t::Ia)]; }
    qreal & R() { return m_variables[int(Var_t::R)]; }

    static qreal & P_ALPHA() { return m_parameters[int(Param_t::ALPHA)]; }
    static qreal & P_BETTA_N() { return m_parameters[int(Param_t::BETA_N)]; }
    static qreal & P_BETTA_L() { return m_parameters[int(Param_t::BETA_L)]; }
    static qreal & P_K_N() { return m_parameters[int(Param_t::K_N)]; }
    static qreal & P_K_L() { return m_parameters[int(Param_t::K_L)]; }
    static qreal & P_X() { return m_parameters[int(Param_t::X)]; }
    static qreal & P_DELTAl() { return m_parameters[int(Param_t::DELTAl)]; }
    static qreal & P_MU() { return m_parameters[int(Param_t::MU)]; }
    static qreal & P_MUs() { return m_parameters[int(Param_t::MUs)]; }
    static qreal & P_GAMMA() { return m_parameters[int(Param_t::GAMMA)]; }
    static qreal & P_P() { return m_parameters[int(Param_t::P)]; }
    static qreal & P_SIGMAs() { return m_parameters[int(Param_t::SIGMAs)]; }
    static qreal & P_SIGMAa() { return m_parameters[int(Param_t::SIGMAa)]; }
    static qreal & P_PI() { return m_parameters[int(Param_t::PI)]; }
    static qreal & P_M() { return m_parameters[int(Param_t::M)]; }

public slots:
    void step();

private:
    qreal & nextS() { return m_nextStepVars[int(Var_t::S)]; }
    qreal & nextBn() { return m_nextStepVars[int(Var_t::Bn)]; }
    qreal & nextBl() { return m_nextStepVars[int(Var_t::Bl)]; }
    qreal & nextIs() { return m_nextStepVars[int(Var_t::Is)]; }
    qreal & nextIa() { return m_nextStepVars[int(Var_t::Ia)]; }
    qreal & nextR() { return m_nextStepVars[int(Var_t::R)]; }

    class ClassInitializer
    {
    public:
        ClassInitializer();
    };
    friend class ClassInitializer;
    static FSMModel::VarNames m_varNames;
    static FSMModel::Variables m_parameters;
    static FSMModel::VarNames m_paramNames;
    static ClassInitializer m_initializer;
};

#endif // CHOLERAMODEL_HPP
