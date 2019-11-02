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

#include <QDomDocument>

#include "Interfaces/parameterInterface.h"
#include "xmlParser.h"

namespace SimulationCommon {

class ParameterImporter
{
public:
    static bool ImportBoolParameters(
        QDomElement componentParameterSetElement,
        ParameterInterface& componentParameterSet);

    static bool ImportIntParameters(
        QDomElement componentParameterSetElement,
        ParameterInterface& componentParameterSet);

    static bool ImportIntVectorParameters(
        QDomElement componentParameterSetElement,
        ParameterInterface& componentParameterSet);

    static bool ImportDoubleParameters(
        QDomElement componentParameterSetElement,
        ParameterInterface& componentParameterSet);

    static bool ImportDoubleVectorParameters(
        QDomElement componentParameterSetElement,
        ParameterInterface& componentParameterSet);

    static bool ImportStringParameters(
        QDomElement componentParameterSetElement,
        ParameterInterface& componentParameterSet);

    static bool ImportNormalDistributionParameters(
        QDomElement componentParameterSetElement,
        ParameterInterface& componentParameterSet);

    static bool ImportParameterLists(
        QDomElement componentParameterSetElement,
        ParameterInterface& componentParameterSet);

    static bool ImportParameters(
        QDomElement componentParameterSetElement,
        ParameterInterface& componentParameterSet);
};

} //namespace SimulationCommon
