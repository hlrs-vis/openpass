/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#include "dllHelper.h"

//-----------------------------------------------------------------------------
//! Helper function evaluating the dll's HMODULE (= base address in memory)
//! inspired by http://stackoverflow.com/questions/557081/how-do-i-get-the-hmodule-for-the-currently-executing-code/557859#557859
//!
//! @return                    HMODULE of the calling dll
//-----------------------------------------------------------------------------
HMODULE DllHelper::getCurrentModule()
{
    HMODULE hModule = NULL;
    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                      GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                      (LPCTSTR)getCurrentModule,
                      &hModule);

    return hModule;
}

//-----------------------------------------------------------------------------
//! Evaluates the path where the calling *.dll file lies in
//!
//! @return                    Path of the dll as a QString
//-----------------------------------------------------------------------------
QString DllHelper::GetQPathOfDll()
{
    HMODULE hmod = getCurrentModule();
    TCHAR currentDllName[MAX_PATH + 1];
    GetModuleFileName(hmod, currentDllName, MAX_PATH);

    QFileInfo fileInfo(QString::fromStdWString(currentDllName));
    return fileInfo.path();
}

//-----------------------------------------------------------------------------
//! Evaluates the path where the calling *.dll file lies in
//!
//! @return                    Path of the dll as an std::string
//-----------------------------------------------------------------------------
const std::string DllHelper::GetPathOfDll()
{
    return GetQPathOfDll().toStdString();
}
