#include "XmlLine.h"

XmlLine::XmlLine(const PCM_Line *line)
{
    this->line = line;
}

bool XmlLine::WriteToXml(QXmlStreamWriter *xmlWriter)
{
    xmlWriter->writeStartElement("line");
    xmlWriter->writeAttribute("id", QString::number(line->GetId()));
    const std::map<int, const PCM_Point *> *pointMap = line->GetPointMap();
    for (const std::pair<const int, const PCM_Point *> pointPair : *pointMap)
    {
        const PCM_Point *point = pointPair.second;
        XmlPoint xmlPoint(point);
        xmlPoint.WriteToXml(xmlWriter);
    }

    xmlWriter->writeEndElement();

    return true;
}
