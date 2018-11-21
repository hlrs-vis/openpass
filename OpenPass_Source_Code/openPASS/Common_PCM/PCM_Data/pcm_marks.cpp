/*********************************************************************
* Copyright (c) 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "pcm_marks.h"

PCM_Marks::PCM_Marks()
{
    markType = MarkType::NONE;
}

PCM_Marks::PCM_Marks(MarkType markType):
    markType(markType)
{}

bool PCM_Marks::SetMarkType(MarkType markType)
{
    this->markType = markType;
    return true;
}

MarkType PCM_Marks::GetMarkType() const
{
    return markType;
}
