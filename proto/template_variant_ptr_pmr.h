//------------------------------------------------------------------------------
// Automatically generated by the Fast Binary Encoding compiler, do not modify!
// https://github.com/chronoxor/FastBinaryEncoding
// Source: template_variant.fbe
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
#include "arena/arena.hpp"

#include "enums_pmr.h"
#include "variants_pmr.h"

namespace template_variant_pmr {
using namespace FBE;
using allocator_type = pmr::polymorphic_allocator<char>;
using namespace ::enums_pmr;
using namespace ::variants_pmr;
} // namespace template_variant_pmr

namespace FBE {
using namespace ::template_variant_pmr;
} // namespace FBE

#include "fbe_ptr.h"

namespace template_variant_pmr {

struct Line : FBE::Base
{
    ArenaManagedCreateOnlyTag;

    ::variants_pmr::V v;
    pmr::vector<::variants_pmr::V> vv;
    pmr::unordered_map<stdb::memory::arena_string, ::variants_pmr::V> vm;
    std::optional<::variants_pmr::V> vo;

    size_t fbe_type() const noexcept { return 1; }

    Line();
    explicit Line(allocator_type alloc);
    Line(::variants_pmr::V&& arg_v, pmr::vector<::variants_pmr::V> arg_vv, pmr::unordered_map<stdb::memory::arena_string, ::variants_pmr::V> arg_vm, std::optional<::variants_pmr::V> arg_vo);
    Line(const Line& other) = default;
    Line(Line&& other) noexcept;
    ~Line() override;

    Line& operator=(const Line& other) = default;
    Line& operator=(Line&& other) noexcept;

    bool operator==(const Line& other) const noexcept;
    bool operator!=(const Line& other) const noexcept { return !operator==(other); }
    bool operator<(const Line& other) const noexcept;
    bool operator<=(const Line& other) const noexcept { return operator<(other) || operator==(other); }
    bool operator>(const Line& other) const noexcept { return !operator<=(other); }
    bool operator>=(const Line& other) const noexcept { return !operator<(other); }

    std::string string() const;

    friend std::ostream& operator<<(std::ostream& stream, const Line& value);

    void swap(Line& other) noexcept;
    friend void swap(Line& value1, Line& value2) noexcept { value1.swap(value2); }
};

} // namespace template_variant_pmr

template<>
struct std::hash<template_variant_pmr::Line>
{
    typedef template_variant_pmr::Line argument_type;
    typedef size_t result_type;

    result_type operator() ([[maybe_unused]] const argument_type& value) const
    {
        result_type result = 17;
        return result;
    }
};

namespace template_variant_pmr {

struct Line2 : FBE::Base
{
    ArenaManagedCreateOnlyTag;

    pmr::unordered_map<::enums_pmr::EnumInt8, ::variants_pmr::V> vm;

    size_t fbe_type() const noexcept { return 2; }

    Line2();
    explicit Line2(allocator_type alloc);
    explicit Line2(pmr::unordered_map<::enums_pmr::EnumInt8, ::variants_pmr::V> arg_vm);
    Line2(const Line2& other) = default;
    Line2(Line2&& other) noexcept;
    ~Line2() override;

    Line2& operator=(const Line2& other) = default;
    Line2& operator=(Line2&& other) noexcept;

    bool operator==(const Line2& other) const noexcept;
    bool operator!=(const Line2& other) const noexcept { return !operator==(other); }
    bool operator<(const Line2& other) const noexcept;
    bool operator<=(const Line2& other) const noexcept { return operator<(other) || operator==(other); }
    bool operator>(const Line2& other) const noexcept { return !operator<=(other); }
    bool operator>=(const Line2& other) const noexcept { return !operator<(other); }

    std::string string() const;

    friend std::ostream& operator<<(std::ostream& stream, const Line2& value);

    void swap(Line2& other) noexcept;
    friend void swap(Line2& value1, Line2& value2) noexcept { value1.swap(value2); }
};

} // namespace template_variant_pmr

template<>
struct std::hash<template_variant_pmr::Line2>
{
    typedef template_variant_pmr::Line2 argument_type;
    typedef size_t result_type;

    result_type operator() ([[maybe_unused]] const argument_type& value) const
    {
        result_type result = 17;
        return result;
    }
};

namespace template_variant_pmr {

struct Line3 : FBE::Base
{
    ArenaManagedCreateOnlyTag;

    ::variants_pmr::Value value;

    size_t fbe_type() const noexcept { return 3; }

    Line3();
    explicit Line3(allocator_type alloc);
    explicit Line3(::variants_pmr::Value&& arg_value);
    Line3(const Line3& other) = default;
    Line3(Line3&& other) noexcept;
    ~Line3() override;

    Line3& operator=(const Line3& other) = default;
    Line3& operator=(Line3&& other) noexcept;

    bool operator==(const Line3& other) const noexcept;
    bool operator!=(const Line3& other) const noexcept { return !operator==(other); }
    bool operator<(const Line3& other) const noexcept;
    bool operator<=(const Line3& other) const noexcept { return operator<(other) || operator==(other); }
    bool operator>(const Line3& other) const noexcept { return !operator<=(other); }
    bool operator>=(const Line3& other) const noexcept { return !operator<(other); }

    std::string string() const;

    friend std::ostream& operator<<(std::ostream& stream, const Line3& value);

    void swap(Line3& other) noexcept;
    friend void swap(Line3& value1, Line3& value2) noexcept { value1.swap(value2); }
};

} // namespace template_variant_pmr

template<>
struct std::hash<template_variant_pmr::Line3>
{
    typedef template_variant_pmr::Line3 argument_type;
    typedef size_t result_type;

    result_type operator() ([[maybe_unused]] const argument_type& value) const
    {
        result_type result = 17;
        return result;
    }
};

namespace template_variant_pmr {

} // namespace template_variant_pmr