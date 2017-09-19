/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  parameterInterface.h
//! @brief This file contains the interface to retrieve the parameters.
//-----------------------------------------------------------------------------

#ifndef PARAMETERINTERFACE_H
#define PARAMETERINTERFACE_H

#include <string>
#include <vector>
#include <map>

//-----------------------------------------------------------------------------
//! Interface provides access to the configuration parameters
//-----------------------------------------------------------------------------
class ParameterInterface
{
public:
    ParameterInterface() = default;
    ParameterInterface(const ParameterInterface &) = delete;
    ParameterInterface(ParameterInterface &&) = delete;
    ParameterInterface &operator=(const ParameterInterface &) = delete;
    ParameterInterface &operator=(ParameterInterface &&) = delete;
    virtual ~ParameterInterface() = default;

    //-----------------------------------------------------------------------------
    //! Retrieves the parameters of type "Double"
    //!
    //! @return                Mapping of "id" to "value"
    //-----------------------------------------------------------------------------
    virtual const std::map<int, double> &GetParametersDouble() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves the parameters of type "Int"
    //!
    //! @return                Mapping of "id" to "value"
    //-----------------------------------------------------------------------------
    virtual const std::map<int, int> &GetParametersInt() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves the parameters of type "Bool"
    //!
    //! @return                Mapping of "id" to "value"
    //-----------------------------------------------------------------------------
    virtual const std::map<int, bool> &GetParametersBool() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves the parameters of type "String"
    //!
    //! @return                Mapping of "id" to "value"
    //-----------------------------------------------------------------------------
    virtual const std::map<int, const std::string> &GetParametersString() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves the parameters of type "DoubleVector"
    //!
    //! @return                Mapping of "id" to "value"
    //-----------------------------------------------------------------------------
    virtual const std::map<int, const std::vector<double>*> &GetParametersDoubleVector() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves the parameters of type "IntVector"
    //!
    //! @return                Mapping of "id" to "value"
    //-----------------------------------------------------------------------------
    virtual const std::map<int, const std::vector<int>*> &GetParametersIntVector() const = 0;

    //-----------------------------------------------------------------------------
    //! Retrieves the parameters of type "BoolVector"
    //!
    //! @return                Mapping of "id" to "value"
    //-----------------------------------------------------------------------------
    virtual const std::map<int, const std::vector<bool>*> &GetParametersBoolVector() const = 0;
};

#endif // PARAMETERINTERFACE_H
