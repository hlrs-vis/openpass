/*********************************************************************
* This Example Content is intended to demonstrate usage of
* Eclipse technology. It is provided to you under the terms
* and conditions of the Eclipse Distribution License v1.0
* which is available at http://www.eclipse.org/org/documents/edl-v10.php
**********************************************************************/

/**
 * @defgroup module_comp2 Comp2
 * The Dynamics component for comp2.
 * \image html Comp2_overview.png "Overview of Comp2"
 */

/**
 * @ingroup module_comp2
 * @defgroup comp2_group1 Description of group1
*/

/**
 * @ingroup module_comp2
 * @defgroup comp2_group2 Description of group2
*/

#include "dynamics_Comp2_implementation.h"

Dynamics_Comp2_Implementation::Dynamics_Comp2_Implementation(int componentId,
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
                                                           AgentInterface *agent) :
    DynamicsInterface(componentId,
                       isInit,
                       priority,
                       offsetTime,
                       responseTime,
                       cycleTime,
                       stochastics,
                       world,
                       parameters,
                       observations,
                       callbacks,
                       agent)
{
    LOGINFO("Constructing Dynamics_Comp2");

	/** @addtogroup comp2_group1
     * doc content for group1.
    */

    std::map<int, bool> parameterMapBoolExternal =
        GetParameters()->GetParametersBool();
    foreach (auto &iterator, parameterMapBool)
    {
        int id = iterator.first;
        parameterMapBool.at(id)->SetValue(parameterMapBoolExternal.at(id));
    }

    std::map<int, double> parameterMapDoubleExternal =
        GetParameters()->GetParametersDouble();
    foreach (auto &iterator, parameterMapDouble)
    {
        int id = iterator.first;
        parameterMapDouble.at(id)->SetValue(parameterMapDoubleExternal.at(id));
    }

    LOGINFO("Constructing Dynamics_Comp2 successful");
}

Dynamics_Comp2_Implementation::~Dynamics_Comp2_Implementation()
{

}

void Dynamics_Comp2_Implementation::UpdateInput(int localLinkId,
                                                       const std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);

    bool success = inputPorts.at(localLinkId)->SetSignalValue(data);
    if (success)
        LOGDEBUG(QString().sprintf("Dynamics_Comp2: Update input #%d successful", localLinkId).toStdString());
    else
        LOGERROR(QString().sprintf("Dynamics_Comp2: Update input #%d failed", localLinkId).toStdString());
}

void Dynamics_Comp2_Implementation::UpdateOutput(int localLinkId,
                                                        std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);

    bool success = outputPorts.at(localLinkId)->GetSignalValue(data);
    if (success)
        LOGDEBUG(QString().sprintf("Dynamics_Comp2: Update output #%d successful", localLinkId).toStdString());
    else
        LOGERROR(QString().sprintf("Dynamics_Comp2: Update output #%d failed", localLinkId).toStdString());
}

void Dynamics_Comp2_Implementation::Trigger(int time)
{
    Q_UNUSED(time);
	
	/** @addtogroup comp2_group2
     * doc content for group2.
    */

    output1.SetValue(INFINITY);

    return;
}
