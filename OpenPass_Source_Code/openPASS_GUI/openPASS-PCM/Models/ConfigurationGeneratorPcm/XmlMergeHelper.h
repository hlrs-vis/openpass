/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef XMLMERGEHELPER_H
#define XMLMERGEHELPER_H

#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <iostream>

class XmlMergeHelper
{
public:
    XmlMergeHelper() = delete;
    XmlMergeHelper(const XmlMergeHelper &) = delete;
    XmlMergeHelper(XmlMergeHelper &&) = delete;
    XmlMergeHelper &operator=(const XmlMergeHelper &) = delete;
    XmlMergeHelper &operator=(XmlMergeHelper &&) = delete;
    virtual ~XmlMergeHelper() = delete;

    static bool loadDomDocument(QString filename, QDomDocument &document);
    static bool setIdOfSystem(QDomElement &root, int id);
};

#endif // XMLMERGEHELPER_H
