/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef XMLCOMPONENT_H
#define XMLCOMPONENT_H

#include <vector>
#include "XmlComponentSignal.h"
#include "XmlComponentObservation.h"
#include "XmlParameter.h"

class XmlComponent: public XmlBaseClass
{
public:
    XmlComponent(int id, XML_COMPONENT_TYPE type, int priority, int offsetTime, int responseTime,
                 int cycleTime, QString library );

    bool WriteToXml( QXmlStreamWriter *xmlWriter );

    void AddInput(      int id, QString name, int channelRef );
    void AddOutput(     int id, QString name, int channelRef );
    void AddParameter(  int id, XML_PARAMETER_TYPE type, QString name, QString value );
    void AddObservation( int id, int observationRef );

private:
    XML_COMPONENT_TYPE type = XML_COMPONENT_TYPE::undefined;   //!< id of the component
    int priority = -1;     //!< priority of the component
    int offsetTime = -1;   //!< offset time of the component
    int responseTime = -1; //!< response time of component
    int cycleTime = -1;    //!< cycle time of the component
    QString library;       //!< library of the component

    std::vector<XmlComponentSignal>
    componentInputs;       //!< vector of all inputs of the component
    std::vector<XmlComponentSignal>
    componentOutputs;      //!< vector of all outputs of the component
    std::vector<XmlParameter>
    componentParameters;   //!< vector of all parameters of the component
    std::vector<XmlComponentObservation>
    componentObservations;  //!< vector of all referenced observation of the component
};

#endif // XMLCOMPONENT_H
