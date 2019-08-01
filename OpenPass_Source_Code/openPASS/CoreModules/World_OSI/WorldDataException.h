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
//! @file  WorldDataException.h
//! @brief This file provides exceptions for the OWL
//-----------------------------------------------------------------------------

#pragma once

#include <exception>
#include "OWL/DataTypes.h"

namespace OWL
{

class NonExistentOsiRef : public std::exception
{
public:
    NonExistentOsiRef(const std::string& elementType, const std::string& data);

    const char* what() const noexcept override;

private:
    const std::string& elementType;
    const std::string& data;
};

} // namespace OWL
