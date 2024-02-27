//------------------------------------------------------------------------------
// Automatically generated by the Fast Binary Encoding compiler, do not modify!
// https://github.com/chronoxor/FastBinaryEncoding
// Source: variants_ptr.fbe
// FBE version: 1.11.0.0
//------------------------------------------------------------------------------

#pragma once

#ifdef isset
#undef isset
#endif

#if defined(__clang__)
#pragma clang system_header
#elif defined(__GNUC__)
#pragma GCC system_header
#elif defined(_MSC_VER)
#pragma system_header
#endif

#include "fbe_custom_models.h"

#include "variants_ptr_ptr_pmr.h"

namespace FBE {

template <>
class FieldModel<::variants_ptr_pmr::Expr>
{
public:
    FieldModel(FBEBuffer& buffer, size_t offset) noexcept;

    // Get the field offset
    size_t fbe_offset() const noexcept { return _offset; }
    // Get the field size
    size_t fbe_size() const noexcept { return 4; }
    // Get the field body size
    size_t fbe_body() const noexcept;
    // Get the field extra size
    size_t fbe_extra() const noexcept;

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept { _offset -= size; }

    // Check if the variant value is valid
    bool verify() const noexcept;

    // Get the variant value
    void get(::variants_ptr_pmr::Expr& fbe_value, pmr::memory_resource* resource) const noexcept;

    // Set the variant value (begin phase)
    size_t set_begin(size_t variant_type_fbe_size, size_t variant_type_index);
    // Set the variant value (end phase)
    void set_end(size_t fbe_begin);

    // Set the variant value
    void set(const ::variants_ptr_pmr::Expr& fbe_value, pmr::memory_resource* resource) noexcept;

private:
    FBEBuffer& _buffer;
    size_t _offset;
};

template <>
class FieldModel<::variants_ptr_pmr::V>
{
public:
    FieldModel(FBEBuffer& buffer, size_t offset) noexcept;

    // Get the field offset
    size_t fbe_offset() const noexcept { return _offset; }
    // Get the field size
    size_t fbe_size() const noexcept { return 4; }
    // Get the field body size
    size_t fbe_body() const noexcept;
    // Get the field extra size
    size_t fbe_extra() const noexcept;

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept { _offset -= size; }

    // Check if the variant value is valid
    bool verify() const noexcept;

    // Get the variant value
    void get(::variants_ptr_pmr::V& fbe_value, pmr::memory_resource* resource) const noexcept;

    // Set the variant value (begin phase)
    size_t set_begin(size_t variant_type_fbe_size, size_t variant_type_index);
    // Set the variant value (end phase)
    void set_end(size_t fbe_begin);

    // Set the variant value
    void set(const ::variants_ptr_pmr::V& fbe_value, pmr::memory_resource* resource) noexcept;

private:
    FBEBuffer& _buffer;
    size_t _offset;
};

template <>
class FieldModel<::variants_ptr_pmr::Scalar1>
{
public:
    FieldModel(FBEBuffer& buffer, size_t offset) noexcept;

    // Get the field offset
    size_t fbe_offset() const noexcept { return _offset; }
    // Get the field size
    size_t fbe_size() const noexcept { return 4; }
    // Get the field body size
    size_t fbe_body() const noexcept;
    // Get the field extra size
    size_t fbe_extra() const noexcept;

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept { _offset -= size; }

    // Check if the variant value is valid
    bool verify() const noexcept;

    // Get the variant value
    void get(::variants_ptr_pmr::Scalar1& fbe_value, pmr::memory_resource* resource) const noexcept;

    // Set the variant value (begin phase)
    size_t set_begin(size_t variant_type_fbe_size, size_t variant_type_index);
    // Set the variant value (end phase)
    void set_end(size_t fbe_begin);

    // Set the variant value
    void set(const ::variants_ptr_pmr::Scalar1& fbe_value, pmr::memory_resource* resource) noexcept;

private:
    FBEBuffer& _buffer;
    size_t _offset;
};

class FieldModelPMRPtr_variants_ptr_Simple
{
public:
    FieldModelPMRPtr_variants_ptr_Simple(FBEBuffer& buffer, size_t offset) noexcept;
    ~FieldModelPMRPtr_variants_ptr_Simple();

    // Get the field offset
    size_t fbe_offset() const noexcept { return _offset; }
    // Get the field size
    size_t fbe_size() const noexcept { return 5; }
    // Get the field extra size
    size_t fbe_extra() const noexcept;
    // Get the field type
    static constexpr size_t fbe_type() noexcept { return 1; }

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept { _offset -= size; }

    // Check if the struct value is valid
    bool verify() const noexcept;
    // Check if the struct ptr is nullptr
    bool has_value() const noexcept;

    // Get the struct value (begin phase)
    size_t get_begin() const noexcept;
    // Get the struct value (end phase)
    void get_end(size_t fbe_begin) const noexcept;

    // Get the struct value
    void get(::variants_ptr_pmr::Simple** fbe_value, pmr::memory_resource* resource) noexcept;
    // Get the struct fields values
    void get_fields(::variants_ptr_pmr::Simple& fbe_value, size_t fbe_struct_size, pmr::memory_resource* resource) noexcept;

    // Set the struct value (begin phase)
    size_t set_begin(bool has_value);
    // Set the struct value (end phase)
    void set_end(size_t fbe_begin);

    // Set the struct value
    void set(const ::variants_ptr_pmr::Simple* fbe_value, pmr::memory_resource* resource) noexcept;
    // Set the struct fields values
    void set_fields(const ::variants_ptr_pmr::Simple* fbe_value, pmr::memory_resource* resource) noexcept;

private:
    FBEBuffer& _buffer;
    size_t _offset;

public:
    BaseFieldModel* ptr{nullptr};
};

class FieldModelPMRPtr_variants_ptr_ExprContainer
{
public:
    FieldModelPMRPtr_variants_ptr_ExprContainer(FBEBuffer& buffer, size_t offset) noexcept;
    ~FieldModelPMRPtr_variants_ptr_ExprContainer();

    // Get the field offset
    size_t fbe_offset() const noexcept { return _offset; }
    // Get the field size
    size_t fbe_size() const noexcept { return 5; }
    // Get the field extra size
    size_t fbe_extra() const noexcept;
    // Get the field type
    static constexpr size_t fbe_type() noexcept { return 2; }

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept { _offset -= size; }

    // Check if the struct value is valid
    bool verify() const noexcept;
    // Check if the struct ptr is nullptr
    bool has_value() const noexcept;

    // Get the struct value (begin phase)
    size_t get_begin() const noexcept;
    // Get the struct value (end phase)
    void get_end(size_t fbe_begin) const noexcept;

    // Get the struct value
    void get(::variants_ptr_pmr::ExprContainer** fbe_value, pmr::memory_resource* resource) noexcept;
    // Get the struct fields values
    void get_fields(::variants_ptr_pmr::ExprContainer& fbe_value, size_t fbe_struct_size, pmr::memory_resource* resource) noexcept;

    // Set the struct value (begin phase)
    size_t set_begin(bool has_value);
    // Set the struct value (end phase)
    void set_end(size_t fbe_begin);

    // Set the struct value
    void set(const ::variants_ptr_pmr::ExprContainer* fbe_value, pmr::memory_resource* resource) noexcept;
    // Set the struct fields values
    void set_fields(const ::variants_ptr_pmr::ExprContainer* fbe_value, pmr::memory_resource* resource) noexcept;

private:
    FBEBuffer& _buffer;
    size_t _offset;

public:
    BaseFieldModel* ptr{nullptr};
};

class FieldModelPMRPtr_variants_ptr_Value
{
public:
    FieldModelPMRPtr_variants_ptr_Value(FBEBuffer& buffer, size_t offset) noexcept;
    ~FieldModelPMRPtr_variants_ptr_Value();

    // Get the field offset
    size_t fbe_offset() const noexcept { return _offset; }
    // Get the field size
    size_t fbe_size() const noexcept { return 5; }
    // Get the field extra size
    size_t fbe_extra() const noexcept;
    // Get the field type
    static constexpr size_t fbe_type() noexcept { return 3; }

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept { _offset -= size; }

    // Check if the struct value is valid
    bool verify() const noexcept;
    // Check if the struct ptr is nullptr
    bool has_value() const noexcept;

    // Get the struct value (begin phase)
    size_t get_begin() const noexcept;
    // Get the struct value (end phase)
    void get_end(size_t fbe_begin) const noexcept;

    // Get the struct value
    void get(::variants_ptr_pmr::Value** fbe_value, pmr::memory_resource* resource) noexcept;
    // Get the struct fields values
    void get_fields(::variants_ptr_pmr::Value& fbe_value, size_t fbe_struct_size, pmr::memory_resource* resource) noexcept;

    // Set the struct value (begin phase)
    size_t set_begin(bool has_value);
    // Set the struct value (end phase)
    void set_end(size_t fbe_begin);

    // Set the struct value
    void set(const ::variants_ptr_pmr::Value* fbe_value, pmr::memory_resource* resource) noexcept;
    // Set the struct fields values
    void set_fields(const ::variants_ptr_pmr::Value* fbe_value, pmr::memory_resource* resource) noexcept;

private:
    FBEBuffer& _buffer;
    size_t _offset;

public:
    BaseFieldModel* ptr{nullptr};
};

class FieldModelPMRPtr_variants_ptr_ValueContainer
{
public:
    FieldModelPMRPtr_variants_ptr_ValueContainer(FBEBuffer& buffer, size_t offset) noexcept;
    ~FieldModelPMRPtr_variants_ptr_ValueContainer();

    // Get the field offset
    size_t fbe_offset() const noexcept { return _offset; }
    // Get the field size
    size_t fbe_size() const noexcept { return 5; }
    // Get the field extra size
    size_t fbe_extra() const noexcept;
    // Get the field type
    static constexpr size_t fbe_type() noexcept { return 4; }

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept { _offset -= size; }

    // Check if the struct value is valid
    bool verify() const noexcept;
    // Check if the struct ptr is nullptr
    bool has_value() const noexcept;

    // Get the struct value (begin phase)
    size_t get_begin() const noexcept;
    // Get the struct value (end phase)
    void get_end(size_t fbe_begin) const noexcept;

    // Get the struct value
    void get(::variants_ptr_pmr::ValueContainer** fbe_value, pmr::memory_resource* resource) noexcept;
    // Get the struct fields values
    void get_fields(::variants_ptr_pmr::ValueContainer& fbe_value, size_t fbe_struct_size, pmr::memory_resource* resource) noexcept;

    // Set the struct value (begin phase)
    size_t set_begin(bool has_value);
    // Set the struct value (end phase)
    void set_end(size_t fbe_begin);

    // Set the struct value
    void set(const ::variants_ptr_pmr::ValueContainer* fbe_value, pmr::memory_resource* resource) noexcept;
    // Set the struct fields values
    void set_fields(const ::variants_ptr_pmr::ValueContainer* fbe_value, pmr::memory_resource* resource) noexcept;

private:
    FBEBuffer& _buffer;
    size_t _offset;

public:
    BaseFieldModel* ptr{nullptr};
};

class FieldModelPMRPtr_variants_ptr_Scalar1Container
{
public:
    FieldModelPMRPtr_variants_ptr_Scalar1Container(FBEBuffer& buffer, size_t offset) noexcept;
    ~FieldModelPMRPtr_variants_ptr_Scalar1Container();

    // Get the field offset
    size_t fbe_offset() const noexcept { return _offset; }
    // Get the field size
    size_t fbe_size() const noexcept { return 5; }
    // Get the field extra size
    size_t fbe_extra() const noexcept;
    // Get the field type
    static constexpr size_t fbe_type() noexcept { return 5; }

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept { _offset -= size; }

    // Check if the struct value is valid
    bool verify() const noexcept;
    // Check if the struct ptr is nullptr
    bool has_value() const noexcept;

    // Get the struct value (begin phase)
    size_t get_begin() const noexcept;
    // Get the struct value (end phase)
    void get_end(size_t fbe_begin) const noexcept;

    // Get the struct value
    void get(::variants_ptr_pmr::Scalar1Container** fbe_value, pmr::memory_resource* resource) noexcept;
    // Get the struct fields values
    void get_fields(::variants_ptr_pmr::Scalar1Container& fbe_value, size_t fbe_struct_size, pmr::memory_resource* resource) noexcept;

    // Set the struct value (begin phase)
    size_t set_begin(bool has_value);
    // Set the struct value (end phase)
    void set_end(size_t fbe_begin);

    // Set the struct value
    void set(const ::variants_ptr_pmr::Scalar1Container* fbe_value, pmr::memory_resource* resource) noexcept;
    // Set the struct fields values
    void set_fields(const ::variants_ptr_pmr::Scalar1Container* fbe_value, pmr::memory_resource* resource) noexcept;

private:
    FBEBuffer& _buffer;
    size_t _offset;

public:
    BaseFieldModel* ptr{nullptr};
};

// Fast Binary Encoding ::variants_ptr_pmr::Simple field model
class FieldModelPMR_variants_ptr_Simple : public BaseFieldModel
{
public:
    FieldModelPMR_variants_ptr_Simple(FBEBuffer& buffer, size_t offset) noexcept;

    // Get the field offset
    size_t fbe_offset() const noexcept override { return _offset; }
    // Get the field size
    size_t fbe_size() const noexcept override { return 4; }
    // Get the field body size
    size_t fbe_body() const noexcept;
    // Get the field extra size
    size_t fbe_extra() const noexcept override;
    // Get the field type
    static constexpr size_t fbe_type() noexcept { return 1; }

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept override { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept override { _offset -= size; }

    // Check if the struct value is valid
    bool verify(bool fbe_verify_type = true) const noexcept override;
    // Check if the struct fields are valid
    bool verify_fields(size_t fbe_struct_size) const noexcept override;

    // Get the struct value (begin phase)
    size_t get_begin() const noexcept override;
    // Get the struct value (end phase)
    void get_end(size_t fbe_begin) const noexcept override;

    // Get the struct value
    void get(::FBE::Base& fbe_value, pmr::memory_resource* resource) noexcept override;
    // Get the struct fields values
    void get_fields(::FBE::Base& fbe_value, size_t fbe_struct_size, pmr::memory_resource* resource) noexcept override;

    // Set the struct value (begin phase)
    size_t set_begin() override;
    // Set the struct value (end phase)
    void set_end(size_t fbe_begin) override;

    // Set the struct value
    void set(const ::FBE::Base& fbe_value, pmr::memory_resource* resource) noexcept override;
    // Set the struct fields values
    void set_fields(const ::FBE::Base& fbe_value, pmr::memory_resource* resource) noexcept override;

private:
    FBEBuffer& _buffer;
    size_t _offset;

public:
    FieldModel<ArenaString> name;
};

namespace variants_ptr_pmr {

// Fast Binary Encoding Simple model
class SimpleModel : public FBE::Model
{
public:
    SimpleModel() : model(this->buffer(), 4) {}
    SimpleModel(const std::shared_ptr<FBEBuffer>& buffer) : FBE::Model(buffer), model(this->buffer(), 4) {}

    // Get the model size
    size_t fbe_size() const noexcept { return model.fbe_size() + model.fbe_extra(); }
    // Get the model type
    static constexpr size_t fbe_type() noexcept { return FieldModelPMR_variants_ptr_Simple::fbe_type(); }

    // Check if the struct value is valid
    bool verify();

    // Create a new model (begin phase)
    size_t create_begin();
    // Create a new model (end phase)
    size_t create_end(size_t fbe_begin);

    // Serialize the struct value
    size_t serialize(const ::variants_ptr_pmr::Simple& value, pmr::memory_resource* resource);
    // Deserialize the struct value
    size_t deserialize(::variants_ptr_pmr::Simple& value, pmr::memory_resource* resource) noexcept;

    // Move to the next struct value
    void next(size_t prev) noexcept { model.fbe_shift(prev); }

public:
    FieldModelPMR_variants_ptr_Simple model;
};

} // namespace variants_ptr_pmr

template<>
struct model_of<::variants_ptr_pmr::Simple> {
using type = variants_ptr_pmr::SimpleModel;
};


// Fast Binary Encoding ::variants_ptr_pmr::ExprContainer field model
class FieldModelPMR_variants_ptr_ExprContainer : public BaseFieldModel
{
public:
    FieldModelPMR_variants_ptr_ExprContainer(FBEBuffer& buffer, size_t offset) noexcept;

    // Get the field offset
    size_t fbe_offset() const noexcept override { return _offset; }
    // Get the field size
    size_t fbe_size() const noexcept override { return 4; }
    // Get the field body size
    size_t fbe_body() const noexcept;
    // Get the field extra size
    size_t fbe_extra() const noexcept override;
    // Get the field type
    static constexpr size_t fbe_type() noexcept { return 2; }

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept override { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept override { _offset -= size; }

    // Check if the struct value is valid
    bool verify(bool fbe_verify_type = true) const noexcept override;
    // Check if the struct fields are valid
    bool verify_fields(size_t fbe_struct_size) const noexcept override;

    // Get the struct value (begin phase)
    size_t get_begin() const noexcept override;
    // Get the struct value (end phase)
    void get_end(size_t fbe_begin) const noexcept override;

    // Get the struct value
    void get(::FBE::Base& fbe_value, pmr::memory_resource* resource) noexcept override;
    // Get the struct fields values
    void get_fields(::FBE::Base& fbe_value, size_t fbe_struct_size, pmr::memory_resource* resource) noexcept override;

    // Set the struct value (begin phase)
    size_t set_begin() override;
    // Set the struct value (end phase)
    void set_end(size_t fbe_begin) override;

    // Set the struct value
    void set(const ::FBE::Base& fbe_value, pmr::memory_resource* resource) noexcept override;
    // Set the struct fields values
    void set_fields(const ::FBE::Base& fbe_value, pmr::memory_resource* resource) noexcept override;

private:
    FBEBuffer& _buffer;
    size_t _offset;

public:
    FieldModel<::variants_ptr_pmr::Expr> e;
    FieldModel<std::optional<::variants_ptr_pmr::Expr>> eo;
    FieldModelStructOptional<FieldModelPMR_variants_ptr_Simple, ::variants_ptr_pmr::Simple> so;
};

namespace variants_ptr_pmr {

// Fast Binary Encoding ExprContainer model
class ExprContainerModel : public FBE::Model
{
public:
    ExprContainerModel() : model(this->buffer(), 4) {}
    ExprContainerModel(const std::shared_ptr<FBEBuffer>& buffer) : FBE::Model(buffer), model(this->buffer(), 4) {}

    // Get the model size
    size_t fbe_size() const noexcept { return model.fbe_size() + model.fbe_extra(); }
    // Get the model type
    static constexpr size_t fbe_type() noexcept { return FieldModelPMR_variants_ptr_ExprContainer::fbe_type(); }

    // Check if the struct value is valid
    bool verify();

    // Create a new model (begin phase)
    size_t create_begin();
    // Create a new model (end phase)
    size_t create_end(size_t fbe_begin);

    // Serialize the struct value
    size_t serialize(const ::variants_ptr_pmr::ExprContainer& value, pmr::memory_resource* resource);
    // Deserialize the struct value
    size_t deserialize(::variants_ptr_pmr::ExprContainer& value, pmr::memory_resource* resource) noexcept;

    // Move to the next struct value
    void next(size_t prev) noexcept { model.fbe_shift(prev); }

public:
    FieldModelPMR_variants_ptr_ExprContainer model;
};

} // namespace variants_ptr_pmr

template<>
struct model_of<::variants_ptr_pmr::ExprContainer> {
using type = variants_ptr_pmr::ExprContainerModel;
};


// Fast Binary Encoding ::variants_ptr_pmr::Value field model
class FieldModelPMR_variants_ptr_Value : public BaseFieldModel
{
public:
    FieldModelPMR_variants_ptr_Value(FBEBuffer& buffer, size_t offset) noexcept;

    // Get the field offset
    size_t fbe_offset() const noexcept override { return _offset; }
    // Get the field size
    size_t fbe_size() const noexcept override { return 4; }
    // Get the field body size
    size_t fbe_body() const noexcept;
    // Get the field extra size
    size_t fbe_extra() const noexcept override;
    // Get the field type
    static constexpr size_t fbe_type() noexcept { return 3; }

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept override { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept override { _offset -= size; }

    // Check if the struct value is valid
    bool verify(bool fbe_verify_type = true) const noexcept override;
    // Check if the struct fields are valid
    bool verify_fields(size_t fbe_struct_size) const noexcept override;

    // Get the struct value (begin phase)
    size_t get_begin() const noexcept override;
    // Get the struct value (end phase)
    void get_end(size_t fbe_begin) const noexcept override;

    // Get the struct value
    void get(::FBE::Base& fbe_value, pmr::memory_resource* resource) noexcept override;
    // Get the struct fields values
    void get_fields(::FBE::Base& fbe_value, size_t fbe_struct_size, pmr::memory_resource* resource) noexcept override;

    // Set the struct value (begin phase)
    size_t set_begin() override;
    // Set the struct value (end phase)
    void set_end(size_t fbe_begin) override;

    // Set the struct value
    void set(const ::FBE::Base& fbe_value, pmr::memory_resource* resource) noexcept override;
    // Set the struct fields values
    void set_fields(const ::FBE::Base& fbe_value, pmr::memory_resource* resource) noexcept override;

private:
    FBEBuffer& _buffer;
    size_t _offset;

public:
    FieldModel<::variants_ptr_pmr::V> v;
    FieldModel<std::optional<::variants_ptr_pmr::V>> vo;
    FieldModel<std::optional<::variants_ptr_pmr::V>> vo2;
};

namespace variants_ptr_pmr {

// Fast Binary Encoding Value model
class ValueModel : public FBE::Model
{
public:
    ValueModel() : model(this->buffer(), 4) {}
    ValueModel(const std::shared_ptr<FBEBuffer>& buffer) : FBE::Model(buffer), model(this->buffer(), 4) {}

    // Get the model size
    size_t fbe_size() const noexcept { return model.fbe_size() + model.fbe_extra(); }
    // Get the model type
    static constexpr size_t fbe_type() noexcept { return FieldModelPMR_variants_ptr_Value::fbe_type(); }

    // Check if the struct value is valid
    bool verify();

    // Create a new model (begin phase)
    size_t create_begin();
    // Create a new model (end phase)
    size_t create_end(size_t fbe_begin);

    // Serialize the struct value
    size_t serialize(const ::variants_ptr_pmr::Value& value, pmr::memory_resource* resource);
    // Deserialize the struct value
    size_t deserialize(::variants_ptr_pmr::Value& value, pmr::memory_resource* resource) noexcept;

    // Move to the next struct value
    void next(size_t prev) noexcept { model.fbe_shift(prev); }

public:
    FieldModelPMR_variants_ptr_Value model;
};

} // namespace variants_ptr_pmr

template<>
struct model_of<::variants_ptr_pmr::Value> {
using type = variants_ptr_pmr::ValueModel;
};


// Fast Binary Encoding ::variants_ptr_pmr::ValueContainer field model
class FieldModelPMR_variants_ptr_ValueContainer : public BaseFieldModel
{
public:
    FieldModelPMR_variants_ptr_ValueContainer(FBEBuffer& buffer, size_t offset) noexcept;

    // Get the field offset
    size_t fbe_offset() const noexcept override { return _offset; }
    // Get the field size
    size_t fbe_size() const noexcept override { return 4; }
    // Get the field body size
    size_t fbe_body() const noexcept;
    // Get the field extra size
    size_t fbe_extra() const noexcept override;
    // Get the field type
    static constexpr size_t fbe_type() noexcept { return 4; }

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept override { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept override { _offset -= size; }

    // Check if the struct value is valid
    bool verify(bool fbe_verify_type = true) const noexcept override;
    // Check if the struct fields are valid
    bool verify_fields(size_t fbe_struct_size) const noexcept override;

    // Get the struct value (begin phase)
    size_t get_begin() const noexcept override;
    // Get the struct value (end phase)
    void get_end(size_t fbe_begin) const noexcept override;

    // Get the struct value
    void get(::FBE::Base& fbe_value, pmr::memory_resource* resource) noexcept override;
    // Get the struct fields values
    void get_fields(::FBE::Base& fbe_value, size_t fbe_struct_size, pmr::memory_resource* resource) noexcept override;

    // Set the struct value (begin phase)
    size_t set_begin() override;
    // Set the struct value (end phase)
    void set_end(size_t fbe_begin) override;

    // Set the struct value
    void set(const ::FBE::Base& fbe_value, pmr::memory_resource* resource) noexcept override;
    // Set the struct fields values
    void set_fields(const ::FBE::Base& fbe_value, pmr::memory_resource* resource) noexcept override;

private:
    FBEBuffer& _buffer;
    size_t _offset;

public:
    FieldModelVector<::variants_ptr_pmr::V> vv;
    FieldModelMap<int32_t, ::variants_ptr_pmr::V> vm;
};

namespace variants_ptr_pmr {

// Fast Binary Encoding ValueContainer model
class ValueContainerModel : public FBE::Model
{
public:
    ValueContainerModel() : model(this->buffer(), 4) {}
    ValueContainerModel(const std::shared_ptr<FBEBuffer>& buffer) : FBE::Model(buffer), model(this->buffer(), 4) {}

    // Get the model size
    size_t fbe_size() const noexcept { return model.fbe_size() + model.fbe_extra(); }
    // Get the model type
    static constexpr size_t fbe_type() noexcept { return FieldModelPMR_variants_ptr_ValueContainer::fbe_type(); }

    // Check if the struct value is valid
    bool verify();

    // Create a new model (begin phase)
    size_t create_begin();
    // Create a new model (end phase)
    size_t create_end(size_t fbe_begin);

    // Serialize the struct value
    size_t serialize(const ::variants_ptr_pmr::ValueContainer& value, pmr::memory_resource* resource);
    // Deserialize the struct value
    size_t deserialize(::variants_ptr_pmr::ValueContainer& value, pmr::memory_resource* resource) noexcept;

    // Move to the next struct value
    void next(size_t prev) noexcept { model.fbe_shift(prev); }

public:
    FieldModelPMR_variants_ptr_ValueContainer model;
};

} // namespace variants_ptr_pmr

template<>
struct model_of<::variants_ptr_pmr::ValueContainer> {
using type = variants_ptr_pmr::ValueContainerModel;
};


// Fast Binary Encoding ::variants_ptr_pmr::Scalar1Container field model
class FieldModelPMR_variants_ptr_Scalar1Container : public BaseFieldModel
{
public:
    FieldModelPMR_variants_ptr_Scalar1Container(FBEBuffer& buffer, size_t offset) noexcept;

    // Get the field offset
    size_t fbe_offset() const noexcept override { return _offset; }
    // Get the field size
    size_t fbe_size() const noexcept override { return 4; }
    // Get the field body size
    size_t fbe_body() const noexcept;
    // Get the field extra size
    size_t fbe_extra() const noexcept override;
    // Get the field type
    static constexpr size_t fbe_type() noexcept { return 5; }

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept override { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept override { _offset -= size; }

    // Check if the struct value is valid
    bool verify(bool fbe_verify_type = true) const noexcept override;
    // Check if the struct fields are valid
    bool verify_fields(size_t fbe_struct_size) const noexcept override;

    // Get the struct value (begin phase)
    size_t get_begin() const noexcept override;
    // Get the struct value (end phase)
    void get_end(size_t fbe_begin) const noexcept override;

    // Get the struct value
    void get(::FBE::Base& fbe_value, pmr::memory_resource* resource) noexcept override;
    // Get the struct fields values
    void get_fields(::FBE::Base& fbe_value, size_t fbe_struct_size, pmr::memory_resource* resource) noexcept override;

    // Set the struct value (begin phase)
    size_t set_begin() override;
    // Set the struct value (end phase)
    void set_end(size_t fbe_begin) override;

    // Set the struct value
    void set(const ::FBE::Base& fbe_value, pmr::memory_resource* resource) noexcept override;
    // Set the struct fields values
    void set_fields(const ::FBE::Base& fbe_value, pmr::memory_resource* resource) noexcept override;

private:
    FBEBuffer& _buffer;
    size_t _offset;

public:
    FieldModelMap<::variants_ptr_pmr::Scalar1, ::variants_ptr_pmr::Expr> s;
};

namespace variants_ptr_pmr {

// Fast Binary Encoding Scalar1Container model
class Scalar1ContainerModel : public FBE::Model
{
public:
    Scalar1ContainerModel() : model(this->buffer(), 4) {}
    Scalar1ContainerModel(const std::shared_ptr<FBEBuffer>& buffer) : FBE::Model(buffer), model(this->buffer(), 4) {}

    // Get the model size
    size_t fbe_size() const noexcept { return model.fbe_size() + model.fbe_extra(); }
    // Get the model type
    static constexpr size_t fbe_type() noexcept { return FieldModelPMR_variants_ptr_Scalar1Container::fbe_type(); }

    // Check if the struct value is valid
    bool verify();

    // Create a new model (begin phase)
    size_t create_begin();
    // Create a new model (end phase)
    size_t create_end(size_t fbe_begin);

    // Serialize the struct value
    size_t serialize(const ::variants_ptr_pmr::Scalar1Container& value, pmr::memory_resource* resource);
    // Deserialize the struct value
    size_t deserialize(::variants_ptr_pmr::Scalar1Container& value, pmr::memory_resource* resource) noexcept;

    // Move to the next struct value
    void next(size_t prev) noexcept { model.fbe_shift(prev); }

public:
    FieldModelPMR_variants_ptr_Scalar1Container model;
};

} // namespace variants_ptr_pmr

template<>
struct model_of<::variants_ptr_pmr::Scalar1Container> {
using type = variants_ptr_pmr::Scalar1ContainerModel;
};


} // namespace FBE
