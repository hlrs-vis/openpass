/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef XMLCOMPONENTSIGNAL_H
#define XMLCOMPONENTSIGNAL_H

#include "XmlBaseClass.h"

class XmlComponentSignal: public XmlBaseClass
{
public:
    XmlComponentSignal(int id, bool isInput, QString name, int channelRef);

    bool WriteToXml( QXmlStreamWriter *xmlWriter );

private:
    bool isInput;      //!< boolean defining whether signal is an input or not (=output)
    QString name;      //!< name of the signal
    int channelRef;    //!< channel reference (its id) of the signal
};

#endif // XMLCOMPONENTSIGNAL_H
