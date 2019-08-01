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

//-----------------------------------------------------------------------------
/** \file  RunStatistic.cpp */
//-----------------------------------------------------------------------------

#include "runStatistic.h"

const QString RunStatistic::StopReasonsStrings[] =
{
    "Due to time out",
    "Due to time after crash",
    "Due to agent reached target position"
};

RunStatistic::RunStatistic(std::uint32_t randomSeed):
    _randomSeed(randomSeed)
{}

void RunStatistic::AddStopReason(int time, StopReason reason)
{
    _stopReason = StopReasonsStrings[static_cast<int>(reason)];
    StopTime = time;
}

std::list<int> *RunStatistic::GetFollowerIds()
{
    return &_followerIds;
}

// ----------------------------- writing out --------------------- //
void RunStatistic::WriteStatistics(std::shared_ptr<QXmlStreamWriter> fileStream)
{
    fileStream->writeStartElement("RandomSeed");
    fileStream->writeCharacters(QString::number(_randomSeed));
    fileStream->writeEndElement();

    fileStream->writeStartElement("VisibilityDistance");
    fileStream->writeCharacters(QString::number(VisibilityDistance));
    fileStream->writeEndElement();

    fileStream->writeStartElement("StopReason");
    fileStream->writeCharacters(_stopReason);
    fileStream->writeEndElement();

    fileStream->writeStartElement("StopTime");
    fileStream->writeCharacters(QString::number(StopTime));
    fileStream->writeEndElement();

    fileStream->writeStartElement("EgoAccident");
    fileStream->writeCharacters(BoolToString(EgoCollision ));
    fileStream->writeEndElement();

    fileStream->writeStartElement("NumberOfAccidentsInFollowers");
    fileStream->writeCharacters(QString::number(NCollisionsFollowers));
    fileStream->writeEndElement();

    fileStream->writeStartElement("NumberOfArbitraryAccidents");
    fileStream->writeCharacters(QString::number(NCollisionsArbitrary));
    fileStream->writeEndElement();

    fileStream->writeStartElement("TotalDistanceTraveled");
    fileStream->writeCharacters(QString::number(TotalDistanceTraveled));
    fileStream->writeEndElement();

    fileStream->writeStartElement("EgoDistanceTraveled");
    fileStream->writeCharacters(QString::number(EgoDistanceTraveled));
    fileStream->writeEndElement();
}

QString RunStatistic::BoolToString(bool b)
{
    return b? "True" : "False";
}
