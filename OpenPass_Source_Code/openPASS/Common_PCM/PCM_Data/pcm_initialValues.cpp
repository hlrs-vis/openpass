/*********************************************************************
* Copyright (c) 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "pcm_initialValues.h"

PCM_InitialValues::PCM_InitialValues(QString inXpos,
                                     QString inYpos,
                                     QString inVx,
                                     QString inVy,
                                     QString inAx,
                                     QString inAy,
                                     QString inPsi):
    xpos(inXpos),
    ypos(inYpos),
    vx(inVx),
    vy(inVy),
    ax(inAx),
    ay(inAy),
    psi(inPsi)
{}

QString PCM_InitialValues::GetXpos() const
{
    return xpos;
}

QString PCM_InitialValues::GetYpos() const
{
    return ypos;
}

QString PCM_InitialValues::GetVx() const
{
    return vx;
}

QString PCM_InitialValues::GetVy() const
{
    return vy;
}

QString PCM_InitialValues::GetAx() const
{
    return ax;
}

QString PCM_InitialValues::GetAy() const
{
    return ay;
}

QString PCM_InitialValues::GetPsi() const
{
    return psi;
}

void PCM_InitialValues::SetXpos(const QString xpos)
{
    this->xpos = xpos;
}

void PCM_InitialValues::SetYpos(const QString ypos)
{
    this->ypos = ypos;
}

void PCM_InitialValues::SetVx(const QString vx)
{
    this->vx = vx;
}

void PCM_InitialValues::SetVy(const QString vy)
{
    this->vy = vy;
}
