//------------------------------------------------------------------------------
// Automatically generated by the Fast Binary Encoding compiler, do not modify!
// https://github.com/chronoxor/FastBinaryEncoding
// Source: arena.fbe
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

#include "arena_pmr.h"

#include "arena_common_pmr_models.h"

namespace FBE {

// Fast Binary Encoding ::arena_pmr::Item field model
template <>
class FieldModel<::arena_pmr::Item>
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
    void get(::arena_pmr::Item& fbe_value) const noexcept;
    // Get the struct fields values
    void get_fields(::arena_pmr::Item& fbe_value, size_t fbe_struct_size) const noexcept;

    // Set the struct value (begin phase)
    size_t set_begin();
    // Set the struct value (end phase)
    void set_end(size_t fbe_begin);

    // Set the struct value
    void set(const ::arena_pmr::Item& fbe_value) noexcept;
    // Set the struct fields values
    void set_fields(const ::arena_pmr::Item& fbe_value) noexcept;

private:
    FBEBuffer& _buffer;
    size_t _offset;

public:
    FieldModel<::arena_common_pmr::Optr> optr;
    FieldModel<::arena_common_pmr::Alias> alias;
    FieldModelVector<::arena_common_pmr::Expression> expressions;
    FieldModelMap<int32_t, ::arena_common_pmr::Alias> aliases_int;
};

namespace arena_pmr {

// Fast Binary Encoding Item model
class ItemModel : public FBE::Model
{
public:
    ItemModel() : model(this->buffer(), 4) {}
    ItemModel(const std::shared_ptr<FBEBuffer>& buffer) : FBE::Model(buffer), model(this->buffer(), 4) {}

    // Get the model size
    size_t fbe_size() const noexcept { return model.fbe_size() + model.fbe_extra(); }
    // Get the model type
    static constexpr size_t fbe_type() noexcept { return FieldModel<::arena_pmr::Item>::fbe_type(); }

    // Check if the struct value is valid
    bool verify();

    // Create a new model (begin phase)
    size_t create_begin();
    // Create a new model (end phase)
    size_t create_end(size_t fbe_begin);

    // Serialize the struct value
    size_t serialize(const ::arena_pmr::Item& value);
    // Deserialize the struct value
    size_t deserialize(::arena_pmr::Item& value) const noexcept;

    // Move to the next struct value
    void next(size_t prev) noexcept { model.fbe_shift(prev); }

public:
    FieldModel<::arena_pmr::Item> model;
};

} // namespace arena_pmr

// Fast Binary Encoding ::arena_pmr::Item2 field model
template <>
class FieldModel<::arena_pmr::Item2>
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
    void get(::arena_pmr::Item2& fbe_value) const noexcept;
    // Get the struct fields values
    void get_fields(::arena_pmr::Item2& fbe_value, size_t fbe_struct_size) const noexcept;

    // Set the struct value (begin phase)
    size_t set_begin();
    // Set the struct value (end phase)
    void set_end(size_t fbe_begin);

    // Set the struct value
    void set(const ::arena_pmr::Item2& fbe_value) noexcept;
    // Set the struct fields values
    void set_fields(const ::arena_pmr::Item2& fbe_value) noexcept;

private:
    FBEBuffer& _buffer;
    size_t _offset;

public:
    FieldModel<FBE::pmr_buffer_t> bytes_v;
};

namespace arena_pmr {

// Fast Binary Encoding Item2 model
class Item2Model : public FBE::Model
{
public:
    Item2Model() : model(this->buffer(), 4) {}
    Item2Model(const std::shared_ptr<FBEBuffer>& buffer) : FBE::Model(buffer), model(this->buffer(), 4) {}

    // Get the model size
    size_t fbe_size() const noexcept { return model.fbe_size() + model.fbe_extra(); }
    // Get the model type
    static constexpr size_t fbe_type() noexcept { return FieldModel<::arena_pmr::Item2>::fbe_type(); }

    // Check if the struct value is valid
    bool verify();

    // Create a new model (begin phase)
    size_t create_begin();
    // Create a new model (end phase)
    size_t create_end(size_t fbe_begin);

    // Serialize the struct value
    size_t serialize(const ::arena_pmr::Item2& value);
    // Deserialize the struct value
    size_t deserialize(::arena_pmr::Item2& value) const noexcept;

    // Move to the next struct value
    void next(size_t prev) noexcept { model.fbe_shift(prev); }

public:
    FieldModel<::arena_pmr::Item2> model;
};

} // namespace arena_pmr

} // namespace FBE
