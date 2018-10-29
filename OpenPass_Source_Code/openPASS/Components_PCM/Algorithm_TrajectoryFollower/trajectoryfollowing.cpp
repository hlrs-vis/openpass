/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "trajectoryfollowing.h"

using std::vector;

const int filterWindowSize = 3;             //!< number of recent values kept in the lowpass filter
const int numVelocityLookAheadPoints =
    3;   //!< if the driver currently has passed waypoint i, he uses the velocity at waypoint i + numVelocityLookAheadPoints as reference value for the PID-controller

//! Function calculating x/limit and cutting to the interval [0,1]
//!
//! @param[in]     x            numerator
//! @param[in]     limit        denominator
//! @return                     x/limit !\in[0,1]
inline double mapValueToUnitInterval(double x, double limit)
{
    return x <= 0 ? 0 : (x > limit ? 1 : x / limit);
}

//! Function limiting a value by two other inputs
//!
//! @param[in]     value        to be limited
//! @param[in]     min          lower limit
//! @param[in]     max          upper limit
//! @return                     value !\in [min,max]

inline double limitValue(double value, double min, double max)
{
    return value < min ? min : (value < max ? value : max);
}

//! Function calculating the all distances between consecutive way points
//!
//! @return                         vector of distances
vector<double> TrajectoryFollowingControl::computeDistancesBetweenConsecutiveWayPoints()
{
    if (numWayPoints_ <= 1)
    {
        return vector<double>(0);
    }
    vector<double> distances(numWayPoints_ - 1);
    Common::Vector2d u;
    for (int i = 0; i < numWayPoints_ - 1; ++i)
    {
        u = waypoints_[i + 1].position - waypoints_[i].position;
        distances[i] = u.Length();
    }
    return distances;
}

//! Function calculating how many indices the look ahead time corresponds to
//!
//! @param[in]     totalDistance        distance that is looked ahead (v*t)
//! @param[in]     distances            vector of distances between consecutive way points
//! @param[in]     fromWayPointIndex    index of the 'starting' waypoint
//! @return                             index surplus
int TrajectoryFollowingControl::computeNumberOfLookaheadPoints(double totalDistance,
                                                               const vector<double> &distances,
                                                               int fromWayPointIndex)
{
    int n = distances.size();
    if (totalDistance <= 0)
    {
        return 1;
    }
    if (n <= fromWayPointIndex)
    {
        return 0;
    }

    int count = 1;
    int i = fromWayPointIndex;
    while (totalDistance > 0 && i < n)
    {
        totalDistance -= distances[i++];
        count++;
    }
    return count;
}

//! Function calculating the index of the waypoint ahead that should be targeted at
//!
//! @param[in]     position             position of the vehicle
//! @param[in]     direction            direction of the vehicle
//! @param[in]     fromIndex            index of the 'starting' waypoint
//! @param[in]     numPointsLookAhead   how many indices are looked ahead
//! @return                             corresponding index
int TrajectoryFollowingControl::findClosestWayPointAheadIndex(Common::Vector2d &position,
                                                              Common::Vector2d &direction,
                                                              int fromIndex, int numPointsLookAhead)
{
    if (numWayPoints_ <= 0)
    {
        return -1;
    }
    double dist, mindist;
    int index = fromIndex;
    Common::Vector2d u = waypoints_[index].position - position;

    mindist = u.Length();
    int currentIndex = index;
    int d = numWayPoints_ - 1 - index;
    if (d < numPointsLookAhead)
    {
        //end of trajectory before maximum lookahead
        numPointsLookAhead = d;
    }
    for (int i = 0; i < numPointsLookAhead; ++i)
    {
        currentIndex = currentIndex + 1;
        u = waypoints_[currentIndex].position - position;
        dist = u.Length();

        if (dist < mindist)
        {
            index = currentIndex;
            mindist = dist;
        }
    }
    //avoid further calculation, next waypoint is not the one we started to search from
    if (index != fromIndex)
    {
        return index;
    }

    //if the waypoint with minimum distance is the first we started the search with (i.e. the last one),
    //check wether it still is in front of position (using direction)
    //if so, return it
    //if not, return the next point if it exists (index does not equal numWaypoints_-1)

    double c = direction.Dot(position);
    //check in which half space (in front or behind the cars position) the calculated waypoint is
    //test wether the calculated waypoint is in front of the car (prevent trying to turn in this case)
    if (direction.Dot(waypoints_[index].position) <= c)
    {
        if (index != numWayPoints_ - 1)
        {
            //return next waypoint
            return index + 1;
        }
        else
        {
            //there is no next waypoint, return -1
            return -1;
        }
    }

    //calculated point is in front, return its index
    return index;
}

//! Constructor that just sets a dummy waypoint at ((0,0),0,0), waypoints are to be set later
//!
//! @param[in]     CarStats         class with information on the vehicle
//! @param[in]     startState       structure with the position and control data
//! @param[in]     GasPID           PID-control algorithm for the gas pedal
//! @param[in]     BrakePID         PID-control algorithm for the brake pedal
//! @param[in]     lookAheadTime    time in s how long the algorithm looks ahead
TrajectoryFollowingControl::TrajectoryFollowingControl(CarStatistics &CarStats, State &startState,
                                                       PIDController &GasPID, PIDController &BrakePID,
                                                       double lookAheadTime):
    numWayPoints_(1), waypoints_(1),
    CarStats_(CarStats), CurrentState_(startState),
    GasPID_(GasPID), BrakePID_(BrakePID),
    generalLowPassFilter(filterWindowSize),
    lookAheadTime_(lookAheadTime)
{
    vector<double> X(1);
    vector<double> Y(1);
    vector<double> V(1);
    vector<double> T(1);

    //default is one waypoint
    X[0] = 0;
    Y[0] = 0;
    V[0] = 0;
    T[0] = 0;

    setWaypoints(X, Y, V, T);
    currentWayPointIndex_ = 0;
    previousWayPointIndex_ = 0;

    //initialize the lowpass filter with duplicates of the initial value
    for (int i = 0; i < filterWindowSize; ++i)
    {
        if (startState.controlData.gas > 0)
        {
            generalLowPassFilter.newValue(startState.controlData.gas);
        }
        else
        {
            generalLowPassFilter.newValue(-startState.controlData.brake);
        }
    }
}

//! Constructor
//!
//! @param[in]     waypoints        vector with the waypoints that the vehicle should approximate
//! @param[in]     CarStats         class with information on the vehicle
//! @param[in]     startState       structure with the position and control data
//! @param[in]     GasPID           PID-control algorithm for the gas pedal
//! @param[in]     BrakePID         PID-control algorithm for the brake pedal
//! @param[in]     lookAheadTime    time in s how long the algorithm looks ahead
TrajectoryFollowingControl::TrajectoryFollowingControl(std::vector<WaypointData> &waypoints,
                                                       CarStatistics &CarStats, State &startState,
                                                       PIDController &GasPID, PIDController &BrakePID, double lookAheadTime):
    numWayPoints_(waypoints.size()), waypoints_(waypoints),
    CarStats_(CarStats), CurrentState_(startState),
    GasPID_(GasPID), BrakePID_(BrakePID),
    generalLowPassFilter(filterWindowSize),
    lookAheadTime_(lookAheadTime)
{
    setWaypoints(waypoints);

    distancesBetweenConsecutiveWayPoints = computeDistancesBetweenConsecutiveWayPoints();
    currentWayPointIndex_ = 0;

    //initialize the lowpass filter with duplicates of the initial value
    for (int i = 0; i < filterWindowSize; ++i)
    {
        if (startState.controlData.gas > 0)
        {
            generalLowPassFilter.newValue(startState.controlData.gas);
        }
        else
        {
            generalLowPassFilter.newValue(-startState.controlData.brake);
        }
    }
}

//! Function calculating the lateral control
//!
//! @return                         the required wheel angle
double TrajectoryFollowingControl::lateralControl()
{
    Common::Vector2d  PredictedPosition;
    double predictedAngle;
    Common::Vector2d  nextWayPoint;
    int    nextWayPointIndex;

    double velocity = CurrentState_.positionData.velocity;
    double frontMidWheelAngle = CurrentState_.controlData.frontWheelAngle;

    //should be precalculated only once while in init
    double temp = CarStats_.NormalForceFront / CarStats_.CorneringStiffnessCoefficientFront - \
                  CarStats_.NormalForceRear / CarStats_.CorneringStiffnessCoefficientRear;
    double denom = CarStats_.wheelbase + temp * velocity * velocity / GRAVITY;

    //kappa as in matlab
    double curvature = 0;
    if (denom == 0)
    {
        if (frontMidWheelAngle != 0)
        {
            //denominator is zero and numerator is not zero: curvature tends to infinty
            curvature = 1e9;
        }
        //else: curvature not defined -> stays zero
    }
    else
    {
        curvature = frontMidWheelAngle / denom;
    }

    double arcLength = lookAheadTime_ * velocity;

    // algorithm not copied exactly because the meaning of arcLength (resp. s) changes in the matlab algorithm which leads to confusion
    double s = (arcLength < 1) ? 1 : arcLength;
    double psi = CurrentState_.positionData.angle;

    predictedAngle = psi;
    double gamma = 0;
    if (curvature != 0)
    {
        gamma = (arcLength * curvature) / 2;
        s = (2 / curvature) * sin(gamma);
        predictedAngle -= gamma;
    }

    PredictedPosition.x = CurrentState_.positionData.position.x + cos(psi + gamma) * s;
    PredictedPosition.y = CurrentState_.positionData.position.y + sin(psi + gamma) * s;
    Common::Vector2d PredictedDirection(cos(predictedAngle), sin(predictedAngle));
    Common::Vector2d CurrentDirection(cos(psi), sin(psi));

    //compute the index of the most recently passed
    previousWayPointIndex_ = findClosestWayPointAheadIndex(CurrentState_.positionData.position,
                                                           CurrentDirection,
                                                           previousWayPointIndex_,
                                                           10);
    //10 waypoint look ahead cycle time needs to respect this, can be adapted later (requires adaptation to general case)
    //look ahead depends on cycle time and velocity

    //-1 to get the waypoint before unless it is the first one
    if (previousWayPointIndex_ > 0)
    {
        previousWayPointIndex_ -= 1;
    }

    //end of trajectory not reached
    if (previousWayPointIndex_ != -1)
    {
        //could be set to numWaypoints_ to use maximum forward look distance (computation time tradeoff)
        int numLookAheadPoints = computeNumberOfLookaheadPoints(arcLength,
                                                                distancesBetweenConsecutiveWayPoints,
                                                                previousWayPointIndex_);

        nextWayPointIndex = findClosestWayPointAheadIndex(PredictedPosition,
                                                          PredictedDirection,
                                                          previousWayPointIndex_,
                                                          numLookAheadPoints > 1 ? numLookAheadPoints : 1); //at least one waypoint lookahead
        currentWayPointIndex_ = nextWayPointIndex;
        if (currentWayPointIndex_ == -1)
        {
            //last waypoint passed, return neutral angle
            return 0;
        }
    }
    else
    {
        currentWayPointIndex_ = -1;
        return 0;   //last waypoint passed, return neutral angle
    }

    nextWayPoint = waypoints_[nextWayPointIndex].position;

    //calculate curvature required to reach this waypoint - depending on the assumed driving trajectory between the points

    double requiredNextCurvature = 0;

    //use refactored formula provided in matlab (simplified equivalent formula evaluates faster)
    double dx = nextWayPoint.x - CurrentState_.positionData.position.x;
    double dy = nextWayPoint.y - CurrentState_.positionData.position.y;

    //check denominator zero (is just the case if both are zero)
    if ( dx != 0 || dy != 0 )
    {
        double ratio = (dx != 0) ? dy / dx : 1e9;
        requiredNextCurvature = 2 * cos(psi) * (ratio - tan(psi)) / (dx + ratio * dy);
    }

    //calculate required front wheel angle for this curvature
    double requiredFrontMidWheelAngle = requiredNextCurvature * denom;

    //assume symmetric limit of angle
    return limitValue(requiredFrontMidWheelAngle, -CarStats_.frontWheelAngleLimit,
                      CarStats_.frontWheelAngleLimit );
}

//! Function calculating the gas control
//!
//! @return                         the required gas value
double TrajectoryFollowingControl::gasControl()
{
    int referenceWaypointIndex;
    if (previousWayPointIndex_ >= 0)
    {
        referenceWaypointIndex =  previousWayPointIndex_ + numVelocityLookAheadPoints;
        if (referenceWaypointIndex >= numWayPoints_)
        {
            referenceWaypointIndex = numWayPoints_ - 1;
        }
    }
    else
    {
        //do not accelerate after last waypoint
        return 0;
    }

    double velocity = CurrentState_.positionData.velocity;
    double referenceVelocity = waypoints_[referenceWaypointIndex].velocity;

    // has car to accelerate?
    if (referenceVelocity > velocity)
    {
        double gas = GasPID_.calculate(referenceVelocity - velocity, lookAheadTime_);
        //adding the controller result (doc) to the old value leads to a worse results and is therefore not used
        /*CurrentState_.controlData.gas + */
        return limitValue(gas, 0, 1);
    }
    else
    {
        GasPID_.resetIntegratedError();
        return 0;
    }
}

//! Function calculating the brake control
//!
//! @return                         the required brake value
double TrajectoryFollowingControl::brakeControl()
{
    double velocity = CurrentState_.positionData.velocity;
    int referenceWaypointIndex;
    if (previousWayPointIndex_ >= 0)
    {
        referenceWaypointIndex =  previousWayPointIndex_ + numVelocityLookAheadPoints;
        if (referenceWaypointIndex >= numWayPoints_)
        {
            referenceWaypointIndex = numWayPoints_ - 1;
        }
    }
    else
    {
        if (velocity > 0)
        {
            return 1;
        }
        else
        {
            return 0;    //brake after last waypoint if velocity is still positive
        }
    }
    double referenceVelocity = waypoints_[referenceWaypointIndex].velocity;

    // has car to brake?
    if (referenceVelocity < velocity)
    {
        double brake = BrakePID_.calculate(velocity - referenceVelocity, lookAheadTime_);
        //adding the controller result (doc) to the old value leads to a worse results and is therefore not used
        /*CurrentState_.controlData.gas + */
        return limitValue(brake, 0, 1);
    }
    else
    {
        BrakePID_.resetIntegratedError();
        return 0;
    }
}

//! Function calling all controls and setting the controlData
ControlData TrajectoryFollowingControl::computeRequiredControl()
{

    Common::Vector2d u = waypoints_[numWayPoints_ - 1].position - waypoints_[numWayPoints_ -
                                                                             2].position;
    Common::Vector2d v = CurrentState_.positionData.position - waypoints_[numWayPoints_ - 2].position;
    double uv = v.Dot(u);

    if (uv > 0.0)
    {
        return ControlData(0.0, 0.0, 0.0);
    }

    CurrentState_.controlData.frontWheelAngle = lateralControl();
    generalLowPassFilter.newValue( gasControl() - brakeControl() );
    double lowPassValue = generalLowPassFilter.getFilteredValue();

    if (lowPassValue > 0)
    {
        CurrentState_.controlData.gas = lowPassValue;
        CurrentState_.controlData.brake = 0;
    }
    else
    {
        CurrentState_.controlData.gas = 0;
        CurrentState_.controlData.brake = -lowPassValue;
    }

    return CurrentState_.controlData;
}

//! Function setting the way points
//!
//! @param[in]     Waypoints        vector of the waypoints
bool TrajectoryFollowingControl::setWaypoints(std::vector<WaypointData> &Waypoints)
{

    waypoints_ = Waypoints;
    numWayPoints_ = waypoints_.size();
    distancesBetweenConsecutiveWayPoints = computeDistancesBetweenConsecutiveWayPoints();
    previousWayPointIndex_ = 0;
    currentWayPointIndex_ = 0;
    return true;
}

//! Function setting the way points
//!
//! @param[in]     X            vector of the x-coordinates
//! @param[in]     Y            vector of the y-coordinates
//! @param[in]     Velocity     vector of the velocities
//! @param[in]     Time         vector of the time
bool TrajectoryFollowingControl::setWaypoints(std::vector<double> &X,
                                              std::vector<double> &Y,
                                              std::vector<double> &Velocity,
                                              std::vector<double> &Time)
{
    unsigned int n = X.size();
    if (Y.size() != n || Velocity.size() != n)
    {
        return false;
    }
    waypoints_.resize(n);

    for (unsigned int i = 0; i < n; ++i)
    {
        waypoints_[i].position = Common::Vector2d(X[i], Y[i]);
        waypoints_[i].velocity = Velocity[i];
        waypoints_[i].time     = Time[i];
    }

    numWayPoints_ = n;
    previousWayPointIndex_ = 0;
    currentWayPointIndex_ = 0;
    return true;
}
