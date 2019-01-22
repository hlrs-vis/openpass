/*********************************************************************
* This Example Content is intended to demonstrate usage of
* Eclipse technology. It is provided to you under the terms
* and conditions of the Eclipse Distribution License v1.0
* which is available at http://www.eclipse.org/org/documents/edl-v10.php
**********************************************************************/

/**
 * @defgroup module_comp1 Comp1
 * The Algorithm component for comp1.
 * \image html Comp1_overview.png "Overview of Comp1"
 */

/**
 * @ingroup module_comp1
 * @defgroup comp1_group1 Description of group1
*/

/**
 * @ingroup module_comp1
 * @defgroup comp1_group2 Description of group2
*/

#include "algorithm_Comp1_implementation.h"

Algorithm_Comp1_Implementation::Algorithm_Comp1_Implementation(int componentId,
                                                           bool isInit,
                                                           int priority,
                                                           int offsetTime,
                                                           int responseTime,
                                                           int cycleTime,
                                                           StochasticsInterface *stochastics,
                                                           const ParameterInterface *parameters, 
                                                           const std::map<int, ObservationInterface *> *observations,
                                                           const CallbackInterface *callbacks,
                                                           int agentId) :
    AlgorithmInterface(componentId,
                       isInit,
                       priority,
                       offsetTime,
                       responseTime,
                       cycleTime,
                       stochastics,
                       parameters,
                       observations,
                       callbacks,
                       agentId)
{
    LOGINFO("Constructing Algorithm_Comp1");

	/** @addtogroup comp1_group1
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

    LOGINFO("Constructing Algorithm_Comp1 successful");
}

Algorithm_Comp1_Implementation::~Algorithm_Comp1_Implementation()
{

}

void Algorithm_Comp1_Implementation::UpdateInput(int localLinkId,
                                                       const std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);

    bool success = inputPorts.at(localLinkId)->SetSignalValue(data);
    if (success)
        LOGDEBUG(QString().sprintf("Algorithm_Comp1: Update input #%d successful", localLinkId).toStdString());
    else
        LOGERROR(QString().sprintf("Algorithm_Comp1: Update input #%d failed", localLinkId).toStdString());
}

void Algorithm_Comp1_Implementation::UpdateOutput(int localLinkId,
                                                        std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);

    bool success = outputPorts.at(localLinkId)->GetSignalValue(data);
    if (success)
        LOGDEBUG(QString().sprintf("Algorithm_Comp1: Update output #%d successful", localLinkId).toStdString());
    else
        LOGERROR(QString().sprintf("Algorithm_Comp1: Update output #%d failed", localLinkId).toStdString());
}

void Algorithm_Comp1_Implementation::Trigger(int time)
{
    Q_UNUSED(time);
	
	/** @addtogroup comp1_group2
     * doc content for group2.
    */

    output1.SetValue(INFINITY);

    return;
}
