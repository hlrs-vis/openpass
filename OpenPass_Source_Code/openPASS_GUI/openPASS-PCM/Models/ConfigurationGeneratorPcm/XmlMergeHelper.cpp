/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "XmlMergeHelper.h"

bool XmlMergeHelper::loadDomDocument(QString filename, QDomDocument &document)
{
    std::locale::global(std::locale("C"));

    QFile xmlFile(filename); // automatic object will be closed on destruction
    if (!xmlFile.open(QIODevice::ReadOnly))
    {
        std::cout << "Error (XmlGenerator): could not open xmlFile: " << filename.toStdString() <<
                  std::endl;
        return false;
    }

    document.setContent(&xmlFile);

    xmlFile.close();

    return true;
}

bool XmlMergeHelper::setIdOfSystem(QDomElement &root, int id)
{
    QDomElement systemElement = root.firstChildElement("system");
    if (systemElement.isNull())
    {
        return false;
    }
    QDomElement idElement = systemElement.firstChildElement("id");
    if (idElement.isNull())
    {
        return false;
    }
    idElement.firstChild().setNodeValue(QString::number(id));

    QDomElement priorityElement = systemElement.firstChildElement("priority");
    if (priorityElement.isNull())
    {
        return false;
    }
    priorityElement.firstChild().setNodeValue(QString::number(id));

    return true;
}
