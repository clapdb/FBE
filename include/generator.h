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
    \file generator.h
    \brief Fast binary encoding generator definition
    \author Ivan Shynkarenka
    \date 20.04.2018
    \copyright AGPL-3.0-or-later
*/

#ifndef GENERATOR_H
#define GENERATOR_H

#include "fbe.h"

namespace FBE {

class Generator
{
public:
    Generator(const std::string& input, const std::string& output, int indent, char space);

    virtual void Generate(const std::shared_ptr<Package>& package) = 0;

protected:
    std::string _buffer;
    std::string _input;
    std::string _output;
    int _cursor{0};
    int _indent;
    char _space;

    void Indent(int count);

    void WriteBegin();
    void Write(const std::string& str);
    void WriteIndent();
    void WriteIndent(const std::string& str);
    void WriteLine();
    void WriteLine(const std::string& str);
    void WriteLineIndent(const std::string& str);
    void WriteEnd();

    void Store(const fs::path& filename);

    // helper functions
    bool IsVariantType(const std::shared_ptr<Package>& p, const std::string& type);
    std::shared_ptr<VariantType> GetVariantType(const std::shared_ptr<Package>& p, const std::string& type);

    static std::string EndLine();
};

} // namespace FBE

#include "generator.inl"

#endif // GENERATOR_H
