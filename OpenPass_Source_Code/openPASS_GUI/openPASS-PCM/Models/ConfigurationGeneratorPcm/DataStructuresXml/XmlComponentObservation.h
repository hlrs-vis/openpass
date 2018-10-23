#ifndef XMLCOMPONENTOBSERVATION_H
#define XMLCOMPONENTOBSERVATION_H

#include "XmlBaseClass.h"

class XmlComponentObservation: public XmlBaseClass
{
public:
    XmlComponentObservation( int id, int oberservationRef );

    bool WriteToXml( QXmlStreamWriter *xmlWriter );

private:
    int observationRef;     //!< id of the referenced observation module
};

#endif // XMLCOMPONENTOBSERVATION_H
