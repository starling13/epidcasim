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
    enum class Var_t
    {
        /** @brief Susceptible */
        S = 0,
        /** @brief Infectious */
        I,
        /** @brief Recovered */
        R,
        NUM_VARS
    };

    enum class Param_t
    {
        BETTA = 0,
        NU,
        M
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
