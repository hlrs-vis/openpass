/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef SELECTOR_H
#define SELECTOR_H

#include <vector>

//! Selector class to select an Output based on a given Input
class Selector
{
public:
    Selector() = default;
    Selector(const Selector &) = delete;
    Selector(Selector &&) = delete;
    Selector &operator=(const Selector &) = delete;
    Selector &operator=(Selector &&) = delete;

    virtual ~Selector() = default;

    //!< Perform a time step with a given input
    void Perform(double driverThrottle,
                 double driverBrake,
                 double driverSteering,
                 bool brakeAssistActive,
                 double brakeAssistBrake,
                 bool laneAssistActive,
                 std::vector<double> laneAssistBrakeSuperpose,
                 bool evasiveSteeringActive,
                 double evasiveSteeringAngle,
                 bool collisionOccured);

    double GetResultingThrottle() const;
    double GetResultingBrake() const;
    double GetResultingSteering() const;
    std::vector<double> GetResultingBrakeSuperpose() const;

private:
    bool collisionState = false; //!< Check if a collision already occured
    double collisionSteering = 0.0; //!< steering origin in collision moment
    bool assistantActivation = false; //! Check if any system was active before

    double resultingThrottle = 0.0; //!< resulting throttle output
    double resultingBrake = 0.0; //!< resulting brake output
    double resultingSteering = 0.0; //!< resulting steering output
    std::vector<double> resultingBrakeSuperpose = {0.0, 0.0, 0.0, 0.0}; //!< resulting brake superpose output

};

#endif // SELECTOR_H
