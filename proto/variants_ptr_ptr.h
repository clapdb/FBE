//------------------------------------------------------------------------------
// Automatically generated by the Fast Binary Encoding compiler, do not modify!
// https://github.com/chronoxor/FastBinaryEncoding
// Source: variants_ptr.fbe
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

#include "fbe.h"

namespace variants_ptr {
using namespace FBE;
} // namespace variants_ptr

namespace FBE {
using namespace ::variants_ptr;
} // namespace FBE

#include "fbe_ptr.h"

namespace variants_ptr {
// forward declaration
struct Simple;
struct Value;
struct ValueContainer;

using Expr = std::variant<bool, stdb::memory::string, int32_t>;
std::ostream& operator<<(std::ostream& stream, [[maybe_unused]] const Expr& value);
auto is_equal(const Expr& lhs, const Expr& rhs) -> bool;

using V = std::variant<int32_t, stdb::memory::string, double, ::variants_ptr::Simple, ::variants_ptr::Simple*, FastVec<::variants_ptr::Simple>, FastVec<int32_t>, std::unordered_map<int32_t, ::variants_ptr::Simple>, FastVec<FBE::buffer_t>, FastVec<stdb::memory::string>, std::unordered_map<int32_t, FBE::buffer_t>, std::unordered_map<stdb::memory::string, FBE::buffer_t>, FastVec<::variants_ptr::Simple*>, ::variants_ptr::Expr>;
std::ostream& operator<<(std::ostream& stream, [[maybe_unused]] const V& value);
auto is_equal(const V& lhs, const V& rhs) -> bool;

struct Simple : FBE::Base
{
    stdb::memory::string name;

    size_t fbe_type() const noexcept { return 1; }

    Simple();
    explicit Simple(const stdb::memory::string& arg_name);
    Simple(const Simple& other) = default;
    Simple(Simple&& other) noexcept;
    ~Simple() override;

    Simple& operator=(const Simple& other) = default;
    Simple& operator=(Simple&& other) noexcept;

    bool operator==(const Simple& other) const noexcept;
    bool operator!=(const Simple& other) const noexcept { return !operator==(other); }
    bool operator<(const Simple& other) const noexcept;
    bool operator<=(const Simple& other) const noexcept { return operator<(other) || operator==(other); }
    bool operator>(const Simple& other) const noexcept { return !operator<=(other); }
    bool operator>=(const Simple& other) const noexcept { return !operator<(other); }

    std::string string() const;

    friend std::ostream& operator<<(std::ostream& stream, const Simple& value);

    void swap(Simple& other) noexcept;
    friend void swap(Simple& value1, Simple& value2) noexcept { value1.swap(value2); }
};

} // namespace variants_ptr

template<>
struct std::hash<variants_ptr::Simple>
{
    typedef variants_ptr::Simple argument_type;
    typedef size_t result_type;

    result_type operator() ([[maybe_unused]] const argument_type& value) const
    {
        result_type result = 17;
        return result;
    }
};

namespace variants_ptr {

struct Value : FBE::Base
{
    ::variants_ptr::V v;
    std::optional<::variants_ptr::V> vo;
    std::optional<::variants_ptr::V> vo2;

    size_t fbe_type() const noexcept { return 2; }

    Value();
    Value(::variants_ptr::V&& arg_v, std::optional<::variants_ptr::V> arg_vo, std::optional<::variants_ptr::V> arg_vo2);
    Value(const Value& other) = default;
    Value(Value&& other) noexcept;
    ~Value() override;

    Value& operator=(const Value& other) = default;
    Value& operator=(Value&& other) noexcept;

    bool operator==(const Value& other) const noexcept;
    bool operator!=(const Value& other) const noexcept { return !operator==(other); }
    bool operator<(const Value& other) const noexcept;
    bool operator<=(const Value& other) const noexcept { return operator<(other) || operator==(other); }
    bool operator>(const Value& other) const noexcept { return !operator<=(other); }
    bool operator>=(const Value& other) const noexcept { return !operator<(other); }

    std::string string() const;

    friend std::ostream& operator<<(std::ostream& stream, const Value& value);

    void swap(Value& other) noexcept;
    friend void swap(Value& value1, Value& value2) noexcept { value1.swap(value2); }
};

} // namespace variants_ptr

template<>
struct std::hash<variants_ptr::Value>
{
    typedef variants_ptr::Value argument_type;
    typedef size_t result_type;

    result_type operator() ([[maybe_unused]] const argument_type& value) const
    {
        result_type result = 17;
        return result;
    }
};

namespace variants_ptr {

struct ValueContainer : FBE::Base
{
    FastVec<::variants_ptr::V> vv;
    std::unordered_map<int32_t, ::variants_ptr::V> vm;

    size_t fbe_type() const noexcept { return 3; }

    ValueContainer();
    ValueContainer(FastVec<::variants_ptr::V> arg_vv, std::unordered_map<int32_t, ::variants_ptr::V> arg_vm);
    ValueContainer(const ValueContainer& other) = default;
    ValueContainer(ValueContainer&& other) noexcept;
    ~ValueContainer() override;

    ValueContainer& operator=(const ValueContainer& other) = default;
    ValueContainer& operator=(ValueContainer&& other) noexcept;

    bool operator==(const ValueContainer& other) const noexcept;
    bool operator!=(const ValueContainer& other) const noexcept { return !operator==(other); }
    bool operator<(const ValueContainer& other) const noexcept;
    bool operator<=(const ValueContainer& other) const noexcept { return operator<(other) || operator==(other); }
    bool operator>(const ValueContainer& other) const noexcept { return !operator<=(other); }
    bool operator>=(const ValueContainer& other) const noexcept { return !operator<(other); }

    std::string string() const;

    friend std::ostream& operator<<(std::ostream& stream, const ValueContainer& value);

    void swap(ValueContainer& other) noexcept;
    friend void swap(ValueContainer& value1, ValueContainer& value2) noexcept { value1.swap(value2); }
};

} // namespace variants_ptr

template<>
struct std::hash<variants_ptr::ValueContainer>
{
    typedef variants_ptr::ValueContainer argument_type;
    typedef size_t result_type;

    result_type operator() ([[maybe_unused]] const argument_type& value) const
    {
        result_type result = 17;
        return result;
    }
};

namespace variants_ptr {

} // namespace variants_ptr
