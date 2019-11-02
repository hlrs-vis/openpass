/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*               2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#pragma once

#include <string>
#include <QFile>
#include <QTextStream>
#include <QXmlStreamWriter>

#include "Interfaces/observationInterface.h"
#include "Interfaces/eventNetworkInterface.h"
#include "observationLogConstants.h"
#include "observationCyclics.h"
#include "runStatistic.h"

//-----------------------------------------------------------------------------
/** \brief Provides the basic logging and observer functionality.
*   \details Provides the basic logging functionality.
*            Writes the framework, events, agents and samples into the output.
*            It has two logging modes either event based or as single output.
*            This module also acts as an observer of agent modules.
*
*   \ingroup ObservationLog
*/
//-----------------------------------------------------------------------------
class ObservationFileHandler
{
public:
    const std::string COMPONENTNAME = "ObservationFileHandler";

    ObservationFileHandler() = default;
    ObservationFileHandler(const ObservationFileHandler&) = delete;
    ObservationFileHandler(ObservationFileHandler&&) = delete;
    ObservationFileHandler& operator=(const ObservationFileHandler&) = delete;
    ObservationFileHandler& operator=(ObservationFileHandler&&) = delete;
    ~ObservationFileHandler() = default;

    /*!
     * \brief Sets the directory where the output is written into
     *
     * \param outputDir path of output directory
     */
    void SetOutputDir(std::string outputDir)
    {
        folder = QString::fromStdString(outputDir);
    }

    void SetSceneryFile (std::string fileName)
    {
        sceneryFile = fileName;
    }

    /*!
     * \brief Creates the output file as simulationOutput.tmp and writes the basic header information
     */
    void WriteStartOfFile();

    /*!
     * \brief This function gets called after each run and writes all information about this run into the output file
     *
     * \param runResult
     * \param runStatistik
     * \param cyclics
     * \param world
     * \param eventNetwork
     */
    void WriteRun(const RunResultInterface& runResult, RunStatistic runStatistik, ObservationCyclics& cyclics,
                  WorldInterface* world, SimulationSlave::EventNetworkInterface* eventNetwork);

    /*!
     * \brief Closes the xml tags flushes the output file, closes it and renames it to simulationOutput.xlm
     */
    void WriteEndOfFile();

private:
    std::shared_ptr<QXmlStreamWriter> fileStream;

    int runNumber;                                               //!< run number
    std::string sceneryFile;

    const OutputAttributes outputAttributes;
    const OutputTags outputTags;

    QString folder;
    QString tmpFilename;
    QString finalFilename;
    QString tmpPath;
    QString finalPath;
    std::shared_ptr<QFile> file;


    //add infos to the file stream
    /*!
    * \brief Returns wether the agent has sensors or not.
    *
    * @return       true if agent has sensors, otherwise false.
    */
    inline bool ContainsSensor(const std::list<SensorParameter>& sensorParameters) const;

    /*!
    * \brief Writes the sensor information into the simulation output.
    *
    * @param[in]    fStream             Shared pointer of the stream writer.
    * @param[in]    sensorName          Name of the sensor.
    * @param[in]    sensorParameters    Parameters of the sensor.
    */
    void AddSensor(std::shared_ptr<QXmlStreamWriter> fStream, const SensorParameter& sensorParameter);

    /*!
    * \brief Writes all sensor information of an agent into the simulation output.
    *
    * @param[in]     fStream    Shared pointer of the stream writer.
    * @param[in]     agent      Pointer to the agent.
    */
    void AddSensors(std::shared_ptr<QXmlStreamWriter> fStream, const AgentInterface* agent);

    /*!
    * \brief Writes the content of an agent into the simulation output.
    *
    * @param[in]     fStream    Shared pointer of the stream writer.
    * @param[in]     agent      Pointer to the agent.
    */
    void AddAgent(std::shared_ptr<QXmlStreamWriter> fStream, const AgentInterface* agent);

    /*!
    * \brief Writes the content of all agent into the simulation output.
    *
    * @param[in]     fStream    Shared pointer of the stream writer.
    */
    void AddAgents(std::shared_ptr<QXmlStreamWriter> fStream, WorldInterface* world);

    /*!
    * \brief Writes all parameters of an event into the simulation output.
    *
    * @param[in]     fStream            Shared pointer of the stream writer.
    * @param[in]     eventParameters    Parameters of an event in string representation.
    */
    void AddEventParameters(std::shared_ptr<QXmlStreamWriter> fStream,
                            std::list<std::pair<std::string, std::string>> eventParameters);

    /*!
    * \brief Writes an event into the simulation output.
    *
    * @param[in]     fStream            Shared pointer of the stream writer.
    * @param[in]     event              Shared pointer of the event.
    */
    void AddEvent(std::shared_ptr<QXmlStreamWriter> fStream, std::shared_ptr<EventInterface> event);

    /*!
    * \brief Writes all events into the simulation output.
    *
    * @param[in]     fStream            Shared pointer of the stream writer.
    */
    void AddEvents(std::shared_ptr<QXmlStreamWriter> fStream, SimulationSlave::EventNetworkInterface* eventNetwork);

    /*!
    * \brief Writes the header into the simulation output during full logging.
    *
    * @param[in]     fStream            Shared pointer of the stream writer.
    */
    void AddHeader(std::shared_ptr<QXmlStreamWriter> fStream, ObservationCyclics& cyclics);

    /*!
    * \brief Writes the samples into the simulation output during full logging.
    *
    * @param[in]     fStream            Shared pointer of the stream writer.
    */
    void AddSamples(std::shared_ptr<QXmlStreamWriter> fStream, ObservationCyclics& cyclics);

    /*!
    * \brief Returns the event type as string.
    *
    * @param[in]     eventType      Event type.
    *
    * @return        Event type as string.
    */
    std::string GetEventString(EventDefinitions::EventType eventType);

private:
    const QString outputFileVersion = "0.2.0";
};


