#ifndef XMLOBSERVATION_H
#define XMLOBSERVATION_H

#include "XmlParameter.h"

class XmlObservation: public XmlBaseClass
{
public:
    XmlObservation( int id, QString library );

    bool WriteToXml( QXmlStreamWriter *xmlWriter );

    void AddParameter( int id, XML_PARAMETER_TYPE type, QString name, QString value );

private:
    QString library;               //!< library of the observation module
    std::vector<XmlParameter>
    observationParameters;    //!< vector of all parameters of the observation module
};

#endif // XMLOBSERVATION_H
