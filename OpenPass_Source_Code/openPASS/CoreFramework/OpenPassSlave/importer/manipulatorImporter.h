/*******************************************************************************
* Copyright (c) 2017, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#pragma once

#include <string>
#include <QDomDocument>
#include "CoreFramework/CoreShare/parameters.h"
#include "Interfaces/scenarioActionInterface.h"

namespace Importer
{
class ManipulatorImporter
{
public:
    ManipulatorImporter() = default;
    ManipulatorImporter(const ManipulatorImporter&) = delete;
    ManipulatorImporter(ManipulatorImporter&&) = delete;
    ManipulatorImporter& operator=(const ManipulatorImporter&) = delete;
    ManipulatorImporter& operator=(ManipulatorImporter&&) = delete;
    virtual ~ManipulatorImporter() = default;

    /*!
     * ------------------------------------------------------------------------
     * \brief ImportManipulator parses an Event element from an openScenario
     *        adherent Scenario file
     *
     * \param[in] eventElement The OpenSCENARIO-adherent Event element from
     *            which to import the manipulator
     * \param[in] eventDetectorNames The names of the event detectors in the
     *            scenario
     * \param[in] actors The names of the actors in the sequence
     * \param[in] sequenceName The name of the sequence
     *
     * \returns a std::shared_ptr to a scenario action interface containing
     *          pertinent information from within the eventElement
     * ------------------------------------------------------------------------
     */
    static std::shared_ptr<ScenarioActionInterface> ImportManipulator(QDomElement& eventElement,
                                                              const std::vector<std::string>& eventDetectorNames,
                                                              const std::vector<std::string>& actors,
                                                              const std::string& sequenceName);

private:
    /*!
     * ------------------------------------------------------------------------
     * \brief ImportManipulatorFromPrivateElement Parses a Private element into
     *        an ActionInterface from which a manipulator may be created
     *
     * \param[in] privateElement the Private element to parse
     *
     * \returns a shared_ptr to the ActionInterface containing the parsed data
     * ------------------------------------------------------------------------
     */
    static std::shared_ptr<ScenarioActionInterface> ImportManipulatorFromPrivateElement(QDomElement& privateElement,
                                                                                const std::vector<std::string>& eventDetectorNames,
                                                                                const std::vector<std::string>& actors,
                                                                                const std::string& sequenceName);

    /*!
     * ------------------------------------------------------------------------
     * \brief ImportManipulatorFromGlobalElement Parses a Global element into
     *        an ActionInterface from which a manipulator may be created
     *
     * \param[in] globalElement the Global element to parse
     *
     * \returns a shared_ptr to the ActionInterface containing the parsed data
     * ------------------------------------------------------------------------
     */
    static std::shared_ptr<ScenarioActionInterface> ImportManipulatorFromGlobalElement(QDomElement& globalElement,
                                                                               const std::vector<std::string>& eventDetectorNames,
                                                                               const std::vector<std::string>& actors,
                                                                               const std::string& sequenceName);

    /*!
     * ------------------------------------------------------------------------
     * \brief ImportManipulatorFromUserDefinedElement Parses a UserDefined element into
     *        an ActionInterface from which a manipulator may be created
     *
     * \param[in] userDefinedElement the UserDefined element to parse
     *
     * \returns a shared_ptr to the ActionInterface containing the parsed data
     * ------------------------------------------------------------------------
     */
    static std::shared_ptr<ScenarioActionInterface> ImportManipulatorFromUserDefinedElement(QDomElement& userDefinedElement,
                                                                                    const std::vector<std::string>& eventDetectorNames,
                                                                                    const std::vector<std::string>& actors,
                                                                                    const std::string& sequenceName);
};
} // Importer
