/*********************************************************************
* Copyright (c) 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef OPENSCENARIOWRITER_H
#define OPENSCENARIOWRITER_H

#include <iostream>
#include <QDir>
#include "DataStructuresXosc/XoscScenario.h"

class OpenScenarioWriter
{
public:
    OpenScenarioWriter() = default;
    virtual ~OpenScenarioWriter() = default;

    const QString CreateScenarioFile(QString &scenarioFile,
                                     std::vector<PCM_Trajectory *> &trajectories);

private:
    const QString WriteScenarioFile(QString &scenarioFile,
                                    XoscScenario &scenarioConfig);
};

#endif // OPENSCENARIOWRITER_H
