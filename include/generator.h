/*
* Copyright (C) 2020 Beijing Jinyi Data Technology Co., Ltd. All rights reserved.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/
/*!
    \file generator.h
    \brief Fast binary encoding generator definition
    \author Ivan Shynkarenka
    \date 20.04.2018
    \copyright MIT License
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
