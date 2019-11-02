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

#include <vector>
#include <memory>

#include "LocalizationCommon.h"
#include "OWL/DataTypes.h"

namespace World {
namespace Localization {

constexpr double STEPBACKEPSILON = 1e-3;

class LaneWalkerInterface
{
public:
    /*!
     * \brief Set iterator to next LaneGeometryElement.
     */
    virtual void Step() = 0;

    /*!
     * \brief Sets the iterator back to a previous LaneGeometryElement near the given sOffset
     * or at most to the start.
     *
     * \param sOffset
     */
    virtual void Rewind(double sOffset) = 0;

    /*!
     * \brief Checks if iterator points to the end
     */
    virtual bool IsDepleted() const  = 0;

    /*!
     * \brief Retrieves the stored lane
     */
    virtual const OWL::Interfaces::Lane& GetLane() const  = 0;

    /*!
     * \brief Retrieves the current LaneGeometryElement
     */
    virtual const OWL::Primitive::LaneGeometryElement& GetLaneGeometryElement() const  = 0;

    /*!
     * \brief Return s-offset of current LaneGeometryElement
     */
    virtual double GetSOffset() const  = 0;

    /*!
     * \brief Clone current Walker by creating a new unique pointer.
     */
    virtual std::unique_ptr<LaneWalkerInterface> Clone() const = 0;

    virtual ~LaneWalkerInterface() = default;
};

class ForwardLaneWalker : public LaneWalkerInterface
{
private:
    const OWL::Interfaces::Lane* lane;

    OWL::Interfaces::LaneGeometryElements::const_iterator element;
    const OWL::Interfaces::LaneGeometryElements::const_iterator startOfElements;
    OWL::Interfaces::LaneGeometryElements::const_iterator endOfElements;

public:
    ForwardLaneWalker(const OWL::Interfaces::Lane& lane, bool startAtRoadStart);

    // copy and assignment
    ForwardLaneWalker(const ForwardLaneWalker& other) = default;
    ForwardLaneWalker& operator=(const ForwardLaneWalker& other) = default;

    virtual void Step() override;
    virtual void Rewind(double sOffset) override;
    virtual bool IsDepleted() const override;

    virtual const OWL::Interfaces::Lane& GetLane() const override;
    virtual const OWL::Primitive::LaneGeometryElement& GetLaneGeometryElement() const override;
    virtual double GetSOffset() const override;

    virtual std::unique_ptr<LaneWalkerInterface> Clone() const override
    {
        return make_unique<ForwardLaneWalker>(*this);
    }
};


class ReverseLaneWalker : public LaneWalkerInterface
{
private:
    const OWL::Interfaces::Lane* lane;

    OWL::Interfaces::LaneGeometryElements::const_reverse_iterator element;
    const OWL::Interfaces::LaneGeometryElements::const_reverse_iterator startOfElements;
    OWL::Interfaces::LaneGeometryElements::const_reverse_iterator endOfElements;

public:
    ReverseLaneWalker(const OWL::Interfaces::Lane& lane, bool startAtRoadStart);

    // copy and assignment
    ReverseLaneWalker(const ReverseLaneWalker& other) = default;
    ReverseLaneWalker& operator=(const ReverseLaneWalker& other) = default;

    virtual void Step() override;
    virtual void Rewind(double sOffset) override;
    virtual bool IsDepleted() const override;

    virtual const OWL::Interfaces::Lane& GetLane() const override;
    virtual const OWL::Primitive::LaneGeometryElement& GetLaneGeometryElement() const override;
    virtual double GetSOffset() const override;

    virtual std::unique_ptr<LaneWalkerInterface> Clone() const override
    {
        return make_unique<ReverseLaneWalker>(*this);
    }
};


class LaneWalker
{
private:
    std::unique_ptr<LaneWalkerInterface> walker;

public:
    /*!
     * \brief Creates either a ForwardLaneWalker or a ReverseLaneWalker
     * depending on the direction of the lane and the flag walkInStreamDirection
     *
     * \param lane                     lane to iterate over
     * \param walkInStreamDirection    specifies wether the LaneWalker should iterate in stream direction
     */
    LaneWalker(const OWL::Interfaces::Lane& lane, bool walkInStreamDirection, bool startAtRoadStart)
    {
        if (lane.IsInStreamDirection() ^ walkInStreamDirection)
        {
            walker = make_unique<ReverseLaneWalker>(lane, startAtRoadStart);
        }
        else
        {
            walker = make_unique<ForwardLaneWalker>(lane, startAtRoadStart);
        }
    }

    LaneWalker(const LaneWalker& other) :
        walker { other.walker->Clone() }
    {}

    LaneWalker& operator=(const LaneWalker& other)
    {
        if (&other == this)
        {
            return *this;
        }

        walker = other.walker->Clone();
        return *this;
    }

    void Step()
    {
        walker->Step();
    }

    void Rewind(double sOffset)
    {
        walker->Rewind(sOffset);
    }

    bool IsDepleted() const
    {
        return walker->IsDepleted();
    }

    const OWL::Interfaces::Lane& GetLane() const
    {
        return walker->GetLane();
    }

    const OWL::Primitive::LaneGeometryElement& GetLaneGeometryElement() const
    {
        return walker->GetLaneGeometryElement();
    }

    double GetSOffset() const
    {
        return walker->GetSOffset();
    }
};

using LaneWalkers = std::vector<LaneWalker>;

} // namespace Localization
} // namespace World
