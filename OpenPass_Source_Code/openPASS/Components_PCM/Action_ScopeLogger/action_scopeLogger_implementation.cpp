/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include <memory>
#include <qglobal.h>
#include "action_scopeLogger_implementation.h"

Action_ScopeLogger_Implementation::Action_ScopeLogger_Implementation(int componentId,
                                                                     bool isInit,
                                                                     int priority,
                                                                     int offsetTime,
                                                                     int responseTime,
                                                                     int cycleTime,
                                                                     StochasticsInterface *stochastics,
                                                                     WorldInterface *world,
                                                                     const ParameterInterface *parameters, const std::map<int, ObservationInterface *> *observations,
                                                                     const CallbackInterface *callbacks,
                                                                     AgentInterface *agent) :
    ActionInterface(componentId,
                    isInit,
                    priority,
                    offsetTime,
                    responseTime,
                    cycleTime,
                    stochastics,
                    world,
                    parameters,
                    observations,
                    callbacks,
                    agent)
{
    std::stringstream log;
    log << "Constructing Action_ScopeLogger";
    LOG(CbkLogLevel::Debug, log.str());
    log.str(std::string());

    std::map<int, const std::string> parameterMapStringExternal =
            GetParameters()->GetParametersString();
    foreach (auto &iterator, parameterMapString) {
        int id = iterator.first;
        parameterMapString.at(id)->SetValue(parameterMapStringExternal.at(id));
    }


    QString in_resultFolderName = QString::fromStdString(resultFolderName.GetValue());
    if (!QDir().exists(in_resultFolderName)) {
        QDir().mkdir(in_resultFolderName);
    }

    fullFileName = fileNamePrefix.GetValue() + "_" + COMPONENTNAME
            + "_" + std::to_string(GetAgent()->GetAgentId())
            + "_" + std::to_string(componentId)
            + fileNameSuffix.GetValue();

    fullResultFilePath = resultFolderName.GetValue() + "/" + fullFileName;

    if (QFile().exists(QString::fromStdString(fullResultFilePath))) {
        QFile::remove(QString::fromStdString(fullResultFilePath));
    }


    log << "Construction of Action_ScopeLogger successful";
    LOG(CbkLogLevel::Debug, log.str());
}

Action_ScopeLogger_Implementation::~Action_ScopeLogger_Implementation()
{
    std::ofstream resultFile;
    resultFile.open(fullResultFilePath, std::ofstream::out | std::ofstream::app);

    if (!resultFile.is_open()) {
        LOG(CbkLogLevel::Debug, "result File could not be opened");
        return;
    }
    std::string sep = ";";
    std::string endLine = "\n";

    //header
    resultFile << "Time" << sep;

    std::vector<std::pair<QString, QVariant>> dataVector = agentData.begin()->second;
    for (std::pair<QString, QVariant> &data : dataVector) {
        resultFile << data.first.toStdString() << sep;
    }

    resultFile << endLine;

    //data
    for (auto const &timeStep : agentData) {
        resultFile << timeStep.first << sep;

        for (const std::pair<QString, QVariant> &dataVector : timeStep.second) {
            resultFile << dataVector.second.toString().toStdString() << sep;
        }
        resultFile << endLine;
    }

    resultFile << endLine;

    resultFile.close();
}

void Action_ScopeLogger_Implementation::UpdateInput(int localLinkId,
                                                    const std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);

    std::stringstream log;
    log << COMPONENTNAME << " UpdateInput";
    LOG(CbkLogLevel::Debug, log.str());
    log.str(std::string());

    bool success = inputPorts.at(localLinkId)->SetSignalValue(data);

    if (success) {
        log << COMPONENTNAME << " UpdateInput successful";
        LOG(CbkLogLevel::Debug, log.str());
    } else {
        log << COMPONENTNAME << " UpdateInput failed";
        LOG(CbkLogLevel::Error, log.str());
    }
}

void Action_ScopeLogger_Implementation::UpdateOutput(int localLinkId,
                                                     std::shared_ptr<SignalInterface const> &data, int time)
{
    Q_UNUSED(time);
    Q_UNUSED(localLinkId);
    Q_UNUSED(data);

    std::runtime_error("invalid link");
}

void Action_ScopeLogger_Implementation::Trigger(int time)
{
    //fill time vector
    //Check for duplicates
    if (timeVector.size() == 0 || timeVector.back() != time) {
        timeVector.push_back(time);
    }

    SaveTimeStepData(time);
}

void Action_ScopeLogger_Implementation::SaveTimeStepData(int time)
{
    std::vector<std::pair<QString, QVariant>> dataVector;
    dataVector.push_back(std::make_pair("XPos", positionX.GetValue()));
    dataVector.push_back(std::make_pair("YPos", positionY.GetValue()));
    dataVector.push_back(std::make_pair("YawAngle", yawAngle.GetValue()));
    dataVector.push_back(std::make_pair("Velocity", velocityX.GetValue()));
    dataVector.push_back(std::make_pair("GasPedal", gasPedal.GetValue()));
    dataVector.push_back(std::make_pair("BrakePedal", brakePedal.GetValue()));
    dataVector.push_back(std::make_pair("FrontWheelAngle", frontWheelAngle.GetValue()));
    dataVector.push_back(std::make_pair("CollisionOccured", collisionOccured.GetValue()));
    agentData.emplace(std::make_pair(time, dataVector));
}
