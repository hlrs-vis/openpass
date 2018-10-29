/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef XMLOBJECT_H
#define XMLOBJECT_H

#include "pcm_object.h"
#include "XmlLine.h"

class XmlObject: public XmlBaseClass
{
public:
    XmlObject() = default;
    XmlObject(PCM_Object *object);

    bool WriteToXml( QXmlStreamWriter *xmlWriter );

private:
    PCM_Object *object = nullptr;              //!< PCM object
};
#endif // XMLOBJECT_H
