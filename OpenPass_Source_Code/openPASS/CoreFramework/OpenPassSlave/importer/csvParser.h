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

#include <vector>
#include <string>

class CSVParser
{
public:
    CSVParser();
    ~CSVParser();

    void ParseFile(std::string file);

    int GetNumberOfLines();

    std::string GetEntryAt(int row,int column);

private:
    std::vector<std::string> ParseLine(std::string line);
    std::vector< std::vector<std::string> > table;
};
