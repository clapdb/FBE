//------------------------------------------------------------------------------
// Automatically generated by the Fast Binary Encoding compiler, do not modify!
// https://github.com/chronoxor/FastBinaryEncoding
// Source: arena_common.fbe
// FBE version: 1.11.0.0
//------------------------------------------------------------------------------

#include "arena_common_pmr.h"

namespace arena_common_pmr {

std::ostream& operator<<(std::ostream& stream, [[maybe_unused]] Optr value)
{
    if (value == Optr::EQ) return stream << "EQ";
    if (value == Optr::GT) return stream << "GT";
    if (value == Optr::LT) return stream << "LT";
    if (value == Optr::GE) return stream << "GE";
    if (value == Optr::LE) return stream << "LE";
    return stream << "<unknown>";
}

std::ostream& operator<<(std::ostream& stream, [[maybe_unused]] const Expr& value)
{
    stream << "Expr(variant|";
    [[maybe_unused]] bool first = true;
    switch (value.index()) {
        case 0:
            stream<< "{bool}";
            stream << std::get<0>(value);
            break;
        case 1:
            stream<< "{int32}";
            stream << std::get<1>(value);
            break;
        case 2:
            stream<< "{string}";
            stream << std::get<2>(value);
            break;
        default:
            static_assert("unreachable branch");
    }
    stream << ")";
    return stream;
}


Alias::Alias()
    : name()
    , optr()
    , expr()
{}

Alias::Alias([[maybe_unused]] allocator_type alloc)
    : name(alloc)
    , optr()
    , expr()
{}

Alias::Alias(const ArenaString& arg_name, const ::arena_common_pmr::Optr& arg_optr, const ::arena_common_pmr::Expr& arg_expr)
    : name(arg_name)
    , optr(arg_optr)
    , expr(arg_expr)
{}

bool Alias::operator==([[maybe_unused]] const Alias& other) const noexcept
{
    return (
        (name == other.name)
        && (optr == other.optr)
        && (expr == other.expr)
        );
}

bool Alias::operator<([[maybe_unused]] const Alias& other) const noexcept
{
    return false;
}

std::string Alias::string() const
{
    std::stringstream ss; ss << *this; return ss.str();
}

void Alias::swap([[maybe_unused]] Alias& other) noexcept
{
    using std::swap;
    swap(name, other.name);
    swap(optr, other.optr);
    swap(expr, other.expr);
}

std::ostream& operator<<(std::ostream& stream, [[maybe_unused]] const Alias& value)
{
    stream << "Alias(";
    stream << "name="; stream << "\"" << value.name << "\"";
    stream << ",optr="; stream << value.optr;
    stream << ",expr="; stream << value.expr;
    stream << ")";
    return stream;
}

Expression::Expression()
    : keys()
    , aliases()
    , alias_int()
{}

Expression::Expression([[maybe_unused]] allocator_type alloc)
    : keys(alloc)
    , aliases(alloc)
    , alias_int(alloc)
{}

Expression::Expression(const pmr::vector<ArenaString>& arg_keys, const pmr::vector<::arena_common_pmr::Alias>& arg_aliases, const pmr::map<int32_t, ::arena_common_pmr::Alias>& arg_alias_int)
    : keys(arg_keys)
    , aliases(arg_aliases)
    , alias_int(arg_alias_int)
{}

bool Expression::operator==([[maybe_unused]] const Expression& other) const noexcept
{
    return (
        (keys == other.keys)
        && (aliases == other.aliases)
        && (alias_int == other.alias_int)
        );
}

bool Expression::operator<([[maybe_unused]] const Expression& other) const noexcept
{
    return false;
}

std::string Expression::string() const
{
    std::stringstream ss; ss << *this; return ss.str();
}

void Expression::swap([[maybe_unused]] Expression& other) noexcept
{
    using std::swap;
    swap(keys, other.keys);
    swap(aliases, other.aliases);
    swap(alias_int, other.alias_int);
}

std::ostream& operator<<(std::ostream& stream, [[maybe_unused]] const Expression& value)
{
    stream << "Expression(";
    {
        bool first = true;
        stream << "keys=[" << value.keys.size() << "][";
        for ([[maybe_unused]] const auto& it : value.keys)
        {
            stream << std::string(first ? "" : ",") << "\"" << it << "\"";
            first = false;
        }
        stream << "]";
    }
    {
        bool first = true;
        stream << ",aliases=[" << value.aliases.size() << "][";
        for ([[maybe_unused]] const auto& it : value.aliases)
        {
            stream << std::string(first ? "" : ",") << it;
            first = false;
        }
        stream << "]";
    }
    {
        bool first = true;
        stream << ",alias_int=[" << value.alias_int.size()<< "]<{";
        for ([[maybe_unused]] const auto& it : value.alias_int)
        {
            stream << std::string(first ? "" : ",") << it.first;
            stream << "->";
            stream << it.second;
            first = false;
        }
        stream << "}>";
    }
    stream << ")";
    return stream;
}

} // namespace arena_common_pmr
