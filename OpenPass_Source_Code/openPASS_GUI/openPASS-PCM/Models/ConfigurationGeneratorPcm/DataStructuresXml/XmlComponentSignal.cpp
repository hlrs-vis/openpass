#include "XmlComponentSignal.h"

XmlComponentSignal::XmlComponentSignal(int id, bool isInput, QString name, int channelRef):
    XmlBaseClass(id), isInput(isInput), name(name), channelRef(channelRef)
{}

bool XmlComponentSignal::WriteToXml(QXmlStreamWriter *xmlWriter)
{
    if (isInput)
    {
        xmlWriter->writeStartElement("ComponentInput");
    }
    else
    {
        xmlWriter->writeStartElement("ComponentOutput");
    }
    xmlWriter->writeAttribute("id", QString::number(_id));
    xmlWriter->writeAttribute("name", name);
    xmlWriter->writeAttribute("channelRef", QString::number(channelRef));

    xmlWriter->writeEndElement();

    return true;
}
