/*********************************************************************
* Copyright (c) 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "pcm_course.h"

PCM_Course::PCM_Course(int betNr)
{
    this->betNr = betNr;
}

int PCM_Course::GetBetNr() const
{
    return betNr;
}
