#include "pcm_participantData.h"

PCM_ParticipantData::PCM_ParticipantData(QString inTypepcm,
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
    type = (inTypepcm == "0") ? "Car" :
           (inTypepcm == "1") ? "Pedestrian" :
           (inTypepcm == "2") ? "Motorbike" :
           (inTypepcm == "3") ? "Bicycle" :
           (inTypepcm == "4") ? "Truck" : "unknown type";
}

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

