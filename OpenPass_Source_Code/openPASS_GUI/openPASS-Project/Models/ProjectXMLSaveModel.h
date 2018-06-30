/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef PROJECTXMLSAVEMODEL_H
#define PROJECTXMLSAVEMODEL_H

#include "Models/ProjectXMLBaseModel.h"
#include "Models/ProjectModel.h"

class QIODevice;
class QXmlStreamWriter;

class ProjectXMLSaveModel : protected ProjectXMLBaseModel
{
public:
    explicit ProjectXMLSaveModel() = delete;
    virtual ~ProjectXMLSaveModel() = default;

public:
    static bool save(QString const &filepath,
                     ProjectModel const * const project);
    static bool save(QIODevice * const device,
                     ProjectModel const * const project);

public:
    static void saveProject(QXmlStreamWriter & xml,
                            ProjectModel const * const project);
};

#endif // PROJECTXMLSAVEMODEL_H
