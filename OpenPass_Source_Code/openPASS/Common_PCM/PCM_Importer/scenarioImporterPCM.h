/*********************************************************************
* Copyright (c) 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

//-----------------------------------------------------------------------------
//! @file  scenarioImporterPCM.h
//! @brief This file contains the importer of the scenario configuration.
//-----------------------------------------------------------------------------

#ifndef SCENARIOIMPORTERPCM_H
#define SCENARIOIMPORTERPCM_H

#include <QFile>
#include <QDomDocument>
#include "pcm_data.h"
#include "pcm_trajectory.h"
#include "globalDefinitions.h"
#include "pcm_helper.h"

/*!
* \brief The ScenarioImporterPCM class
* This class is responsible to import pcm data from a given scenario configuration file.
*/

class ScenarioImporterPCM
{
public:
    ScenarioImporterPCM() = default;
    ScenarioImporterPCM(const ScenarioImporterPCM &) = delete;
    ScenarioImporterPCM(ScenarioImporterPCM &&) = delete;
    ScenarioImporterPCM &operator=(const ScenarioImporterPCM &) = delete;
    ScenarioImporterPCM &operator=(ScenarioImporterPCM &&) = delete;
    virtual ~ScenarioImporterPCM() = default;

    //-----------------------------------------------------------------------------
    //! Imports data structures from the scenario configuration PCM file
    //!
    //! @param[in]  filename       path to PCM file
    //! @param[in]  pcmData        pcmData object to fill with data from pcm file
    //! @return                    true on success
    //-----------------------------------------------------------------------------
    bool Import(const std::string &filename,
                std::map<int, PCM_Trajectory> &trajectories);

private:

    //! Parse all trajectories.
    //!
    //! \param[in] trajectoryNode    node with trajectory
    //! \param[in,out] trajectories  trajectories map to be filled
    //! \return true for success
    bool ParseTrajectories(int id, QDomNode trajectoryNode,
                           std::map<int, PCM_Trajectory> &trajectories);
};

#endif // SCENARIOIMPORTERPCM_H
