/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* Copyright (c) 2018 in-tech GmbH
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  roadLaneInterface.h
//! @brief This file contains a interface representing a road lane.
//-----------------------------------------------------------------------------

#pragma once

#include "roadInterface.h"
#include "roadLaneInterface.h"
#include "roadLaneSectionInterface.h"
#include "roadLaneRoadMark.h"
//-----------------------------------------------------------------------------
//! Class representing a road lane.
//-----------------------------------------------------------------------------
class RoadLaneWidth;

class RoadLaneInterface
{
public:
    RoadLaneInterface() = default;
    RoadLaneInterface(const RoadLaneInterface&) = delete;
    RoadLaneInterface(RoadLaneInterface&&) = delete;
    RoadLaneInterface& operator=(const RoadLaneInterface&) = delete;
    RoadLaneInterface& operator=(RoadLaneInterface&&) = delete;
    virtual ~RoadLaneInterface() = default;

    //-----------------------------------------------------------------------------
    //! Adds a new polynomial calculating the width of a lane to a road lane.
    //!
    //! @param[in]  sOffset             Offset relative to the preceding lane section
    //! @param[in]  a                   Constant factor from the polynomial
    //! @param[in]  b                   Linear factor from the polynomial
    //! @param[in]  c                   Quadratic factor from the polynomial
    //! @param[in]  d                   Cubic factor from the polynomial
    //!
    //! @return                         False if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    virtual bool AddWidth(double sOffset,
                  double a,
                  double b,
                  double c,
                  double d) = 0;

    //-----------------------------------------------------------------------------
    //! Adds a new roadmark to a road lane.
    //!
    //! @param[in]  sOffset             Offset relative to the preceding lane section
    //! @param[in]  roadMark            RoadMark
    //!
    //! @return                         False if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    virtual bool AddRoadMark(double sOffset,
                             RoadLaneRoadDescriptionType descType,
                             RoadLaneRoadMarkType roadMark,
                             RoadLaneRoadMarkColor color,
                             RoadLaneRoadMarkLaneChange laneChange) = 0;

    //-----------------------------------------------------------------------------
    //! Adds the ID of a successor lane to a road lane.
    //!
    //! @param[in]  id                  ID of a successor lane
    //!
    //! @return                         False if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    virtual bool AddSuccessor(int id) = 0;

    //-----------------------------------------------------------------------------
    //! Adds the ID of a predecessor lane to a road lane.
    //!
    //! @param[in]  id                  ID of a predecessor lane
    //!
    //! @return                         False if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    virtual bool AddPredecessor(int id) = 0;

    //-----------------------------------------------------------------------------
    //! Returns the ID of the road lane.
    //!
    //! @return                         ID of the road lane
    //-----------------------------------------------------------------------------
    virtual int GetId() const = 0;

    //-----------------------------------------------------------------------------
    //! Returns the RoadLaneTypeType of the road lane.
    //!
    //! @return                         RoadLaneTypeType of the road lane
    //-----------------------------------------------------------------------------
    virtual RoadLaneTypeType GetType() const = 0;

    //-----------------------------------------------------------------------------
    //! Returns the widths of the road lane.
    //!
    //! @return                         RoadLaneTypeType of the road lane
    //-----------------------------------------------------------------------------
    virtual std::list< RoadLaneWidth*> &GetWidths() = 0;

    //-----------------------------------------------------------------------------
    //! Returns the successors of a road lane, can have at most one element.
    //!
    //! @return                         Successors of the road lane
    //-----------------------------------------------------------------------------
    virtual const std::list<int> &GetSuccessor() const = 0;

    //-----------------------------------------------------------------------------
    //! Returns the predecessors of a road lane, can have at most one element.
    //!
    //! @return                         Predecessors of the road lane
    //-----------------------------------------------------------------------------
    virtual const std::list<int> &GetPredecessor() const = 0;

    //-----------------------------------------------------------------------------
    //! Sets the flag, if the road lane is in the reference direction or not.
    //!
    //! @param[in]  inDirection         Flag, if the road lane is in the reference
    //!                                 direction or not
    //-----------------------------------------------------------------------------
    virtual void SetInDirection(bool inDirection) = 0;

    //-----------------------------------------------------------------------------
    //! Returns if the road lane is in the reference direction or not.
    //!
    //! @return                         Flag, if the road lane is in the reference
    //!                                 direction or not
    //-----------------------------------------------------------------------------
    virtual bool GetInDirection() const = 0;

    //-----------------------------------------------------------------------------
    //! Returns the RoadLaneSection to which the road lane belongs.
    //!
    //! @return                         RoadLaneSection to which the road lane belongs
    //-----------------------------------------------------------------------------
    virtual RoadLaneSectionInterface *GetLaneSection() = 0;

    //! Returns the roadmarktype of the road lane.
    //!
    //! @return                         RoadLaneRoadMarkType
    //-----------------------------------------------------------------------------

    virtual std::list<RoadLaneRoadMark*> &getRoadMarks() = 0;

};


