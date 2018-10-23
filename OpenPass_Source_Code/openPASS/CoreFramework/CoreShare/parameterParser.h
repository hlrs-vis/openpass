/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

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
