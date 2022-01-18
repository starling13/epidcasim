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

#include "choleramodel.hpp"
#include "model.hpp"

#define ALPHA   0.5f
#define BETA_N  1.0f
#define BETA_L  1.0f
#define K_N     K_L / 100
#define K_L     1000000.0
#define X       0.9f
#define DELTA_L 0.03f
#define MU      0.000044877
#define MUs     0.005
#define GAMMA   0.2f
#define P       0.76
#define SIGMA_S 50.0f
#define SIGMA_A 0.5f
#define PI      0.0f
#define M       0.001f

FSMModel::VarNames CholeraModel::m_varNames;
FSMModel::Variables CholeraModel::m_parameters;
FSMModel::VarNames CholeraModel::m_paramNames;
CholeraModel::ClassInitializer CholeraModel::m_initializer;

CholeraModel::ClassInitializer::ClassInitializer()
{
    CholeraModel::m_varNames.push_back("S");
    CholeraModel::m_varNames.push_back("Bn");
    CholeraModel::m_varNames.push_back("Bl");
    CholeraModel::m_varNames.push_back("Is");
    CholeraModel::m_varNames.push_back("Ia");
    CholeraModel::m_varNames.push_back("R");

    CholeraModel::m_paramNames.push_back("alpha");
    CholeraModel::m_paramNames.push_back("beta_n");
    CholeraModel::m_paramNames.push_back("beta_l");
    CholeraModel::m_paramNames.push_back("k_n");
    CholeraModel::m_paramNames.push_back("k_l");
    CholeraModel::m_paramNames.push_back("x");
    CholeraModel::m_paramNames.push_back("delta_l");
    CholeraModel::m_paramNames.push_back("mu");
    CholeraModel::m_paramNames.push_back("mu_s");
    CholeraModel::m_paramNames.push_back("gamma");
    CholeraModel::m_paramNames.push_back("p");
    CholeraModel::m_paramNames.push_back("sigma_s");
    CholeraModel::m_paramNames.push_back("sigma_a");
    CholeraModel::m_paramNames.push_back("pi");
    CholeraModel::m_paramNames.push_back("M");

    CholeraModel::m_parameters.push_back(ALPHA);
    CholeraModel::m_parameters.push_back(BETA_N);
    CholeraModel::m_parameters.push_back(BETA_L);
    CholeraModel::m_parameters.push_back(K_N);
    CholeraModel::m_parameters.push_back(K_L);
    CholeraModel::m_parameters.push_back(X);
    CholeraModel::m_parameters.push_back(DELTA_L);
    CholeraModel::m_parameters.push_back(MU);
    CholeraModel::m_parameters.push_back(MUs);
    CholeraModel::m_parameters.push_back(GAMMA);
    CholeraModel::m_parameters.push_back(P);
    CholeraModel::m_parameters.push_back(SIGMA_S);
    CholeraModel::m_parameters.push_back(SIGMA_A);
    CholeraModel::m_parameters.push_back(PI);
    CholeraModel::m_parameters.push_back(M);
}

CholeraModel::CholeraModel(Cell *parentCell, QObject *parent) :
    FSMModel(parentCell, parent)
{
    m_variables.resize(int(Var_t::NUM_VARS));
    for (int i=0; i<m_variables.size(); ++i)
        m_variables[i] = 0.0;

    m_nextStepVars.resize(int(Var_t::NUM_VARS));
    for (int i=0; i<m_nextStepVars.size(); ++i)
        m_nextStepVars[i] = 0.0;

    // Initial variables values
    S() = 1000.0;
}

FSMModel *CholeraModel::clone(Cell *parentCell) const
{
    return new CholeraModel(parentCell);
}

void CholeraModel::step()
{
    // Singleton of the random numbers generator
    static std::default_random_engine generator;

    // State-equations polynoms members
    qreal ALPHA_BETAn_S_Bn_over_Kn_PLUS_Bn = 0.0,
          ALPHA_BETAl_S_Bl_over_Kl_PLUS_Bl = 0.0,
          MU_S = 0.0,
          GAMMA_Is = 0.0,
          MUs_Is = 0.0,
          GAMMA_Ia = 0.0,
          MU_Ia = 0.0,
          MU_R = 0.0,
          SIGMAs_Is = 0.0,
          SIGMAa_Ia = 0.0,
          X_Bn = 0.0,
          DELTAl_Bl = 0.0;

    switch (m_type)
    {
    case FSMModel::ModelType_t::DETERMINATE:
    // Determinate model
        ALPHA_BETAn_S_Bn_over_Kn_PLUS_Bn =
                P_ALPHA()*P_BETTA_N()*S()*Bn()/( P_K_N()+Bn() );

        ALPHA_BETAl_S_Bl_over_Kl_PLUS_Bl =
                P_ALPHA()*P_BETTA_L()*S()*Bl()/( P_K_L()+Bl() );

        MU_S = P_MU()*S();

        GAMMA_Is = P_GAMMA()*Is();

        MUs_Is = P_MUs()*Is();

        GAMMA_Ia = P_GAMMA()*Ia();

        MU_Ia = P_MU()*Ia();

        MU_R = P_MU()*R();

        SIGMAs_Is = P_SIGMAs()*Is();

        SIGMAa_Ia = P_SIGMAa()*Ia();

        X_Bn = P_X()*Bn();

        DELTAl_Bl = P_DELTAl()*Bl();

        break;
    case FSMModel::ModelType_t::STOCHASTIC:
    {
    // Stochastic model

        if (S() > 0)
        {
            qreal p = P_ALPHA()*P_BETTA_N()*Bn()/( P_K_N()+Bn() );
            std::binomial_distribution<int>
                ALPHA_BETAn_S_Bn_over_Kn_PLUS_Bn_d(S(), p);
            ALPHA_BETAn_S_Bn_over_Kn_PLUS_Bn =
                    ALPHA_BETAn_S_Bn_over_Kn_PLUS_Bn_d(generator);

            std::binomial_distribution<int>
                    ALPHA_BETAl_S_Bl_over_Kl_PLUS_Bl_d(S(),
                         P_ALPHA()*P_BETTA_L()*Bl()/( P_K_L()+Bl() ));
            ALPHA_BETAl_S_Bl_over_Kl_PLUS_Bl =
                    ALPHA_BETAl_S_Bl_over_Kl_PLUS_Bl_d(generator);

            std::binomial_distribution<int> MU_S_d(S(), P_MU());
            MU_S = MU_S_d(generator);
        }
        if (Is() > 0)
        {
            std::binomial_distribution<int> GAMMA_Is_d(Is(), P_GAMMA());
            GAMMA_Is = GAMMA_Is_d(generator);

            std::binomial_distribution<int> MUs_Is_d(Is(), P_MUs());
            MUs_Is = MUs_Is_d(generator);

            std::binomial_distribution<int> SIGMAs_Is_d(Is(), P_SIGMAs());
            SIGMAs_Is = SIGMAs_Is_d(generator);
        }
        if (Ia() > 0)
        {
            std::binomial_distribution<int> GAMMA_Ia_d(Ia(), P_GAMMA());
            GAMMA_Ia = GAMMA_Ia_d(generator);

            std::binomial_distribution<int> MU_Ia_d(Ia(), P_MU());
            MU_Ia = MU_Ia_d(generator);

            std::binomial_distribution<int> SIGMAa_Ia_d(Ia(), P_SIGMAa());
            SIGMAa_Ia = SIGMAa_Ia_d(generator);
        }
        if (R() > 0)
        {
            std::binomial_distribution<int> MU_R_d(R(), P_MU());
            MU_R = MU_R_d(generator);
        }
        if (Bn() > 0)
        {
            std::binomial_distribution<int> X_Bn_d(Bn(), P_X());
            X_Bn = X_Bn_d(generator);
        }
        if (Bl() > 0)
        {
            std::binomial_distribution<int> DELTAl_Bl_d(Bl(), P_DELTAl());
            DELTAl_Bl = DELTAl_Bl_d(generator);
        }
    }
        break;
    }

    // Вычисление значений переменных состояния для следующего шага
    // конечно-разностного решения

    nextS()  = S()
             + P_PI()
             - ALPHA_BETAn_S_Bn_over_Kn_PLUS_Bn
             - ALPHA_BETAl_S_Bl_over_Kl_PLUS_Bl
             - MU_S;

    nextIs() = Is()
             + (1-P_P()) * ALPHA_BETAn_S_Bn_over_Kn_PLUS_Bn
             + (1-P_P()) * ALPHA_BETAl_S_Bl_over_Kl_PLUS_Bl
             - GAMMA_Is
             - MUs_Is;

    nextIa() = Is()
             + P_P() * ALPHA_BETAn_S_Bn_over_Kn_PLUS_Bn
             + P_P() * ALPHA_BETAl_S_Bl_over_Kl_PLUS_Bl
             - GAMMA_Ia
             - MU_Ia;

    nextR()  = R()
             + GAMMA_Is
             + GAMMA_Ia
             - MU_R;

    nextBn() = Bn()
             + SIGMAs_Is
             + SIGMAa_Ia
             - X_Bn;

    nextBl() = Bl()
             + X_Bn
             - DELTAl_Bl;

    // Получение ссылок на соседние элементы клеточно-автоматной модели
    ModelIndex::List neighbours =
            m_parentCell->grid()->neighModel().neighbours(
                m_parentCell->index());
    for ( ModelIndex index : neighbours )
    {
    // Вычисление и применение миграционных потоков
        CholeraModel *model = qobject_cast<CholeraModel*>
                ( &m_parentCell->grid()->cell(index).fsModel() );
        switch (m_type)
        {
        case FSMModel::ModelType_t::DETERMINATE:

            nextS()  += P_M()*model->S();
            nextS()  -= P_M()*S();

            nextIs() += P_M()*model->Is();
            nextIs() -= P_M()*Is();

            nextIa() += P_M()*model->Ia();
            nextIa() -= P_M()*Ia();

            nextR() += P_M()*model->R();
            nextR() -= P_M()*R();

            break;
        case FSMModel::ModelType_t::STOCHASTIC:
        {
            if (model->S() > 0)
            {
                std::binomial_distribution<int> MmodelS_d(model->S(), P_M() );
                nextS()  += MmodelS_d(generator);
            }
            if (model->Is() > 0)
            {
                std::binomial_distribution<int> MmodelIs_d(model->Is(), P_M() );
                nextIs() += MmodelIs_d(generator);
            }
            if (model->Ia() > 0)
            {
                std::binomial_distribution<int> MmodelIa_d(model->Ia(), P_M() );
                nextIa() += MmodelIa_d(generator);
            }
            if (model->R() > 0)
            {
                std::binomial_distribution<int> MmodelR_d(model->R(), P_M() );
                nextR() += MmodelR_d(generator);
            }

            if (S() > 0)
            {
                std::binomial_distribution<int> MS_d(S(), P_M() );
                nextS()  -= MS_d(generator);
            }
            if (Is() > 0)
            {
                std::binomial_distribution<int> MIs_d(Is(), P_M() );
                nextIs() -= MIs_d(generator);
            }
            if (Ia() > 0)
            {
                std::binomial_distribution<int> MIa_d(Ia(), P_M() );
                nextIa() -= MIa_d(generator);
            }
            if (R() > 0)
            {
                std::binomial_distribution<int> MR_d(R(), P_M() );
                nextR() -= MR_d(generator);
            }
        }
            break;
        }
    }
}
