/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Models/SystemComponentManagerModel.h"

#include <QList>
#include <QString>

SystemComponentManagerModel::SystemComponentManagerModel(ComponentInterface * const component,
                                                         QObject * const parent)
    : SystemComponentManagerInterface(parent)
    , components(component->createComponentMap(this))
{
}

bool SystemComponentManagerModel::loadFromDirectory(QDir const & directory)
{
    // Load the component file names and remove the xml extension
    QList<QString> componentFiles = directory.entryList({"*.xml"},QDir::Files | QDir::Readable, QDir::Name);

    // Load the components and update the component Map
    QList<QString> componentList = {};
    for (QString filename : componentFiles)
    {
        components->add(filename.remove(filename.count() - 4, 4),
                        directory.absoluteFilePath(filename));
        componentList.insert(0,filename);
    }

    // Remove components no longer existing
    QList<QString> names = {};
    for (ComponentMapInterface::Item * const component : *components)
    {
        if (!componentList.contains(component->getName()))
        {
            names.insert(0, component->getName());
        }
    }
    if (!names.isEmpty())
    {
        for (QString name : names)
        {
            components->remove(name);
        }
    }

    return true;
}

QList<ComponentItemInterface::Title> SystemComponentManagerModel::listTitlesByType(ComponentItemInterface::Type const & type) const
{
    QList<ComponentItemInterface::Title> titles;
    for (ComponentMapInterface::Item * const item : *components)
    {
        if (item->getType() == type)
        {
            titles << item->getTitle();
        }
    }
    return titles;
}

ComponentItemInterface * SystemComponentManagerModel::lookupItemByName(ComponentItemInterface::Name const & name) const
{
    for (ComponentMapInterface::Item * const item : *components)
    {
        if (item->getName() == name)
        {
            return item;
        }
    }
    return nullptr;
}

ComponentItemInterface * SystemComponentManagerModel::lookupItemByTitle(ComponentItemInterface::Title const & title) const
{
    for (ComponentMapInterface::Item * const item : *components)
    {
        if (item->getTitle() == title)
        {
            return item;
        }
    }
    return nullptr;
}
