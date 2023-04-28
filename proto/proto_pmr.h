//------------------------------------------------------------------------------
// Automatically generated by the Fast Binary Encoding compiler, do not modify!
// https://github.com/chronoxor/FastBinaryEncoding
// Source: proto.fbe
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

namespace proto_pmr {
using namespace FBE;
using allocator_type = pmr::polymorphic_allocator<char>;
} // namespace proto_pmr

namespace FBE {
using namespace ::proto_pmr;
} // namespace FBE

namespace proto_pmr {

enum class OrderSide : uint8_t
{
    buy,
    sell,
};

std::ostream& operator<<(std::ostream& stream, [[maybe_unused]] OrderSide value);

#if defined(LOGGING_PROTOCOL)
CppLogging::Record& operator<<(CppLogging::Record& record, OrderSide value);
#endif

enum class OrderType : uint8_t
{
    market,
    limit,
    stop,
};

std::ostream& operator<<(std::ostream& stream, [[maybe_unused]] OrderType value);

#if defined(LOGGING_PROTOCOL)
CppLogging::Record& operator<<(CppLogging::Record& record, OrderType value);
#endif

enum class State : uint8_t
{
    unknown = (uint8_t)0x00u,
    invalid = (uint8_t)0x01u,
    initialized = (uint8_t)0x02u,
    calculated = (uint8_t)0x04u,
    broken = (uint8_t)0x08u,
    good = initialized  |  calculated,
    bad = unknown  |  invalid  |  broken,
};

FBE_ENUM_FLAGS(State)

std::ostream& operator<<(std::ostream& stream, [[maybe_unused]] State value);

#if defined(LOGGING_PROTOCOL)
CppLogging::Record& operator<<(CppLogging::Record& record, State value);
#endif

struct Order
{
    ArenaManagedCreateOnlyTag;

    int32_t id;
    stdb::memory::arena_string symbol;
    ::proto_pmr::OrderSide side;
    ::proto_pmr::OrderType type;
    double price;
    double volume;

    size_t fbe_type() const noexcept { return 1; }

    Order();
    explicit Order(allocator_type alloc);
    Order(int32_t arg_id, const stdb::memory::arena_string& arg_symbol, const ::proto_pmr::OrderSide& arg_side, const ::proto_pmr::OrderType& arg_type, double arg_price, double arg_volume);
    Order(const Order& other) = default;
    Order(Order&& other) = default;
    ~Order() = default;

    Order& operator=(const Order& other) = default;
    Order& operator=(Order&& other) = default;

    bool operator==(const Order& other) const noexcept;
    bool operator!=(const Order& other) const noexcept { return !operator==(other); }
    bool operator<(const Order& other) const noexcept;
    bool operator<=(const Order& other) const noexcept { return operator<(other) || operator==(other); }
    bool operator>(const Order& other) const noexcept { return !operator<=(other); }
    bool operator>=(const Order& other) const noexcept { return !operator<(other); }

    std::string string() const;

    friend std::ostream& operator<<(std::ostream& stream, [[maybe_unused]] const Order& value);

    void swap(Order& other) noexcept;
    friend void swap(Order& value1, Order& value2) noexcept { value1.swap(value2); }
};

} // namespace proto_pmr

template<>
struct std::hash<proto_pmr::Order>
{
    typedef proto_pmr::Order argument_type;
    typedef size_t result_type;

    result_type operator() ([[maybe_unused]] const argument_type& value) const
    {
        result_type result = 17;
        result = result * 31 + std::hash<decltype(value.id)>()(value.id);
        return result;
    }
};

namespace proto_pmr {

struct Balance
{
    ArenaManagedCreateOnlyTag;

    stdb::memory::arena_string currency;
    double amount;

    size_t fbe_type() const noexcept { return 2; }

    Balance();
    explicit Balance(allocator_type alloc);
    Balance(const stdb::memory::arena_string& arg_currency, double arg_amount);
    Balance(const Balance& other) = default;
    Balance(Balance&& other) = default;
    ~Balance() = default;

    Balance& operator=(const Balance& other) = default;
    Balance& operator=(Balance&& other) = default;

    bool operator==(const Balance& other) const noexcept;
    bool operator!=(const Balance& other) const noexcept { return !operator==(other); }
    bool operator<(const Balance& other) const noexcept;
    bool operator<=(const Balance& other) const noexcept { return operator<(other) || operator==(other); }
    bool operator>(const Balance& other) const noexcept { return !operator<=(other); }
    bool operator>=(const Balance& other) const noexcept { return !operator<(other); }

    std::string string() const;

    friend std::ostream& operator<<(std::ostream& stream, [[maybe_unused]] const Balance& value);

    void swap(Balance& other) noexcept;
    friend void swap(Balance& value1, Balance& value2) noexcept { value1.swap(value2); }
};

} // namespace proto_pmr

template<>
struct std::hash<proto_pmr::Balance>
{
    typedef proto_pmr::Balance argument_type;
    typedef size_t result_type;

    result_type operator() ([[maybe_unused]] const argument_type& value) const
    {
        result_type result = 17;
        result = result * 31 + std::hash<decltype(value.currency)>()(value.currency);
        return result;
    }
};

namespace proto_pmr {

struct Account
{
    ArenaManagedCreateOnlyTag;

    int32_t id;
    stdb::memory::arena_string name;
    ::proto_pmr::State state;
    ::proto_pmr::Balance wallet;
    std::optional<::proto_pmr::Balance> asset;
    pmr::vector<::proto_pmr::Order> orders;

    size_t fbe_type() const noexcept { return 3; }

    Account();
    explicit Account(allocator_type alloc);
    Account(int32_t arg_id, const stdb::memory::arena_string& arg_name, const ::proto_pmr::State& arg_state, const ::proto_pmr::Balance& arg_wallet, const std::optional<::proto_pmr::Balance>& arg_asset, const pmr::vector<::proto_pmr::Order>& arg_orders);
    Account(const Account& other) = default;
    Account(Account&& other) = default;
    ~Account() = default;

    Account& operator=(const Account& other) = default;
    Account& operator=(Account&& other) = default;

    bool operator==(const Account& other) const noexcept;
    bool operator!=(const Account& other) const noexcept { return !operator==(other); }
    bool operator<(const Account& other) const noexcept;
    bool operator<=(const Account& other) const noexcept { return operator<(other) || operator==(other); }
    bool operator>(const Account& other) const noexcept { return !operator<=(other); }
    bool operator>=(const Account& other) const noexcept { return !operator<(other); }

    std::string string() const;

    friend std::ostream& operator<<(std::ostream& stream, [[maybe_unused]] const Account& value);

    void swap(Account& other) noexcept;
    friend void swap(Account& value1, Account& value2) noexcept { value1.swap(value2); }
};

} // namespace proto_pmr

template<>
struct std::hash<proto_pmr::Account>
{
    typedef proto_pmr::Account argument_type;
    typedef size_t result_type;

    result_type operator() ([[maybe_unused]] const argument_type& value) const
    {
        result_type result = 17;
        result = result * 31 + std::hash<decltype(value.id)>()(value.id);
        return result;
    }
};

namespace proto_pmr {

struct CharMap
{
    ArenaManagedCreateOnlyTag;

    pmr::unordered_map<char, stdb::memory::arena_string> abbr;

    size_t fbe_type() const noexcept { return 1; }

    CharMap();
    explicit CharMap(allocator_type alloc);
    explicit CharMap(const pmr::unordered_map<char, stdb::memory::arena_string>& arg_abbr);
    CharMap(const CharMap& other) = default;
    CharMap(CharMap&& other) = default;
    ~CharMap() = default;

    CharMap& operator=(const CharMap& other) = default;
    CharMap& operator=(CharMap&& other) = default;

    bool operator==(const CharMap& other) const noexcept;
    bool operator!=(const CharMap& other) const noexcept { return !operator==(other); }
    bool operator<(const CharMap& other) const noexcept;
    bool operator<=(const CharMap& other) const noexcept { return operator<(other) || operator==(other); }
    bool operator>(const CharMap& other) const noexcept { return !operator<=(other); }
    bool operator>=(const CharMap& other) const noexcept { return !operator<(other); }

    std::string string() const;

    friend std::ostream& operator<<(std::ostream& stream, [[maybe_unused]] const CharMap& value);

    void swap(CharMap& other) noexcept;
    friend void swap(CharMap& value1, CharMap& value2) noexcept { value1.swap(value2); }
};

} // namespace proto_pmr

template<>
struct std::hash<proto_pmr::CharMap>
{
    typedef proto_pmr::CharMap argument_type;
    typedef size_t result_type;

    result_type operator() ([[maybe_unused]] const argument_type& value) const
    {
        result_type result = 17;
        return result;
    }
};

namespace proto_pmr {

struct OrderMessage
{
    ArenaManagedCreateOnlyTag;

    ::proto_pmr::Order body;

    size_t fbe_type() const noexcept { return 2; }

    OrderMessage();
    explicit OrderMessage(allocator_type alloc);
    explicit OrderMessage(const ::proto_pmr::Order& arg_body);
    OrderMessage(const OrderMessage& other) = default;
    OrderMessage(OrderMessage&& other) = default;
    ~OrderMessage() = default;

    OrderMessage& operator=(const OrderMessage& other) = default;
    OrderMessage& operator=(OrderMessage&& other) = default;

    bool operator==(const OrderMessage& other) const noexcept;
    bool operator!=(const OrderMessage& other) const noexcept { return !operator==(other); }
    bool operator<(const OrderMessage& other) const noexcept;
    bool operator<=(const OrderMessage& other) const noexcept { return operator<(other) || operator==(other); }
    bool operator>(const OrderMessage& other) const noexcept { return !operator<=(other); }
    bool operator>=(const OrderMessage& other) const noexcept { return !operator<(other); }

    std::string string() const;

    friend std::ostream& operator<<(std::ostream& stream, [[maybe_unused]] const OrderMessage& value);

    void swap(OrderMessage& other) noexcept;
    friend void swap(OrderMessage& value1, OrderMessage& value2) noexcept { value1.swap(value2); }
};

} // namespace proto_pmr

template<>
struct std::hash<proto_pmr::OrderMessage>
{
    typedef proto_pmr::OrderMessage argument_type;
    typedef size_t result_type;

    result_type operator() ([[maybe_unused]] const argument_type& value) const
    {
        result_type result = 17;
        return result;
    }
};

namespace proto_pmr {

struct BalanceMessage
{
    ArenaManagedCreateOnlyTag;

    ::proto_pmr::Balance body;

    size_t fbe_type() const noexcept { return 3; }

    BalanceMessage();
    explicit BalanceMessage(allocator_type alloc);
    explicit BalanceMessage(const ::proto_pmr::Balance& arg_body);
    BalanceMessage(const BalanceMessage& other) = default;
    BalanceMessage(BalanceMessage&& other) = default;
    ~BalanceMessage() = default;

    BalanceMessage& operator=(const BalanceMessage& other) = default;
    BalanceMessage& operator=(BalanceMessage&& other) = default;

    bool operator==(const BalanceMessage& other) const noexcept;
    bool operator!=(const BalanceMessage& other) const noexcept { return !operator==(other); }
    bool operator<(const BalanceMessage& other) const noexcept;
    bool operator<=(const BalanceMessage& other) const noexcept { return operator<(other) || operator==(other); }
    bool operator>(const BalanceMessage& other) const noexcept { return !operator<=(other); }
    bool operator>=(const BalanceMessage& other) const noexcept { return !operator<(other); }

    std::string string() const;

    friend std::ostream& operator<<(std::ostream& stream, [[maybe_unused]] const BalanceMessage& value);

    void swap(BalanceMessage& other) noexcept;
    friend void swap(BalanceMessage& value1, BalanceMessage& value2) noexcept { value1.swap(value2); }
};

} // namespace proto_pmr

template<>
struct std::hash<proto_pmr::BalanceMessage>
{
    typedef proto_pmr::BalanceMessage argument_type;
    typedef size_t result_type;

    result_type operator() ([[maybe_unused]] const argument_type& value) const
    {
        result_type result = 17;
        return result;
    }
};

namespace proto_pmr {

struct AccountMessage
{
    ArenaManagedCreateOnlyTag;

    ::proto_pmr::Account body;

    size_t fbe_type() const noexcept { return 4; }

    AccountMessage();
    explicit AccountMessage(allocator_type alloc);
    explicit AccountMessage(const ::proto_pmr::Account& arg_body);
    AccountMessage(const AccountMessage& other) = default;
    AccountMessage(AccountMessage&& other) = default;
    ~AccountMessage() = default;

    AccountMessage& operator=(const AccountMessage& other) = default;
    AccountMessage& operator=(AccountMessage&& other) = default;

    bool operator==(const AccountMessage& other) const noexcept;
    bool operator!=(const AccountMessage& other) const noexcept { return !operator==(other); }
    bool operator<(const AccountMessage& other) const noexcept;
    bool operator<=(const AccountMessage& other) const noexcept { return operator<(other) || operator==(other); }
    bool operator>(const AccountMessage& other) const noexcept { return !operator<=(other); }
    bool operator>=(const AccountMessage& other) const noexcept { return !operator<(other); }

    std::string string() const;

    friend std::ostream& operator<<(std::ostream& stream, [[maybe_unused]] const AccountMessage& value);

    void swap(AccountMessage& other) noexcept;
    friend void swap(AccountMessage& value1, AccountMessage& value2) noexcept { value1.swap(value2); }
};

} // namespace proto_pmr

template<>
struct std::hash<proto_pmr::AccountMessage>
{
    typedef proto_pmr::AccountMessage argument_type;
    typedef size_t result_type;

    result_type operator() ([[maybe_unused]] const argument_type& value) const
    {
        result_type result = 17;
        return result;
    }
};

namespace proto_pmr {

} // namespace proto_pmr
