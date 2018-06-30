#include "XmlMergeHelper.h"

bool XmlMergeHelper::loadDomDocument(QString filename, QDomDocument &document)
{
    std::locale::global(std::locale("C"));

    QFile xmlFile(filename); // automatic object will be closed on destruction
    if (!xmlFile.open(QIODevice::ReadOnly))
    {
        std::cout << "Error (XmlGenerator): could not open xmlFile: " << filename.toStdString() <<
                  std::endl;
        return false;
    }

    if (!document.setContent(&xmlFile))
    {
        std::cout << "Error (XmlGenerator): failed to parse file into dom tree" << std::endl;
        return false;
    }
    xmlFile.close();

    return true;
}

bool XmlMergeHelper::setIdOfSystem(QDomElement &root, int id)
{
    QDomElement systemElement = root.firstChildElement("system");
    if (systemElement.isNull())
    {
        return false;
    }
    QDomElement idElement = systemElement.firstChildElement("id");
    if (idElement.isNull())
    {
        return false;
    }
    idElement.firstChild().setNodeValue(QString::number(id));

    QDomElement priorityElement = systemElement.firstChildElement("priority");
    if (priorityElement.isNull())
    {
        return false;
    }
    priorityElement.firstChild().setNodeValue(QString::number(id));

    return true;
}
