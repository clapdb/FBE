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
    \file generator_go.h
    \brief Fast binary encoding Go generator definition
    \author Ivan Shynkarenka
    \date 20.11.2018
    \copyright AGPL-3.0-or-later
*/

#ifndef GENERATOR_GO_H
#define GENERATOR_GO_H

#include "fbe.h"
#include "generator.h"

namespace FBE {

class GeneratorGo : public Generator
{
public:
    using Generator::Generator;

    // Final protocol code generation
    bool Final() const noexcept { return _final; }
    GeneratorGo& Final(bool final) noexcept { _final = final; return *this; }

    // JSON protocol code generation
    bool JSON() const noexcept { return _json; }
    GeneratorGo& JSON(bool json) noexcept { _json = json; return *this; }

    // Sender/Receiver protocol code generation
    bool Proto() const noexcept { return _proto; }
    GeneratorGo& Proto(bool proto) noexcept { _proto = proto; return *this; }

    std::string ModulePath() const noexcept { return _module_path; }
    GeneratorGo& ModulePath(const std::string& module_path) noexcept {
        _module_path = module_path;
         return *this;
    }

    void Generate(const std::shared_ptr<Package>& package) override;

private:
    bool _final{false};
    bool _json{false};
    bool _proto{false};
    std::string _module_path{".."};

    void GenerateHeader(const std::string& source);
    void GenerateFooter();
    void GenerateFBEPackage(const std::string& package);
    void GenerateFBEConstants(const std::string& package);
    void GenerateFBEJson(const std::string& package);
    void GenerateFBEOptional(const std::string& package);
    void GenerateFBEPtr(const std::string& package);
    void GenerateFBETypes(const std::string& package);
    void GenerateFBEVersion(const std::string& package, const std::string& source);
    void GenerateFBEBuffer(const std::string& package);
    void GenerateFBEFieldModel(const std::string& package, const std::string& name, const std::string& type, const std::string& size, const std::string& defaults);
    void GenerateFBEFieldModelDecimal(const std::string& package);
    void GenerateFBEFieldModelTimestamp(const std::string& package);
    void GenerateFBEFieldModelUUID(const std::string& package);
    void GenerateFBEFieldModelBytes(const std::string& package);
    void GenerateFBEFieldModelString(const std::string& package);
    void GenerateFBEFieldModelOptional(const std::shared_ptr<Package>& p, const std::string& name, const std::string& model, const StructField& field, const fs::path& path);
    void GenerateFBEFieldModelOptional(const std::shared_ptr<Package>& p, const std::string& name, const std::string& model, const VariantType& variant, const fs::path& path);
    void GenerateFBEFieldModelPtr(const std::shared_ptr<Package>& p, const std::string& name, const std::string& model, const StructField& field, const fs::path& path);
    void GenerateFBEFieldModelArray(const std::shared_ptr<Package>& p, const std::string& name, const std::string& model, const StructField& field, const fs::path& path);
    void GenerateFBEFieldModelVector(const std::shared_ptr<Package>& p, const std::string& name, const std::string& model, const StructField& field, const fs::path& path);
    void GenerateFBEFieldModelSet(const std::shared_ptr<Package>& p, const std::string& name, const std::string& model, const StructField& field, const fs::path& path);
    void GenerateFBEFieldModelMap(const std::shared_ptr<Package>& p, const std::string& key_name, const std::string& key_model, const std::string& value_name, const std::string& value_model, const StructField& field, const fs::path& path);
    void GenerateFBEFieldModelEnumFlags(const std::string& package, const std::string& name, const std::string& type);
    void GenerateFBEFinalModel(const std::string& package, const std::string& name, const std::string& type, const std::string& size, const std::string& defaults);
    void GenerateFBEFinalModelDecimal(const std::string& package);
    void GenerateFBEFinalModelTimestamp(const std::string& package);
    void GenerateFBEFinalModelUUID(const std::string& package);
    void GenerateFBEFinalModelBytes(const std::string& package);
    void GenerateFBEFinalModelString(const std::string& package);
    void GenerateFBEFinalModelOptional(const std::shared_ptr<Package>& p, const std::string& name, const std::string& model, const StructField& field, const fs::path& path);
    void GenerateFBEFinalModelArray(const std::shared_ptr<Package>& p, const std::string& name, const std::string& model, const StructField& field, const fs::path& path);
    void GenerateFBEFinalModelVector(const std::shared_ptr<Package>& p, const std::string& name, const std::string& model, const StructField& field, const fs::path& path);
    void GenerateFBEFinalModelSet(const std::shared_ptr<Package>& p, const std::string& name, const std::string& model, const StructField& field, const fs::path& path);
    void GenerateFBEFinalModelMap(const std::shared_ptr<Package>& p, const std::string& key_name, const std::string& key_model, const std::string& value_name, const std::string& value_model, const StructField& field, const fs::path& path);
    void GenerateFBEFinalModelEnumFlags(const std::string& package, const std::string& name, const std::string& type);
    void GenerateFBESender(const std::string& package);
    void GenerateFBEReceiver(const std::string& package);
    void GenerateImports(const std::shared_ptr<Package>& p);

    void GenerateContainers(const std::shared_ptr<Package>& p, const fs::path& path, bool final);
    void GeneratePackage(const std::shared_ptr<Package>& p);
    void GenerateEnum(const std::shared_ptr<Package>& p, const std::shared_ptr<EnumType>& e, const fs::path& path);
    void GenerateFlags(const std::shared_ptr<Package>& p, const std::shared_ptr<FlagsType>& f, const fs::path& path);
    void GenerateVariant(const std::shared_ptr<Package>& p, const std::shared_ptr<VariantType>& v, const fs::path& path);
    void GenerateVariantFieldModel(const std::shared_ptr<Package>& p, const std::shared_ptr<VariantType>& v, const fs::path& path);
    void GenerateStruct(const std::shared_ptr<Package>& p, const std::shared_ptr<StructType>& s, const fs::path& path);
    void GenerateStructFieldModel(const std::shared_ptr<Package>& p, const std::shared_ptr<StructType>& s, const fs::path& path);
    void GenerateStructModel(const std::shared_ptr<Package>& p, const std::shared_ptr<StructType>& s, const fs::path& path);
    void GenerateStructFinalModel(const std::shared_ptr<Package>& p, const std::shared_ptr<StructType>& s, const fs::path& path);
    void GenerateStructModelFinal(const std::shared_ptr<Package>& p, const std::shared_ptr<StructType>& s, const fs::path& path);
    void GenerateProtocolVersion(const std::shared_ptr<Package>& p, const fs::path& path);
    void GenerateSender(const std::shared_ptr<Package>& p, const fs::path& path, bool final);
    void GenerateReceiver(const std::shared_ptr<Package>& p, const fs::path& path, bool final);
    void GenerateProxy(const std::shared_ptr<Package>& p, const fs::path& path, bool final);

    bool IsPrimitiveType(const std::string& type);
    bool IsGoType(const std::string& type);

    std::string ConvertToLower(const std::string& type);
    std::string ConvertToUpper(const std::string& type);
    std::string ConvertEnumBase(const std::string& type);
    std::string ConvertEnumSize(const std::string& type);
    std::string ConvertEnumType(const std::string& type);
    std::string ConvertEnumConstant(const std::string& value);
    std::string ConvertEnumConstant(const std::string& name, const std::string& type, const std::string& value);
    std::string ConvertBaseName(const std::string& type);
    std::string ConvertKeyName(const std::string& type);
    std::string ConvertModelName(const std::string& type, const std::string& model);
    std::string ConvertNewName(const std::string& type);
    std::string ConvertOptional(const std::string& type);
    std::string ConvertPtr(const std::string& type);
    std::string ConvertOptional(const std::string& type, const std::string& value);
    std::string ConvertPtr(const std::string& type, const std::string& value);
    std::string ConvertTypeName(const std::string& type, bool optional, bool ptr);
    std::string ConvertTypeName(const StructField& field);
    std::string ConvertTypeName(const VariantValue& variant);
    std::string ConvertTypeFieldName(const std::string& type);
    std::string ConvertTypeFieldType(const std::string& type, bool optional, bool ptr);
    std::string ConvertTypeFieldDeclaration(const std::string& type, bool optional, bool ptr, bool final);
    std::string ConvertTypeFieldDeclaration(const StructField& field, bool final);
    std::string ConvertTypeFieldInitialization(const std::string& type, bool optional, const std::string& offset, bool final);
    std::string ConvertTypeFieldInitialization(const StructField& field, const std::string& offset, bool final);
    std::string ConvertVariantTypeFieldInitialization(const std::string& type);
    std::string ConvertVariantTypeFieldInitialization(const VariantValue& field);
    std::string ConvertConstant(const std::string& type, const std::string& value, bool optional, bool ptr);
    std::string ConvertDefault(const std::string& type, bool optional, bool ptr);
    std::string ConvertDefault(const StructField& field);
    std::string ConvertDefault(const std::shared_ptr<VariantType>& variant);

    void WriteOutputStreamType(const std::string& type, const std::string& name, bool optional, bool ptr);
    void WriteOutputStreamValue(const std::string& type, const std::string& name, bool optional, bool ptr, bool separate);
};

} // namespace FBE

#endif // GENERATOR_GO_H
