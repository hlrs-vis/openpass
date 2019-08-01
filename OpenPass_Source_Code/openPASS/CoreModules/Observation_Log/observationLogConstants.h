/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#pragma once

#include <QString>

struct OutputAttributes
{
    const QString SCHEMAVERSION = "SchemaVersion";
    const QString RUNID = "RunId";
    const QString FAILUREPROBABITLITY= "FailureProbability";
    const QString LATENCY = "Latency";
    const QString OPENINGANGLEH = "OpeningAngleH";
    const QString OPENINGANGLEV = "OpeningAngleV";
    const QString MOUNTINGPOSITIONLONGITUDINAL = "MountingPosLongitudinal";
    const QString MOUNTINGPOSITIONLATERAL = "MountingPosLateral";
    const QString MOUNTINGPOSITIONHEIGHT = "MountingPosHeight";
    const QString ORIENTATIONYAW = "OrientationYaw";
    const QString ORIENTATIONPITCH = "OrientationPitch";
    const QString ORIENTATIONROLL = "OrientationRoll";
    const QString DETECTIONRANGE = "DetectionRange";
    const QString VEHICLEMODELTYPE = "VehicleModelType";
    const QString DRIVERPROFILENAME = "DriverProfileName";
    const QString AGENTTYPEGROUPNAME = "AgentTypeGroupName";
    const QString AGENTTYPENAME = "AgentTypeName";
    const QString TIME = "Time";
    const QString TYPE = "Type";
    const QString TRIGGERINGEVENTID = "TriggeringEventId";
    const QString KEY = "Key";
    const QString VALUE = "Value";
    const QString ID = "Id";
    const QString SOURCE = "Source";
};

struct OutputTags
{
    const QString RUNRESULTS = "RunResults";
    const QString RUNRESULT = "RunResult";
    const QString RUNSTATISTICS = "RunStatistics";
    const QString SIMULATIONOUTPUT = "SimulationOutput";
    const QString EVENTS = "Events";
    const QString EVENT = "Event";
    const QString EVENTPARAMETER = "EventParameter";
    const QString AGENTS = "Agents";
    const QString AGENT = "Agent";
    const QString SENSORS = "Sensors";
    const QString SENSOR = "Sensor";
    const QString CYCLICS = "Cyclics";
    const QString HEADER = "Header";
    const QString SAMPLES = "Samples";
    const QString SAMPLE = "Sample";
    const QString SCENERYFILE = "SceneryFile";
};
