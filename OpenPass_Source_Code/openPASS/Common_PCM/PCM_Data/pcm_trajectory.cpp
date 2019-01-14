/*********************************************************************
* Copyright (c) 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "pcm_trajectory.h"

int PCM_Trajectory::GetEndTime()
{
    return timeVec->back();
}

const std::vector<int> *PCM_Trajectory::GetTimeVec() const
{
    return timeVec;
}

const std::vector<double> *PCM_Trajectory::GetXPosVec() const
{
    return xPosVec;
}

const std::vector<double> *PCM_Trajectory::GetYPosVec() const
{
    return yPosVec;
}

const std::vector<double> *PCM_Trajectory::GetUVelVec() const
{
    return uVelVec;
}

const std::vector<double> *PCM_Trajectory::GetVVelVec() const
{
    return vVelVec;
}

const std::vector<double> *PCM_Trajectory::GetPsiVec() const
{
    return psiVec;
}


QString PCM_Trajectory::GetTimeVecString() const
{
    return VecIntToCsv(timeVec);
}

QString PCM_Trajectory::GetXPosVecString() const
{
    return VecDoubleToCsv(xPosVec);
}

QString PCM_Trajectory::GetYPosVecString() const
{
    return VecDoubleToCsv(yPosVec);
}

QString PCM_Trajectory::GetUVelVecString() const
{
    return VecDoubleToCsv(uVelVec);
}

QString PCM_Trajectory::GetVVelVecString() const
{
    return VecDoubleToCsv(vVelVec);
}

QString PCM_Trajectory::GetPsiVecString() const
{
    return VecDoubleToCsv(psiVec);
}

QString PCM_Trajectory::VecIntToCsv(const std::vector<int> *inVec) const
{
    QString outString = "";

//    for (uint index = 0; index < inVec->size() - 1; index++)
//    {
//        outString += (QString::number(inVec->at(index), 'g', 10) + ", ");
//    }

//    outString += QString::number(inVec->at(inVec->size() - 1), 'g', 10);



    if (!inVec->empty())
    {
        std::ostringstream oss;

        // Convert all but the last element to avoid a trailing ","
        std::copy(inVec->begin(), inVec->end()-1, std::ostream_iterator<int>(oss, ", "));

        // Now add the last element with no delimiter
        oss << inVec->back();
        outString = QString::fromStdString(oss.str());
    }


    return outString;
}

QString PCM_Trajectory::VecDoubleToCsv(const std::vector<double> *inVec) const
{
    QString outString = "";

    for (uint index = 0; index < inVec->size() - 1; index++)
    {
        outString += (QString::number(inVec->at(index), 'g', 7) + ", ");  // output precision up to 10 digits
    }

    outString += QString::number(inVec->at(inVec->size() - 1), 'g', 7);


//    if (!inVec->empty())
//    {
//        std::ostringstream oss;

//        // Convert all but the last element to avoid a trailing ","
//        std::copy(inVec->begin(), inVec->end()-1, std::ostream_iterator<double>(oss, ", "));

//        // Now add the last element with no delimiter
//        oss << inVec->back();
//        outString = QString::fromStdString(oss.str());
//    }

    return outString;
}

void PCM_Trajectory::Clear()
{
    if (timeVec != nullptr)
    {
        delete timeVec ;
        timeVec = nullptr;
    }
    if (xPosVec != nullptr)
    {
        delete xPosVec ;
        xPosVec = nullptr;
    }
    if (yPosVec != nullptr)
    {
        delete yPosVec ;
        yPosVec = nullptr;
    }
    if (uVelVec != nullptr)
    {
        delete uVelVec ;
        uVelVec = nullptr;
    }
    if (vVelVec != nullptr)
    {
        delete vVelVec ;
        vVelVec = nullptr;
    }
    if (psiVec != nullptr)
    {
        delete psiVec ;
        psiVec = nullptr;
    }
}
