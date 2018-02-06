#include "XmlMarks.h"

XmlMarks::XmlMarks(PCM_Marks *marks)
{
    this->marks = marks;
}

bool XmlMarks::WriteToXml(QXmlStreamWriter *xmlWriter)
{
    if (marks != nullptr)
    {
        xmlWriter->writeStartElement(QString::fromStdString(PCM_Helper::ConvertMarkTypeToDBString(
                                                                marks->GetMarkType())));
        const std::map<int, PCM_Line *> *lineMap = marks->GetLineMap();
        for (std::pair<int, PCM_Line *> linePair : *lineMap)
        {
            PCM_Line *line = linePair.second;
            XmlLine xmlLine(line);
            xmlLine.WriteToXml(xmlWriter);
        }
        xmlWriter->writeEndElement();
    }

    return true;
}
