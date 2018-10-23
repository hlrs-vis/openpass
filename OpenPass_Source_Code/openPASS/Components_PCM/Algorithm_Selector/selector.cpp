#include "selector.h"


void Selector::Perform(double driverThrottle, double driverBrake, double driverSteering,
                       bool brakeAssistActive, double brakeAssistBrake, bool laneAssistActive,
                       std::vector<double> laneAssistBrakeSuperpose, bool evasiveSteeringActive,
                       double evasiveSteeringAngle, bool collisionOccured)
{
    resultingThrottle = driverThrottle;
    resultingBrake = driverBrake;
    resultingSteering = driverSteering;
    resultingBrakeSuperpose = {0.0, 0.0, 0.0, 0.0};

    if (collisionState == false)
    {
        if (collisionOccured == true)
        {
            collisionState = true;
            collisionSteering = driverSteering;
        }
    }

    if (collisionState == true)
    {
        resultingThrottle = 0.0;
        resultingBrake = 1.0;
        resultingSteering = collisionSteering;
        resultingBrakeSuperpose = std::vector<double> {0.0, 0.0, 0.0, 0.0};
    }
    else if (evasiveSteeringActive == true)
    {
        resultingThrottle = 0.0;
        resultingBrake = 0.0;
        resultingSteering = evasiveSteeringAngle;
        resultingBrakeSuperpose = std::vector<double> {0.0, 0.0, 0.0, 0.0};
        assistantActivation = true;
    }
    else if (brakeAssistActive == true)
    {
        resultingThrottle = 0.0;
        resultingBrake = brakeAssistBrake;
        resultingSteering = driverSteering;
        resultingBrakeSuperpose = std::vector<double> {0.0, 0.0, 0.0, 0.0};
        assistantActivation = true;
    }
    else if (laneAssistActive == true)
    {
        resultingThrottle = 0.0;
        resultingBrake = 0.0;
        resultingSteering = 0.0;
        resultingBrakeSuperpose = laneAssistBrakeSuperpose;
        assistantActivation = true;
    }
    else if (assistantActivation == true)
    {
        resultingThrottle = 0.0;
        resultingBrake = 0.0;
        resultingSteering = 0.0;
        resultingBrakeSuperpose = std::vector<double> {0.0, 0.0, 0.0, 0.0};
    }
}

double Selector::GetResultingThrottle() const
{
    return resultingThrottle;
}

double Selector::GetResultingBrake() const
{
    return resultingBrake;
}

double Selector::GetResultingSteering() const
{
    return resultingSteering;
}

std::vector<double> Selector::GetResultingBrakeSuperpose() const
{
    return resultingBrakeSuperpose;
}
