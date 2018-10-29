/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef XMLLINE_H
#define XMLLINE_H

#include "pcm_line.h"
#include "XmlPoint.h"

class XmlLine : public XmlBaseClass
{
public:
    XmlLine() = default;
    XmlLine(const PCM_Line *line);

    bool WriteToXml( QXmlStreamWriter *xmlWriter );

private:
    const PCM_Line *line = nullptr;              //!< PCM line
};

#endif // XMLLINE_H
