/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

//-----------------------------------------------------------------------------
//! @file  RandomInterface.h
//! @brief ...
//!
//-----------------------------------------------------------------------------

#ifndef RANDOMINTERFACE_H
#define RANDOMINTERFACE_H

#include <QtGlobal>
#include <QDateTime>

class RandomInterface
{
public:
    explicit RandomInterface() = delete;
    virtual ~RandomInterface() = default;

public:
    static void initialize()
    {
        qsrand(QDateTime::currentMSecsSinceEpoch());
    }

    static double random(double const & minimum = 0.0,
                         double const & maximum = 1.0)
    {
        return (static_cast<double>(qrand()) / RAND_MAX * (maximum - minimum)) + minimum;
    }
};

#endif // RANDOMINTERFACE_H
