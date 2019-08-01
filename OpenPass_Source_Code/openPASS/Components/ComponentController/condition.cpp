/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
#include <QtGlobal>

#include "condition.h"

using namespace ComponentControl;

FixedComponentStateExpression::FixedComponentStateExpression(ComponentState value) : value(value)
{

}

ComponentState FixedComponentStateExpression::Get(const std::map<std::string, std::pair<ComponentType, ComponentState>> &componentNameToComponentTypeAndStateMap) const
{
    Q_UNUSED(componentNameToComponentTypeAndStateMap);
    return value;
}

VehicleComponentStateExpression::VehicleComponentStateExpression(const std::string& component) : component(component)
{

}

ComponentState VehicleComponentStateExpression::Get(const std::map<std::string, std::pair<ComponentType, ComponentState>> &componentNameToComponentTypeAndStateMap) const
{
    try
    {
        auto componentInformation = componentNameToComponentTypeAndStateMap.at(component);
        return componentInformation.second;
    }
    catch (const std::out_of_range& e)
    {
        Q_UNUSED(e);
        return ComponentState::Undefined;
    }
}


ComponentStateEquality::ComponentStateEquality(std::unique_ptr<ComponentStateExpression> expression1, std::unique_ptr<ComponentStateExpression> expression2)
{
    this->expression1 = std::move(expression1);
    this->expression2 = std::move(expression2);
}

bool ComponentStateEquality::IsFullfilled(const std::map<std::string, std::pair<ComponentType, ComponentState>> &componentNameToComponentTypeAndStateMap) const
{
    return expression1->Get(componentNameToComponentTypeAndStateMap) == expression2->Get(componentNameToComponentTypeAndStateMap);
}
