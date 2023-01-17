//------------------------------------------------------------------------------
// Automatically generated by the Fast Binary Encoding compiler, do not modify!
// https://github.com/chronoxor/FastBinaryEncoding
// Source: protoex.fbe
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

#include "protoex_pmr.h"

#include "proto_pmr_models.h"

namespace FBE {

// Fast Binary Encoding ::protoex_pmr::OrderSide field model
template <>
class FieldModel<::protoex_pmr::OrderSide> : public FieldModelBase<::protoex_pmr::OrderSide, uint8_t>
{
public:
    using FieldModelBase<::protoex_pmr::OrderSide, uint8_t>::FieldModelBase;
};

// Fast Binary Encoding ::protoex_pmr::OrderType field model
template <>
class FieldModel<::protoex_pmr::OrderType> : public FieldModelBase<::protoex_pmr::OrderType, uint8_t>
{
public:
    using FieldModelBase<::protoex_pmr::OrderType, uint8_t>::FieldModelBase;
};

// Fast Binary Encoding ::protoex_pmr::StateEx field model
template <>
class FieldModel<::protoex_pmr::StateEx> : public FieldModelBase<::protoex_pmr::StateEx, uint8_t>
{
public:
    using FieldModelBase<::protoex_pmr::StateEx, uint8_t>::FieldModelBase;
};

// Fast Binary Encoding ::protoex_pmr::Order field model
template <>
class FieldModel<::protoex_pmr::Order>
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
    void get(::protoex_pmr::Order& fbe_value) const noexcept;
    // Get the struct fields values
    void get_fields(::protoex_pmr::Order& fbe_value, size_t fbe_struct_size) const noexcept;

    // Set the struct value (begin phase)
    size_t set_begin();
    // Set the struct value (end phase)
    void set_end(size_t fbe_begin);

    // Set the struct value
    void set(const ::protoex_pmr::Order& fbe_value) noexcept;
    // Set the struct fields values
    void set_fields(const ::protoex_pmr::Order& fbe_value) noexcept;

private:
    FBEBuffer& _buffer;
    size_t _offset;

public:
    FieldModel<int32_t> id;
    FieldModel<stdb::memory::arena_string> symbol;
    FieldModel<::protoex_pmr::OrderSide> side;
    FieldModel<::protoex_pmr::OrderType> type;
    FieldModel<double> price;
    FieldModel<double> volume;
    FieldModel<double> tp;
    FieldModel<double> sl;
};

namespace protoex_pmr {

// Fast Binary Encoding Order model
class OrderModel : public FBE::Model
{
public:
    OrderModel() : model(this->buffer(), 4) {}
    OrderModel(const std::shared_ptr<FBEBuffer>& buffer) : FBE::Model(buffer), model(this->buffer(), 4) {}

    // Get the model size
    size_t fbe_size() const noexcept { return model.fbe_size() + model.fbe_extra(); }
    // Get the model type
    static constexpr size_t fbe_type() noexcept { return FieldModel<::protoex_pmr::Order>::fbe_type(); }

    // Check if the struct value is valid
    bool verify();

    // Create a new model (begin phase)
    size_t create_begin();
    // Create a new model (end phase)
    size_t create_end(size_t fbe_begin);

    // Serialize the struct value
    size_t serialize(const ::protoex_pmr::Order& value);
    // Deserialize the struct value
    size_t deserialize(::protoex_pmr::Order& value) const noexcept;

    // Move to the next struct value
    void next(size_t prev) noexcept { model.fbe_shift(prev); }

public:
    FieldModel<::protoex_pmr::Order> model;
};

} // namespace protoex_pmr

// Fast Binary Encoding ::protoex_pmr::Balance field model
template <>
class FieldModel<::protoex_pmr::Balance>
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
    static constexpr size_t fbe_type() noexcept { return FieldModel<::proto_pmr::Balance>::fbe_type(); }

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
    void get(::protoex_pmr::Balance& fbe_value) const noexcept;
    // Get the struct fields values
    void get_fields(::protoex_pmr::Balance& fbe_value, size_t fbe_struct_size) const noexcept;

    // Set the struct value (begin phase)
    size_t set_begin();
    // Set the struct value (end phase)
    void set_end(size_t fbe_begin);

    // Set the struct value
    void set(const ::protoex_pmr::Balance& fbe_value) noexcept;
    // Set the struct fields values
    void set_fields(const ::protoex_pmr::Balance& fbe_value) noexcept;

private:
    FBEBuffer& _buffer;
    size_t _offset;

public:
    FieldModel<::proto_pmr::Balance> parent;
    FieldModel<double> locked;
};

namespace protoex_pmr {

// Fast Binary Encoding Balance model
class BalanceModel : public FBE::Model
{
public:
    BalanceModel() : model(this->buffer(), 4) {}
    BalanceModel(const std::shared_ptr<FBEBuffer>& buffer) : FBE::Model(buffer), model(this->buffer(), 4) {}

    // Get the model size
    size_t fbe_size() const noexcept { return model.fbe_size() + model.fbe_extra(); }
    // Get the model type
    static constexpr size_t fbe_type() noexcept { return FieldModel<::protoex_pmr::Balance>::fbe_type(); }

    // Check if the struct value is valid
    bool verify();

    // Create a new model (begin phase)
    size_t create_begin();
    // Create a new model (end phase)
    size_t create_end(size_t fbe_begin);

    // Serialize the struct value
    size_t serialize(const ::protoex_pmr::Balance& value);
    // Deserialize the struct value
    size_t deserialize(::protoex_pmr::Balance& value) const noexcept;

    // Move to the next struct value
    void next(size_t prev) noexcept { model.fbe_shift(prev); }

public:
    FieldModel<::protoex_pmr::Balance> model;
};

} // namespace protoex_pmr

// Fast Binary Encoding ::protoex_pmr::Account field model
template <>
class FieldModel<::protoex_pmr::Account>
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
    static constexpr size_t fbe_type() noexcept { return 3; }

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
    void get(::protoex_pmr::Account& fbe_value) const noexcept;
    // Get the struct fields values
    void get_fields(::protoex_pmr::Account& fbe_value, size_t fbe_struct_size) const noexcept;

    // Set the struct value (begin phase)
    size_t set_begin();
    // Set the struct value (end phase)
    void set_end(size_t fbe_begin);

    // Set the struct value
    void set(const ::protoex_pmr::Account& fbe_value) noexcept;
    // Set the struct fields values
    void set_fields(const ::protoex_pmr::Account& fbe_value) noexcept;

private:
    FBEBuffer& _buffer;
    size_t _offset;

public:
    FieldModel<int32_t> id;
    FieldModel<stdb::memory::arena_string> name;
    FieldModel<::protoex_pmr::StateEx> state;
    FieldModel<::protoex_pmr::Balance> wallet;
    FieldModel<std::optional<::protoex_pmr::Balance>> asset;
    FieldModelVector<::protoex_pmr::Order> orders;
};

namespace protoex_pmr {

// Fast Binary Encoding Account model
class AccountModel : public FBE::Model
{
public:
    AccountModel() : model(this->buffer(), 4) {}
    AccountModel(const std::shared_ptr<FBEBuffer>& buffer) : FBE::Model(buffer), model(this->buffer(), 4) {}

    // Get the model size
    size_t fbe_size() const noexcept { return model.fbe_size() + model.fbe_extra(); }
    // Get the model type
    static constexpr size_t fbe_type() noexcept { return FieldModel<::protoex_pmr::Account>::fbe_type(); }

    // Check if the struct value is valid
    bool verify();

    // Create a new model (begin phase)
    size_t create_begin();
    // Create a new model (end phase)
    size_t create_end(size_t fbe_begin);

    // Serialize the struct value
    size_t serialize(const ::protoex_pmr::Account& value);
    // Deserialize the struct value
    size_t deserialize(::protoex_pmr::Account& value) const noexcept;

    // Move to the next struct value
    void next(size_t prev) noexcept { model.fbe_shift(prev); }

public:
    FieldModel<::protoex_pmr::Account> model;
};

} // namespace protoex_pmr

// Fast Binary Encoding ::protoex_pmr::OrderMessage field model
template <>
class FieldModel<::protoex_pmr::OrderMessage>
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
    static constexpr size_t fbe_type() noexcept { return 11; }

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
    void get(::protoex_pmr::OrderMessage& fbe_value) const noexcept;
    // Get the struct fields values
    void get_fields(::protoex_pmr::OrderMessage& fbe_value, size_t fbe_struct_size) const noexcept;

    // Set the struct value (begin phase)
    size_t set_begin();
    // Set the struct value (end phase)
    void set_end(size_t fbe_begin);

    // Set the struct value
    void set(const ::protoex_pmr::OrderMessage& fbe_value) noexcept;
    // Set the struct fields values
    void set_fields(const ::protoex_pmr::OrderMessage& fbe_value) noexcept;

private:
    FBEBuffer& _buffer;
    size_t _offset;

public:
    FieldModel<::protoex_pmr::Order> body;
};

namespace protoex_pmr {

// Fast Binary Encoding OrderMessage model
class OrderMessageModel : public FBE::Model
{
public:
    OrderMessageModel() : model(this->buffer(), 4) {}
    OrderMessageModel(const std::shared_ptr<FBEBuffer>& buffer) : FBE::Model(buffer), model(this->buffer(), 4) {}

    // Get the model size
    size_t fbe_size() const noexcept { return model.fbe_size() + model.fbe_extra(); }
    // Get the model type
    static constexpr size_t fbe_type() noexcept { return FieldModel<::protoex_pmr::OrderMessage>::fbe_type(); }

    // Check if the struct value is valid
    bool verify();

    // Create a new model (begin phase)
    size_t create_begin();
    // Create a new model (end phase)
    size_t create_end(size_t fbe_begin);

    // Serialize the struct value
    size_t serialize(const ::protoex_pmr::OrderMessage& value);
    // Deserialize the struct value
    size_t deserialize(::protoex_pmr::OrderMessage& value) const noexcept;

    // Move to the next struct value
    void next(size_t prev) noexcept { model.fbe_shift(prev); }

public:
    FieldModel<::protoex_pmr::OrderMessage> model;
};

} // namespace protoex_pmr

// Fast Binary Encoding ::protoex_pmr::BalanceMessage field model
template <>
class FieldModel<::protoex_pmr::BalanceMessage>
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
    static constexpr size_t fbe_type() noexcept { return 12; }

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
    void get(::protoex_pmr::BalanceMessage& fbe_value) const noexcept;
    // Get the struct fields values
    void get_fields(::protoex_pmr::BalanceMessage& fbe_value, size_t fbe_struct_size) const noexcept;

    // Set the struct value (begin phase)
    size_t set_begin();
    // Set the struct value (end phase)
    void set_end(size_t fbe_begin);

    // Set the struct value
    void set(const ::protoex_pmr::BalanceMessage& fbe_value) noexcept;
    // Set the struct fields values
    void set_fields(const ::protoex_pmr::BalanceMessage& fbe_value) noexcept;

private:
    FBEBuffer& _buffer;
    size_t _offset;

public:
    FieldModel<::protoex_pmr::Balance> body;
};

namespace protoex_pmr {

// Fast Binary Encoding BalanceMessage model
class BalanceMessageModel : public FBE::Model
{
public:
    BalanceMessageModel() : model(this->buffer(), 4) {}
    BalanceMessageModel(const std::shared_ptr<FBEBuffer>& buffer) : FBE::Model(buffer), model(this->buffer(), 4) {}

    // Get the model size
    size_t fbe_size() const noexcept { return model.fbe_size() + model.fbe_extra(); }
    // Get the model type
    static constexpr size_t fbe_type() noexcept { return FieldModel<::protoex_pmr::BalanceMessage>::fbe_type(); }

    // Check if the struct value is valid
    bool verify();

    // Create a new model (begin phase)
    size_t create_begin();
    // Create a new model (end phase)
    size_t create_end(size_t fbe_begin);

    // Serialize the struct value
    size_t serialize(const ::protoex_pmr::BalanceMessage& value);
    // Deserialize the struct value
    size_t deserialize(::protoex_pmr::BalanceMessage& value) const noexcept;

    // Move to the next struct value
    void next(size_t prev) noexcept { model.fbe_shift(prev); }

public:
    FieldModel<::protoex_pmr::BalanceMessage> model;
};

} // namespace protoex_pmr

// Fast Binary Encoding ::protoex_pmr::AccountMessage field model
template <>
class FieldModel<::protoex_pmr::AccountMessage>
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
    static constexpr size_t fbe_type() noexcept { return 13; }

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
    void get(::protoex_pmr::AccountMessage& fbe_value) const noexcept;
    // Get the struct fields values
    void get_fields(::protoex_pmr::AccountMessage& fbe_value, size_t fbe_struct_size) const noexcept;

    // Set the struct value (begin phase)
    size_t set_begin();
    // Set the struct value (end phase)
    void set_end(size_t fbe_begin);

    // Set the struct value
    void set(const ::protoex_pmr::AccountMessage& fbe_value) noexcept;
    // Set the struct fields values
    void set_fields(const ::protoex_pmr::AccountMessage& fbe_value) noexcept;

private:
    FBEBuffer& _buffer;
    size_t _offset;

public:
    FieldModel<::protoex_pmr::Account> body;
};

namespace protoex_pmr {

// Fast Binary Encoding AccountMessage model
class AccountMessageModel : public FBE::Model
{
public:
    AccountMessageModel() : model(this->buffer(), 4) {}
    AccountMessageModel(const std::shared_ptr<FBEBuffer>& buffer) : FBE::Model(buffer), model(this->buffer(), 4) {}

    // Get the model size
    size_t fbe_size() const noexcept { return model.fbe_size() + model.fbe_extra(); }
    // Get the model type
    static constexpr size_t fbe_type() noexcept { return FieldModel<::protoex_pmr::AccountMessage>::fbe_type(); }

    // Check if the struct value is valid
    bool verify();

    // Create a new model (begin phase)
    size_t create_begin();
    // Create a new model (end phase)
    size_t create_end(size_t fbe_begin);

    // Serialize the struct value
    size_t serialize(const ::protoex_pmr::AccountMessage& value);
    // Deserialize the struct value
    size_t deserialize(::protoex_pmr::AccountMessage& value) const noexcept;

    // Move to the next struct value
    void next(size_t prev) noexcept { model.fbe_shift(prev); }

public:
    FieldModel<::protoex_pmr::AccountMessage> model;
};

} // namespace protoex_pmr

} // namespace FBE
