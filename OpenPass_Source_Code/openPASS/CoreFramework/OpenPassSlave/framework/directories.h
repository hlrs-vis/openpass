/*******************************************************************************
* Copyright (c) 2018, 2019 in-tech GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*******************************************************************************/

#pragma once

#include <string>

/// Directories takes care of a consistent handling of paths and directories
/// without making the underlying implementation public or a dependency
///
/// Once initialized, the paths are resolved w.r.t the applications base directory
/// if the paths are relative, or kept absolute
///
/// Note that all directories are returned without a trailing path seperator
/// Use Concat if you need to concat a path and a file with the current systems seperator
class Directories
{
public:
    Directories(const std::string& applicationDir,
                const std::string& libraryDir,
                const std::string& configurationDir,
                const std::string& outputDir);

    // This class should not be moved or assigned,
    // as there should be only one instance throughout the system
    Directories() = delete;
    Directories(const Directories&) = delete;
    Directories(Directories&&) = delete;
    Directories& operator=(Directories&&) = delete;

    const std::string baseDir;              ///!< path of the executed application
    const std::string configurationDir;     ///!< directory of the configuration files
    const std::string libraryDir;           ///!< directory of the libraries
    const std::string outputDir;            ///!< directory for outputs

    /// \brief  Concats a path and a file with the seperator used by the current system
    /// \param  path     e.g. /the_path
    /// \param  file     e.g. the_file
    /// \return Concaternated string, e.g. /the_path/the_file
    ///
    static const std::string Concat(const std::string& path, const std::string& file);
private:

    /// \brief  Internally used to get a cononical path w.r.t to the application path
    /// \param  applicationPath  absolute path of the executed file, e.g. /baseDir
    /// \param  path             relative or absolute path, e.g. path or /path
    /// \return Resolved string  e.g. /baseDir/path or /path, respectively
    ///
    static const std::string Resolve(const std::string& applicationPath, const std::string& path);
};

