/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef XMLMARKS_H
#define XMLMARKS_H

#include "pcm_marks.h"
#include "XmlLine.h"

class XmlMarks: public XmlBaseClass
{
public:
    XmlMarks(PCM_Marks *marks);

    bool WriteToXml( QXmlStreamWriter *xmlWriter );

private:
    PCM_Marks *marks;              //!< PCM marks
};

#endif // XMLMARKS_H
