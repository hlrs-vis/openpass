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

#include "gmock/gmock.h"
#include "OWL/DataTypes.h"

class FakeTrafficSign : public OWL::Interfaces::TrafficSign
{
public:
    MOCK_CONST_METHOD0(GetId,
                       std::string());
	MOCK_CONST_METHOD0(GetS,
                       double());
	MOCK_CONST_METHOD0(GetValue,
                       double());					   
	MOCK_CONST_METHOD0(GetType,
                       CommonTrafficSign::Type());					   
  
    MOCK_METHOD1(SetS,
                 void(double sPos));
    MOCK_METHOD1(SetValue,
                 void(double value));
    MOCK_METHOD1(SetValidForLane,
                 void(OWL::Id laneId));
    MOCK_METHOD1(SetType,
                 void(RoadSignalType type));	

    MOCK_CONST_METHOD1(IsValidForLane,
                 bool(OWL::Id laneId));				 

    MOCK_CONST_METHOD1(CopyToGroundTruth, void(osi3::GroundTruth&));
};
