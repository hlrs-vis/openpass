/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2016, 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  roadLaneSectionInterface.h
//! @brief This file contains a interface representing a road lane section that
//!        can contain several road lanes.
//-----------------------------------------------------------------------------

#ifndef ROADLANESECTIONINTERFACE
#define ROADLANESECTIONINTERFACE

#include "roadInterface.h"

//-----------------------------------------------------------------------------
//! Class representing a road lane section that can contain several road lanes.
//-----------------------------------------------------------------------------
class RoadLaneInterface;
class RoadInterface;

class RoadLaneSectionInterface
{
public:
    RoadLaneSectionInterface() = default;
    RoadLaneSectionInterface(const RoadLaneSectionInterface&) = delete;
    RoadLaneSectionInterface(RoadLaneSectionInterface&&) = delete;
    RoadLaneSectionInterface& operator=(const RoadLaneSectionInterface&) = delete;
    RoadLaneSectionInterface& operator=(RoadLaneSectionInterface&&) = delete;
    virtual ~RoadLaneSectionInterface() = default;


    //-----------------------------------------------------------------------------
    //! Adds and returns a road lane.
    //!
    //! @param[in]  id                  ID of the lane
    //! @param[in]  type                RoadLaneTypeType of the lane
    //!
    //! @return                         False if an error occurred, true otherwise
    //-----------------------------------------------------------------------------
    virtual RoadLaneInterface *AddRoadLane(int id,
                          RoadLaneType type) = 0;

    //-----------------------------------------------------------------------------
    //! Returns the stored road lanes as a mapping from their respective IDs.
    //!
    //! @return                         Stored road lanes as a mapping from their
    //!                                 respective IDs
    //-----------------------------------------------------------------------------
    virtual std::map<int, RoadLaneInterface*> &GetLanes() = 0;

    //-----------------------------------------------------------------------------
    //! Returns the starting offset of the road lane section.
    //!
    //! @return                         Starting offset of the road lane section
    //-----------------------------------------------------------------------------
    virtual double GetStart() const = 0;

    //-----------------------------------------------------------------------------
    //! Sets the flag, if the lane section is in the reference direction or not.
    //!
    //! @param[in]  inDirection         Flag, if the road lane is in the reference
    //!                                 direction or not
    //-----------------------------------------------------------------------------
    virtual void SetInDirection(bool inDirection) = 0;

    //-----------------------------------------------------------------------------
    //! Sets the index offset for the road lanes in this section.
    //!
    //! @param[in]  laneIndexOffset     Index offset for the road lanes in this section
    //-----------------------------------------------------------------------------
    virtual void SetLaneIndexOffset(int laneIndexOffset) = 0;

    //-----------------------------------------------------------------------------
    //! Sets the ID.
    //!
    //! @param[in] Id             ID
    //-----------------------------------------------------------------------------
    virtual void SetId(int Id) = 0;

    //-----------------------------------------------------------------------------
    //! Returns if the lane section is in the reference direction or not.
    //!
    //! @return                         Flag, if the lane section is in the reference
    //!                                 direction or not
    //-----------------------------------------------------------------------------
    virtual bool GetInDirection() const = 0;

    //-----------------------------------------------------------------------------
    //! Returns the index offset for the road lanes in this section.
    //!
    //! @return                         Index offset for the road lanes in this section
    //-----------------------------------------------------------------------------
    virtual int GetLaneIndexOffset() const = 0;

    //-----------------------------------------------------------------------------
    //! Returns the ID.
    //!
    //! @return                         ID
    //-----------------------------------------------------------------------------
    virtual int GetId() const = 0;

    //-----------------------------------------------------------------------------
    //! Returns the road from which this section is a part of.
    //!
    //! @return                         Road from which this section is a part of
    //-----------------------------------------------------------------------------
    virtual RoadInterface *GetRoad() = 0;
};

#endif // ROADLANESECTIONINTERFACE

