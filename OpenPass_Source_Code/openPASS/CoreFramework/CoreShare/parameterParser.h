/*********************************************************************
* Copyright (c) 2017 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

//-----------------------------------------------------------------------------
//! @file  parameterParser.h
//! @brief This file contains helper functions to parse the configuration
//!        parameters.
//-----------------------------------------------------------------------------

#ifndef PARAMETERPARSER_H
#define PARAMETERPARSER_H

#include <QDomDocument>
#include "parameters.h"

namespace SimulationCommon
{

extern bool ImportParameters(QDomElement &parametersElement, Parameters &parameters);
extern bool ImportSystemParameters(QDomElement &parametersElement, Parameters &parameters);

} // namespace SimulationCommon

#endif // PARAMETERPARSER_H
