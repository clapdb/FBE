//------------------------------------------------------------------------------
// Automatically generated by the Fast Binary Encoding compiler, do not modify!
// https://github.com/chronoxor/FastBinaryEncoding
// Source: proto.fbe
// FBE version: 1.11.0.0
//------------------------------------------------------------------------------

#include "proto_pmr.h"

namespace proto_pmr {

std::ostream& operator<<(std::ostream& stream, [[maybe_unused]] OrderSide value)
{
    if (value == OrderSide::buy) return stream << "buy";
    if (value == OrderSide::sell) return stream << "sell";
    return stream << "<unknown>";
}

std::ostream& operator<<(std::ostream& stream, [[maybe_unused]] OrderType value)
{
    if (value == OrderType::market) return stream << "market";
    if (value == OrderType::limit) return stream << "limit";
    if (value == OrderType::stop) return stream << "stop";
    return stream << "<unknown>";
}

std::ostream& operator<<(std::ostream& stream, [[maybe_unused]] State value)
{
    bool first = true;
    if ((value & State::unknown) && ((value & State::unknown) == State::unknown))
    {
        stream << (first ? "" : "|") << "unknown";
        first = false;
    }
    if ((value & State::invalid) && ((value & State::invalid) == State::invalid))
    {
        stream << (first ? "" : "|") << "invalid";
        first = false;
    }
    if ((value & State::initialized) && ((value & State::initialized) == State::initialized))
    {
        stream << (first ? "" : "|") << "initialized";
        first = false;
    }
    if ((value & State::calculated) && ((value & State::calculated) == State::calculated))
    {
        stream << (first ? "" : "|") << "calculated";
        first = false;
    }
    if ((value & State::broken) && ((value & State::broken) == State::broken))
    {
        stream << (first ? "" : "|") << "broken";
        first = false;
    }
    if ((value & State::good) && ((value & State::good) == State::good))
    {
        stream << (first ? "" : "|") << "good";
        first = false;
    }
    if ((value & State::bad) && ((value & State::bad) == State::bad))
    {
        stream << (first ? "" : "|") << "bad";
        first = false;
    }
    return stream;
}

Order::Order()
    : id((int32_t)0ll)
    , symbol()
    , side()
    , type()
    , price((double)0.0)
    , volume((double)0.0)
{}

Order::Order([[maybe_unused]] allocator_type alloc)
    : id((int32_t)0ll)
    , symbol(alloc)
    , side()
    , type()
    , price((double)0.0)
    , volume((double)0.0)
{}

Order::Order(int32_t arg_id, const ArenaString& arg_symbol, const ::proto_pmr::OrderSide& arg_side, const ::proto_pmr::OrderType& arg_type, double arg_price, double arg_volume)
    : id(arg_id)
    , symbol(arg_symbol)
    , side(arg_side)
    , type(arg_type)
    , price(arg_price)
    , volume(arg_volume)
{}

bool Order::operator==([[maybe_unused]] const Order& other) const noexcept
{
    return (
        (id == other.id)
        && (symbol == other.symbol)
        && (side == other.side)
        && (type == other.type)
        && (price == other.price)
        && (volume == other.volume)
        );
}

bool Order::operator<([[maybe_unused]] const Order& other) const noexcept
{
    if (id < other.id)
        return true;
    if (other.id < id)
        return false;
    return false;
}

std::string Order::string() const
{
    std::stringstream ss; ss << *this; return ss.str();
}

void Order::swap([[maybe_unused]] Order& other) noexcept
{
    using std::swap;
    swap(id, other.id);
    swap(symbol, other.symbol);
    swap(side, other.side);
    swap(type, other.type);
    swap(price, other.price);
    swap(volume, other.volume);
}

std::ostream& operator<<(std::ostream& stream, [[maybe_unused]] const Order& value)
{
    stream << "Order(";
    stream << "id="; stream << value.id;
    stream << ",symbol="; stream << "\"" << value.symbol << "\"";
    stream << ",side="; stream << value.side;
    stream << ",type="; stream << value.type;
    stream << ",price="; stream << value.price;
    stream << ",volume="; stream << value.volume;
    stream << ")";
    return stream;
}

Balance::Balance()
    : currency()
    , amount((double)0.0)
{}

Balance::Balance([[maybe_unused]] allocator_type alloc)
    : currency(alloc)
    , amount((double)0.0)
{}

Balance::Balance(const ArenaString& arg_currency, double arg_amount)
    : currency(arg_currency)
    , amount(arg_amount)
{}

bool Balance::operator==([[maybe_unused]] const Balance& other) const noexcept
{
    return (
        (currency == other.currency)
        && (amount == other.amount)
        );
}

bool Balance::operator<([[maybe_unused]] const Balance& other) const noexcept
{
    if (currency < other.currency)
        return true;
    if (other.currency < currency)
        return false;
    return false;
}

std::string Balance::string() const
{
    std::stringstream ss; ss << *this; return ss.str();
}

void Balance::swap([[maybe_unused]] Balance& other) noexcept
{
    using std::swap;
    swap(currency, other.currency);
    swap(amount, other.amount);
}

std::ostream& operator<<(std::ostream& stream, [[maybe_unused]] const Balance& value)
{
    stream << "Balance(";
    stream << "currency="; stream << "\"" << value.currency << "\"";
    stream << ",amount="; stream << value.amount;
    stream << ")";
    return stream;
}

Account::Account()
    : id((int32_t)0ll)
    , name()
    , state(State::initialized  |  State::bad)
    , wallet()
    , asset()
    , orders()
{}

Account::Account([[maybe_unused]] allocator_type alloc)
    : id((int32_t)0ll)
    , name(alloc)
    , state(State::initialized  |  State::bad)
    , wallet(alloc)
    , asset(std::nullopt)
    , orders(alloc)
{}

Account::Account(int32_t arg_id, const ArenaString& arg_name, const ::proto_pmr::State& arg_state, const ::proto_pmr::Balance& arg_wallet, const std::optional<::proto_pmr::Balance>& arg_asset, const pmr::vector<::proto_pmr::Order>& arg_orders)
    : id(arg_id)
    , name(arg_name)
    , state(arg_state)
    , wallet(arg_wallet)
    , asset(arg_asset)
    , orders(arg_orders)
{}

bool Account::operator==([[maybe_unused]] const Account& other) const noexcept
{
    return (
        (id == other.id)
        && (name == other.name)
        && (state == other.state)
        && (wallet == other.wallet)
        && (asset == other.asset)
        && (orders == other.orders)
        );
}

bool Account::operator<([[maybe_unused]] const Account& other) const noexcept
{
    if (id < other.id)
        return true;
    if (other.id < id)
        return false;
    return false;
}

std::string Account::string() const
{
    std::stringstream ss; ss << *this; return ss.str();
}

void Account::swap([[maybe_unused]] Account& other) noexcept
{
    using std::swap;
    swap(id, other.id);
    swap(name, other.name);
    swap(state, other.state);
    swap(wallet, other.wallet);
    swap(asset, other.asset);
    swap(orders, other.orders);
}

std::ostream& operator<<(std::ostream& stream, [[maybe_unused]] const Account& value)
{
    stream << "Account(";
    stream << "id="; stream << value.id;
    stream << ",name="; stream << "\"" << value.name << "\"";
    stream << ",state="; stream << value.state;
    stream << ",wallet="; stream << value.wallet;
    stream << ",asset="; if (value.asset) stream << *value.asset; else stream << "null";
    {
        bool first = true;
        stream << ",orders=[" << value.orders.size() << "][";
        for ([[maybe_unused]] const auto& it : value.orders)
        {
            stream << std::string(first ? "" : ",") << it;
            first = false;
        }
        stream << "]";
    }
    stream << ")";
    return stream;
}

CharMap::CharMap()
    : abbr()
{}

CharMap::CharMap([[maybe_unused]] allocator_type alloc)
    : abbr(alloc)
{}

CharMap::CharMap(const pmr::unordered_map<char, ArenaString>& arg_abbr)
    : abbr(arg_abbr)
{}

bool CharMap::operator==([[maybe_unused]] const CharMap& other) const noexcept
{
    return (
        (abbr == other.abbr)
        );
}

bool CharMap::operator<([[maybe_unused]] const CharMap& other) const noexcept
{
    return false;
}

std::string CharMap::string() const
{
    std::stringstream ss; ss << *this; return ss.str();
}

void CharMap::swap([[maybe_unused]] CharMap& other) noexcept
{
    using std::swap;
    swap(abbr, other.abbr);
}

std::ostream& operator<<(std::ostream& stream, [[maybe_unused]] const CharMap& value)
{
    stream << "CharMap(";
    {
        bool first = true;
        stream << "abbr=[" << value.abbr.size()<< "][{";
        for ([[maybe_unused]] const auto& it : value.abbr)
        {
            stream << std::string(first ? "" : ",") << "'" << it.first << "'";
            stream << "->";
            stream << "\"" << it.second << "\"";
            first = false;
        }
        stream << "}]";
    }
    stream << ")";
    return stream;
}

OrderMessage::OrderMessage()
    : body()
{}

OrderMessage::OrderMessage([[maybe_unused]] allocator_type alloc)
    : body(alloc)
{}

OrderMessage::OrderMessage(const ::proto_pmr::Order& arg_body)
    : body(arg_body)
{}

bool OrderMessage::operator==([[maybe_unused]] const OrderMessage& other) const noexcept
{
    return (
        (body == other.body)
        );
}

bool OrderMessage::operator<([[maybe_unused]] const OrderMessage& other) const noexcept
{
    return false;
}

std::string OrderMessage::string() const
{
    std::stringstream ss; ss << *this; return ss.str();
}

void OrderMessage::swap([[maybe_unused]] OrderMessage& other) noexcept
{
    using std::swap;
    swap(body, other.body);
}

std::ostream& operator<<(std::ostream& stream, [[maybe_unused]] const OrderMessage& value)
{
    stream << "OrderMessage(";
    stream << "body="; stream << value.body;
    stream << ")";
    return stream;
}

BalanceMessage::BalanceMessage()
    : body()
{}

BalanceMessage::BalanceMessage([[maybe_unused]] allocator_type alloc)
    : body(alloc)
{}

BalanceMessage::BalanceMessage(const ::proto_pmr::Balance& arg_body)
    : body(arg_body)
{}

bool BalanceMessage::operator==([[maybe_unused]] const BalanceMessage& other) const noexcept
{
    return (
        (body == other.body)
        );
}

bool BalanceMessage::operator<([[maybe_unused]] const BalanceMessage& other) const noexcept
{
    return false;
}

std::string BalanceMessage::string() const
{
    std::stringstream ss; ss << *this; return ss.str();
}

void BalanceMessage::swap([[maybe_unused]] BalanceMessage& other) noexcept
{
    using std::swap;
    swap(body, other.body);
}

std::ostream& operator<<(std::ostream& stream, [[maybe_unused]] const BalanceMessage& value)
{
    stream << "BalanceMessage(";
    stream << "body="; stream << value.body;
    stream << ")";
    return stream;
}

AccountMessage::AccountMessage()
    : body()
{}

AccountMessage::AccountMessage([[maybe_unused]] allocator_type alloc)
    : body(alloc)
{}

AccountMessage::AccountMessage(const ::proto_pmr::Account& arg_body)
    : body(arg_body)
{}

bool AccountMessage::operator==([[maybe_unused]] const AccountMessage& other) const noexcept
{
    return (
        (body == other.body)
        );
}

bool AccountMessage::operator<([[maybe_unused]] const AccountMessage& other) const noexcept
{
    return false;
}

std::string AccountMessage::string() const
{
    std::stringstream ss; ss << *this; return ss.str();
}

void AccountMessage::swap([[maybe_unused]] AccountMessage& other) noexcept
{
    using std::swap;
    swap(body, other.body);
}

std::ostream& operator<<(std::ostream& stream, [[maybe_unused]] const AccountMessage& value)
{
    stream << "AccountMessage(";
    stream << "body="; stream << value.body;
    stream << ")";
    return stream;
}

PremiumAccount::PremiumAccount()
    : id((int32_t)0ll)
    , name()
    , info()
    , private_wallet()
    , private_orders()
    , private_state(State::bad)
{}

PremiumAccount::PremiumAccount([[maybe_unused]] allocator_type alloc)
    : id((int32_t)0ll)
    , name(alloc)
    , info(alloc)
    , private_wallet(alloc)
    , private_orders(alloc)
    , private_state(State::bad)
{}

PremiumAccount::PremiumAccount(int32_t arg_id, const ArenaString& arg_name, const ArenaString& arg_info, const ::proto_pmr::Balance& arg_private_wallet, const pmr::vector<::proto_pmr::Order>& arg_private_orders, const ::proto_pmr::State& arg_private_state)
    : id(arg_id)
    , name(arg_name)
    , info(arg_info)
    , private_wallet(arg_private_wallet)
    , private_orders(arg_private_orders)
    , private_state(arg_private_state)
{}

bool PremiumAccount::operator==([[maybe_unused]] const PremiumAccount& other) const noexcept
{
    return (
        (id == other.id)
        && (name == other.name)
        && (info == other.info)
        && (private_wallet == other.private_wallet)
        && (private_orders == other.private_orders)
        && (private_state == other.private_state)
        );
}

bool PremiumAccount::operator<([[maybe_unused]] const PremiumAccount& other) const noexcept
{
    if (id < other.id)
        return true;
    if (other.id < id)
        return false;
    return false;
}

std::string PremiumAccount::string() const
{
    std::stringstream ss; ss << *this; return ss.str();
}

void PremiumAccount::swap([[maybe_unused]] PremiumAccount& other) noexcept
{
    using std::swap;
    swap(id, other.id);
    swap(name, other.name);
    swap(info, other.info);
    swap(private_wallet, other.private_wallet);
    swap(private_orders, other.private_orders);
    swap(private_state, other.private_state);
}

std::ostream& operator<<(std::ostream& stream, [[maybe_unused]] const PremiumAccount& value)
{
    stream << "PremiumAccount(";
    stream << "id="; stream << value.id;
    stream << ",name="; stream << "\"" << value.name << "\"";
    stream << ",info="; stream << "\"" << value.info << "\"";
    stream << ",private_wallet="; stream << value.private_wallet;
    {
        bool first = true;
        stream << ",private_orders=[" << value.private_orders.size() << "][";
        for ([[maybe_unused]] const auto& it : value.private_orders)
        {
            stream << std::string(first ? "" : ",") << it;
            first = false;
        }
        stream << "]";
    }
    stream << ",private_state="; stream << value.private_state;
    stream << ")";
    return stream;
}

} // namespace proto_pmr
