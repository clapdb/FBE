//------------------------------------------------------------------------------
// Automatically generated by the Fast Binary Encoding compiler, do not modify!
// https://github.com/chronoxor/FastBinaryEncoding
// Source: ptrpkg.fbe
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

#include "variants_ptr_ptr_pmr.h"

namespace ptrpkg_pmr {
using namespace FBE;
using allocator_type = pmr::polymorphic_allocator<char>;
using namespace ::variants_ptr_pmr;
} // namespace ptrpkg_pmr

namespace FBE {
using namespace ::ptrpkg_pmr;
} // namespace FBE

#include "fbe_ptr.h"

namespace ptrpkg_pmr {

struct Line : FBE::Base
{
    ArenaManagedCreateOnlyTag;

    ::variants_ptr_pmr::Value value;
    ::variants_ptr_pmr::Value* value_ptr;

    size_t fbe_type() const noexcept { return 1; }

    Line();
    explicit Line(allocator_type alloc);
    Line(::variants_ptr_pmr::Value&& arg_value, std::unique_ptr<::variants_ptr_pmr::Value> arg_value_ptr);
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

} // namespace ptrpkg_pmr

template<>
struct std::hash<ptrpkg_pmr::Line>
{
    typedef ptrpkg_pmr::Line argument_type;
    typedef size_t result_type;

    result_type operator() ([[maybe_unused]] const argument_type& value) const
    {
        result_type result = 17;
        return result;
    }
};

namespace ptrpkg_pmr {

} // namespace ptrpkg_pmr
