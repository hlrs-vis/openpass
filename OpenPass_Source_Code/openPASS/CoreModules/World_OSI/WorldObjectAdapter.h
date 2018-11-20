/******************************************************************************
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

#pragma once

namespace WorldObjectCommon {

double GetFrontDeltaS(double length, double width, double hdg, double distanceReferencePointToLeadingEdge);
double GetRearDeltaS(double length, double width, double hdg, double distanceReferencePointToLeadingEdge);

} // namespace WorldObjectCommon
