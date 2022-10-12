/*!
    \file generator_cpp.h
    \brief Fast binary encoding C++ generator definition
    \author Ivan Shynkarenka
    \date 20.04.2018
    \copyright MIT License
*/

#ifndef GENERATOR_CPP_H
#define GENERATOR_CPP_H

#include "generator.h"

namespace FBE {

class GeneratorCpp : public Generator
{
public:
    using Generator::Generator;

    // Ptr-based code generation
    bool Ptr() const noexcept { return _ptr; }
    GeneratorCpp& Ptr(bool ptr) noexcept { _ptr = ptr; return *this; }

    // Final protocol code generation
    bool Final() const noexcept { return _final; }
    GeneratorCpp& Final(bool final) noexcept { _final = final; return *this; }

    // JSON protocol code generation
    bool JSON() const noexcept { return _json; }
    GeneratorCpp& JSON(bool json) noexcept { _json = json; return *this; }

    // Sender/Receiver protocol code generation
    bool Proto() const noexcept { return _proto; }
    GeneratorCpp& Proto(bool proto) noexcept { _proto = proto; return *this; }

    // Logging protocol code generation
    bool Logging() const noexcept { return _logging; }
    GeneratorCpp& Logging(bool logging) noexcept { _logging = logging; return *this; }

    // Arena code generation
    bool Arena() const noexcept { return _arena; }
    GeneratorCpp& Arena(bool arena) noexcept { _arena = arena; return *this; }

    std::string ArenaHeader() const noexcept { return "memory/arena/arena.hpp"; }

    std::vector<std::string> ArenaTags() const noexcept { return {"ArenaManagedCreateOnlyTag"}; }

    bool ImportPtr() const noexcept { return _import_ptr; }
    GeneratorCpp& ImportPtr(bool import_ptr) noexcept { _import_ptr = import_ptr; return *this; }

    void Generate(const std::shared_ptr<Package>& package) override;

private:
    bool _ptr{false};
    bool _final{false};
    bool _json{false};
    bool _proto{false};
    bool _logging{false};
    bool _arena{false};
    bool _import_ptr{false};

    void GenerateHeader(const std::string& source);
    void GenerateInline(const std::string& source);
    void GenerateSource(const std::string& source);
    void GenerateWarningsHeader();
    void GenerateWarningsFooter();
    void GenerateFooter();
    void GenerateImports();
    void GenerateImportsSource();
    void GenerateImports(const std::string& source);
    void GenerateImports(const std::shared_ptr<Package>& p);
    void GenerateImportsModels(const std::shared_ptr<Package>& p, bool final, bool ptr);
    void GenerateImportsProtocol(const std::shared_ptr<Package>& p, bool final);
    void GenerateImportsJson();
    void GenerateImportsJson(const std::shared_ptr<Package>& p);
    void GenerateUnalignedAccessor_Header();
    void GenerateImportHelper_Header();
    void GenerateVariantVisitHelper_Header();
    void GenerateBufferWrapper_Header();
    void GenerateBufferWrapper_Source();
    void GeneratePMRBufferWrapper_Header();
    void GeneratePMRBufferWrapper_Source();
    void GenerateDecimalWrapper_Header();
    void GenerateFlagsWrapper_Header();
    void GenerateTimeWrapper_Header();
    void GenerateTimeWrapper_Source();
    void GenerateUUIDWrapper_Header();
    void GenerateUUIDWrapper_Source();
    void GenerateFBEBuffer_Header();
    void GenerateFBEBuffer_Source();
    void GenerateFBEBaseModel_Header();
    void GenerateFBEBaseFieldModel_Header();
    void GenerateFBEFieldModel_Header();
    void GenerateFBEFieldModel_Inline();
    void GenerateFBEFieldModelDecimal_Header();
    void GenerateFBEFieldModelDecimal_Source();
    void GenerateFBEFieldModelUUID_Header();
    void GenerateFBEFieldModelUUID_Source();
    void GenerateFBEFieldModelBytes_Header();
    void GenerateFBEFieldModelBytes_Source();
    void GenerateFBEFieldModelPMRBytes_Header();
    void GenerateFBEFieldModelPMRBytes_Source();
    void GenerateFBEFieldModelString_Header();
    void GenerateFBEFieldModelString_Source();
    // TODO(liuqi): exists for backwards compatibility
    void GenerateFBEFieldModelPMRString_Header();
    void GenerateFBEFieldModelPMRString_Source();
    void GenerateFBEFieldModelMemoryString_Header();
    void GenerateFBEFieldModelMemoryString_Source();
    void GenerateFBEFieldModelMemoryArenaString_Header();
    void GenerateFBEFieldModelMemoryArenaString_Source();
    void GenerateFBEFieldModelOptional_Header();
    void GenerateFBEFieldModelOptional_Inline();
    void GenerateFBEFieldModelStructOptional_Header();
    void GenerateFBEFieldModelStructOptional_Inline();
    void GenerateFBEFieldModelArray_Header();
    void GenerateFBEFieldModelArray_Inline();
    void GenerateFBEFieldModelCustomArray_Header();
    void GenerateFBEFieldModelCustomArray_Inline();
    void GenerateFBEFieldModelVector_Header();
    void GenerateFBEFieldModelVector_Inline();
    void GenerateFBEFieldModelCustomVector_Header();
    void GenerateFBEFieldModelCustomVector_Inline();
    void GenerateFBEFieldModelMap_Header();
    void GenerateFBEFieldModelMap_Inline();
    void GenerateFBEFieldModelCustomMap_Header();
    void GenerateFBEFieldModelCustomMap_Inline();
    void GenerateFBEFinalModel_Header();
    void GenerateFBEFinalModel_Inline();
    void GenerateFBEFinalModelDecimal_Header();
    void GenerateFBEFinalModelDecimal_Source();
    void GenerateFBEFinalModelUUID_Header();
    void GenerateFBEFinalModelUUID_Source();
    void GenerateFBEFinalModelBytes_Header();
    void GenerateFBEFinalModelBytes_Source();
    void GenerateFBEFinalModelString_Header();
    void GenerateFBEFinalModelString_Source();
    void GenerateFBEFinalModelOptional_Header();
    void GenerateFBEFinalModelOptional_Inline();
    void GenerateFBEFinalModelArray_Header();
    void GenerateFBEFinalModelArray_Inline();
    void GenerateFBEFinalModelVector_Header();
    void GenerateFBEFinalModelVector_Inline();
    void GenerateFBEFinalModelMap_Header();
    void GenerateFBEFinalModelMap_Inline();
    void GenerateFBESender_Header();
    void GenerateFBESender_Source();
    void GenerateFBEReceiver_Header();
    void GenerateFBEReceiver_Source();
    void GenerateFBEJson();
    void GenerateFBE_Header(const CppCommon::Path& path);
    void GenerateFBE_Source(const CppCommon::Path& path);
    void GenerateFBEPtr_Header(const CppCommon::Path& path);
    void GenerateFBEModels_Header(const CppCommon::Path& path);
    void GenerateFBEModels_Inline(const CppCommon::Path& path);
    void GenerateFBEModels_Source(const CppCommon::Path& path);
    void GenerateFBECustomModels_Header(const CppCommon::Path& path);
    void GenerateFBECustomModels_Inline(const CppCommon::Path& path);
    void GenerateFBEFinalModels_Header(const CppCommon::Path& path);
    void GenerateFBEFinalModels_Inline(const CppCommon::Path& path);
    void GenerateFBEFinalModels_Source(const CppCommon::Path& path);
    void GenerateFBEProtocol_Header(const CppCommon::Path& path);
    void GenerateFBEProtocol_Source(const CppCommon::Path& path);
    void GenerateFBEJson_Header(const CppCommon::Path& path);

    void GeneratePackage_Header(const std::shared_ptr<Package>& p);
    void GeneratePackage_Source(const std::shared_ptr<Package>& p);
    void GeneratePtrPackage_Header(const std::shared_ptr<Package>& p);
    void GeneratePtrPackage_Source(const std::shared_ptr<Package>& p);
    void GeneratePackage_Json(const std::shared_ptr<Package>& p);
    void GeneratePackageModels_Header(const std::shared_ptr<Package>& p);
    void GeneratePackageModels_Source(const std::shared_ptr<Package>& p);
    void GeneratePtrPackageModels_Header(const std::shared_ptr<Package>& p);
    void GeneratePtrPackageModels_Source(const std::shared_ptr<Package>& p);
    void GeneratePackageFinalModels_Header(const std::shared_ptr<Package>& p);
    void GeneratePackageFinalModels_Source(const std::shared_ptr<Package>& p);
    void GeneratePackageProtocol_Header(const std::shared_ptr<Package>& p, bool final);
    void GeneratePackageProtocol_Source(const std::shared_ptr<Package>& p, bool final);
    void GenerateEnum(const std::shared_ptr<Package>& p, const std::shared_ptr<EnumType>& e);
    void GenerateEnumOutputStream(const std::shared_ptr<EnumType>& e);
    void GenerateEnumLoggingStream(const std::shared_ptr<EnumType>& e);
    void GenerateEnumJson(const std::shared_ptr<Package>& p, const std::shared_ptr<EnumType>& e);
    void GenerateEnumFieldModel(const std::shared_ptr<Package>& p, const std::shared_ptr<EnumType>& e);
    void GenerateEnumFinalModel(const std::shared_ptr<Package>& p, const std::shared_ptr<EnumType>& e);
    void GenerateFlags(const std::shared_ptr<Package>& p, const std::shared_ptr<FlagsType>& f);
    void GenerateFlagsOutputStream(const std::shared_ptr<FlagsType>& f);
    void GenerateFlagsLoggingStream(const std::shared_ptr<FlagsType>& f);
    void GenerateFlagsJson(const std::shared_ptr<Package>& p, const std::shared_ptr<FlagsType>& f);
    void GenerateFlagsFieldModel(const std::shared_ptr<Package>& p, const std::shared_ptr<FlagsType>& f);
    void GenerateFlagsFinalModel(const std::shared_ptr<Package>& p, const std::shared_ptr<FlagsType>& f);
    void GeneratorStructForwardDeclaration(const std::vector<std::shared_ptr<StructType>>& structs);
    void GenerateVariantAlias(const std::shared_ptr<Package>& p, const std::shared_ptr<VariantType>& v);
    void GenerateVariantOutputStream(const std::shared_ptr<Package>& p, const std::shared_ptr<VariantType>& v);
    void GenerateStruct_Header(const std::shared_ptr<Package>& p, const std::shared_ptr<StructType>& s);
    void GenerateStruct_Source(const std::shared_ptr<Package>& p, const std::shared_ptr<StructType>& s);
    void GeneratePtrStruct_Header(const std::shared_ptr<Package>& p, const std::shared_ptr<StructType>& s);
    void GeneratePtrStruct_Source(const std::shared_ptr<Package>& p, const std::shared_ptr<StructType>& s);
    void GenerateStructOutputStream(const std::shared_ptr<Package>& p, const std::shared_ptr<StructType>& s);
    void GenerateStructLoggingStream(const std::shared_ptr<Package>& p, const std::shared_ptr<StructType>& s);
    void GenerateStructFormatter(const std::shared_ptr<Package>& p, const std::shared_ptr<StructType>& s);
    void GenerateStructHash(const std::shared_ptr<Package>& p, const std::shared_ptr<StructType>& s);
    void GenerateStructJson(const std::shared_ptr<Package>& p, const std::shared_ptr<StructType>& s);
    void GenerateVariantFieldModel_Header(const std::shared_ptr<Package>& p, const std::shared_ptr<VariantType>& v);
    void GenerateVariantFieldModel_Source(const std::shared_ptr<Package>& p, const std::shared_ptr<VariantType>& v);
    void GenerateStructFieldPtrModel_Header(const std::shared_ptr<Package>& p, const std::shared_ptr<StructType>& s);
    void GenerateStructFieldModel_Header(const std::shared_ptr<Package>& p, const std::shared_ptr<StructType>& s);
    void GenerateStructFieldModel_Source(const std::shared_ptr<Package>& p, const std::shared_ptr<StructType>& s);
    void GenerateStructFieldPtrModel_Source(const std::shared_ptr<Package>& p, const std::shared_ptr<StructType>& s);
    void GeneratePtrStructFieldModel_Header(const std::shared_ptr<Package>& p, const std::shared_ptr<StructType>& s);
    void GeneratePtrStructFieldModel_Source(const std::shared_ptr<Package>& p, const std::shared_ptr<StructType>& s);
    void GenerateStructModel_Header(const std::shared_ptr<Package>& p, const std::shared_ptr<StructType>& s);
    void GenerateStructModel_Source(const std::shared_ptr<Package>& p, const std::shared_ptr<StructType>& s);
    void GeneratePtrStructModel_Header(const std::shared_ptr<Package>& p, const std::shared_ptr<StructType>& s);
    void GeneratePtrStructModel_Source(const std::shared_ptr<Package>& p, const std::shared_ptr<StructType>& s);
    void GenerateStructFinalModel_Header(const std::shared_ptr<Package>& p, const std::shared_ptr<StructType>& s);
    void GenerateStructFinalModel_Source(const std::shared_ptr<Package>& p, const std::shared_ptr<StructType>& s);
    void GenerateStructModelFinal_Header(const std::shared_ptr<Package>& p, const std::shared_ptr<StructType>& s);
    void GenerateStructModelFinal_Source(const std::shared_ptr<Package>& p, const std::shared_ptr<StructType>& s);
    void GenerateProtocolVersion(const std::shared_ptr<Package>& p);
    void GenerateSender_Header(const std::shared_ptr<Package>& p, bool final);
    void GenerateSender_Source(const std::shared_ptr<Package>& p, bool final);
    void GenerateReceiver_Header(const std::shared_ptr<Package>& p, bool final);
    void GenerateReceiver_Source(const std::shared_ptr<Package>& p, bool final);
    void GenerateProxy_Header(const std::shared_ptr<Package>& p, bool final);
    void GenerateProxy_Source(const std::shared_ptr<Package>& p, bool final);
    void GenerateClient_Header(const std::shared_ptr<Package>& p, bool final);
    void GenerateClient_Source(const std::shared_ptr<Package>& p, bool final);

    bool IsKnownType(const std::string& type);
    bool IsPrimitiveType(const std::string& type, bool optional);
    bool IsContainerType(const StructField& field);
    bool IsContainerType(const VariantValue& variant);
    bool IsStructType(const std::shared_ptr<Package>& p, const std::string& field_type);
    bool IsVariantType(const std::shared_ptr<Package>& p, const std::string& type);
    bool IsCurrentPackageType(const std::string& field_type, const std::string& delimiter = ".");

    std::string ConvertVariantTypeName(const std::string& package, const VariantValue& variant);
    std::string ConvertVariantTypeNameAsArgument(const std::string& package, const VariantValue& variant);
    std::string ConvertPtrFieldModelType(const std::shared_ptr<Package>& p, const std::shared_ptr<StructField>& field);
    std::string ConvertPtrVariantFieldModelType(const std::shared_ptr<Package>& p, const std::shared_ptr<VariantValue>& variant);
    std::string ConvertEnumType(const std::string& type);
    std::string ConvertTypeName(const std::string& package, const std::string& type, bool optional);
    std::string ConvertTypeName(const std::string& package, const StructField& field);
    std::string ConvertTypeNameAsArgument(const std::string& package, const StructField& field);
    std::string ConvertPtrTypeName(const std::string& package, const std::string& type);
    std::string ConvertPtrTypeName(const std::string& package, const std::string& type, bool optional, bool typeptr, bool as_argument);
    std::string ConvertPtrTypeName(const std::string& package, const StructField& field, bool as_argument);
    std::string ConvertPtrTypeNameAsArgument(const std::string& package, const StructField& field);
    std::string ConvertConstant(const std::string& type, const std::string& value, bool optional);
    std::string ConvertConstantPrefix(const std::string& type);
    std::string ConvertConstantSuffix(const std::string& type);
    std::string ConvertDefault(const std::string& package, const std::string& type);
    std::string ConvertDefault(const std::string& package, const StructField& field);

    std::string ConvertOutputStreamType(const std::string& type, const std::string& name, bool ptr, bool optional);
    std::string ConvertOutputStreamValue(const std::string& type, const std::string& name, bool ptr, bool optional, bool separate);

    std::string ConvertLoggingStreamType(const std::string& type, const std::string& name, bool optional);
    std::string ConvertLoggingStreamValue(const std::string& type, const std::string& name, bool optional, bool separate);
};

} // namespace FBE

#endif // GENERATOR_CPP_H
