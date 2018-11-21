/******************************************************************************
* Copyright (c) 2018 AMFD GmbH
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

//-----------------------------------------------------------------------------
/** @file  AlgorithmLongitudinalDriverImplementation.cpp */
//-----------------------------------------------------------------------------

#include <cassert>
#include <memory>
#include <qglobal.h>

#include "AlgorithmLongitudinalDriverImplementation.h"

#include "ALongitudinalDriverToDLongitudinalSignal.h"
#include "brakingSignal.h"
#include "lateralDriverSignal.h"

void AlgorithmLongitudinalDriverImplementation::UpdateInput(int localLinkId, const std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);

    if (localLinkId == 0)
    {
        const std::shared_ptr<BrakingSignal const> signal = std::dynamic_pointer_cast<BrakingSignal const>(data);

        if (!signal)
        {
            const std::string msg = COMPONENTNAME + " invalid signaltype";
            LOG(CbkLogLevel::Debug, msg);
            throw std::runtime_error(msg);
        }

        in_aVehicle = signal->acceleration;

    }
    else if (localLinkId == 1)
    {
        const std::shared_ptr<LateralDriverSignal const> signal = std::dynamic_pointer_cast<LateralDriverSignal const>(data);

        if (!signal)
        {
            const std::string msg = COMPONENTNAME + " invalid signaltype";
            LOG(CbkLogLevel::Debug, msg);
            throw std::runtime_error(msg);
        }

        in_vVehicle = signal->vLongEgo;

    }
    else
    {
        const std::string msg = COMPONENTNAME + " invalid link";
        LOG(CbkLogLevel::Debug, msg);
        throw std::runtime_error(msg);
    }
}

void AlgorithmLongitudinalDriverImplementation::UpdateOutput(int localLinkId, std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);

    if(localLinkId == 0)
    {
        try {
            data = std::make_shared<ALongitudinalDriverToDLongitudinalSignal const>(
                        out_accPedalPos,
                        out_brakePedalPos,
                        out_gear,
                        _carParameters);
        }
        catch(const std::bad_alloc&)
        {
            const std::string msg = COMPONENTNAME + " could not instantiate signal";
            LOG(CbkLogLevel::Debug, msg);
            throw std::runtime_error(msg);
        }
    }
    else
    {
        const std::string msg = COMPONENTNAME + " invalid link";
        LOG(CbkLogLevel::Debug, msg);
        throw std::runtime_error(msg);
    }
}

void AlgorithmLongitudinalDriverImplementation::Trigger(int time)
{
    Q_UNUSED(time);

    // get current values from agent
    double xVel = in_vVehicle; // the current velocity of the vehicle
    double xAcc = GetAgent()->GetAccelerationX(); // the current acceleration of the vehicle
    int lastGear = GetAgent()->GetGear();

    // reset output variables
    out_accPedalPos = 0;
    out_brakePedalPos = 0;
    out_gear = {_carParameters.maxGear, lastGear};

    // get stored values from last calculation
    double lastPedalPosition = _lastPedalPosition;
    double lastEngineSpeed = _lastEngineSpeed;

    // correct initial values
    if(lastGear==_errorVal||_lastEngineSpeed==_errorVal||_lastXVel==_errorVal||_lastIn_aVehicle==_errorVal) {

        std::vector<double> initialGearAndEngineSpeed = ALongitudinalDataProvider::Instance().GetBestGear(xVel, xAcc, _carParameters, _showValues);
        lastGear = static_cast<int>(initialGearAndEngineSpeed.at(0));
        _lastEngineSpeed = initialGearAndEngineSpeed.at(1);
        lastEngineSpeed = initialGearAndEngineSpeed.at(1);
        _lastXVel = xVel;
        _lastIn_aVehicle = in_aVehicle;
    }

    // stopped car
    if (xVel==0&&in_aVehicle==0)
    {
        out_gear = {_carParameters.maxGear, 0};
        _lastPedalPosition = 0;
    }
    else
    {


        // calculate (open-loop controller, stationary)
        // -------------------------------------------

        std::vector<double> pedalPosOLCStat = ALongitudinalDataProvider::Instance().PedalPosition(xVel, lastGear, &_lastEngineSpeed, in_aVehicle, _carParameters, _showValues);

        int chosenGear = static_cast<int>(pedalPosOLCStat.at(0));
        double wishPedalPosition = pedalPosOLCStat.at(1);

        // initialize lastPedalPosition with wishPedalPosition
        if (_lastPedalPosition==_errorVal)
            _lastPedalPosition = wishPedalPosition;

        // adjust
        // ------

        // apply transient behaviour on open-loop controller (PT1-charateristic)
        double pedalPosOLC = ALongitudinalDataProvider::Instance().ApplyFirstOrderTransientBehaviour(_lastPedalPosition, wishPedalPosition);

        // apply closed-loop controller (P-controller, based on velocity difference)
        double pedalPosOLCplusCLC = ALongitudinalDataProvider::Instance().ApplyClosedLoopController(xVel, _lastXVel, _lastIn_aVehicle, _cycleTime, pedalPosOLC);

        // bound pedal position to the car's maximum possible values
        double pedalPos = ALongitudinalDataProvider::Instance().BoundPedalPosition(pedalPosOLCplusCLC);

        pedalPositionsHelper.push_back(pedalPos);
        // check for first pedal actuation to save foot position
        if(_firstActuation)
        {
            if (pedalPos > 0)
            {
                 _footOnGas = true;
            }else if(pedalPos < 0)
            {
                 _footOnBrake = true;
            }
       }

        // check for pedal change
        // when a pedal change occurs a pedal-changetime is generated

        if(((_lastPedalPosition * pedalPos) < 0.0 || (_lastPedalPosition == 0.0 && pedalPos < 0.0 && _footOnGas) || (_lastPedalPosition == 0.0 && pedalPos > 0.0 && _footOnBrake))&& !_firstActuation && !_pedalChange)
        {
            _pedalChange = true;
            _pedalChangeTime = GetPedalChangeTime();
            if(_footOnGas)_lastFootPosition = true;
                    else _lastFootPosition = false;
            _footOnBrake = false;
            _footOnGas = false;
        }

        //foot during changetime not on any pedalero

        if(_pedalChange)
        {
            //until the changetime isn't over the pedalpositions are stored
            if(!IsPedalChangeFinished(_pedalChangeTime,_cycleTime))
            {
                pedalPositions.push_back(pedalPos);
             //   std::cout << GetAgent()->GetAgentId()<< ": "<< pedalPos<<"time "<<_pedalChangeTime<<"Zeit: " <<time<< "\n";
            }else
            {
                _pedalChange = false;

                if(_lastFootPosition == true)
                {
                    _footOnGas = false;
                    _footOnBrake = true;
                }else
                {
                    _footOnGas = true;
                    _footOnBrake = false;
                }

                //check if all stored pedalpositions have the same algebraic sign, if not a possible error occured

                std::vector<double> negValue;
                std::vector<double> posValue;

                for(auto it = pedalPositions.begin(); it != pedalPositions.end(); it++)
                {
                    if(*it<0)
                    {
                        negValue.push_back(std::distance(pedalPositions.begin(),it));
                    }else
                    {
                        posValue.push_back(std::distance(pedalPositions.begin(),it));
                    }
                }

                if((negValue.size() == pedalPositions.size() && pedalPos<0) || (posValue.size() == pedalPositions.size() && pedalPos>0))
                {
                    _correction = true;

                } //helper for jumping signals, deleted if signal is more real and not jumping
                else
                    {
                        _correction = false;
                        pedalPositions.clear();
                        if(pedalPos>0)
                        {
                            _footOnGas = true;
                            _footOnBrake = false;
                        }else
                        {
                            _footOnGas = false;
                            _footOnBrake = true;
                        }
                    }
            }
        }




        // Generate the output

        if(_pedalChange)
        {
            pedalPos = 0;

        }else if(_correction)
                {
                    if(lastPedalPosition > pedalPos)
                    {
                        _correction = false;
                        pedalPositions.clear();
                    }

                    if(pedalPositions.size()> 1)
                    {
                        pedalPos = lastPedalPosition + ((pedalPos - lastPedalPosition)*0.5);
                        pedalPositions.erase(pedalPositions.begin());
                    }
                    if(pedalPositions.size()== 1)
                    {
                        _correction = false;
                         pedalPositions.clear();
                    }

                }

  //      pedalPositionsOut.push_back(pedalPos);

        if (pedalPos > 0)
            out_accPedalPos = pedalPos; // gas pedal position
        else if (pedalPos < 0) // -> brake, if engine drag is not enough
            out_brakePedalPos = std::fabs(pedalPos); // brake pedal position
             else
             { out_accPedalPos = 0;
               out_brakePedalPos = 0;
             }

  //      gasPedalPositionsHelper.push_back(out_accPedalPos);
  //      brakePedalPositionsHelper.push_back(out_brakePedalPos);


        out_gear = {_carParameters.maxGear, chosenGear}; // gear is not changed when braking

        // testing
        if(_showValues)
            PrintCurrentValues(lastPedalPosition, lastEngineSpeed, wishPedalPosition, lastGear, chosenGear, _carParameters, xVel, xAcc, out_accPedalPos, out_brakePedalPos, pedalPosOLC, pedalPosOLCplusCLC, pedalPos);

        // save values
        _lastPedalPosition = pedalPos;

        if(pedalPos != 0 && _firstActuation == true)
            {
                _firstActuation = false;
               if(pedalPos>0)_lastFootPosition=true;
                else _lastFootPosition=false;
            }
    }

    _lastXVel = xVel;
    _lastIn_aVehicle = in_aVehicle;

    }

    void AlgorithmLongitudinalDriverImplementation::PrintCurrentValues(double lastPedalPosition, double lastEngineSpeed, double wishPedalPosition, int lastGear, int chosenGear, ALongitudinalDataProvider::CarParameters _carParameters, double xVel, double xAcc, double out_accPedalPos, double out_brakePedalPos, double loweredPedalPosition, double regulatedPedalPosition, double nextPedalPosition)
    {
        int agentId = GetAgent()->GetAgentId();

        std::cout << std::endl << "  PRINT_CURRENT_VALUES  (ID:" << agentId << ") ";
        std::cout << std::endl << "  ********************  " << std::endl;

        std::cout << std::endl << "previous gear / GetAgent()->GetGear(): " << lastGear << std::endl;
        std::cout << std::endl << "xVel: " << xVel;
        std::cout << std::endl << "xAcc: " << xAcc;
        std::cout << std::endl << "in_aVehicle: " << in_aVehicle;
/*
        std::cout << std::endl << _carParameters.carMass << "----------_carParameters.carMass--------------";
        std::cout << std::endl << _carParameters.iDiffential << "--------_carParameters.iDiffential----------------";
        std::cout << std::endl << _carParameters.maxGear << "-------_carParameters.maxGear-----------------";
        std::cout << std::endl << _carParameters.MEngMax << "-------_carParameters.MEngMax-----------------";
        std::cout << std::endl << _carParameters.MEngMin << "-------_carParameters.MEngMin----------------";
        std::cout << std::endl << _carParameters.nEngMax << "-------_carParameters.nEngMax-----------------";
        std::cout << std::endl << _carParameters.nEngMin << "-------_carParameters.nEngMin-----------------";
        std::cout << std::endl << _carParameters.rStat << "-------_carParameters.rStat-----------------"<< std::endl;
*/
        std::cout << std::endl << "lastEngineSpeed: " << lastEngineSpeed;
        std::cout << std::endl << "currentEngineSpeed: " << _lastEngineSpeed << std::endl;

        std::cout << std::endl << "lastPedalPosition: " << lastPedalPosition << std::endl;

            std::cout << std::endl << "wishPedalPosition: " << wishPedalPosition;
            std::cout << std::endl << "loweredPedalPosition: " << loweredPedalPosition;
            std::cout << std::endl << "regulatedPedalPosition: " << regulatedPedalPosition << "(cut => " << nextPedalPosition << ")" << std::endl;

        if (nextPedalPosition >= 0){
                std::cout << std::endl << "GAS ("<< chosenGear <<" von " <<_carParameters.maxGear<<") PedalPos: " << out_accPedalPos;
        }
        else
        {
                std::cout << std::endl << "BRAKE ("<< chosenGear <<" von " <<_carParameters.maxGear<<") PedalPos: " << out_brakePedalPos;
        }

        std::cout << std::endl << std::endl << "------------(AGENT: " << agentId << ")------------" << std::endl;
    }

    double AlgorithmLongitudinalDriverImplementation::GetPedalChangeTime()
    {
        return (_stochastics->GetLogNormalDistributed(0.29,0.073)*1000);
    }

    bool AlgorithmLongitudinalDriverImplementation::IsPedalChangeFinished(double pedalChangeTime, double cycleTime)
    {

        if(_startPedalChange)
        {
            double rest = fmod(pedalChangeTime,cycleTime);
            //round the pedalChangeTime to its nearest hundred
            if (rest <= cycleTime*0.5)
            {
                _timeSteps = (int)pedalChangeTime/(int)cycleTime;
            }else
            {
                _timeSteps = (int)((pedalChangeTime/(int)cycleTime) + 1);
            }
            _startPedalChange = false;
        }

        if(_timeSteps<=0)
        {

            _startPedalChange = true;
            return _startPedalChange;

        }else
        {
            _timeSteps-=1;
            return _startPedalChange;
        }
    }
