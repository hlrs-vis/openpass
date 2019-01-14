/*********************************************************************
* This Example Content is intended to demonstrate usage of
* Eclipse technology. It is provided to you under the terms
* and conditions of the Eclipse Distribution License v1.0
* which is available at http://www.eclipse.org/org/documents/edl-v10.php
**********************************************************************/

#ifndef DYNAMICS_COMP2_IMPLEMENTATION_H
#define DYNAMICS_COMP2_IMPLEMENTATION_H

#include "modelInterface.h"
#include "observationInterface.h"
#include "primitiveSignals.h"
#include "vectorSignals.h"
#include "componentPorts.h"

/**
 * \addtogroup GroupRoot DESCRIPTION
 * @{
 * \addtogroup Dynamics_Comp2
 *
 * \brief Dynamics for comp2
 *
 * \details to-be-completed.
 *
 * @}
 */

/*!
 * \copydoc Dynamics_Comp2
 * \ingroup Dynamics_Comp2
 */
class Dynamics_Comp2_Implementation : public DynamicsInterface
{
public:
    const std::string COMPONENTNAME = "Dynamics_Comp2";

    //! Constructor
    //!
    //! @param[in]     componentId    Corresponds to "id" of "Component"
    //! @param[in]     isInit         Corresponds to "init" of "Component"
    //! @param[in]     priority       Corresponds to "priority" of "Component"
    //! @param[in]     offsetTime     Corresponds to "offsetTime" of "Component"
    //! @param[in]     responseTime   Corresponds to "responseTime" of "Component"
    //! @param[in]     cycleTime      Corresponds to "cycleTime" of "Component"
    //! @param[in]     stochastics    Pointer to the stochastics class loaded by the framework
    //! @param[in]     parameters     Pointer to the parameters of the module
    //! @param[in]     evaluations    Pointer to the evaluations of the module
    //! @param[in]     callbacks      Pointer to the callbacks
    Dynamics_Comp2_Implementation(int componentId,
                                         bool isInit,
                                         int priority,
                                         int offsetTime,
                                         int responseTime,
                                         int cycleTime,
                                         StochasticsInterface *stochastics,
                                         WorldInterface *world,
                                         const ParameterInterface *parameters,
                                         const std::map<int, ObservationInterface *> *observations,
                                         const CallbackInterface *callbacks, 
                                         AgentInterface *agent);

    //! Destructor
    virtual ~Dynamics_Comp2_Implementation();

    //! Function is called by framework when another component delivers a signal over
    //! a channel to this component (scheduler calls update taks of other component).
    //!
    //! @param[in]     localLinkId    Corresponds to "id" of "ComponentInput"
    //! @param[in]     data           Referenced signal (copied by sending component)
    //! @param[in]     time           Current scheduling time
    //! @return                       True on success
    virtual void UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const> &data,
                             int time);

    //! Function is called by framework when this component has to deliver a signal over
    //! a channel to another component (scheduler calls update task of this component).
    //!
    //! @param[in]     localLinkId    Corresponds to "id" of "ComponentOutput"
    //! @param[out]    data           Referenced signal (copied by this component)
    //! @param[in]     time           Current scheduling time
    //! @return                       True on success
    virtual void UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const> &data, int time);

    //! Function is called by framework when the scheduler calls the trigger task
    //! of this component
    //!
    //! @param[in]     time           Current scheduling time
    //! @return                       True on success
    virtual void Trigger(int time);

private:

    std::map<int, ComponentPort *> inputPorts; //!< map for all InputPort

    /** \addtogroup Dynamics_Comp2
     *  @{
     *      \name InputPort
     *      All input ports with PortId
     *      @{
     */
    InputPort<DoubleSignal, double> input1 {0, &inputPorts}; //!< MEANING OF INPUT1
    InputPort<SignalVectorDouble, std::vector<double>> input2 {1, &inputPorts}; //!< MEANING OF INPUT2
    /**
     *      @}
     *  @}
     */

    std::map<int, ComponentPort *> outputPorts; //!< map for all OutputPort
    /** \addtogroup Dynamics_Comp2
     *  @{
     *      \name OutputPorts
     *      All output ports with PortId
     *      @{
     */
    OutputPort<DoubleSignal, double> output1 {0, &outputPorts}; //!< MEANING OF OUTPUT1
    /**
     *      @}
     *  @}
     */

    std::map<int, externalParameter<bool>*> parameterMapBool;
    std::map<int, externalParameter<double>*> parameterMapDouble;
    /** \addtogroup Dynamics_Comp2
     *  @{
     *      \name External Parameter
     *      Parameter which are set externally in agentConfiguration file.
     *      @{
     */
    externalParameter<bool> param1 {0, &parameterMapBool }; //!< MEANING OF PARAM1
    externalParameter<double> param2 {1, &parameterMapDouble }; //!< MEANING OF PARAM2
    /**
     *      @}
     *  @}
     */

    /**
     *    \name Internal objects
     *    @{
    */

    /**
     *    @}
    */
};

#endif // DYNAMICS_COMP2_IMPLEMENTATION_H
