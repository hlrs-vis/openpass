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

#include "Interfaces/scenarioActionInterface.h"

#include <string>

class Action : public ScenarioActionInterface
{
public:
    Action(const std::vector<std::string>& eventDetectorNames,
           const std::vector<std::string>& actors,
           const std::string& sequenceNames);
    Action() = delete;
    Action(const Action&) = delete;
    Action(Action&&) = delete;
    Action& operator=(const Action&) = delete;
    Action& operator=(Action&&) = delete;

    /*!
     * ------------------------------------------------------------------------
     * \brief GetEventDetectorNames returns the collection of names of the
     *        event detectors that relate to this action.
     *
     * \returns a reference to the collection of names of the event detectors
     *          that relate to this action.
     * ------------------------------------------------------------------------
     */
    const std::vector<std::string>& GetEventDetectorNames() const;

    /*!
     * ------------------------------------------------------------------------
     * \brief GetActors returns the collection of names of the actor entities
     *        this action will target.
     *
     * \returns a reference to the collection of names of the actor entities
     *          this action will target.
     * ------------------------------------------------------------------------
     */
    const std::vector<std::string>& GetActors() const;

    /*!
     * ------------------------------------------------------------------------
     * \brief GetSequenceName returns the name of the sequence of which this
     *        action is a part.
     *
     * \returns a reference to the name of the sequence of which this action is
     *          a part.
     * ------------------------------------------------------------------------
     */
    const std::string& GetSequenceName() const;
private:
    std::vector<std::string> eventDetectorNames;
    std::vector<std::string> actors;
    std::string sequenceName;
};

class GlobalAction : public Action
{
public:
    GlobalAction(const std::vector<std::string>& eventDetectorNames,
                 const std::vector<std::string>& actors,
                 const std::string& sequenceNames);
    GlobalAction() = delete;
    GlobalAction(const GlobalAction&) = delete;
    GlobalAction(GlobalAction&&) = delete;
    GlobalAction& operator=(const GlobalAction&) = delete;
    GlobalAction& operator=(GlobalAction&&) = delete;
private:
};

enum GlobalEntityActionType
{
    Delete = 0,
    Add
};

class GlobalEntityAction : public GlobalAction
{
public:
    GlobalEntityAction(const std::vector<std::string>& eventDetectorNames,
                       const std::vector<std::string>& actors,
                       const std::string& sequenceNames,
                       GlobalEntityActionType type,
                       const std::string& name);

    GlobalEntityAction() = delete;
    GlobalEntityAction(const GlobalEntityAction&) = delete;
    GlobalEntityAction(GlobalEntityAction&&) = delete;
    GlobalEntityAction& operator=(const GlobalEntityAction&) = delete;
    GlobalEntityAction& operator=(GlobalEntityAction&&) = delete;

    /*!
     * ------------------------------------------------------------------------
     * \brief GetType returns the type of Global Entity Action this Action is.
     *
     * \returns GlobalEntityActionType::Delete or GlobalEntityActionType::Add.
     * ------------------------------------------------------------------------
     */
    GlobalEntityActionType GetType() const;
    /*!
     * ------------------------------------------------------------------------
     * \brief GetName returns the name of the entity specified by this action.
     *
     * \returns the name of the entity specified by this action.
     * ------------------------------------------------------------------------
     */
    const std::string& GetName() const;
private:
    GlobalEntityActionType type;
    std::string name;
    // we'll ignore the "Add" element's "Position" child, even though required by standard
};

class PrivateAction : public Action
{
public:
    PrivateAction(const std::vector<std::string>& eventDetectorNames,
                  const std::vector<std::string>& actors,
                  const std::string& sequenceNames);

    PrivateAction() = delete;
    PrivateAction(const PrivateAction&) = delete;
    PrivateAction(PrivateAction&&) = delete;
    PrivateAction& operator=(const PrivateAction&) = delete;
    PrivateAction& operator=(PrivateAction&&) = delete;
private:
};

enum PrivateLateralLaneChangeActionType
{
    Absolute,
    Relative
};

class PrivateLateralLaneChangeAction : public PrivateAction
{
public:
    PrivateLateralLaneChangeAction(const std::vector<std::string>& eventDetectorNames,
                                   const std::vector<std::string>& actors,
                                   const std::string& sequenceNames,
                                   PrivateLateralLaneChangeActionType type,
                                   int value,
                                   const std::string& object = "");

    PrivateLateralLaneChangeAction() = delete;
    PrivateLateralLaneChangeAction(const PrivateLateralLaneChangeAction&) = delete;
    PrivateLateralLaneChangeAction(PrivateLateralLaneChangeAction&&) = delete;
    PrivateLateralLaneChangeAction& operator=(const PrivateLateralLaneChangeAction&) = delete;
    PrivateLateralLaneChangeAction& operator=(PrivateLateralLaneChangeAction&&) = delete;

    /*!
     * ------------------------------------------------------------------------
     * \brief GetType returns the type of Private Lateral LaneChange Action
     *        this Action is.
     *
     * \returns PrivateLateralLaneChangeActionType::Absolute or
     *          PrivateLateralLaneChangeActionType::Relative.
     * ------------------------------------------------------------------------
     */
    PrivateLateralLaneChangeActionType GetType() const;

    /*!
     * ------------------------------------------------------------------------
     * \brief GetValue returns the lane offset value specified as a target by
     *        this Action.
     *
     * \returns the lane offset value specified as a target by this Action.
     * ------------------------------------------------------------------------
     */
    int GetValue() const;
    /*!
     * ------------------------------------------------------------------------
     * \brief GetObject returns the name of the entity from which relative
     *        offsets are specified.
     *
     * \returns the name of the entity from which relative offsets are
     *          specified. Returns "" if no such entity was specified.
     * ------------------------------------------------------------------------
     */
    const std::string& GetObject() const;
private:
    PrivateLateralLaneChangeActionType type;
    int value;
    std::string object; // optional
    // we'll ignore the LaneChange element's "Dynamics" tag for now
    // we'll ignore the LaneChange element's "targetLaneOffset" attribute for now
};

class UserDefinedAction : public Action
{
public:
    UserDefinedAction(const std::vector<std::string>& eventDetectorNames,
                      const std::vector<std::string>& actors,
                      const std::string& sequenceNames);
    UserDefinedAction() = delete;
    UserDefinedAction(const UserDefinedAction&) = delete;
    UserDefinedAction(UserDefinedAction&&) = delete;
    UserDefinedAction& operator=(const UserDefinedAction&) = delete;
    UserDefinedAction& operator=(UserDefinedAction&&) = delete;
private:
};

class UserDefinedCommandAction : public UserDefinedAction
{
public:
    UserDefinedCommandAction(const std::vector<std::string>& eventDetectorNames,
                             const std::vector<std::string>& actors,
                             const std::string& sequenceNames,
                             const std::string& command);

    UserDefinedCommandAction() = delete;
    UserDefinedCommandAction(const UserDefinedCommandAction&) = delete;
    UserDefinedCommandAction(UserDefinedCommandAction&&) = delete;
    UserDefinedCommandAction& operator=(const UserDefinedCommandAction&) = delete;
    UserDefinedCommandAction& operator=(UserDefinedCommandAction&&) = delete;

    /*!
     * ------------------------------------------------------------------------
     * \brief GetCommand returns the command string specified for this Action
     *
     * \returns the command string specified for this Action
     * ------------------------------------------------------------------------
     */
    const std::string& GetCommand() const;
private:
    std::string command;
};
