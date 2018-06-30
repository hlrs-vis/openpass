/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include "pcm_course.h"

PCM_Course::PCM_Course(int betNr)
{
    this->betNr = betNr;
}

int PCM_Course::GetBetNr() const
{
    return betNr;
}
