/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <iostream>

#include "csvParser.h"

CSVParser::CSVParser()
{
}

CSVParser::~CSVParser()
{
}

void CSVParser::ParseFile(std::string file)
{
    const char* file_c_str = file.c_str();
    std::ifstream infile(file_c_str);

    std::string line;

    while (std::getline(infile, line))
    {
        if(line.back() == '\r')
        {
            line.pop_back();
        }

        std::vector<std::string> lineData = ParseLine(line);
        table.push_back(lineData);
    }
}

std::vector<std::string> CSVParser::ParseLine(std::string line)
{
    std::vector<std::string> lineData;

   std::stringstream ss;
   ss.str(line);
   std::string item;
   while (std::getline(ss, item, ';'))
   {
       lineData.push_back(item);
   }

   return lineData;
}

int CSVParser::GetNumberOfLines()
{
    return table.size();
}

std::string CSVParser::GetEntryAt(int row, int column)
{
    std::vector<std::string> rowEntry = table.at(row);

    if(rowEntry.size() <= (size_t)column) return "";
    else return rowEntry.at(column);
}
