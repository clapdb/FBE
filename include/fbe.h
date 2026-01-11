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
    \file fbe.h
    \brief Fast binary encoding definitions
    \author Ivan Shynkarenka
    \date 11.04.2018
    \copyright AGPL-3.0-or-later
*/

#ifndef FBE_H
#define FBE_H

#include "version.h"

/*
#include "filesystem/directory.h"
#include "filesystem/file.h"
#include "string/string_utils.h"
#include "system/environment.h"
#include "system/stream.h"
#include "threads/file_lock.h"
#include "threads/thread.h"
*/

#include <filesystem>
#include <algorithm>
#include <cstdint>
#include <memory>
#include <regex>
#include <set>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

// file and directory section
namespace fs = std::filesystem;

inline auto unique_name() -> std::string  {
    using boost::lexical_cast;
    using boost::uuids::uuid;
    using boost::uuids::random_generator;
    return lexical_cast<std::string>((random_generator())());
}

inline void create_dir(const fs::path& path) {
    std::error_code code;
    if (fs::create_directories(path, code)) {
        fs::permissions(path, fs::perms::owner_all, code);
        return;
    }
    // the folder exists
}

inline auto read_all(const fs::path& path) -> std::string {
// Sanity check
    if (!std::filesystem::is_regular_file(path))
        return { };
    // Open the file
    // Note that we have to use binary mode as we want to return a string
    // representing matching the bytes of the file on the file system.
    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (!file.is_open())
        return { };
    // Read contents
    std::string content{std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>()};
    // Close the file
    file.close();
    return content;
}

inline void write_all(const fs::path& path, const std::string& content) {
    std::ofstream file(path, std::ios::out);
    file << content;
    file.close();
}

// string section
inline auto replace_all(std::string& inout, std::string_view what, std::string_view with) -> size_t
{
    if (what.empty())
        return 0;  // Avoid infinite loop with empty search string
    std::size_t count{};
    for (std::string::size_type pos{};
         inout.npos != (pos = inout.find(what.data(), pos, what.length()));
         pos += with.length(), ++count) {
        inout.replace(pos, what.length(), with.data(), with.length());
    }
    return count;
}
inline auto trim(const std::string &s) -> std::string
{
    auto wsfront=std::find_if_not(s.begin(),s.end(),[](int c){return std::isspace(c);});
    auto wsback=std::find_if_not(s.rbegin(),s.rend(),[](int c){return std::isspace(c);}).base();
    return (wsback<=wsfront ? std::string() : std::string(wsfront,wsback));
}

inline auto to_lower(std::string& src) -> std::string& {
    std::transform(src.cbegin(), src.cend(), src.begin(), [](unsigned char c) {return std::tolower(c);});
    return src;
}

inline auto to_upper(std::string& src) -> std::string& {
    std::transform(src.cbegin(), src.cend(), src.begin(), [](unsigned char c) {return std::toupper(c);});
    return src;
}

inline auto to_upper(const std::string& src) -> std::string {
    auto rst = src;
    to_upper(rst);
    return rst;
}

inline auto startswith(const std::string& src, const std::string& prefix) -> bool {
    return src.starts_with(prefix);
}

inline auto endswith(const std::string& src, const std::string& postfix) -> bool {
    return src.ends_with(postfix);
}

inline auto split(std::string_view str, std::string_view delimiter, bool skip_empty) -> std::vector<std::string> {
    std::vector<std::string> tokens;

    size_t pos_current;
    size_t pos_last = 0;
    size_t length;

    while (true)
    {
        pos_current = str.find(delimiter, pos_last);
        if (pos_current == std::string::npos)
            pos_current = str.size();

        length = pos_current - pos_last;
        if (!skip_empty || (length != 0))
            tokens.emplace_back(str.substr(pos_last, length));

        if (pos_current == str.size())
            break;
        else
            pos_last = pos_current + delimiter.size();
    }
    return tokens;
}

inline auto split(std::string_view str, char delimiter, bool skip_empty) -> std::vector<std::string> {
    std::vector<std::string> tokens;

    size_t pos_current;
    size_t pos_last = 0;
    size_t length;

    while (true)
    {
        pos_current = str.find(delimiter, pos_last);
        if (pos_current == std::string::npos)
            pos_current = str.size();

        length = pos_current - pos_last;
        if (!skip_empty || (length != 0))
            tokens.emplace_back(str.substr(pos_last, length));

        if (pos_current == str.size())
            break;
        else
            pos_last = pos_current + 1;
    }

    return tokens;
}

inline auto contains(const std::string& src, char ch) -> bool {
    return src.find(ch) != std::string ::npos;
}
inline auto count_char(const std::string& src, char ch) -> std::size_t {
    return std::count_if(src.cbegin(), src.cend(), [ch](char c){ return c == ch; });
}

inline auto is_escaped_name(const std::string& name) -> bool {
    if (name.size() > 4 and name.starts_with("__") and name.ends_with("__"))
        return true;
    return false;
}

inline void extract_escaped_name_inplace(std::string& name) {
    // is_escaped_name requires size > 4, so substr(2, size-4) is always valid
    if (!is_escaped_name(name))
        return;  // Invalid input - do nothing
    name.assign(name.substr(2, name.size() - 4));
}

int yyerror(const char* msg);
int yyerror(const std::string& msg);

namespace FBE {

struct Attributes
{
    bool deprecated{false};
    bool hidden{false};

    void Merge(Attributes* attributes);
};

struct EnumConst
{
    std::shared_ptr<std::string> constant;
    std::shared_ptr<std::string> reference;
};

struct EnumValue
{
    std::shared_ptr<Attributes> attributes;
    std::shared_ptr<std::string> name;
    std::shared_ptr<EnumConst> value;
};

struct EnumBody
{
    std::vector<std::shared_ptr<EnumValue>> values;

    void AddValue(EnumValue* v);
};

struct EnumType
{
    std::shared_ptr<Attributes> attributes;
    std::shared_ptr<std::string> name;
    std::shared_ptr<std::string> base;
    std::shared_ptr<EnumBody> body;
};

struct FlagsConst
{
    std::shared_ptr<std::string> constant;
    std::shared_ptr<std::string> reference;
};

struct FlagsValue
{
    std::shared_ptr<Attributes> attributes;
    std::shared_ptr<std::string> name;
    std::shared_ptr<FlagsConst> value;
};

struct FlagsBody
{
    std::vector<std::shared_ptr<FlagsValue>> values;

    void AddValue(FlagsValue* v);
};

struct FlagsType
{
    std::shared_ptr<Attributes> attributes;
    std::shared_ptr<std::string> name;
    std::shared_ptr<std::string> base;
    std::shared_ptr<FlagsBody> body;
};

struct StructField
{
    std::shared_ptr<Attributes> attributes;
    std::shared_ptr<std::string> name;
    std::shared_ptr<std::string> key;
    std::shared_ptr<std::string> type;
    std::shared_ptr<std::string> value;
    bool id{false};
    bool keys{false};
    bool optional{false};
    bool reseter{false};
    bool array{false};
    bool vector{false};
    bool list{false};
    bool set{false};
    bool map{false};
    bool hash{false};
    bool ptr{false};
    bool variant{false}; // TODO(liuqi): unused?
    int N{0};

    void SetArraySize(int size);
};

struct StructBody
{
    std::vector<std::shared_ptr<StructField>> fields;

    void AddField(StructField* field);
};

struct VariantValue
{
    std::shared_ptr<std::string> key;
    std::shared_ptr<std::string> type;
    bool ptr{false};
    bool vector{false};
    bool list{false};
    bool map{false};
    bool hash{false};

    bool operator==(const VariantValue& other) const noexcept
    {
        return (
            ((key && other.key && *key == *other.key) || (key == nullptr && other.key == nullptr)) && 
            *type == *other.type &&
            ptr == other.ptr &&
            vector == other.vector &&
            list == other.list &&
            map == other.map &&
            hash == other.hash
        );
    }

    // mock struct to reuse helper functions
    StructField toStructField() const noexcept
    {
        StructField field;
        field.key = key;
        field.type = type;
        field.ptr = ptr;
        field.vector = vector;
        field.list = list;
        field.map = map;
        field.hash = hash;
        return field;
    }
};

struct VariantBody
{
    std::vector<std::shared_ptr<VariantValue>> values;

    void AddValue(VariantValue* v);
};


struct VariantType
{
    std::shared_ptr<Attributes> attributes;
    std::shared_ptr<std::string> name;
    std::shared_ptr<VariantBody> body;
};

struct StructRequest
{
};

struct StructResponse
{
    std::shared_ptr<std::string> response;
};

struct StructReject
{
    std::shared_ptr<std::string> reject;
    bool global;
};

struct StructRejects
{
    std::vector<StructReject> rejects;

    void AddReject(std::string* r, bool g);
};

struct StructType
{
    int type;
    bool fixed;
    bool id{false};
    bool keys{false};
    bool message{false};
    std::shared_ptr<Attributes> attributes;
    std::shared_ptr<std::string> name;
    std::shared_ptr<std::string> base;
    std::shared_ptr<StructRequest> request;
    std::shared_ptr<StructResponse> response;
    std::shared_ptr<StructRejects> rejects;
    std::shared_ptr<StructBody> body;

    static int stype;

    StructType(int t, bool f);
};

struct Statement
{
    std::shared_ptr<EnumType> e;
    std::shared_ptr<FlagsType> f;
    std::shared_ptr<VariantType> v;
    std::shared_ptr<StructType> s;
};

struct Statements
{
    std::vector<std::shared_ptr<EnumType>> enums;
    std::vector<std::shared_ptr<FlagsType>> flags;
    std::vector<std::shared_ptr<VariantType>> variants;
    std::vector<std::shared_ptr<StructType>> structs;

    void AddStatement(Statement* st);

    void AddEnum(std::shared_ptr<EnumType>& e);
    void AddFlags(std::shared_ptr<FlagsType>& f);
    void AddStruct(std::shared_ptr<StructType>& s);
    void AddVariant(std::shared_ptr<VariantType>& v);
};

struct Import
{
    std::vector<std::shared_ptr<std::string>> imports;

    void AddImport(std::string* i);
};

struct Version
{
    int major{0};
    int minor{0};

    Version() = default;
    Version(const std::string& v);
};

struct Package
{
    int offset;
    std::shared_ptr<std::string> domain;
    std::shared_ptr<std::string> name;
    std::shared_ptr<Import> import;
    std::shared_ptr<Version> version;
    std::shared_ptr<Statements> body;

    static std::shared_ptr<Package> root;

    Package(int o);

    void initialize();
};

} // namespace FBE

#endif // FBE_H
