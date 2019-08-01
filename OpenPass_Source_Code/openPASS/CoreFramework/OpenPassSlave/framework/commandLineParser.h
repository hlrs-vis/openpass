/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#pragma once

#include <QStringList>
#include <QCoreApplication>
#include "QCommandLineParser"

struct CommandLineArguments
{
    int logLevel;
    std::string libPath;
    std::string logFile;
    std::string configsPath;
    std::string resultsPath;
};

struct CommandLineOption
{
    QString name;
    QString description;
    QString valueName;
    QString defaultValue;
};

class CommandLineParser
{
public:
    static CommandLineArguments Parse(const QStringList& arguments);
    static std::list<std::string> GetParsingLog();
private:
    static QList<QCommandLineOption> GetOptions();
    static CommandLineArguments CompileCommandLineArguments(const QCommandLineParser& commandLineParser);
    static void EvaluateDefaultedValues(const QCommandLineParser& commandLineParser);

    static std::list<std::string> parsingLog;
    static const std::list<CommandLineOption> commandLineOptions;
};
