/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef PROJECTXMLLOADMODEL_H
#define PROJECTXMLLOADMODEL_H

#include "Models/ProjectXMLBaseModel.h"
#include "Models/ProjectModel.h"

class QIODevice;
class QXmlStreamReader;

class ProjectXMLLoadModel : protected ProjectXMLBaseModel
{
public:
    explicit ProjectXMLLoadModel() = delete;
    virtual ~ProjectXMLLoadModel() = default;

public:
    static bool load(QString const & filepath,
                     ProjectModel * const project);
    static bool load(QIODevice * const device,
                     ProjectModel * const project);

protected:
    static bool loadProject(QXmlStreamReader & xml,
                            ProjectModel * const project);
};

#endif // PROJECTXMLLOADMODEL_H
