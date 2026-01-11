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
    \file fbe.cpp
    \brief Fast binary encoding implementation
    \author Ivan Shynkarenka
    \date 11.04.2018
    \copyright AGPL-3.0-or-later
*/

#include "fbe.h"

namespace FBE {

void Attributes::Merge(Attributes* attributes)
{
    deprecated |= attributes->deprecated;
    hidden |= attributes->hidden;
}

void EnumBody::AddValue(EnumValue* v)
{
    if (v == nullptr)
        yyerror("Enum is null!");
    if (v->name->empty())
        yyerror("Enum name is invalid!");

    // Check for duplicates
    auto it = std::find_if(values.begin(), values.end(), [v](auto item)->bool { return *item->name.get() == *v->name.get(); });
    if (it != values.end())
        yyerror("Duplicate enum name " + *v->name.get());

    values.push_back(std::shared_ptr<EnumValue>(v));
}

void FlagsBody::AddValue(FlagsValue* v)
{
    if (v == nullptr)
        yyerror("Flags is null!");
    if (v->name->empty())
        yyerror("Flags name is invalid!");
    if (!v->value || (!v->value->constant && !v->value->reference))
        yyerror("Flags value is invalid!");

    // Check for duplicates
    auto it = std::find_if(values.begin(), values.end(), [v](auto item)->bool { return *item->name.get() == *v->name.get(); });
    if (it != values.end())
        yyerror("Duplicate flags name " + *v->name.get());

    values.push_back(std::shared_ptr<FlagsValue>(v));
}

void VariantBody::AddValue(VariantValue* v)
{
    if (v == nullptr)
        yyerror("Variant is null!");
    if (v->type->empty())
        yyerror("Variant type is invalid!");

    // Check for duplicates
    auto it = std::find_if(values.begin(), values.end(), [v](auto item)->bool { return *item == *v; });
    if (it != values.end())
        yyerror("Duplicate Variant type " + *v->type.get());

    values.push_back(std::shared_ptr<VariantValue>(v));
}

void StructField::SetArraySize(int size)
{
    if (size <= 0)
        yyerror("Array size should be greater than zero!");

    N = size;
}

void StructBody::AddField(StructField* f)
{
    if (f == nullptr)
        yyerror("Struct field is null!");
    if (f->name->empty())
        yyerror("Struct field name is invalid!");
    if (f->type->empty())
        yyerror("Struct field type is invalid!");
    if (is_escaped_name(*f->name))
        extract_escaped_name_inplace(*f->name);

    // Check for duplicates
    auto it = std::find_if(fields.begin(), fields.end(), [f](auto item)->bool { return *item->name.get() == *f->name.get(); });
    if (it != fields.end())
        yyerror("Duplicate struct field name " + *f->name.get());

    fields.push_back(std::shared_ptr<StructField>(f));
}

void StructRejects::AddReject(std::string* r, bool g)
{
    if (r == nullptr)
        yyerror("Reject is null!");
    if (r->empty())
        yyerror("Reject is invalid!");

    // Check for duplicates
    auto it = std::find_if(rejects.begin(), rejects.end(), [r](auto item)->bool { return *item.reject.get() == *r; });
    if (it != rejects.end())
        yyerror("Duplicate reject " + *r);

    rejects.push_back({ std::shared_ptr<std::string>(r), g });
}

int StructType::stype = 0;

StructType::StructType(int t, bool f) : type(t), fixed(f)
{
    if (type < 0)
        yyerror("Struct type should not be negative!");

    if (!fixed)
    {
        if (type == 0)
            type = ++stype;
        else
            stype = type;
    }
}

void Statements::AddStatement(Statement* st)
{
    if (st == nullptr)
        yyerror("Statement is null!");

    if (st->e)
        AddEnum(st->e);
    if (st->f)
        AddFlags(st->f);
    if (st->s)
        AddStruct(st->s);
    if (st->v)
        AddVariant(st->v);
    delete st;
}

void Statements::AddEnum(std::shared_ptr<EnumType>& e)
{
    if (e == nullptr)
        yyerror("Enum is null!");
    if (e->name->empty())
        yyerror("Enum name is invalid!");
    if (!e->body)
        yyerror("Enum is empty - " + *e->name.get());

    // Check for duplicates
    auto it = std::find_if(enums.begin(), enums.end(), [&e](auto item)->bool { return *item->name.get() == *e->name.get(); });
    if (it != enums.end())
        yyerror("Duplicate enum name " + *e->name.get());

    enums.push_back(e);
}

void Statements::AddFlags(std::shared_ptr<FlagsType>& f)
{
    if (f == nullptr)
        yyerror("Flags is null!");
    if (f->name->empty())
        yyerror("Flags name is invalid!");
    if (!f->body)
        yyerror("Flags is empty - " + *f->name.get());

    // Check for duplicates
    auto it = std::find_if(flags.begin(), flags.end(), [&f](auto item)->bool { return *item->name.get() == *f->name.get(); });
    if (it != flags.end())
        yyerror("Duplicate flags name " + *f->name.get());

    flags.push_back(f);
}

void Statements::AddVariant(std::shared_ptr<VariantType> &v)
{
    if (v == nullptr)
        yyerror("Variant is null!");
    if (v->name->empty())
        yyerror("Variant name is invalid!");
    if (!v->body)
        yyerror("Variant is empty - " + *v->name.get());

    // Check for duplicates
    auto it = std::find_if(variants.begin(), variants.end(), [&v](auto item)->bool { return *item->name.get() == *v->name.get(); });
    if (it != variants.end())
        yyerror("Duplicate variant name " + *v->name.get());

    variants.push_back(v);
}

void Statements::AddStruct(std::shared_ptr<StructType>& s)
{
    if (s == nullptr)
        yyerror("Struct is null!");
    if (s->name->empty())
        yyerror("Struct name is invalid!");
    if (!s->body)
        yyerror("Struct is empty - " + *s->name.get());

    // Check for duplicates
    auto it = std::find_if(structs.begin(), structs.end(), [&s](auto item)->bool { return *item->name.get() == *s->name.get(); });
    if (it != structs.end())
        yyerror("Duplicate struct name " + *s->name.get());
    it = std::find_if(structs.begin(), structs.end(), [&s](auto item)->bool { return (item->type == s->type) && item->fixed && s->fixed; });
    if (it != structs.end())
        yyerror("Duplicate struct type " + std::to_string(s->type));

    structs.push_back(s);
}

void Import::AddImport(std::string* i)
{
    if (i == nullptr)
        yyerror("Import is null!");
    if (i->empty())
        yyerror("Import package is invalid!");

    // Check for duplicates
    auto it = std::find_if(imports.begin(), imports.end(), [i](auto item)->bool { return *item.get() == *i; });
    if (it != imports.end())
        yyerror("Duplicate import package " + *i);

    imports.push_back(std::shared_ptr<std::string>(i));
}

Version::Version(const std::string& v)
{
    if (v.empty())
        yyerror("Version string is empty!");

    auto pos = v.find('.');
    if (pos == v.npos)
    {
        // Single number means minor version only
        std::size_t parsed_chars = 0;
        try {
            minor = std::stoi(v, &parsed_chars);
        } catch (const std::exception&) {
            yyerror("Invalid version string: " + v);
        }
        if (parsed_chars != v.size())
            yyerror("Invalid version string (extra characters): " + v);
    }
    else
    {
        // major.minor format
        if (pos == 0)
            yyerror("Invalid version string (empty major): " + v);
        if (pos + 1 >= v.size())
            yyerror("Invalid version string (empty minor): " + v);

        std::string major_str = v.substr(0, pos);
        std::string minor_str = v.substr(pos + 1);
        std::size_t parsed_chars = 0;

        try {
            major = std::stoi(major_str, &parsed_chars);
        } catch (const std::exception&) {
            yyerror("Invalid major version: " + major_str);
        }
        if (parsed_chars != major_str.size())
            yyerror("Invalid major version (extra characters): " + major_str);

        try {
            minor = std::stoi(minor_str, &parsed_chars);
        } catch (const std::exception&) {
            yyerror("Invalid minor version: " + minor_str);
        }
        if (parsed_chars != minor_str.size())
            yyerror("Invalid minor version (extra characters): " + minor_str);
    }
}

std::shared_ptr<Package> Package::root = std::make_shared<Package>(0);

Package::Package(int o) : offset(o)
{
    if (offset < 0)
        yyerror("Package offset should not be negative!");
}

void Package::initialize()
{
    if (body)
    {
        for (const auto& child_s : body->structs)
        {
            // Add offset to all structs in the package
            if (!child_s->fixed)
                child_s->type += offset;

            // Find structs with id & key flags
            if (child_s->body)
            {
                std::vector<std::shared_ptr<StructField>> fields;
                for (const auto& field : child_s->body->fields)
                {
                    if (field->id)
                    {
                        if (child_s->id)
                            yyerror("Struct " + *child_s->name.get() + " must have only one [id] field!");
                        child_s->id = true;
                        field->keys = true;
                    }
                    if (field->keys)
                        child_s->keys = true;
                    if (field->reseter)
                        field->optional = true;
                    fields.push_back(field);
                    if (field->reseter)
                    {
                        auto reseter = std::make_shared<StructField>();
                        reseter->name = std::make_shared<std::string>(*field->name + "Reset");
                        reseter->type = std::make_shared<std::string>("bool");
                        fields.push_back(reseter);
                    }
                }
                child_s->body->fields = fields;
            }
        }
    }
}

} // namespace FBE
