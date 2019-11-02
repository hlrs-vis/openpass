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

class FakeSection : public OWL::Interfaces::Section
{
public:
    MOCK_CONST_METHOD0(GetId,
                       OWL::Id(void));
    MOCK_CONST_METHOD0(IsInStreamDirection,
                       bool());
    MOCK_METHOD0(UpdateStreamId,
                 void());
    MOCK_CONST_METHOD0(GetStreamId,
                       OWL::Id());
    MOCK_CONST_METHOD0(GetNext,
                       const OWL::Interfaces::Section * ());
    MOCK_CONST_METHOD0(GetPrevious,
                       const OWL::Interfaces::Section * ());
    MOCK_METHOD1(AddNext,
                 void(const OWL::Interfaces::Section& section));
    MOCK_METHOD1(AddPrevious,
                 void(const OWL::Interfaces::Section& section));
    MOCK_METHOD1(AddLane,
                 void (const OWL::Interfaces::Lane& lane));
    MOCK_CONST_METHOD0(GetLanes,
                       const OWL::Interfaces::Lanes & (void));
    MOCK_CONST_METHOD0(GetLength,
                       double (void));
    MOCK_CONST_METHOD0(GetSOffset,
                       double());
    MOCK_CONST_METHOD1(GetDistance,
                       double(OWL::MeasurementPoint mp));
    MOCK_CONST_METHOD1(Covers,
                       bool(double)); 
    MOCK_CONST_METHOD2(CoversInterval,
                       bool(double startDistance, double endDistance));
    MOCK_METHOD1(SetRoad,
                 void(OWL::Interfaces::Road*));
    MOCK_CONST_METHOD0(GetRoad,
                       const OWL::Interfaces::Road & (void));

};
