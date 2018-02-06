#include "XmlObservation.h"

XmlObservation::XmlObservation(int id, QString library):
    XmlBaseClass(id), library(library)
{}

void XmlObservation::AddParameter(int id, XML_PARAMETER_TYPE type, QString name, QString value)
{
    observationParameters.push_back( XmlParameter( id, type, name, value ) );
}

bool XmlObservation::WriteToXml(QXmlStreamWriter *xmlWriter)
{
    xmlWriter->writeStartElement("Observation");
    xmlWriter->writeAttribute( "id", QString::number(_id) );
    xmlWriter->writeAttribute( "library", library );

    xmlWriter->writeStartElement("ObservationParameters");
    for (XmlParameter parameter : observationParameters)
    {
        if ( !parameter.WriteToXml(xmlWriter) )
        {
            return false;
        }
    }
    xmlWriter->writeEndElement(); // ObservationParameters

    xmlWriter->writeEndElement(); // Observation

    return true;
}
