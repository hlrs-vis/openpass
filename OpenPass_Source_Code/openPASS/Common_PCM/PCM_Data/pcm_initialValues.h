#ifndef PCM_INITIALVALUES_H
#define PCM_INITIALVALUES_H

#include <QString>

class PCM_InitialValues
{
public:
    //-----------------------------------------------------------------------------
    //! Constructor
    //!
    //! @param[in]     inXpos   initial x-coordinate
    //! @param[in]     inYpos   initial y-coordinate
    //! @param[in]     inVx     initial lateral velocity
    //! @param[in]     inVy     initial vertical velocity
    //! @param[in]     inAx     initial lateral acceleration
    //! @param[in]     inAy     initial vertical acceleration
    //! @param[in]     inPsi    initial yaw angle
    //-----------------------------------------------------------------------------
    PCM_InitialValues(QString inXpos,
                      QString inYpos,
                      QString inVx,
                      QString inVy,
                      QString inAx,
                      QString inAy,
                      QString inPsi);

    virtual ~PCM_InitialValues() = default;

    QString GetXpos() const;
    QString GetYpos() const;
    QString GetVx() const;
    QString GetVy() const;
    QString GetAx() const;
    QString GetAy() const;
    QString GetPsi() const;

private:
    QString xpos;   //!< initial x-coordinate
    QString ypos;   //!< initial y-coordinate
    QString vx;     //!< initial lateral velocity
    QString vy;     //!< initial vertical velocity
    QString ax;     //!< initial lateral acceleration
    QString ay;     //!< initial vertical acceleration
    QString psi;    //!< initial yaw angle
};

#endif // PCM_INITIALVALUES_H
