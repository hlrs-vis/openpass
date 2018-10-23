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
