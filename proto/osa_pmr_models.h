//------------------------------------------------------------------------------
// Automatically generated by the Fast Binary Encoding compiler, do not modify!
// https://github.com/chronoxor/FastBinaryEncoding
// Source: osa.fbe
// FBE version: 1.10.0.0
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

#include "fbe_models.h"

#include "osa_pmr.h"

namespace FBE {

// Fast Binary Encoding ::osa_pmr::Sex field model
template <>
class FieldModel<::osa_pmr::Sex> : public FieldModelBase<::osa_pmr::Sex, int32_t>
{
public:
    using FieldModelBase<::osa_pmr::Sex, int32_t>::FieldModelBase;
};

// Fast Binary Encoding ::osa_pmr::MyFLags field model
template <>
class FieldModel<::osa_pmr::MyFLags> : public FieldModelBase<::osa_pmr::MyFLags, int32_t>
{
public:
    using FieldModelBase<::osa_pmr::MyFLags, int32_t>::FieldModelBase;
};

// Fast Binary Encoding ::osa_pmr::Extra field model
template <>
class FieldModel<::osa_pmr::Extra>
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
    // Get the field type
    static constexpr size_t fbe_type() noexcept { return 1; }

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept { _offset -= size; }

    // Check if the struct value is valid
    bool verify(bool fbe_verify_type = true) const noexcept;
    // Check if the struct fields are valid
    bool verify_fields(size_t fbe_struct_size) const noexcept;

    // Get the struct value (begin phase)
    size_t get_begin() const noexcept;
    // Get the struct value (end phase)
    void get_end(size_t fbe_begin) const noexcept;

    // Get the struct value
    void get(::osa_pmr::Extra& fbe_value) const noexcept;
    // Get the struct fields values
    void get_fields(::osa_pmr::Extra& fbe_value, size_t fbe_struct_size) const noexcept;

    // Set the struct value (begin phase)
    size_t set_begin();
    // Set the struct value (end phase)
    void set_end(size_t fbe_begin);

    // Set the struct value
    void set(const ::osa_pmr::Extra& fbe_value) noexcept;
    // Set the struct fields values
    void set_fields(const ::osa_pmr::Extra& fbe_value) noexcept;

private:
    FBEBuffer& _buffer;
    size_t _offset;

public:
    FieldModel<stdb::memory::arena_string> name;
    FieldModel<stdb::memory::arena_string> detail;
    FieldModel<::osa_pmr::Sex> sex;
    FieldModel<::osa_pmr::MyFLags> flag;
};

namespace osa_pmr {

// Fast Binary Encoding Extra model
class ExtraModel : public FBE::Model
{
public:
    ExtraModel() : model(this->buffer(), 4) {}
    ExtraModel(const std::shared_ptr<FBEBuffer>& buffer) : FBE::Model(buffer), model(this->buffer(), 4) {}

    // Get the model size
    size_t fbe_size() const noexcept { return model.fbe_size() + model.fbe_extra(); }
    // Get the model type
    static constexpr size_t fbe_type() noexcept { return FieldModel<::osa_pmr::Extra>::fbe_type(); }

    // Check if the struct value is valid
    bool verify();

    // Create a new model (begin phase)
    size_t create_begin();
    // Create a new model (end phase)
    size_t create_end(size_t fbe_begin);

    // Serialize the struct value
    size_t serialize(const ::osa_pmr::Extra& value);
    // Deserialize the struct value
    size_t deserialize(::osa_pmr::Extra& value) const noexcept;

    // Move to the next struct value
    void next(size_t prev) noexcept { model.fbe_shift(prev); }

public:
    FieldModel<::osa_pmr::Extra> model;
};

} // namespace osa_pmr

// Fast Binary Encoding ::osa_pmr::Simple field model
template <>
class FieldModel<::osa_pmr::Simple>
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
    // Get the field type
    static constexpr size_t fbe_type() noexcept { return 2; }

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept { _offset -= size; }

    // Check if the struct value is valid
    bool verify(bool fbe_verify_type = true) const noexcept;
    // Check if the struct fields are valid
    bool verify_fields(size_t fbe_struct_size) const noexcept;

    // Get the struct value (begin phase)
    size_t get_begin() const noexcept;
    // Get the struct value (end phase)
    void get_end(size_t fbe_begin) const noexcept;

    // Get the struct value
    void get(::osa_pmr::Simple& fbe_value) const noexcept;
    // Get the struct fields values
    void get_fields(::osa_pmr::Simple& fbe_value, size_t fbe_struct_size) const noexcept;

    // Set the struct value (begin phase)
    size_t set_begin();
    // Set the struct value (end phase)
    void set_end(size_t fbe_begin);

    // Set the struct value
    void set(const ::osa_pmr::Simple& fbe_value) noexcept;
    // Set the struct fields values
    void set_fields(const ::osa_pmr::Simple& fbe_value) noexcept;

private:
    FBEBuffer& _buffer;
    size_t _offset;

public:
    FieldModel<stdb::memory::arena_string> name;
    FieldModel<int32_t> depth;
    FieldModelArray<::osa_pmr::Extra, 1> sa;
    FieldModel<::osa_pmr::Sex> sex;
};

namespace osa_pmr {

// Fast Binary Encoding Simple model
class SimpleModel : public FBE::Model
{
public:
    SimpleModel() : model(this->buffer(), 4) {}
    SimpleModel(const std::shared_ptr<FBEBuffer>& buffer) : FBE::Model(buffer), model(this->buffer(), 4) {}

    // Get the model size
    size_t fbe_size() const noexcept { return model.fbe_size() + model.fbe_extra(); }
    // Get the model type
    static constexpr size_t fbe_type() noexcept { return FieldModel<::osa_pmr::Simple>::fbe_type(); }

    // Check if the struct value is valid
    bool verify();

    // Create a new model (begin phase)
    size_t create_begin();
    // Create a new model (end phase)
    size_t create_end(size_t fbe_begin);

    // Serialize the struct value
    size_t serialize(const ::osa_pmr::Simple& value);
    // Deserialize the struct value
    size_t deserialize(::osa_pmr::Simple& value) const noexcept;

    // Move to the next struct value
    void next(size_t prev) noexcept { model.fbe_shift(prev); }

public:
    FieldModel<::osa_pmr::Simple> model;
};

} // namespace osa_pmr

} // namespace FBE
