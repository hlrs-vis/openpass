/*******************************************************************************
* Copyright (c) 2019 in-tech GmbH
*               2017, 2018, 2019 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include <QApplication>
#include <iostream>
#include <sstream>
#include <string>
#include <QDomDocument>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <optional>

#include "masterConfig.h"
#include "masterConfigImporter.h"
#include "xmlParser.h"
#include "slaveConfig.h"

namespace SimulationMaster {
namespace Configuration {

template <typename T>
///
/// \brief Gets the value of a given tag
/// \param element root element
/// \param tag     tag
/// \return value if tag was found, std::nullopt else
///
std::optional<T> GetValue(QDomElement element, const std::string& tag)
{
    QDomNode node = element.firstChildElement(QString::fromStdString(tag));
    if (!node.isNull())
    {
        T result;
        if (SimulationCommon::Parse<T>(element, tag, result))
        {
            return std::make_optional<T>(result);
        }
    }
    return std::nullopt;
}

/// \brief Helper method to dig into the xml tree
/// <a><b><c> => b = get(a), c = get(b), etc...
/// If tag is not available, a runtime_error is issued
///
/// \param[in] element root
/// \param[in] tag     tag of child
/// \return child element
QDomElement GetChildOrThrow(const QDomElement& element, const std::string tag)
{
    QDomElement child;
    SimulationCommon::GetFirstChildElement(element, tag, child);
    if (child.isNull())
    {
        throw std::runtime_error("Missing tag " + tag);
    }
    return child;
}

MasterConfig MasterConfigImporter::Import(const QString& filename)
{
    std::locale::global(std::locale("C"));

    // file handling
    QString absoluteFilePath = GetAbsoluteFilePath(filename);
    QFile xmlFile(absoluteFilePath);
    QDomElement document = ReadDocument(xmlFile, absoluteFilePath.toStdString());

    return MasterConfig
    {
        GetValue<int>(document, "logLevel"),
        GetValue<std::string>(document, "logFileMaster"),
        GetValue<std::string>(document, "slave"),
        GetValue<std::string>(document, "libraries"),
        ParseSlaveConfigs(document)
    };
}

SlaveConfigs MasterConfigImporter::ParseSlaveConfigs(const QDomElement& element)
{
    auto slaveConfigRoot = GetChildOrThrow(element, "slaveConfigs");

    const std::string slaveConfigEntryTag {"slaveConfig"};
    auto slaveConfigEntry = GetChildOrThrow(slaveConfigRoot, slaveConfigEntryTag);

    SlaveConfigs slaveConfigs;
    while (!slaveConfigEntry.isNull())
    {
        slaveConfigs.emplace_back(ParseSlaveConfig(slaveConfigEntry));
        slaveConfigEntry = slaveConfigEntry.nextSiblingElement(QString::fromStdString(slaveConfigEntryTag));
    }

    return slaveConfigs;
}

SlaveConfig MasterConfigImporter::ParseSlaveConfig(const QDomElement& element)
{
    return SlaveConfig
    {
        GetValue<std::string>(element, "logFileSlave"),
        GetValue<std::string>(element, "configurations"),
        GetValue<std::string>(element, "results")
    };
}

QDomElement MasterConfigImporter::ReadDocument(QFile& xmlFile, const std::string& file)
{
    if (!xmlFile.open(QIODevice::ReadOnly))
    {
        throw std::runtime_error("Could not open file " + file);
    }

    QByteArray xmlData(xmlFile.readAll());
    QDomDocument document;
    QString errorMsg;
    int errorLine;

    if (!document.setContent(xmlData, &errorMsg, &errorLine))
    {
        throw std::runtime_error("Invalid xml file: " + file + "(" + std::to_string(errorLine) + ")");
    }

    QDomElement documentRoot = document.documentElement();
    if (documentRoot.isNull())
    {
        throw std::runtime_error("Could not find xml root in " + file);
    }

    return documentRoot;
}

QString MasterConfigImporter::GetAbsoluteFilePath(const QString& filename)
{
    QString absoluteFilePath = "";

    if (QFileInfo(filename).isRelative())
    {
        QDir baseDir = QCoreApplication::applicationDirPath();
        absoluteFilePath = baseDir.absoluteFilePath(filename);
        absoluteFilePath = baseDir.cleanPath(absoluteFilePath);
    }
    else
    {
        absoluteFilePath = filename;
    }

    if (!QFileInfo(absoluteFilePath).exists())
    {
        throw std::runtime_error(absoluteFilePath.toStdString() + " does not exist");
    }

    return absoluteFilePath;
}


} // namespace Configuration
} // namespace SimulationMaster
