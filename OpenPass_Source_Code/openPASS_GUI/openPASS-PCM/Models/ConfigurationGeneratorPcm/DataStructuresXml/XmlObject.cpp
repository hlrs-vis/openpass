#include "XmlObject.h"

XmlObject::XmlObject(PCM_Object *object)
{
    this->object = object;
}

bool XmlObject::WriteToXml(QXmlStreamWriter *xmlWriter)
{
    if (object != nullptr)
    {
        xmlWriter->writeStartElement(QString::fromStdString(PCM_Helper::ConvertObjectTypeToDBString(
                                                                ObjectType::OBJECT)));

        const std::map<int, PCM_Line *> *lineMap = object->GetLineMap();
        for (std::pair<int, PCM_Line *> linePair : *lineMap)
        {
            PCM_Line *line = linePair.second;
            XmlLine xmlLine(line);
            xmlLine.WriteToXml(xmlWriter);
        }
        xmlWriter->writeTextElement("ObjectType", QString::number(object->GetObjectType()));

        xmlWriter->writeEndElement();
    }

    return true;
}
