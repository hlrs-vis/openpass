/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include <iostream>

#include "trajectoryImporter.h"
#include "CoreFramework/CoreShare/xmlParser.h"
#include "CoreFramework/CoreShare/log.h"

namespace Importer
{

bool TrajectoryImporter::Import(const std::string filename, TrajectoryInterface *trajectory)
{
    QFile xmlFile(filename.c_str()); // automatic object will be closed on destruction
    if(!xmlFile.open(QIODevice::ReadOnly))
    {
        LOG_INTERN(LogLevel::DebugUser) << "Could not open Trajectories.";
        return false;
    }

    QByteArray xmlData(xmlFile.readAll());
    QDomDocument document;
    if(!document.setContent(xmlData))
    {
        throw std::runtime_error("Invalid xml format of Trajectories");
    }

    QDomElement documentRoot = document.documentElement();
    if(documentRoot.isNull())
    {
        LOG_INTERN(LogLevel::DebugUser) << "Trajectory file was empty.";
        return false;
    }

    if(!ImportTrajectory(documentRoot, trajectory))
    {
        LOG_INTERN(LogLevel::DebugUser) << "Trajectories file was empty.";
        return false;
    }

    return true;
}

bool TrajectoryImporter::DetermineTrajectoryType(QDomElement trajectoryCoordinateElement, TrajectoryInterface *trajectory)
{
    double firstSCoord = -1;
    if (SimulationCommon::ParseAttributeDouble(trajectoryCoordinateElement, "S", firstSCoord))
    {
        if(firstSCoord == 0)
        {
            trajectory->SetTrajectoryType(TrajectoryType::RoadCoordinatesRelative);
        }
        else
        {
            trajectory->SetTrajectoryType(TrajectoryType::RoadCoordinatesAbsolute);
        }
    }
    else if (trajectoryCoordinateElement.hasAttribute(QString::fromStdString("X")))
    {
        trajectory->SetTrajectoryType(TrajectoryType::WorldCoordinatesAbsolute);
    }
    else
    {
        LOG_INTERN(LogLevel::Error) << "No valid trajectory type found.";
        return false;
    }

    return true;
}

bool TrajectoryImporter::ImportTrajectory(QDomElement trajectoryElement, TrajectoryInterface *trajectory)
{
    QDomElement trajectoryCoordinateElement;
    if(!SimulationCommon::GetFirstChildElement(trajectoryElement, "TrajectoryCoordinate", trajectoryCoordinateElement))
    {
        LOG_INTERN(LogLevel::Error) << "No trajectory coordinates found.";
        return false;
    }

    if(!DetermineTrajectoryType(trajectoryCoordinateElement, trajectory))
    {
        return false;
    }

    TrajectoryType type = trajectory->GetTrajectoryType();
    while(!trajectoryCoordinateElement.isNull())
    {
        int trajectoryCoordinateTime;
        if(!SimulationCommon::ParseAttributeInt(trajectoryCoordinateElement, "Time", trajectoryCoordinateTime))
        {
            LOG_INTERN(LogLevel::Error) << "Trajectory coordinate must contain Time.";
            return false;
        }

        switch (type) {
        case TrajectoryType::RoadCoordinatesAbsolute:
        case TrajectoryType::RoadCoordinatesRelative:
        {
            RoadPosition roadCoordinate;

            if(!SimulationCommon::ParseAttributeDouble(trajectoryCoordinateElement, "S", roadCoordinate.s)
                    || !SimulationCommon::ParseAttributeDouble(trajectoryCoordinateElement, "T", roadCoordinate.t)
                    || !SimulationCommon::ParseAttributeDouble(trajectoryCoordinateElement, "Hdg", roadCoordinate.hdg))
            {
                LOG_INTERN(LogLevel::Error) << "Could not import road coordinate.";
                return false;
            }

            if(!trajectory->AddRoadCoordinate(trajectoryCoordinateTime, roadCoordinate))
            {
                LOG_INTERN(LogLevel::Error) << "Trajectory coordinate time must be unique.";
                return false;
            }

            break;
        }
        case TrajectoryType::WorldCoordinatesAbsolute:
        {
            Position worldCoordinate;

            if(!SimulationCommon::ParseAttributeDouble(trajectoryCoordinateElement, "X", worldCoordinate.xPos)
                    || !SimulationCommon::ParseAttributeDouble(trajectoryCoordinateElement, "Y", worldCoordinate.yPos)
                    || !SimulationCommon::ParseAttributeDouble(trajectoryCoordinateElement, "Yaw", worldCoordinate.yawAngle))
            {
                LOG_INTERN(LogLevel::Error) << "Could not import world coordinate.";
                return false;
            }

            if(!trajectory->AddWorldCoordinate(trajectoryCoordinateTime, worldCoordinate))
            {
                LOG_INTERN(LogLevel::Error) << "Trajectory coordinate time must be unique.";
                return false;
            }

            break;
        }
        default:
            throw std::runtime_error("Unknown trajectory type.");
        }
        trajectoryCoordinateElement = trajectoryCoordinateElement.nextSiblingElement("TrajectoryCoordinate");
    }

    return true;
}

} //namespace Importer
