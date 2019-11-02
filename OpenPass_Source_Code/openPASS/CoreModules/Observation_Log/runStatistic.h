/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2016, 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#pragma once

#include <string>
#include <list>
#include <QXmlStreamWriter>
#include "observationTypes.h"
#include "Interfaces/observationInterface.h"

enum class StopReason
{
    DueToTimeOut
};

class RunStatistic
{
public:
    RunStatistic(std::uint32_t randomSeed);

    void AddStopReason(int time, StopReason reason);
    void WriteStatistics(std::shared_ptr<QXmlStreamWriter> fileStream);

    std::list<int> *GetFollowerIds();

    // general
    int StopTime = -1; //this stays on UNDEFINED_NUMBER, if due time out -> replace in c#
    bool EgoCollision = false;
    int NCollisionsFollowers = 0;
    int NCollisionsArbitrary = 0;
    double TotalDistanceTraveled = 0.0;
    double EgoDistanceTraveled = 0.0;
    double VisibilityDistance = -999.0; //Visibility distance of world in current run (defined in slaveConfig.xml)

    static QString BoolToString(bool b);

    ~RunStatistic()
    {
    }

private:
    std::uint32_t _randomSeed;
    std::list<int> _followerIds;

    static const QString StopReasonsStrings[];
    QString _stopReason = StopReasonsStrings[static_cast<int>(StopReason::DueToTimeOut)];
}; // class RunStatistic


