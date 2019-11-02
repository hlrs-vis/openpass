/*******************************************************************************
* Copyright (c) 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/
#pragma once

#include <gmock/gmock.h>
#include <map>

#include "DataTypes.h"
#include "WorldData.h"

namespace osi3 {
class SensorView;
class SensorViewConfiguration;
}

class FakeWorldData : public OWL::Interfaces::WorldData
{
public:
    MOCK_METHOD0(Clear, void());
    MOCK_METHOD0(Reset, void());
    MOCK_CONST_METHOD0(GetRoads,
                       const std::unordered_map<OWL::Id, OWL::Interfaces::Road*>& ());
    MOCK_METHOD1(AddMovingObject,
                 OWL::Interfaces::MovingObject & (void* linkedObject));
    MOCK_METHOD1(AddStationaryObject,
                 OWL::Interfaces::StationaryObject & (void* linkedObject));
    MOCK_METHOD1(RemoveMovingObjectById,
                 void(OWL::Id id));
    MOCK_CONST_METHOD0(GetRoadIdMapping,
                       const std::unordered_map<OWL::Id, std::string>& ());
    MOCK_CONST_METHOD0(GetLaneIdMapping,
                       const std::unordered_map<OWL::Id, OWL::OdId>& ());
    MOCK_CONST_METHOD0(GetLanes,
                       const std::unordered_map<OWL::Id, OWL::Interfaces::Lane*>& ());
    MOCK_CONST_METHOD0(GetSections,
                       const std::map<OWL::Id, OWL::Interfaces::Section*>& ());
    MOCK_METHOD2(AddLane, void(RoadLaneSectionInterface&, const RoadLaneInterface&));
    MOCK_METHOD2(AddSection, void(const RoadInterface&, const RoadLaneSectionInterface&));
    MOCK_METHOD1(AddRoad, void(const RoadInterface&));
    MOCK_METHOD1(AddJunction, void (const JunctionInterface* odJunction));
    MOCK_METHOD2(AddJunctionConnection, void (const JunctionInterface* odJunction, const RoadInterface& odRoad));

    MOCK_METHOD7(AddLaneGeometryPoint, void(const RoadLaneInterface&,
                                            const Common::Vector2d&,
                                            const Common::Vector2d&,
                                            const Common::Vector2d&,
                                            const double,
                                            const double,
                                            const double));

    MOCK_METHOD2(AddLaneSuccessor, void(/* const */ RoadLaneInterface&,
                 /* const */ RoadLaneInterface&));

    MOCK_METHOD2(AddLanePredecessor, void(/* const */ RoadLaneInterface&,
                 /* const */ RoadLaneInterface&));

    MOCK_METHOD2(SetRoadSuccessor, void(const RoadInterface&, const RoadInterface&));

    MOCK_METHOD2(SetRoadPredecessor, void(const RoadInterface&, const RoadInterface&));


    MOCK_METHOD2(SetSectionSuccessor, void(const RoadLaneSectionInterface&, const RoadLaneSectionInterface&));

    MOCK_METHOD2(SetSectionPredecessor, void(const RoadLaneSectionInterface&, const RoadLaneSectionInterface&));

    MOCK_METHOD4(ConnectLanes, void(/* const */ RoadLaneSectionInterface&,
                 /* const */ RoadLaneSectionInterface&,
                 const std::map<int, int>&,
                 bool));

    MOCK_METHOD0(AddTrafficSign, OWL::Interfaces::TrafficSign&());

    MOCK_CONST_METHOD0(GetTrafficSigns, const std::unordered_map<OWL::Id, OWL::Interfaces::TrafficSign*>& ());

    MOCK_METHOD2(GetSensorView, osi3::SensorView(osi3::SensorViewConfiguration&, int));

    const OWL::Implementation::InvalidLane& GetInvalidLane() const override
    {
        return invalidLane;
    }

    const OWL::Implementation::InvalidLane invalidLane;
};
