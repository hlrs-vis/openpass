/*********************************************************************
* Copyright (c) 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef PCM_HELPER_H
#define PCM_HELPER_H

#include "globalDefinitions.h"
#include "vector2d.h"
#include "pcm_point.h"

/*!
 * \brief The PCM_Helper class
 * This class is responsible to provide pcm related helper functions.
 */
class PCM_Helper
{
public:

    //! Retrieve a string of the name of the mark type.
    //!
    //! @param[in]     markType mark type to convert
    //! @return                 QString which is equivalent to the String in the
    //!                         database of this type of mark
    static std::string ConvertMarkTypeToDBString(MarkType markType)
    {
        std::string markTypeName = "";
        switch (markType)
        {
        case MarkType::CONTINUOUS:
            markTypeName = "marks_continuous";
            break;
        case MarkType::INTERRUPTED_LONG:
            markTypeName = "marks_interrupted_long";
            break;
        case MarkType::INTERRUPTED_SHORT:
            markTypeName = "marks_interrupted_short";
            break;
        case MarkType::ROADSIDE:
            markTypeName = "roadside";
            break;
        default:
            markTypeName = "";
            break;
        }

        return markTypeName;
    }

    //! Retrieve a MarktType equivalent to a string.
    //!
    //! @param[in]     markTypeString   String to convert
    //! @return                         MarkType which is equivalent to the string
    static MarkType ConvertStringToMarkType(std::string markTypeString)
    {
        if (markTypeString.compare("marks_continuous"))
        {
            return  MarkType::CONTINUOUS;
        }
        else if (markTypeString.compare("marks_interrupted_long"))
        {
            return  MarkType::INTERRUPTED_LONG;
        }
        else if (markTypeString.compare("marks_interrupted_short"))
        {
            return  MarkType::INTERRUPTED_SHORT;
        }
        else if (markTypeString.compare("roadside"))
        {
            return  MarkType::ROADSIDE;
        }
        else
        {
            return MarkType::NONE;
        }
    }

    //! Retrieve a string of the name of the object type.
    //!
    //! @param[in]     objectType mark type to convert
    //! @return                 QString which is equivalent to the string in the
    //!                         database of this type of objects
    static std::string ConvertObjectTypeToDBString(ObjectType objectType)
    {
        std::string objectTypeName = "";
        switch (objectType)
        {
        case ObjectType::OBJECT:
            objectTypeName = "objects";
            break;
        case ObjectType::VIEWOBJECT:
            objectTypeName = "view_object";
            break;
        default:
            objectTypeName = "";
            break;
        }

        return objectTypeName;
    }

    //! Retrieve a ObjectType equivalent to a string.
    //!
    //! @param[in]     objectTypeString   String to convert
    //! @return                           ObjectType which is equivalent to the string
    static ObjectType ConvertStringToObjectType(std::string objectTypeString)
    {
        if (objectTypeString.compare("objects"))
        {
            return  ObjectType::OBJECT;
        }
        else if (objectTypeString.compare("view_object"))
        {
            return  ObjectType::VIEWOBJECT;
        }
        else
        {
            return ObjectType::NONE;
        }
    }

    //! Retrieve a string of the name of the globalData in the database.
    static std::string GetGlobalDataDBString()
    {
        return "global_data";
    }

    //! Retrieve a string of the name of the intended_course in the database.
    static std::string GetIntendedCourseDBString()
    {
        return "intended_course";
    }

    //! Transform a pcm point to a vector.
    //!
    //! @param[in]    point     point
    //! @return                 vector
    static Common::Vector2d TransformPointToVector2d(const PCM_Point *point)
    {
        Common::Vector2d pointVec(point->GetX(), point->GetY());
        return pointVec;
    }

    //! Transform a vector to a pcm point.
    //!
    //! @param[in]    pointVec  point vector
    //! @return                 point
    static const PCM_Point TransformVector2dToPoint(Common::Vector2d pointVec)
    {
        const PCM_Point point(-1, pointVec.x, pointVec.y, INFINITY);
        return point;
    }

    //! Transform a pcm point to a vector in the coordination system of a
    //! source point in a direction.
    //!
    //! @param[in]    point     point
    //! @return                 vector
    static Common::Vector2d TransformPointToSourcePointCoord(const PCM_Point *point,
                                                             const PCM_Point *sourcePoint,
                                                             double direction)
    {
        Common::Vector2d pointVec = TransformPointToVector2d(point);
        Common::Vector2d sourcePointVec = TransformPointToVector2d(sourcePoint);

        pointVec.Translate(sourcePointVec * (-1));
        pointVec.Rotate(direction * (-1));

        return pointVec;
    }

    //! Transform a pcm point to a vector in the coordination system of a
    //! source point in a direction.
    //!
    //! @param[in]    point     point
    //! @return                 vector
    static PCM_Point TransformPointFromSourcePointCoord(Common::Vector2d pointVec,
                                                        const PCM_Point *sourcePoint,
                                                        double direction)
    {
        Common::Vector2d sourcePointVec = TransformPointToVector2d(sourcePoint);

        pointVec.Rotate(direction);
        pointVec.Translate(sourcePointVec);

        PCM_Point point(-1, pointVec.x, pointVec.y, 0);

        return point;
    }

    //! Calculate the distance between two pcm points.
    //!
    //! @param[in]    firstPoint     firstPoint
    //! @param[in]    secondPoint    secondPoint
    //! @return                 distance between two pcm points
    static double CalcDistanceBetweenPoints(const PCM_Point *firstPoint,
                                            const PCM_Point *secondPoint)
    {
        if ((!CheckPointValid(firstPoint)) || (!CheckPointValid(secondPoint)))
        {
            return INFINITY;
        }
        Common::Vector2d firstPointVec = PCM_Helper::TransformPointToVector2d(firstPoint);
        Common::Vector2d secondPointVec = PCM_Helper::TransformPointToVector2d(secondPoint);

        double distance = (secondPointVec - firstPointVec).Length();

        return distance;
    }

    //! Calculate the absolute angle between two pcm points.
    //!
    //! @param[in]    firstPoint     firstPoint
    //! @param[in]    secondPoint    secondPoint
    //! @return                 distance between two pcm points
    static double CalcAngleBetweenPoints(const PCM_Point *firstPoint,
                                         const PCM_Point *secondPoint)
    {
        if ((!CheckPointValid(firstPoint)) || (!CheckPointValid(secondPoint)))
        {
            return INFINITY;
        }

        Common::Vector2d firstPointVec = PCM_Helper::TransformPointToVector2d(firstPoint);
        Common::Vector2d secondPointVec = PCM_Helper::TransformPointToVector2d(secondPoint);

        double angle = (secondPointVec - firstPointVec).Angle();

        return angle;
    }

    //! Checks if point is valid.
    //!
    //! @return                     true if point is nullptr -->valid
    //!                             false if any point is nullptr --> not valid
    static bool CheckPointValid(const PCM_Point *point)
    {
        bool valid = true;
        if (point == nullptr)
        {
            valid = false;
        }
        else if ((std::isinf(point->GetX())) || (std::isinf(point->GetY())))
        {
            valid = false;
        }
        return valid;
    }
};

#endif // PCM_HELPER_H
