/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/SystemModel.h"

#include "openPASS-Project/ProjectInterface.h"
#include "Models/SystemComponentManagerModel.h"
#include "Models/SystemMapModel.h"
#include "Models/SystemXMLLoadModel.h"
#include "Models/SystemXMLSaveModel.h"

SystemModel::SystemModel(ComponentInterface * const component,
                         ProjectInterface * const project,
                         QObject * const parent)
    : SystemInterface(parent)
    , components(new SystemComponentManagerModel(component, this))
    , _project(project)
    , systems(new SystemMapModel(this))
{
    // Load components
    components->loadFromDirectory(_project->getLibrary());
}

bool SystemModel::clear()
{
    if (systems->clear())
    {
        Q_EMIT cleared();
        return true;
    }
    return false;
}

bool SystemModel::load(QString const & filepath)
{
    clear();
    if (SystemXMLLoadModel::load(filepath, *systems->begin(), components))
    {
        Q_EMIT loaded();
        return true;
    }
    return false;
}

bool SystemModel::load(QIODevice * const device)
{
    clear();
    if (SystemXMLLoadModel::load(device, *systems->begin(), components))
    {
        Q_EMIT loaded();
        return true;
    }
    return false;
}

bool SystemModel::save(QString const & filepath) const
{
    if (SystemXMLSaveModel::save(filepath, systems))
    {
        Q_EMIT saved();
        return true;
    }
    return false;
}

bool SystemModel::save(QIODevice * const device) const
{
    if (SystemXMLSaveModel::save(device, systems))
    {
        Q_EMIT saved();
        return true;
    }
    return false;
}

SystemComponentManagerInterface * SystemModel::getComponents() const
{
    return components;
}

SystemMapInterface * SystemModel::getSystems() const
{
    return systems;
}
