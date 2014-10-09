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
     * @param parent
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
    Variables m_variables;
    Variables m_nextStepVars;
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
