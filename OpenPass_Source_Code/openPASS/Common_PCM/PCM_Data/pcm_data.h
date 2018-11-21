/*********************************************************************
* Copyright (c) 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

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
#include "pcm_agent.h"
#include "../../Interfaces/callbackInterface.h"

struct AgentDetection
{
    int egoId = -1;
    const PCM_Point *egoPoint = nullptr;
    double viewAngle = INFINITY;
    double range = INFINITY;

    int oppId = -1;
    PCM_Point oppPoint;
    PCM_LineSegment fullOppLineSegment;
    PCM_LineSegment subOppLineSegment;
    double distance = INFINITY;
};

class PCM_Data
{
public:
    //-----------------------------------------------------------------------------
    //! Default Constructor
    //-----------------------------------------------------------------------------
    PCM_Data() = default;
//    PCM_Data(const CallbackInterface *callbacks):callbacks(callbacks) {}

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
    //! Function to add a agent as rectangle to the data.
    //!
    //! @param[in]      id          id of agent
    //! @param[in]      xPos        x position of cog of agent
    //! @param[in]      yPos        y position of cog of agent
    //! @param[in]      yawAngle    yaw angle of agent
    //! @param[in]      width       width of agent
    //! @param[in]      height      height of agent
    //!
    //! @return         true for success
    //-----------------------------------------------------------------------------
    bool AddPCM_Agent(int id, double xPos, double yPos, double yawAngle, double width, double height);

    //-----------------------------------------------------------------------------
    //! Clear the agent data.
    //!
    //! @return
    //-----------------------------------------------------------------------------
    void ClearAgentData();

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
    //! @param[in]     viewAngle       viewAngle (INFINITY if no viewAngle
    //!                                is set in function)
    //! @param[in]     range           viewing range (INFINITY if no range is set in
    //!                                in function)
    //! @return                        MarkType of nearest LineSegment,
    //!                                MarkType::NONE if there are no marks
    //-----------------------------------------------------------------------------
    MarkType GetMarkTypeOfNearestLineSegment(const PCM_Point *point,
                                             double viewAngle = INFINITY,
                                             double range = INFINITY) const;

    //-----------------------------------------------------------------------------
    //! Retrieve the nearest agent.
    //!
    //! @param[in]     egoId           id of base agent to exclude from searching
    //! @param[in]     point           base point
    //! @param[in]     viewAngle       viewAngle (INFINITY if no viewAngle
    //!                                is set in function)
    //! @param[in]     range           viewing range (INFINITY if no range is set in
    //!                                in function)
    //! @return                        MarkType of nearest LineSegment,
    //!                                MarkType::NONE if there are no marks
    //-----------------------------------------------------------------------------
    AgentDetection GetNearestAgent(int egoId,
                                  const PCM_Point *point,
                                  double viewAngle = INFINITY,
                                  double range = INFINITY) const;

    //-----------------------------------------------------------------------------
    //! Retrieve the object type of the nearest LineSegment.
    //!
    //! @param[in]     point           base point
    //! @param[in]     viewAngle       viewAngle (INFINITY if no viewAngle
    //!                                is set in function)
    //! @param[in]     range           viewing range (INFINITY if no range is set in
    //!                                in function)
    //! @return                        ObjectType of nearest LineSegment,
    //!                                ObjectType::NONE if there are no objects
    //-----------------------------------------------------------------------------
    ObjectType GetObjectTypeOfNearestLineSegment(const PCM_Point *point,
                                                 double viewAngle = INFINITY,
                                                 double range = INFINITY) const;

    //-----------------------------------------------------------------------------
    //! Retrieve the nearest line segment(two points of a line) of a mark from a
    //! pcm point either in a specific viewAngle, within a range or none.
    //!
    //! @param[in]     markType        type of mark to look for (none - for any mark)
    //! @param[in]     point           base point
    //! @param[in]     viewAngle       viewAngle (INFINITY if no viewAngle
    //!                                is set in function)
    //! @param[in]     range           viewing range (INFINITY if no range is set in
    //!                                in function)
    //! @return                        pair of points which are the nearest,
    //!                                pair of nullptr if there are no points
    //-----------------------------------------------------------------------------
    PCM_LineSegment GetNearestLineSegmentOfMarks(MarkType markType, const PCM_Point *point,
                                                 double viewAngle = INFINITY,
                                                 double range = INFINITY) const;

    //-----------------------------------------------------------------------------
    //! Retrieve the nearest line segment(two points of a line) of an agent from a
    //! pcm point either in a specific viewAngle, within a range or none.
    //!
    //! @param[in]     agentId              id of agent as searching scope
    //! @param[in]     agentIdExclude       id of agent to exclude from searching
    //! @param[in]     point                base point
    //! @param[in]     viewAngle            viewAngle (INFINITY if no viewAngle
    //!                                     is set in function)
    //! @param[in]     calculateSubLine     whether to return a sub line segment based on the view range
    //! @param[in]     range                viewing range (INFINITY if no range is set in
    //!                                     in function)
    //! @return                             pair of points which are the nearest,
    //!                                     pair of nullptr if there are no points
    //-----------------------------------------------------------------------------
    PCM_LineSegment GetNearestLineSegmentOfAgents(int agentId, int agentIdExclude, const PCM_Point *point,
                                                  double viewAngle = INFINITY,
                                                  double range = INFINITY,
                                                  bool calculateSubLine = true) const;

    //-----------------------------------------------------------------------------
    //! Retrieve the nearest line segment(two points of a line) of a object from a
    //! pcm point either in a specific viewAngle, within a range or none.
    //!
    //! @param[in]     objectType      type of object to look for (none - for any object)
    //! @param[in]     point           base point
    //! @param[in]     viewAngle       viewAngle (INFINITY if no viewAngle
    //!                                is set in function)
    //! @param[in]     range           viewing range (INFINITY if no range is set in
    //!                                in function)
    //! @return                        pair of points which are the nearest,
    //!                                pair of nullptr if there are no points
    //-----------------------------------------------------------------------------
    PCM_LineSegment GetNearestLineSegmentOfObject(ObjectType objectType, const PCM_Point *point,
                                                  double viewAngle = INFINITY,
                                                  double range = INFINITY) const;

    //-----------------------------------------------------------------------------
    //! Retrieve the nearest point on a line segment of all lines of a marks from a
    //! pcm point either in a specific viewAngle, within a range or none.
    //!
    //! @param[in]     markType        type of mark to look for (none - for any mark)
    //! @param[in]     point           base point
    //! @param[in]     viewAngle       viewAngle (INFINITY if no viewAngle
    //!                                is set in function)
    //! @param[in]     range           viewing range (INFINITY if no range is set in
    //!                                in function)
    //! @return                        nearest point of all lines
    //-----------------------------------------------------------------------------
    PCM_Point GetNearestPointOfMarks(MarkType markType, const PCM_Point *point,
                                     double viewAngle = INFINITY,
                                     double range = INFINITY) const;

    //-----------------------------------------------------------------------------
    //! Retrieve the nearest point on a line segment of all lines of agents from a
    //! pcm point either in a specific viewAngle, within a range or none.
    //!
    //! @param[in]     agentId         id of agent as searching scope
    //! @param[in]     agentIdExclude  id of agent to exclude from searching
    //! @param[in]     point           base point
    //! @param[in]     viewAngle       viewAngle (INFINITY if no viewAngle
    //!                                is set in function)
    //! @param[in]     range           viewing range (INFINITY if no range is set in
    //!                                in function)
    //! @return                        nearest point of all lines
    //-----------------------------------------------------------------------------
    PCM_Point GetNearestPointOfAgents(int agentId, int agentIdExclude, const PCM_Point *point,
                                      double viewAngle = INFINITY,
                                      double range = INFINITY) const;

    //-----------------------------------------------------------------------------
    //! Retrieve the nearest point on a line segment of all lines from an object
    //! pcm point either in a specific viewAngle, within a range or none.
    //!
    //! @param[in]     objectType      type of object to look for (none - for any object)
    //! @param[in]     point           base point
    //! @param[in]     viewAngle       viewAngle (INFINITY if no viewAngle
    //!                                is set in function)
    //! @param[in]     range           viewing range (INFINITY if no range is set in
    //!                                in function)
    //! @return                        nearest point of all lines
    //-----------------------------------------------------------------------------
    PCM_Point GetNearestPointOfObject(ObjectType objectType, const PCM_Point *point,
                                      double viewAngle = INFINITY,
                                      double range = INFINITY) const;

    //-----------------------------------------------------------------------------
    //! Retrieve the object
    //! @return                        object
    //-----------------------------------------------------------------------------
    const PCM_Object *GetObject() const;

    //-----------------------------------------------------------------------------
    //! Retrieve the view object
    //! @return                        viewObject
    //-----------------------------------------------------------------------------
    const PCM_ViewObject *GetViewObject() const;

    //-----------------------------------------------------------------------------
    //! Function to set the callbacks
    //!
    //! @param[in]  callbacks   callbacks to be set
    //! @return                 true on success
    //-----------------------------------------------------------------------------
    bool SetCallbacks(const CallbackInterface *callbacks);

protected:
    //-----------------------------------------------------------------------------
    //! Provides callback to LOG() macro
    //!
    //! @param[in]     logLevel    Importance of log
    //! @param[in]     file        Name of file where log is called
    //! @param[in]     line        Line within file where log is called
    //! @param[in]     message     Message to log
    //-----------------------------------------------------------------------------
    void Log(CbkLogLevel logLevel,
             const char *file,
             int line,
             const std::string &message) const
    {
        if (callbacks)
        {
            callbacks->Log(logLevel,
                           file,
                           line,
                           message);
        }
    }

private:
    std::vector<const PCM_Marks *> marksVec;         //!< vector of marks
    const PCM_Object *object = nullptr;              //!< pcm object
    const PCM_ViewObject *viewObject = nullptr;      //!< pcm viewObject
    std::vector<const PCM_Course *> intendedCourses; //!< vector of pcm courses
    const PCM_GlobalData *globalData = nullptr;      //!< pcm global data
    std::vector<const PCM_Agent *> agentVec;         //!< vector of agents

    const CallbackInterface *callbacks = nullptr;
};

#endif // PCM_DATA_H
