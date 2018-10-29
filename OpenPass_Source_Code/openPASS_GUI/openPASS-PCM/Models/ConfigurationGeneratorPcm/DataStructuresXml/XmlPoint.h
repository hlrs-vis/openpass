/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef XMLPOINT_H
#define XMLPOINT_H

#include "pcm_point.h"
#include "XmlBaseClass.h"

class XmlPoint : public XmlBaseClass
{
public:
    XmlPoint() = default;
    XmlPoint(const PCM_Point *point);

    bool WriteToXml( QXmlStreamWriter *xmlWriter );

private:
    const PCM_Point *point = nullptr;              //!< PCM point
};

#endif // XMLPOINT_H
