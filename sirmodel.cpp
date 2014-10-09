#include "sirmodel.hpp"
#include "model.hpp"

#define BETTA 0.001f
#define NU 0.003f
#define M 0.002f

FSMModel::VarNames SIRModel::m_varNames;
FSMModel::Variables SIRModel::m_parameters;
FSMModel::VarNames SIRModel::m_paramNames;
SIRModel::ClassInitializer SIRModel::m_initializer;

SIRModel::SIRModel(Cell *parentCell, QObject *parent) :
    FSMModel(parentCell, parent)
{
    this->m_variables.resize(int(Var_t::NUM_VARS));
    S() = 1000.0;
    I() = 0.0;
    R() = 0.0;

    this->m_nextStepVars.resize(int(Var_t::NUM_VARS));
    nextS() = 0.0;
    nextI() = 0.0;
    nextR() = 0.0;
}

SIRModel::ClassInitializer::ClassInitializer()
{
    SIRModel::m_varNames.push_back("Susceptible");
    SIRModel::m_varNames.push_back("Infectious");
    SIRModel::m_varNames.push_back("Recovered");

    SIRModel::m_paramNames.push_back("Beta");
    SIRModel::m_paramNames.push_back("Nu");
    SIRModel::m_paramNames.push_back("M");

    SIRModel::m_parameters.push_back(BETTA);
    SIRModel::m_parameters.push_back(NU);
    SIRModel::m_parameters.push_back(M);
}

SIRModel::ClassInitializer::~ClassInitializer()
{

}

FSMModel *SIRModel::clone(Cell *parentCell) const
{
    return new SIRModel(parentCell);
}

void SIRModel::step()
{
    qreal BETTA_SI = 0.0,
          NU_I = 0.0;
    static std::default_random_engine generator;

    switch (m_type)
    {
    case FSMModel::ModelType_t::DETERMINATE:
        BETTA_SI = P_BETTA() * S() * I();
        NU_I = P_NU() * I();
        break;
    case FSMModel::ModelType_t::STOCHASTIC:
    {
        if (S() > 0)
        {
            std::binomial_distribution<int> BETTA_I_d(S(), P_BETTA() * I());
            BETTA_SI = BETTA_I_d(generator);
        }
        if (I() > 0)
        {
            std::binomial_distribution<int> NU_I_d(I(), P_NU() );
            NU_I = NU_I_d(generator);
        }
    }
        break;
    }

    ModelIndex::List neighbours =
            m_parentCell->grid()->neighModel().neighbours(
                m_parentCell->index());

    nextS() = S() - BETTA_SI;
    nextI() = I() + BETTA_SI - NU_I;
    nextR() = R() + NU_I;

    for ( ModelIndex index : neighbours )
    {
        SIRModel *model = qobject_cast<SIRModel*>
                ( &m_parentCell->grid()->cell(index).fsModel() );
        switch (m_type)
        {
        case FSMModel::ModelType_t::DETERMINATE:
            nextS() += P_M()*model->S();
            nextS() -= P_M()*S();

            nextI() += P_M()*model->I();
            nextI() -= P_M()*I();

            nextR() += P_M()*model->R();
            nextR() -= P_M()*R();

            break;
        case FSMModel::ModelType_t::STOCHASTIC:
        {
            if (model->S() > 0)
            {
                std::binomial_distribution<int> MmodelS_d(model->S(), P_M() );
                nextS() += MmodelS_d(generator);
            }
            if (model->I() > 0)
            {
                std::binomial_distribution<int> MmodelI_d(model->I(), P_M() );
                nextI() += MmodelI_d(generator);
            }
            if (model->R() > 0)
            {
                std::binomial_distribution<int> MmodelR_d(model->R(), P_M() );
                nextR() += MmodelR_d(generator);
            }

            if ( S() > 0 )
            {
                std::binomial_distribution<int> MS_d(S(), P_M() );
                nextS() -= MS_d(generator);
            }
            if (I() > 0)
            {
                std::binomial_distribution<int> MI_d(I(), P_M() );
                nextI() -= MI_d(generator);
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
