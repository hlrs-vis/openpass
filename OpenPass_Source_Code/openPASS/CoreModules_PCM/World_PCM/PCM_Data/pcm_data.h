/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  pcm_data.h
//! @brief class to store all data of a pcm file
//!
//! This class is responsible to manage data of a pcm file.
//-----------------------------------------------------------------------------

#ifndef PCM_DATA_H
#define PCM_DATA_H

#include <vector>
#include "pcm_marks.h"
#include "pcm_object.h"
#include "pcm_viewObject.h"
#include "pcm_course.h"
#include "pcm_globalData.h"

class PCM_Data
{
public:
    //-----------------------------------------------------------------------------
    //! Default Constructor
    //-----------------------------------------------------------------------------
    PCM_Data() = default;

    // removing operators
    PCM_Data(const PCM_Data &) = delete;
    PCM_Data(PCM_Data &&) = delete;
    PCM_Data &operator=(const PCM_Data &) = delete;
    PCM_Data &operator=(PCM_Data &&) = delete;

    //-----------------------------------------------------------------------------
    //! Destructor
    //-----------------------------------------------------------------------------
    ~PCM_Data();

    //-----------------------------------------------------------------------------
    //! Function to add a marks to the data
    //!
    //! @param[in]     marks    PCM_Marks to add
    //! @return                 true on success
    //-----------------------------------------------------------------------------
    bool AddPCM_Marks(const PCM_Marks *marks);

    //-----------------------------------------------------------------------------
    //! Function to set the pcm object
    //!
    //! @param[in]     object   PCM_Object to be set
    //! @return                 true on success
    //-----------------------------------------------------------------------------
    bool SetPCM_Object(const PCM_Object *object);

    //-----------------------------------------------------------------------------
    //! Function to set the pcm viewObject
    //!
    //! @param[in]     viewObject   PCM_ViewObject to be set
    //! @return                     true on success
    //-----------------------------------------------------------------------------
    bool SetPCM_ViewObject(const PCM_ViewObject *viewObject);

    //-----------------------------------------------------------------------------
    //! Function to set the pcm global data
    //!
    //! @param[in]     globalData   PCM_GlobalData to be set
    //! @return                     true on success
    //-----------------------------------------------------------------------------
    bool SetPCM_GlobalData(const PCM_GlobalData *globalData);

    //-----------------------------------------------------------------------------
    //! Function to add a course to the data
    //!
    //! @param[in]     course   PCM_Course to add
    //! @return                 true on success
    //-----------------------------------------------------------------------------
    bool AddPCM_Course(const PCM_Course *course);

    //-----------------------------------------------------------------------------
    //! Get the vector of marks
    //!
    //! @return                 vector of points
    //-----------------------------------------------------------------------------
    std::vector<const PCM_Marks *> *GetMarksVec();

    //-----------------------------------------------------------------------------
    //! Get the PCM_Marks of a specific type.
    //!
    //! @return                 nullptr if not found
    //-----------------------------------------------------------------------------
    const PCM_Marks *GetMarksOfType(MarkType markType) const;

    //-----------------------------------------------------------------------------
    //! Clear all data.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    void Clear();

    //-----------------------------------------------------------------------------
    //! Retrieve the mark type of the nearest LineSegment.
    //!
    //! @param[in]     point           base point
    //! @param[in]     viewDirection   viewDirection (INFINITY if no viewDirection
    //!                                is set in function)
    //! @param[in]     range           viewing range (INFINITY if no range is set in
    //!                                in function)
    //! @return                        MarkType of nearest LineSegment,
    //!                                MarkType::NONE if there are no marks
    //-----------------------------------------------------------------------------
    MarkType GetTypeOfNearestLineSegment(const PCM_Point *point,
                                         double viewDirection = INFINITY,
                                         double range = INFINITY) const;

    //-----------------------------------------------------------------------------
    //! Retrieve the nearest line segment(two points of a line) from a pcm point
    //! either in a specific viewDirection, within a range or none.
    //!
    //! @param[in]     markType        type of mark to look for (none - for any mark)
    //! @param[in]     point           base point
    //! @param[in]     viewDirection   viewDirection (INFINITY if no viewDirection
    //!                                is set in function)
    //! @param[in]     range           viewing range (INFINITY if no range is set in
    //!                                in function)
    //! @return                        pair of points which are the nearest,
    //!                                pair of nullptr if there are no points
    //-----------------------------------------------------------------------------
    PCM_LineSegment GetNearestLineSegment(MarkType markType, const PCM_Point *point,
                                          double viewDirection = INFINITY,
                                          double range = INFINITY) const;

    //-----------------------------------------------------------------------------
    //! Retrieve the nearest point on a line segment of all lines from a
    //! pcm point either in a specific viewDirection, within a range or none.
    //!
    //! @param[in]     markType        type of mark to look for (none - for any mark)
    //! @param[in]     point           base point
    //! @param[in]     viewDirection   viewDirection (INFINITY if no viewDirection
    //!                                is set in function)
    //! @param[in]     range           viewing range (INFINITY if no range is set in
    //!                                in function)
    //! @return                        nearest point of all lines
    //-----------------------------------------------------------------------------
    PCM_Point GetNearestPoint(MarkType markType, const PCM_Point *point,
                              double viewDirection = INFINITY,
                              double range = INFINITY) const;

private:
    std::vector<const PCM_Marks *> marksVec;         //!< vector of marks
    const PCM_Object *object = nullptr;              //!< pcm object
    const PCM_ViewObject *viewObject = nullptr;      //!< pcm viewObject
    std::vector<const PCM_Course *> intendedCourses; //!< vector of pcm courses
    const PCM_GlobalData *globalData = nullptr;      //!< pcm global data
};

#endif // PCM_DATA_H
