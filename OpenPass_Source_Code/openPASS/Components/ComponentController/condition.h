/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
#pragma once

#include <QtGlobal>
#include <memory>

#include "Interfaces/signalInterface.h"
#include "componentControllerCommon.h"
#include "Common/globalDefinitions.h"

namespace ComponentControl {

/*!
 * \brief This class represents a Condition based on all the information the ComponentController has gathered.
 *
 * The Statemanager uses this to decide which maxReachableState a component has. The different
 * kinds of conditions are subclasses of this class
 */
class Condition
{
public:
    Condition() = default;
    virtual ~Condition() = default;

    /*!
     * \brief Returns wether this Condition is fullfilled.
     * \param componentNameToComponentTypeAndStatesMap    Information the ComponentController has gathered
     */
    virtual bool IsFullfilled(const std::map<std::string, std::pair<ComponentType, ComponentState>> &componentNameToComponentTypeAndStatesMap) const = 0;
};

/*!
 * \brief This class represents an expression that can be evaluated as ComponentState
 *
 * A ComponentStateExpression is either a fixed value or the ComponentState of some
 * named component.
 */
class ComponentStateExpression
{
public:
    ComponentStateExpression() = default;
    virtual ~ComponentStateExpression() = default;

    /*!
     * \brief Evaluates this expression
     * \param componentNameToComponentTypeAndStatesMap    Information the ComponentController has gathered
     * \return ComponentState this expression evaluates to
     */
    virtual ComponentState Get(const std::map<std::string, std::pair<ComponentType, ComponentState>> &componentNameToComponentTypeAndStateMap) const = 0;
};

/*!
 * \brief The FixedComponentStateExpression is a ComponentStateExpression that
 * always evaluates to the same ComponentState
 */
class FixedComponentStateExpression : public ComponentStateExpression
{
public:
    /*!
     * \param value The fixed ComponentState
     */
    FixedComponentStateExpression(ComponentState value);

    ComponentState Get(const std::map<std::string, std::pair<ComponentType, ComponentState>> &componentNameToComponentTypeAndStateMap) const override;

private:
    ComponentState value;
};

/*!
 * \brief The VehicleComponentStateExpression is a ComponentStateExpression that
 * evaluates to the current state of some fixed component
 */
class VehicleComponentStateExpression : public ComponentStateExpression
{
public:
    /*!
     * \param component Name of the component of which the state is evaluated
     */
    VehicleComponentStateExpression(const std::string& component);

    ComponentState Get(const std::map<std::string, std::pair<ComponentType, ComponentState>> &componentNameToComponentTypeAndStateMap) const override;

private:
    const std::string component;
};

/*!
 * \brief This Condition is fullfilled if two ComponentStateExpressions are
 * evaluated to the same value
 *
 * The ComponentStateEquality is used either to check, wether the state of two components
 * is the same, or to check of the state of some component is equal to some fixed value (e.g. Acting).
 */
class ComponentStateEquality : public Condition
{
public:
    ComponentStateEquality(std::unique_ptr<ComponentStateExpression> expression1, std::unique_ptr<ComponentStateExpression> expression2);

    bool IsFullfilled(const std::map<std::string, std::pair<ComponentType, ComponentState>> &componentNameToComponentTypeAndStateMap) const override;

private:
    std::unique_ptr<ComponentStateExpression> expression1;
    std::unique_ptr<ComponentStateExpression> expression2;
};

} // namespace ComponentControl
