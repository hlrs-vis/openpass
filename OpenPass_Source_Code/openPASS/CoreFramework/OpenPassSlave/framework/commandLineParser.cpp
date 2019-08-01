/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include "commandLineParser.h"
#include "QCommandLineParser"
#include "QCoreApplication"
#include <algorithm>
#include <iostream>

CommandLineArguments CommandLineParser::Parse(const QStringList& arguments)
{
    QCommandLineParser commandLineParser;
    commandLineParser.addOptions(GetOptions());
    commandLineParser.process(arguments);
    EvaluateDefaultedValues(commandLineParser);
    return CompileCommandLineArguments(commandLineParser);
}

std::list<std::string> CommandLineParser::GetParsingLog()
{
    return parsingLog;
}

QList<QCommandLineOption> CommandLineParser::GetOptions()
{
    QList<QCommandLineOption> options;
    std::transform(begin(commandLineOptions), end(commandLineOptions),
                   std::back_inserter(options), [](const CommandLineOption & o)
    {
        return QCommandLineOption {o.name, o.description, o.valueName, o.defaultValue};
    });
    return options;
}

CommandLineArguments CommandLineParser::CompileCommandLineArguments(const QCommandLineParser& commandLineParser)
{
    CommandLineArguments parsedArguments;

    parsedArguments.logLevel = commandLineParser.value("logLevel").toInt();
    parsedArguments.logFile = commandLineParser.value("logFile").toStdString();
    parsedArguments.libPath = commandLineParser.value("lib").toStdString();
    parsedArguments.configsPath = commandLineParser.value("configs").toStdString();
    parsedArguments.resultsPath = commandLineParser.value("results").toStdString();

    return parsedArguments;
}

void CommandLineParser::EvaluateDefaultedValues(const QCommandLineParser& commandLineParser)
{
    parsingLog.clear();
    for (const auto& option : commandLineOptions)
    {
        if (!commandLineParser.isSet(option.name))
        {
            parsingLog.push_back("No value supplied for " + option.name.toStdString()
                                 + ", falling back to default value " + option.defaultValue.toStdString());
        }
    }
}

std::list<std::string> CommandLineParser::parsingLog {};

/* Put all information for command line options here.
 *
 * PopulateCommandLineParser() will convert this information into QCommandLineOptions
 * and add them to the command line parser.
 *
 * Syntax:
 * 1) name used on the command line for this flag (e.g "-o", "--output")
 * 2) description - a description of what the flag does
 * 3) valueName - REQUIRED for options that take an input value;
 * 4) defaultValue
 *
 * Don't forget to update test GivenNoValues_SetDefaultsAndLogsEntryForEachDefaultedValue too
 */
const std::list<CommandLineOption> CommandLineParser::commandLineOptions
{
    {
        "logLevel",
        "Log Level (0 - 5)",
        "logLevel",
        "0"
    },
    {
        "logFile",
        "Path to the log file",
        "logFilePath",
        "OpenPassSlave.log"
    },
    {
        "lib",
        "Root path of the libraries",
        "libraryPath",
        "lib"
    },
    {
        "configs",
        "Path where to retrieve configuration files",
        "resultPath",
        "configs"
    },
    {
        "results",
        "Path where to put result files",
        "resultPath",
        "results"
    }
};
