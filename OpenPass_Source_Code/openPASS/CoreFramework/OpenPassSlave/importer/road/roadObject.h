/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*               2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#pragma once

#include "Interfaces/roadInterface/roadInterface.h"
#include "Interfaces/roadInterface/roadObjectInterface.h"

class RoadObject : public RoadObjectInterface
{
public:
    RoadObject(RoadInterface* road, const RoadObjectSpecification object) :
        road{road},
        object{object}
    {}

    //-----------------------------------------------------------------------------
    //! @brief Returns the unique identification string of the road object
    //! @return                     id
    //-----------------------------------------------------------------------------
    std::string GetId() const;

    //-----------------------------------------------------------------------------
    //! @brief Returns the s coordinate of the road object
    //! @return                     s coordinate [m]
    //-----------------------------------------------------------------------------
    double GetS() const;

    //-----------------------------------------------------------------------------
    //! @brief Returns the t coordinate of the road object
    //! @return                     t coordinate [m]
    //-----------------------------------------------------------------------------
    double GetT() const;

    //-----------------------------------------------------------------------------
    //! @brief Returns the heading of the road object (relative to road direction)
    //! @return                     heading [rad]
    //-----------------------------------------------------------------------------
    double GetHdg() const;

    virtual double GetHeight() const;
    virtual double GetPitch() const;
    virtual double GetRoll() const;

    //-----------------------------------------------------------------------------
    //! @brief Check, if road object is valid for provided lane id
    //!
    //! @param[in]  laneId          lane id to test validity for
    //!
    //! @return                     True if valid, false otherwise.
    //-----------------------------------------------------------------------------
    bool IsValidForLane(int laneId) const;

    //-----------------------------------------------------------------------------
    //! @brief Returns the type of the road object
    //! @return                     type
    //-----------------------------------------------------------------------------
    RoadObjectType GetType() const;

    //-----------------------------------------------------------------------------
    //! @brief Returns the length of the road object
    //! @return                     length [m]
    //-----------------------------------------------------------------------------
    double GetLength() const;

    //-----------------------------------------------------------------------------
    //! @brief Returns the width of the road object
    //! @return                     width [m]
    //-----------------------------------------------------------------------------
    double GetWidth() const;

private:
    RoadInterface* road;
    const RoadObjectSpecification object;

public:
    RoadObject(const RoadObject&) = delete;
    RoadObject(RoadObject&&) = delete;
    RoadObject& operator=(const RoadObject&) = delete;
    RoadObject& operator=(RoadObject&&) = delete;
    virtual ~RoadObject() = default;
};
