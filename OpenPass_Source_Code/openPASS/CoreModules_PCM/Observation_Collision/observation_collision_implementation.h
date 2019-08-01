/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*               2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#ifndef OBSERVATION_COLLISION_IMPLEMENTATION_H
#define OBSERVATION_COLLISION_IMPLEMENTATION_H

#include <string>
#include <tuple>
#include <QFile>
#include <QTextStream>
#include <QXmlStreamWriter>
#include "componentPorts.h"
#include "observationInterface.h"

/**
* \addtogroup CoreModules_PCM openPASS CoreModules pcm
* @{
* \addtogroup Observation_Collision
*
* \brief Observation module to set the end condition after a collision.
*
* This module forces the simulation the end if a collision occured and all agents have come
* to a specific end velocity which is specified in the parameter #endVelocity.
*
* @see ObservationInterface
* @}
*/

/*!
 * \copydoc Observation_Collision
 * \ingroup Observation_Collision
 */
class Observation_Collision_Implementation : public ObservationInterface
{
public:
    const std::string COMPONENTNAME = "Observation_Collision";

    Observation_Collision_Implementation(StochasticsInterface *stochastics,
                                         WorldInterface *world,
                                         const ParameterInterface *parameters,
                                         const CallbackInterface *callbacks);
    Observation_Collision_Implementation(const Observation_Collision_Implementation &) = delete;
    Observation_Collision_Implementation(Observation_Collision_Implementation &&) = delete;
    Observation_Collision_Implementation &operator=(const Observation_Collision_Implementation &) =
        delete;
    Observation_Collision_Implementation &operator=(Observation_Collision_Implementation &&) = delete;
    virtual ~Observation_Collision_Implementation() = default;

    //-----------------------------------------------------------------------------
    //! Called by framework in master before each simulation run starts
    //-----------------------------------------------------------------------------
    virtual void MasterPreHook() {}

    //-----------------------------------------------------------------------------
    //! Called by framework in master after each simulation run ends
    //!
    //! @param[in]     filename      Name of file containing the simulation run results from the slave
    //-----------------------------------------------------------------------------
    virtual void MasterPostHook(const std::string &filename)
    {
        Q_UNUSED(filename)
    }

    virtual void SlavePreHook(const std::string &path);

    //-----------------------------------------------------------------------------
    //! Called by framework in slave before each simulation run starts.
    //-----------------------------------------------------------------------------
    virtual void SlavePreRunHook();

    //-----------------------------------------------------------------------------
    //! Called by framework in slave at each time step.
    //! Observation module can indicate end of simulation run here.
    //!
    //! @param[in]     time        Current scheduling time
    //! @param[in,out] runResult   Reference to run result
    //-----------------------------------------------------------------------------
    virtual void SlaveUpdateHook(int time, RunResultInterface &runResult);

    //-----------------------------------------------------------------------------
    //! Called by framework in slave after each simulation run ends.
    //! Observation module can observe the current simulation run here.
    //!
    //! @param[in]     runResult   Reference to run result
    //-----------------------------------------------------------------------------
    virtual void SlavePostRunHook(const RunResultInterface &runResult);

    //-----------------------------------------------------------------------------
    //! Called by framework in slave after all simulation runs end.
    //-----------------------------------------------------------------------------
    virtual void SlavePostHook();

    //-----------------------------------------------------------------------------
    //! Called by framework in slave after all simulation runs end to transfer the
    //! observation module results to the master.
    //!
    //! @return                      File to be transferred
    //-----------------------------------------------------------------------------
    virtual const std::string SlaveResultFile()
    {
        return "";   //dummy
    }

private:
    std::map<int, externalParameter<double>*> parameterMapDouble;
    /** \addtogroup Observation_Collision
     *  @{
     *      \name External Parameter
     *      Parameter which are set externally in runConfiguration file.
     *      @{
     */
    externalParameter<double> endVelocity {0, &parameterMapDouble };  //!< maximal velocity of an agent to end simulation after a collision occured
    /**
     *      @}
     *  @}
     */

    bool collisionOccured = false; //!< flag to indicate if a collision already occured
};

#endif // OBSERVATION_COLLISION_IMPLEMENTATION_H
