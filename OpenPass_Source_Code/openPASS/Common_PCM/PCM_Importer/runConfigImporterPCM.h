/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

//-----------------------------------------------------------------------------
//! @file  runConfigImporterPCM.h
//! @brief This file contains the importer of the simulation run configuration.
//-----------------------------------------------------------------------------

#ifndef RUNCONFIGIMPORTERPCM_H
#define RUNCONFIGIMPORTERPCM_H

#include <QString>
#include <QDomElement>
#include "pcm_initialValues.h"
#include "pcm_participantData.h"
#include "pcm_trajectory.h"
#include "parameters.h"

class RunConfigImporterPCM
{
public:    
    RunConfigImporterPCM();
    RunConfigImporterPCM(const RunConfigImporterPCM&) = delete;
    RunConfigImporterPCM(RunConfigImporterPCM&&) = delete;
    RunConfigImporterPCM& operator=(const RunConfigImporterPCM&) = delete;
    RunConfigImporterPCM& operator=(RunConfigImporterPCM&&) = delete;
    virtual ~RunConfigImporterPCM();

    //    static bool ImportParticipantData(const QString &filename, std::vector<PCM_ParticipantData> &participants);

    bool Import(const QString &filename);

    std::vector<PCM_InitialValues> GetInitials();
    std::vector<PCM_ParticipantData> GetParticipants();
    std::vector<PCM_Trajectory *> GetTrajectories();
    QString GetCaseNumber();

private:
    bool GetDocumentRoot(const QString &filename, QDomElement &documentRoot);
    bool ImportInitials(QDomElement &documentRoot);
    bool ImportParticipants(QDomElement &documentRoot);
    bool ImportObservations(QDomElement &documentRoot);

    SimulationCommon::Parameters *observationParams;
    std::vector<PCM_InitialValues> initials;
    std::vector<PCM_ParticipantData> participants;
    std::vector<PCM_Trajectory *> trajectories;
    QString pcmCase;
};

#endif // RUNCONFIGIMPORTERPCM_H
