/*******************************************************************************
* Copyright (c) 2017, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include "manipulatorImporter.h"
#include "xmlParser.h"
#include "Common/openScenarioDefinitions.h"

#include <boost/algorithm/string/trim.hpp>

namespace Importer
{
std::shared_ptr<ScenarioActionInterface> ManipulatorImporter::ImportManipulator(QDomElement& eventElement,
                                                                        const std::vector<std::string>& eventDetectorNames,
                                                                        const std::vector<std::string>& actors,
                                                                        const std::string& sequenceName)
{
    QDomElement actionElement;
    if (SimulationCommon::GetFirstChildElement(eventElement, "Action", actionElement))
    {
        QDomElement actionTypeElement;
        if (SimulationCommon::GetFirstChildElement(actionElement, "UserDefined", actionTypeElement))
        {
            return ImportManipulatorFromUserDefinedElement(actionTypeElement,
                                                           eventDetectorNames,
                                                           actors,
                                                           sequenceName);
        }
        else if (SimulationCommon::GetFirstChildElement(actionElement, "Private", actionTypeElement))
        {
            return ImportManipulatorFromPrivateElement(actionTypeElement,
                                                       eventDetectorNames,
                                                       actors,
                                                       sequenceName);
        }
        else if (SimulationCommon::GetFirstChildElement(actionElement, "Global", actionTypeElement))
        {
            return ImportManipulatorFromGlobalElement(actionTypeElement,
                                                      eventDetectorNames,
                                                      actors,
                                                      sequenceName);
        }
    }

    return std::shared_ptr<ScenarioActionInterface>(nullptr);
}

std::shared_ptr<ScenarioActionInterface> ManipulatorImporter::ImportManipulatorFromUserDefinedElement(QDomElement& userDefinedElement,
                                                                                              const std::vector<std::string>& eventDetectorNames,
                                                                                              const std::vector<std::string>& actors,
                                                                                              const std::string& sequenceName)
{
    QDomElement userDefinedChildElement;
    if (SimulationCommon::GetFirstChildElement(userDefinedElement, "Command", userDefinedChildElement))
    {
        std::string command = userDefinedChildElement.text().toStdString();
        boost::algorithm::trim(command);

        return std::shared_ptr<ScenarioActionInterface>(std::make_shared<UserDefinedCommandAction>(eventDetectorNames,
                                                                                           actors,
                                                                                           sequenceName,
                                                                                           command));
    }

    return std::shared_ptr<ScenarioActionInterface>(nullptr);
}

std::shared_ptr<ScenarioActionInterface> ManipulatorImporter::ImportManipulatorFromPrivateElement(QDomElement& privateElement,
                                                                                          const std::vector<std::string>& eventDetectorNames,
                                                                                          const std::vector<std::string>& actors,
                                                                                          const std::string& sequenceName)
{
    QDomElement privateChildElement;
    if(SimulationCommon::GetFirstChildElement(privateElement, "Lateral", privateChildElement))
    {
        QDomElement lateralChildElement;
        if (SimulationCommon::GetFirstChildElement(privateChildElement, "LaneChange", lateralChildElement))
        {
            QDomElement targetElement;
            if (SimulationCommon::GetFirstChildElement(lateralChildElement, "Target", targetElement))
            {
                QDomElement typeElement;
                if (SimulationCommon::GetFirstChildElement(targetElement, "Relative", typeElement))
                {
                    std::string object;
                    double value{};

                    if (SimulationCommon::ParseAttributeString(typeElement, "object", object)
                        && SimulationCommon::ParseAttributeDouble(typeElement, "value", value))
                    {
                        return std::shared_ptr<ScenarioActionInterface>(std::make_shared<PrivateLateralLaneChangeAction>(eventDetectorNames,
                                                                                                                 actors,
                                                                                                                 sequenceName,
                                                                                                                 PrivateLateralLaneChangeActionType::Relative,
                                                                                                                 static_cast<int>(std::rint(value)),
                                                                                                                 object));
                    }

                }
                else if (SimulationCommon::GetFirstChildElement(targetElement, "Absolute", typeElement))
                {
                    double value;

                    if (SimulationCommon::ParseAttributeDouble(typeElement, "value", value))
                    {
                        return std::shared_ptr<ScenarioActionInterface>(std::make_shared<PrivateLateralLaneChangeAction>(eventDetectorNames,
                                                                                                                 actors,
                                                                                                                 sequenceName,
                                                                                                                 PrivateLateralLaneChangeActionType::Absolute,
                                                                                                                 static_cast<int>(std::rint(value))));
                    }
                }
            }
        }
    }

    return std::shared_ptr<ScenarioActionInterface>(nullptr);
}

std::shared_ptr<ScenarioActionInterface> ManipulatorImporter::ImportManipulatorFromGlobalElement(QDomElement &globalElement,
                                                                                         const std::vector<std::string>& eventDetectorNames,
                                                                                         const std::vector<std::string>& actors,
                                                                                         const std::string& sequenceName)
{
    QDomElement globalChildElement;
    if (SimulationCommon::GetFirstChildElement(globalElement, "Entity", globalChildElement))
    {
        std::string name;
        SimulationCommon::ParseAttributeString(globalChildElement, "name", name);

        QDomElement globalChildIdentifierElement;
        if (SimulationCommon::GetFirstChild(globalChildElement, globalChildIdentifierElement))
        {
            if (globalChildIdentifierElement.tagName().toStdString() == "Add")
            {
                return std::shared_ptr<ScenarioActionInterface>(std::make_shared<GlobalEntityAction>(eventDetectorNames,
                                                                                             actors,
                                                                                             sequenceName,
                                                                                             GlobalEntityActionType::Add,
                                                                                             name));
            }
            else if (globalChildIdentifierElement.tagName().toStdString() == "Delete")
            {
                return std::shared_ptr<ScenarioActionInterface>(std::make_shared<GlobalEntityAction>(eventDetectorNames,
                                                                                             actors,
                                                                                             sequenceName,
                                                                                             GlobalEntityActionType::Delete,
                                                                                             name));
            }
        }
    }

    return std::shared_ptr<ScenarioActionInterface>(nullptr);
}
}
