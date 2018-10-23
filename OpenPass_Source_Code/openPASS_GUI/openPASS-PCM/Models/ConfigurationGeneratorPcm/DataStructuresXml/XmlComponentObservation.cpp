#include "XmlComponentObservation.h"

XmlComponentObservation::XmlComponentObservation( int id, int observationRef ):
    XmlBaseClass(id), observationRef(observationRef)
{}

bool XmlComponentObservation::WriteToXml(QXmlStreamWriter *xmlWriter)
{
    xmlWriter->writeStartElement("ComponentObservation");

    xmlWriter->writeAttribute("id", QString::number(_id));
    xmlWriter->writeAttribute("observationRef", QString::number(observationRef));

    xmlWriter->writeEndElement();

    return true;
}
