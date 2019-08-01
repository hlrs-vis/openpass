\page adas Advanced Driver Assistance Systems

\tableofcontents

\section dev_modules_adas_aeb Autonomous Emergency Braking (AEB)

The Autonomous Emergency Braking system checks if a collision is likely to occur in the near future and, if necessary, brakes to avoid the collision. In the ProfilesCatalog, there are several parameters (see the [corresponding section](\ref io_input_profilescatalog_adasprofiles_aeb) of the Input Output documentation):

* TTC (sec): TTC Threshold for the stage to activate
* Acceleration (m/s²): Maximum breaking acceleration for this the stage
* CollisionDetectionLongitudinalBoundary (m): Additional length added to the vehicle boundary when checking for collision detection
* CollisionDetectionLateralBoundary (m): Additional width added to the vehicle boundary when checking for collision detection

In each timestep, the system evaluates all objects detected by the SensorGeometric2D and calculates the time to collision (TTC) for this object based on the perceived movement of the object. If, for any object, the TTC is lower than the threshold of the component, then the component gets activated. The system deactivates if the TTC is larger than 1,5 times the threshold of the component.

---
