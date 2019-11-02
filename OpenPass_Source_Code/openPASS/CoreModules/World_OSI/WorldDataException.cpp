/*******************************************************************************
* Copyright (c) 2017, 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  WorldDataException.cpp
//! @brief This file provides exceptions for the OWL
//-----------------------------------------------------------------------------

#include <exception>

#include "WorldDataException.h"

using namespace OWL;

NonExistentOsiRef::NonExistentOsiRef(const std::string& elementType, const std::string& data)
    : elementType(elementType),
      data(data)
{

}

const char* NonExistentOsiRef::what() const noexcept
{
    std::string err("tried to access a non-existent OSI object for '");

    err += elementType + "' id " + data;

    return err.c_str();
}

