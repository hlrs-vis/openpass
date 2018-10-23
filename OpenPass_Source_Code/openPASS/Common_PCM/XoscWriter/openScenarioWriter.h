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
