/*********************************************************************
* Copyright (c) 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "pcm_participantData.h"
#include "globalDefinitions.h"

PCM_ParticipantData::PCM_ParticipantData(QString inTypeCode,
                                         QString inWidth,
                                         QString inLength,
                                         QString inDistcgfa,
                                         QString inWeight,
                                         QString inHeightcg,
                                         QString inWheelbase,
                                         QString inIxx,
                                         QString inIyy,
                                         QString inIzz,
                                         QString inMue,
                                         QString inTrackwidth,
                                         QString inHeight,
                                         QString inCgfront) :
    width(inWidth),
    length(inLength),
    distcgfa(inDistcgfa),
    weight(inWeight),
    heightcg(inHeightcg),
    wheelbase(inWheelbase),
    ixx(inIxx),
    iyy(inIyy),
    izz(inIzz),
    mue(inMue),
    trackwidth(inTrackwidth),
    height(inHeight),
    cgfront(inCgfront)
{
    type = QString::fromStdString(GetAgentVehicleTypeStr(inTypeCode.toStdString()));
}

PCM_ParticipantData::PCM_ParticipantData(QString typeName) : type(typeName) {}

QString PCM_ParticipantData::GetType() const
{
    return type;
}

QString PCM_ParticipantData::GetWidth() const
{
    return width;
}

QString PCM_ParticipantData::GetLength() const
{
    return length;
}

QString PCM_ParticipantData::GetDistcgfa() const
{
    return distcgfa;
}

QString PCM_ParticipantData::GetWeight() const
{
    return weight;
}

QString PCM_ParticipantData::GetHeightcg() const
{
    return heightcg;
}

QString PCM_ParticipantData::GetWheelbase() const
{
    return wheelbase;
}

QString PCM_ParticipantData::GetIxx() const
{
    return ixx;
}

QString PCM_ParticipantData::GetIyy() const
{
    return iyy;
}

QString PCM_ParticipantData::GetIzz() const
{
    return izz;
}

QString PCM_ParticipantData::GetMue() const
{
    return mue;
}

QString PCM_ParticipantData::GetTrackwidth() const
{
    return trackwidth;
}

QString PCM_ParticipantData::GetHeight() const
{
    return height;
}

QString PCM_ParticipantData::GetCgfront() const
{
    return cgfront;
}

