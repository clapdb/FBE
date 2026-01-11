/*
 * Copyright (C) STDB Holdings Limited
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * This file is part of FBE (Fast Binary Encoding).
 *
 * FBE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FBE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with FBE. If not, see <https://www.gnu.org/licenses/>.
 */
/*!
    \file generator.cpp
    \brief Fast binary encoding generator implementation
    \author Ivan Shynkarenka
    \date 20.04.2018
    \copyright AGPL-3.0-or-later
*/

#include "generator.h"

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#include <windows.h>
#endif

namespace FBE {

void Generator::Store(const fs::path& filename)
{
    std::string previous;

    // Try to read the previous file content
    /*if (fs::exists(filename))
    {
        CppCommon::File file = filename;
        file.Open(true, false);
        previous = file.ReadAllText();
        file.Close();
    }*/
    previous = read_all(filename);

    // Compare the buffer with previous content in order to store only new content
    if (_buffer == previous)
        return;

    // Store the buffer into unique file
//    fs::path unique = filename.parent_path() / fs::path::unique();
    fs::path unique = filename.parent_path() / unique_name();
    write_all(unique, _buffer);

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
    // Sometimes it happens that Visual Studio opens generated files on a fly to analyze.
    // It opens them with mapping into its process with CreateFileMapping/MapViewOfFile.
    // As the result files cannot be replaced or removed. Therefore we need to have some
    // retry attempts with small delay to give a change for the file to be created.
    // https://stackoverflow.com/questions/41844842/when-error-1224-error-user-mapped-file-occurs
    const int attempts = 1000;
    const int sleep = 100;
    for (int attempt = 0; attempt < attempts; ++attempt)
    {
        try
        {
            // Rename the unique file inside a loop with retries
            fs::path::Rename(unique, filename);
            return;
        }
        catch (const CppCommon::FileSystemException& ex)
        {
            if ((ex.system_error() == ERROR_ACCESS_DENIED) || (ex.system_error() == ERROR_USER_MAPPED_FILE))
            {
                CppCommon::Thread::Sleep(sleep);
                continue;
            }
            throw;
        }
    }
    throwex CppCommon::FileSystemException("Cannot generate the output file!").Attach(filename);
#else
    // Rename the unique file inside a loop with retries
    fs::rename(unique, filename);
#endif
}

bool Generator::IsVariantType(const std::shared_ptr<Package>& p, const std::string& type) {
    auto& variants = p->body->variants;
    return std::find_if(variants.begin(), variants.end(), [&type](const auto& v) -> bool {
        return *v->name == type;
    }) != variants.end();
}

std::shared_ptr<VariantType> Generator::GetVariantType(const std::shared_ptr<Package>& p, const std::string& type) {
    auto& variants = p->body->variants;
    if (auto iter =  std::find_if(variants.begin(), variants.end(), [&type](const auto& v) -> bool {
        return *v->name == type;
    });iter != variants.end()) {
        return *iter;
    }
    return nullptr;
}

} // namespace FBE
