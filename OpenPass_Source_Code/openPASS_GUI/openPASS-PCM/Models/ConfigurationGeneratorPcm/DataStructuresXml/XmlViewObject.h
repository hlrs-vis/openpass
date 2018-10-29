/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef XMLVIEWOBJECT_H
#define XMLVIEWOBJECT_H

#include "pcm_viewObject.h"
#include "XmlLine.h"

class XmlViewObject: public XmlBaseClass
{
public:
    XmlViewObject() = default;
    XmlViewObject(PCM_ViewObject *viewObject);

    bool WriteToXml( QXmlStreamWriter *xmlWriter );

private:
    PCM_ViewObject *viewObject = nullptr;              //!< PCM viewObject
};

#endif // XMLVIEWOBJECT_H
