//------------------------------------------------------------------------------
// Automatically generated by the Fast Binary Encoding compiler, do not modify!
// https://github.com/chronoxor/FastBinaryEncoding
// Source: proto.fbe
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

#include "fbe_models.h"

#include "proto_pmr.h"

namespace FBE {

// Fast Binary Encoding ::proto_pmr::OrderSide field model
template <>
class FieldModel<::proto_pmr::OrderSide> : public FieldModelBase<::proto_pmr::OrderSide, uint8_t>
{
public:
    using FieldModelBase<::proto_pmr::OrderSide, uint8_t>::FieldModelBase;
};

// Fast Binary Encoding ::proto_pmr::OrderType field model
template <>
class FieldModel<::proto_pmr::OrderType> : public FieldModelBase<::proto_pmr::OrderType, uint8_t>
{
public:
    using FieldModelBase<::proto_pmr::OrderType, uint8_t>::FieldModelBase;
};

// Fast Binary Encoding ::proto_pmr::State field model
template <>
class FieldModel<::proto_pmr::State> : public FieldModelBase<::proto_pmr::State, uint8_t>
{
public:
    using FieldModelBase<::proto_pmr::State, uint8_t>::FieldModelBase;
};

// Fast Binary Encoding ::proto_pmr::Order field model
template <>
class FieldModel<::proto_pmr::Order>
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
    void get(::proto_pmr::Order& fbe_value, pmr::memory_resource* resource) const noexcept;
    // Get the struct fields values
    void get_fields(::proto_pmr::Order& fbe_value, size_t fbe_struct_size, pmr::memory_resource* resource) const noexcept;

    // Set the struct value (begin phase)
    size_t set_begin();
    // Set the struct value (end phase)
    void set_end(size_t fbe_begin);

    // Set the struct value
    void set(const ::proto_pmr::Order& fbe_value, pmr::memory_resource* resource) noexcept;
    // Set the struct fields values
    void set_fields(const ::proto_pmr::Order& fbe_value, pmr::memory_resource* resource) noexcept;

private:
    FBEBuffer& _buffer;
    size_t _offset;

public:
    FieldModel<int32_t> id;
    FieldModel<ArenaString> symbol;
    FieldModel<::proto_pmr::OrderSide> side;
    FieldModel<::proto_pmr::OrderType> type;
    FieldModel<double> price;
    FieldModel<double> volume;
};

namespace proto_pmr {

// Fast Binary Encoding Order model
class OrderModel : public FBE::Model
{
public:
    OrderModel() : model(this->buffer(), 4) {}
    OrderModel(const std::shared_ptr<FBEBuffer>& buffer) : FBE::Model(buffer), model(this->buffer(), 4) {}

    // Get the model size
    size_t fbe_size() const noexcept { return model.fbe_size() + model.fbe_extra(); }
    // Get the model type
    static constexpr size_t fbe_type() noexcept { return FieldModel<::proto_pmr::Order>::fbe_type(); }

    // Check if the struct value is valid
    bool verify();

    // Create a new model (begin phase)
    size_t create_begin();
    // Create a new model (end phase)
    size_t create_end(size_t fbe_begin);

    // Serialize the struct value
    size_t serialize(const ::proto_pmr::Order& value, pmr::memory_resource* resource);
    // Deserialize the struct value
    size_t deserialize(::proto_pmr::Order& value, pmr::memory_resource* resource) const noexcept;

    // Move to the next struct value
    void next(size_t prev) noexcept { model.fbe_shift(prev); }

public:
    FieldModel<::proto_pmr::Order> model;
};

} // namespace proto_pmr

template<>
struct model_of<::proto_pmr::Order> {
using type = proto_pmr::OrderModel;
};


// Fast Binary Encoding ::proto_pmr::Balance field model
template <>
class FieldModel<::proto_pmr::Balance>
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
    void get(::proto_pmr::Balance& fbe_value, pmr::memory_resource* resource) const noexcept;
    // Get the struct fields values
    void get_fields(::proto_pmr::Balance& fbe_value, size_t fbe_struct_size, pmr::memory_resource* resource) const noexcept;

    // Set the struct value (begin phase)
    size_t set_begin();
    // Set the struct value (end phase)
    void set_end(size_t fbe_begin);

    // Set the struct value
    void set(const ::proto_pmr::Balance& fbe_value, pmr::memory_resource* resource) noexcept;
    // Set the struct fields values
    void set_fields(const ::proto_pmr::Balance& fbe_value, pmr::memory_resource* resource) noexcept;

private:
    FBEBuffer& _buffer;
    size_t _offset;

public:
    FieldModel<ArenaString> currency;
    FieldModel<double> amount;
};

namespace proto_pmr {

// Fast Binary Encoding Balance model
class BalanceModel : public FBE::Model
{
public:
    BalanceModel() : model(this->buffer(), 4) {}
    BalanceModel(const std::shared_ptr<FBEBuffer>& buffer) : FBE::Model(buffer), model(this->buffer(), 4) {}

    // Get the model size
    size_t fbe_size() const noexcept { return model.fbe_size() + model.fbe_extra(); }
    // Get the model type
    static constexpr size_t fbe_type() noexcept { return FieldModel<::proto_pmr::Balance>::fbe_type(); }

    // Check if the struct value is valid
    bool verify();

    // Create a new model (begin phase)
    size_t create_begin();
    // Create a new model (end phase)
    size_t create_end(size_t fbe_begin);

    // Serialize the struct value
    size_t serialize(const ::proto_pmr::Balance& value, pmr::memory_resource* resource);
    // Deserialize the struct value
    size_t deserialize(::proto_pmr::Balance& value, pmr::memory_resource* resource) const noexcept;

    // Move to the next struct value
    void next(size_t prev) noexcept { model.fbe_shift(prev); }

public:
    FieldModel<::proto_pmr::Balance> model;
};

} // namespace proto_pmr

template<>
struct model_of<::proto_pmr::Balance> {
using type = proto_pmr::BalanceModel;
};


// Fast Binary Encoding ::proto_pmr::Account field model
template <>
class FieldModel<::proto_pmr::Account>
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
    void get(::proto_pmr::Account& fbe_value, pmr::memory_resource* resource) const noexcept;
    // Get the struct fields values
    void get_fields(::proto_pmr::Account& fbe_value, size_t fbe_struct_size, pmr::memory_resource* resource) const noexcept;

    // Set the struct value (begin phase)
    size_t set_begin();
    // Set the struct value (end phase)
    void set_end(size_t fbe_begin);

    // Set the struct value
    void set(const ::proto_pmr::Account& fbe_value, pmr::memory_resource* resource) noexcept;
    // Set the struct fields values
    void set_fields(const ::proto_pmr::Account& fbe_value, pmr::memory_resource* resource) noexcept;

private:
    FBEBuffer& _buffer;
    size_t _offset;

public:
    FieldModel<int32_t> id;
    FieldModel<ArenaString> name;
    FieldModel<::proto_pmr::State> state;
    FieldModel<::proto_pmr::Balance> wallet;
    FieldModel<std::optional<::proto_pmr::Balance>> asset;
    FieldModelVector<::proto_pmr::Order> orders;
};

namespace proto_pmr {

// Fast Binary Encoding Account model
class AccountModel : public FBE::Model
{
public:
    AccountModel() : model(this->buffer(), 4) {}
    AccountModel(const std::shared_ptr<FBEBuffer>& buffer) : FBE::Model(buffer), model(this->buffer(), 4) {}

    // Get the model size
    size_t fbe_size() const noexcept { return model.fbe_size() + model.fbe_extra(); }
    // Get the model type
    static constexpr size_t fbe_type() noexcept { return FieldModel<::proto_pmr::Account>::fbe_type(); }

    // Check if the struct value is valid
    bool verify();

    // Create a new model (begin phase)
    size_t create_begin();
    // Create a new model (end phase)
    size_t create_end(size_t fbe_begin);

    // Serialize the struct value
    size_t serialize(const ::proto_pmr::Account& value, pmr::memory_resource* resource);
    // Deserialize the struct value
    size_t deserialize(::proto_pmr::Account& value, pmr::memory_resource* resource) const noexcept;

    // Move to the next struct value
    void next(size_t prev) noexcept { model.fbe_shift(prev); }

public:
    FieldModel<::proto_pmr::Account> model;
};

} // namespace proto_pmr

template<>
struct model_of<::proto_pmr::Account> {
using type = proto_pmr::AccountModel;
};


// Fast Binary Encoding ::proto_pmr::CharMap field model
template <>
class FieldModel<::proto_pmr::CharMap>
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
    void get(::proto_pmr::CharMap& fbe_value, pmr::memory_resource* resource) const noexcept;
    // Get the struct fields values
    void get_fields(::proto_pmr::CharMap& fbe_value, size_t fbe_struct_size, pmr::memory_resource* resource) const noexcept;

    // Set the struct value (begin phase)
    size_t set_begin();
    // Set the struct value (end phase)
    void set_end(size_t fbe_begin);

    // Set the struct value
    void set(const ::proto_pmr::CharMap& fbe_value, pmr::memory_resource* resource) noexcept;
    // Set the struct fields values
    void set_fields(const ::proto_pmr::CharMap& fbe_value, pmr::memory_resource* resource) noexcept;

private:
    FBEBuffer& _buffer;
    size_t _offset;

public:
    FieldModelMap<char, ArenaString> abbr;
};

namespace proto_pmr {

// Fast Binary Encoding CharMap model
class CharMapModel : public FBE::Model
{
public:
    CharMapModel() : model(this->buffer(), 4) {}
    CharMapModel(const std::shared_ptr<FBEBuffer>& buffer) : FBE::Model(buffer), model(this->buffer(), 4) {}

    // Get the model size
    size_t fbe_size() const noexcept { return model.fbe_size() + model.fbe_extra(); }
    // Get the model type
    static constexpr size_t fbe_type() noexcept { return FieldModel<::proto_pmr::CharMap>::fbe_type(); }

    // Check if the struct value is valid
    bool verify();

    // Create a new model (begin phase)
    size_t create_begin();
    // Create a new model (end phase)
    size_t create_end(size_t fbe_begin);

    // Serialize the struct value
    size_t serialize(const ::proto_pmr::CharMap& value, pmr::memory_resource* resource);
    // Deserialize the struct value
    size_t deserialize(::proto_pmr::CharMap& value, pmr::memory_resource* resource) const noexcept;

    // Move to the next struct value
    void next(size_t prev) noexcept { model.fbe_shift(prev); }

public:
    FieldModel<::proto_pmr::CharMap> model;
};

} // namespace proto_pmr

template<>
struct model_of<::proto_pmr::CharMap> {
using type = proto_pmr::CharMapModel;
};


// Fast Binary Encoding ::proto_pmr::OrderMessage field model
template <>
class FieldModel<::proto_pmr::OrderMessage>
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
    void get(::proto_pmr::OrderMessage& fbe_value, pmr::memory_resource* resource) const noexcept;
    // Get the struct fields values
    void get_fields(::proto_pmr::OrderMessage& fbe_value, size_t fbe_struct_size, pmr::memory_resource* resource) const noexcept;

    // Set the struct value (begin phase)
    size_t set_begin();
    // Set the struct value (end phase)
    void set_end(size_t fbe_begin);

    // Set the struct value
    void set(const ::proto_pmr::OrderMessage& fbe_value, pmr::memory_resource* resource) noexcept;
    // Set the struct fields values
    void set_fields(const ::proto_pmr::OrderMessage& fbe_value, pmr::memory_resource* resource) noexcept;

private:
    FBEBuffer& _buffer;
    size_t _offset;

public:
    FieldModel<::proto_pmr::Order> body;
};

namespace proto_pmr {

// Fast Binary Encoding OrderMessage model
class OrderMessageModel : public FBE::Model
{
public:
    OrderMessageModel() : model(this->buffer(), 4) {}
    OrderMessageModel(const std::shared_ptr<FBEBuffer>& buffer) : FBE::Model(buffer), model(this->buffer(), 4) {}

    // Get the model size
    size_t fbe_size() const noexcept { return model.fbe_size() + model.fbe_extra(); }
    // Get the model type
    static constexpr size_t fbe_type() noexcept { return FieldModel<::proto_pmr::OrderMessage>::fbe_type(); }

    // Check if the struct value is valid
    bool verify();

    // Create a new model (begin phase)
    size_t create_begin();
    // Create a new model (end phase)
    size_t create_end(size_t fbe_begin);

    // Serialize the struct value
    size_t serialize(const ::proto_pmr::OrderMessage& value, pmr::memory_resource* resource);
    // Deserialize the struct value
    size_t deserialize(::proto_pmr::OrderMessage& value, pmr::memory_resource* resource) const noexcept;

    // Move to the next struct value
    void next(size_t prev) noexcept { model.fbe_shift(prev); }

public:
    FieldModel<::proto_pmr::OrderMessage> model;
};

} // namespace proto_pmr

template<>
struct model_of<::proto_pmr::OrderMessage> {
using type = proto_pmr::OrderMessageModel;
};


// Fast Binary Encoding ::proto_pmr::BalanceMessage field model
template <>
class FieldModel<::proto_pmr::BalanceMessage>
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
    void get(::proto_pmr::BalanceMessage& fbe_value, pmr::memory_resource* resource) const noexcept;
    // Get the struct fields values
    void get_fields(::proto_pmr::BalanceMessage& fbe_value, size_t fbe_struct_size, pmr::memory_resource* resource) const noexcept;

    // Set the struct value (begin phase)
    size_t set_begin();
    // Set the struct value (end phase)
    void set_end(size_t fbe_begin);

    // Set the struct value
    void set(const ::proto_pmr::BalanceMessage& fbe_value, pmr::memory_resource* resource) noexcept;
    // Set the struct fields values
    void set_fields(const ::proto_pmr::BalanceMessage& fbe_value, pmr::memory_resource* resource) noexcept;

private:
    FBEBuffer& _buffer;
    size_t _offset;

public:
    FieldModel<::proto_pmr::Balance> body;
};

namespace proto_pmr {

// Fast Binary Encoding BalanceMessage model
class BalanceMessageModel : public FBE::Model
{
public:
    BalanceMessageModel() : model(this->buffer(), 4) {}
    BalanceMessageModel(const std::shared_ptr<FBEBuffer>& buffer) : FBE::Model(buffer), model(this->buffer(), 4) {}

    // Get the model size
    size_t fbe_size() const noexcept { return model.fbe_size() + model.fbe_extra(); }
    // Get the model type
    static constexpr size_t fbe_type() noexcept { return FieldModel<::proto_pmr::BalanceMessage>::fbe_type(); }

    // Check if the struct value is valid
    bool verify();

    // Create a new model (begin phase)
    size_t create_begin();
    // Create a new model (end phase)
    size_t create_end(size_t fbe_begin);

    // Serialize the struct value
    size_t serialize(const ::proto_pmr::BalanceMessage& value, pmr::memory_resource* resource);
    // Deserialize the struct value
    size_t deserialize(::proto_pmr::BalanceMessage& value, pmr::memory_resource* resource) const noexcept;

    // Move to the next struct value
    void next(size_t prev) noexcept { model.fbe_shift(prev); }

public:
    FieldModel<::proto_pmr::BalanceMessage> model;
};

} // namespace proto_pmr

template<>
struct model_of<::proto_pmr::BalanceMessage> {
using type = proto_pmr::BalanceMessageModel;
};


// Fast Binary Encoding ::proto_pmr::AccountMessage field model
template <>
class FieldModel<::proto_pmr::AccountMessage>
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
    static constexpr size_t fbe_type() noexcept { return 4; }

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
    void get(::proto_pmr::AccountMessage& fbe_value, pmr::memory_resource* resource) const noexcept;
    // Get the struct fields values
    void get_fields(::proto_pmr::AccountMessage& fbe_value, size_t fbe_struct_size, pmr::memory_resource* resource) const noexcept;

    // Set the struct value (begin phase)
    size_t set_begin();
    // Set the struct value (end phase)
    void set_end(size_t fbe_begin);

    // Set the struct value
    void set(const ::proto_pmr::AccountMessage& fbe_value, pmr::memory_resource* resource) noexcept;
    // Set the struct fields values
    void set_fields(const ::proto_pmr::AccountMessage& fbe_value, pmr::memory_resource* resource) noexcept;

private:
    FBEBuffer& _buffer;
    size_t _offset;

public:
    FieldModel<::proto_pmr::Account> body;
};

namespace proto_pmr {

// Fast Binary Encoding AccountMessage model
class AccountMessageModel : public FBE::Model
{
public:
    AccountMessageModel() : model(this->buffer(), 4) {}
    AccountMessageModel(const std::shared_ptr<FBEBuffer>& buffer) : FBE::Model(buffer), model(this->buffer(), 4) {}

    // Get the model size
    size_t fbe_size() const noexcept { return model.fbe_size() + model.fbe_extra(); }
    // Get the model type
    static constexpr size_t fbe_type() noexcept { return FieldModel<::proto_pmr::AccountMessage>::fbe_type(); }

    // Check if the struct value is valid
    bool verify();

    // Create a new model (begin phase)
    size_t create_begin();
    // Create a new model (end phase)
    size_t create_end(size_t fbe_begin);

    // Serialize the struct value
    size_t serialize(const ::proto_pmr::AccountMessage& value, pmr::memory_resource* resource);
    // Deserialize the struct value
    size_t deserialize(::proto_pmr::AccountMessage& value, pmr::memory_resource* resource) const noexcept;

    // Move to the next struct value
    void next(size_t prev) noexcept { model.fbe_shift(prev); }

public:
    FieldModel<::proto_pmr::AccountMessage> model;
};

} // namespace proto_pmr

template<>
struct model_of<::proto_pmr::AccountMessage> {
using type = proto_pmr::AccountMessageModel;
};


// Fast Binary Encoding ::proto_pmr::PremiumAccount field model
template <>
class FieldModel<::proto_pmr::PremiumAccount>
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
    static constexpr size_t fbe_type() noexcept { return 5; }

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
    void get(::proto_pmr::PremiumAccount& fbe_value, pmr::memory_resource* resource) const noexcept;
    // Get the struct fields values
    void get_fields(::proto_pmr::PremiumAccount& fbe_value, size_t fbe_struct_size, pmr::memory_resource* resource) const noexcept;

    // Set the struct value (begin phase)
    size_t set_begin();
    // Set the struct value (end phase)
    void set_end(size_t fbe_begin);

    // Set the struct value
    void set(const ::proto_pmr::PremiumAccount& fbe_value, pmr::memory_resource* resource) noexcept;
    // Set the struct fields values
    void set_fields(const ::proto_pmr::PremiumAccount& fbe_value, pmr::memory_resource* resource) noexcept;

private:
    FBEBuffer& _buffer;
    size_t _offset;

public:
    FieldModel<int32_t> id;
    FieldModel<ArenaString> name;
    FieldModel<ArenaString> info;
    FieldModel<::proto_pmr::Balance> private_wallet;
    FieldModelVector<::proto_pmr::Order> private_orders;
    FieldModel<::proto_pmr::State> private_state;
};

namespace proto_pmr {

// Fast Binary Encoding PremiumAccount model
class PremiumAccountModel : public FBE::Model
{
public:
    PremiumAccountModel() : model(this->buffer(), 4) {}
    PremiumAccountModel(const std::shared_ptr<FBEBuffer>& buffer) : FBE::Model(buffer), model(this->buffer(), 4) {}

    // Get the model size
    size_t fbe_size() const noexcept { return model.fbe_size() + model.fbe_extra(); }
    // Get the model type
    static constexpr size_t fbe_type() noexcept { return FieldModel<::proto_pmr::PremiumAccount>::fbe_type(); }

    // Check if the struct value is valid
    bool verify();

    // Create a new model (begin phase)
    size_t create_begin();
    // Create a new model (end phase)
    size_t create_end(size_t fbe_begin);

    // Serialize the struct value
    size_t serialize(const ::proto_pmr::PremiumAccount& value, pmr::memory_resource* resource);
    // Deserialize the struct value
    size_t deserialize(::proto_pmr::PremiumAccount& value, pmr::memory_resource* resource) const noexcept;

    // Move to the next struct value
    void next(size_t prev) noexcept { model.fbe_shift(prev); }

public:
    FieldModel<::proto_pmr::PremiumAccount> model;
};

} // namespace proto_pmr

template<>
struct model_of<::proto_pmr::PremiumAccount> {
using type = proto_pmr::PremiumAccountModel;
};


} // namespace FBE
