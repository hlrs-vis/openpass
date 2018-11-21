/*********************************************************************
* Copyright (c) 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "openScenarioWriter.h"

const QString OpenScenarioWriter::CreateScenarioFile(QString &scenarioFile,
                                                     std::vector<PCM_Trajectory *> &trajectories)
{
    XoscScenario scenarioConfig;

    for (size_t i = 0; i < trajectories.size(); i++)
    {
        scenarioConfig.AddTrajectory(i, trajectories.at(i));
    }

    return WriteScenarioFile(scenarioFile, scenarioConfig);
}

const QString OpenScenarioWriter::WriteScenarioFile(QString &scenarioFile,
                                                    XoscScenario &scenarioConfig)
{
    // write the xml agent file
    QFile file(scenarioFile);

    //open file
    if (!file.open(QIODevice::WriteOnly))
    {
        // show error message if not able to open file
        std::cout << "Error (OpenScenarioWriter): "
                  "could not open scenario File" << std::endl;
        return "";
    }

    // if file is successfully opened, create XML
    QXmlStreamWriter xmlWriter(&file);

    xmlWriter.setAutoFormatting(true);

    xmlWriter.writeStartDocument();

    scenarioConfig.WriteToXml(&xmlWriter);

    xmlWriter.writeEndDocument();

    file.close();
    file.flush();

    return scenarioFile;
}
