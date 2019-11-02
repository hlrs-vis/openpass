/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
#include "openScenarioDefinitions.h"

/*
 * ----------------------------------------------------------------------------
 * Action implementation
 * ----------------------------------------------------------------------------
 */
Action::Action(const std::vector<std::string>& eventDetectorNames,
               const std::vector<std::string>& actors,
               const std::string& sequenceName):
    ScenarioActionInterface(),
    eventDetectorNames{eventDetectorNames},
    actors{actors},
    sequenceName{sequenceName}
{
}

const std::vector<std::string>& Action::GetEventDetectorNames() const
{
    return eventDetectorNames;
}

const std::vector<std::string>& Action::GetActors() const
{
    return actors;
}

const std::string& Action::GetSequenceName() const
{
    return sequenceName;
}

/*
 * ----------------------------------------------------------------------------
 * GlobalAction implementation
 * ----------------------------------------------------------------------------
 */
GlobalAction::GlobalAction(const std::vector<std::string>& eventDetectorNames,
                           const std::vector<std::string>& actors,
                           const std::string& sequenceName):
    Action(eventDetectorNames,
           actors,
           sequenceName)
{
}

/*
 * ----------------------------------------------------------------------------
 * GlobalEntityAction implementation
 * ----------------------------------------------------------------------------
 */
GlobalEntityAction::GlobalEntityAction(const std::vector<std::string>& eventDetectorNames,
                                       const std::vector<std::string>& actors,
                                       const std::string& sequenceName,
                                       GlobalEntityActionType type,
                                       const std::string& name):
    GlobalAction(eventDetectorNames,
                 actors,
                 sequenceName),
    type{type},
    name{name}
{
}

GlobalEntityActionType GlobalEntityAction::GetType() const
{
    return type;
}

const std::string& GlobalEntityAction::GetName() const
{
    return name;
}

/*
 * ----------------------------------------------------------------------------
 * PrivateAction implementation
 * ----------------------------------------------------------------------------
 */
PrivateAction::PrivateAction(const std::vector<std::string>& eventDetectorNames,
                             const std::vector<std::string>& actors,
                             const std::string& sequenceName):
    Action(eventDetectorNames,
           actors,
           sequenceName)
{
}

/*
 * ----------------------------------------------------------------------------
 * PrivateLateralLaneChangeAction implementation
 * ----------------------------------------------------------------------------
 */
PrivateLateralLaneChangeAction::PrivateLateralLaneChangeAction(const std::vector<std::string>& eventDetectorNames,
                                                               const std::vector<std::string>& actors,
                                                               const std::string& sequenceName,
                                                               PrivateLateralLaneChangeActionType type,
                                                               int value,
                                                               const std::string& object):
    PrivateAction(eventDetectorNames,
                  actors,
                  sequenceName),
    type{type},
    value{value},
    object{object}
{
}

PrivateLateralLaneChangeActionType PrivateLateralLaneChangeAction::GetType() const
{
    return type;
}

int PrivateLateralLaneChangeAction::GetValue() const
{
    return value;
}

const std::string& PrivateLateralLaneChangeAction::GetObject() const
{
    return object;
}

/*
 * ----------------------------------------------------------------------------
 * UserDefinedAction implementation
 * ----------------------------------------------------------------------------
 */
UserDefinedAction::UserDefinedAction(const std::vector<std::string>& eventDetectorNames,
                                     const std::vector<std::string>& actors,
                                     const std::string& sequenceName):
    Action(eventDetectorNames,
           actors,
           sequenceName)
{
}

/*
 * ----------------------------------------------------------------------------
 * UserDefinedCommandAction implementation
 * ----------------------------------------------------------------------------
 */
UserDefinedCommandAction::UserDefinedCommandAction(const std::vector<std::string>& eventDetectorNames,
                                                   const std::vector<std::string>& actors,
                                                   const std::string& sequenceName,
                                                   const std::string& command):
    UserDefinedAction(eventDetectorNames,
                      actors,
                      sequenceName),
    command{command}
{
}

const std::string& UserDefinedCommandAction::GetCommand() const
{
    return command;
}
