//------------------------------------------------------------------------------
// Automatically generated by the Fast Binary Encoding compiler, do not modify!
// https://github.com/chronoxor/FastBinaryEncoding
// Source: arena_common.fbe
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

namespace arena_common {
using namespace FBE;
} // namespace arena_common

namespace FBE {
using namespace ::arena_common;
} // namespace FBE

namespace arena_common {

enum class Optr : uint8_t
{
    EQ = (uint8_t)0u,
    GT,
    LT,
    GE,
    LE,
};

std::ostream& operator<<(std::ostream& stream, [[maybe_unused]] Optr value);

#if defined(FMT_VERSION)
} template <> struct fmt::formatter<arena_common::Optr> : formatter<string_view> {}; namespace arena_common {
#endif

#if defined(LOGGING_PROTOCOL)
CppLogging::Record& operator<<(CppLogging::Record& record, Optr value);
#endif
// forward declaration
struct Alias;
struct Expression;

using Expr = std::variant<bool, int32_t, stdb::memory::string>;
std::ostream& operator<<(std::ostream& stream, [[maybe_unused]] const Expr& value);

struct Alias
{
    stdb::memory::string name;
    ::arena_common::Optr optr;
    ::arena_common::Expr expr;

    size_t fbe_type() const noexcept { return 1; }

    Alias();
    Alias(const stdb::memory::string& arg_name, const ::arena_common::Optr& arg_optr, const ::arena_common::Expr& arg_expr);
    Alias(const Alias& other) = default;
    Alias(Alias&& other) = default;
    ~Alias() = default;

    Alias& operator=(const Alias& other) = default;
    Alias& operator=(Alias&& other) = default;

    bool operator==(const Alias& other) const noexcept;
    bool operator!=(const Alias& other) const noexcept { return !operator==(other); }
    bool operator<(const Alias& other) const noexcept;
    bool operator<=(const Alias& other) const noexcept { return operator<(other) || operator==(other); }
    bool operator>(const Alias& other) const noexcept { return !operator<=(other); }
    bool operator>=(const Alias& other) const noexcept { return !operator<(other); }

    std::string string() const;

    friend std::ostream& operator<<(std::ostream& stream, [[maybe_unused]] const Alias& value);

    void swap(Alias& other) noexcept;
    friend void swap(Alias& value1, Alias& value2) noexcept { value1.swap(value2); }
};

} // namespace arena_common

#if defined(FMT_VERSION)
template <> struct fmt::formatter<arena_common::Alias> : formatter<string_view> {};
#endif

template<>
struct std::hash<arena_common::Alias>
{
    typedef arena_common::Alias argument_type;
    typedef size_t result_type;

    result_type operator() ([[maybe_unused]] const argument_type& value) const
    {
        result_type result = 17;
        return result;
    }
};

namespace arena_common {

struct Expression
{
    FastVec<stdb::memory::string> keys;
    FastVec<::arena_common::Alias> aliases;
    std::map<int32_t, ::arena_common::Alias> alias_int;

    size_t fbe_type() const noexcept { return 2; }

    Expression();
    Expression(const FastVec<stdb::memory::string>& arg_keys, const FastVec<::arena_common::Alias>& arg_aliases, const std::map<int32_t, ::arena_common::Alias>& arg_alias_int);
    Expression(const Expression& other) = default;
    Expression(Expression&& other) = default;
    ~Expression() = default;

    Expression& operator=(const Expression& other) = default;
    Expression& operator=(Expression&& other) = default;

    bool operator==(const Expression& other) const noexcept;
    bool operator!=(const Expression& other) const noexcept { return !operator==(other); }
    bool operator<(const Expression& other) const noexcept;
    bool operator<=(const Expression& other) const noexcept { return operator<(other) || operator==(other); }
    bool operator>(const Expression& other) const noexcept { return !operator<=(other); }
    bool operator>=(const Expression& other) const noexcept { return !operator<(other); }

    std::string string() const;

    friend std::ostream& operator<<(std::ostream& stream, [[maybe_unused]] const Expression& value);

    void swap(Expression& other) noexcept;
    friend void swap(Expression& value1, Expression& value2) noexcept { value1.swap(value2); }
};

} // namespace arena_common

#if defined(FMT_VERSION)
template <> struct fmt::formatter<arena_common::Expression> : formatter<string_view> {};
#endif

template<>
struct std::hash<arena_common::Expression>
{
    typedef arena_common::Expression argument_type;
    typedef size_t result_type;

    result_type operator() ([[maybe_unused]] const argument_type& value) const
    {
        result_type result = 17;
        return result;
    }
};

namespace arena_common {

} // namespace arena_common
