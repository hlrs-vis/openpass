/*********************************************************************
* Copyright (c) 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef PCM_PARTICIPANTDATA_H
#define PCM_PARTICIPANTDATA_H

#include <QString>

//-----------------------------------------------------------------------------
//! Class to store the information on the pcm participants
//-----------------------------------------------------------------------------
class PCM_ParticipantData
{
public:
    //-----------------------------------------------------------------------------
    //! Constructor
    //!
    //! @param[in]     inTypepcm        the type of the participant, pcm coded
    //! @param[in]     inWidth          the width of the participant
    //! @param[in]     inLength         the length of the participant
    //! @param[in]     inDistcgfa       the distance of the center of gravity (COG) to the front axle
    //! @param[in]     inWeight         the weight of the participant
    //! @param[in]     inHeightcg       the height of the COG of the participant
    //! @param[in]     inWheelbase      the wheelbase of the participant
    //! @param[in]     inIxx            the inertia for rotation around the x axis of the participant
    //! @param[in]     inIyy            the inertia for rotation around the y axis of the participant
    //! @param[in]     inIzz            the inertia for rotation around the z axis of the participant
    //! @param[in]     inMue            the friction coefficients that holds for the participant
    //! @param[in]     inTrackwidth     the trackwidth of the participant
    //! @param[in]     inHeight         the height of the participant
    //! @param[in]     inCgfront        the distance of the COG to the front of the participant
    //-----------------------------------------------------------------------------
    PCM_ParticipantData(QString inTypeCode,
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
                        QString inCgfront);

    PCM_ParticipantData(QString typeName);

    virtual ~PCM_ParticipantData() = default;

    QString GetType() const;
    QString GetWidth() const;
    QString GetLength() const;
    QString GetDistcgfa() const;
    QString GetWeight() const;
    QString GetHeightcg() const;
    QString GetWheelbase() const;
    QString GetIxx() const;
    QString GetIyy() const;
    QString GetIzz() const;
    QString GetMue() const;
    QString GetTrackwidth() const;
    QString GetHeight() const;
    QString GetCgfront() const;

private:
    QString type;       //!< the type of the participant, pcm coded
    QString width;      //!< the width of the participant
    QString length;     //!< the length of the participant
    QString distcgfa;   //!< the distance of the COG to the front axle
    QString weight;     //!< the weight of the participant
    QString heightcg;   //!< the height of the COG of the participant
    QString wheelbase;  //!< the wheelbase of the participant
    QString ixx;        //!< the inertia for rotation around the x axis of the participant
    QString iyy;        //!< the inertia for rotation around the y axis of the participant
    QString izz;        //!< the inertia for rotation around the z axis of the participant
    QString mue;        //!< the friction coefficients that holds for the participant
    QString trackwidth; //!< the trackwidth of the participant
    QString height;     //!< the height of the participant
    QString cgfront;    //!< the distance of the COG to the front of the participant
};

#endif // PCM_PARTICIPANTDATA_H
