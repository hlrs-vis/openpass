/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

//-----------------------------------------------------------------------------
//! @file  dllHelper.h
//! @brief Interface-file for the static class DllHelper
//!
//! This class contains some functionality to handle the dynamic libraries.
//-----------------------------------------------------------------------------

#ifndef DLL_HELPER_H
#define DLL_HELPER_H

#include <string>
#include <windows.h>
#include <QFileInfo>

//-----------------------------------------------------------------------------
//! Class containing some functionality to handle the dynamic libraries
//-----------------------------------------------------------------------------
class DllHelper
{
public:
    //-----------------------------------------------------------------------------
    //! Evaluates the path where the calling *.dll file lies in
    //!
    //! @return                    Path of the dll as a QString
    //-----------------------------------------------------------------------------
    static QString GetQPathOfDll();

    //-----------------------------------------------------------------------------
    //! Evaluates the path where the calling *.dll file lies in
    //!
    //! @return                    Path of the dll as an std::string
    //-----------------------------------------------------------------------------
    static const std::string GetPathOfDll();

private:
    //-----------------------------------------------------------------------------
    //! Helper function evaluating the dll's HMODULE (= base address in memory)
    //!
    //! @return                    HMODULE of the calling dll
    //-----------------------------------------------------------------------------
    static HMODULE getCurrentModule();
};

#endif // DLL_HELPER_H
