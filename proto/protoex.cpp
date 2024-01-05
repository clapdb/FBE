//------------------------------------------------------------------------------
// Automatically generated by the Fast Binary Encoding compiler, do not modify!
// https://github.com/chronoxor/FastBinaryEncoding
// Source: protoex.fbe
// FBE version: 1.11.0.0
//------------------------------------------------------------------------------

#include "protoex.h"

namespace protoex {

std::ostream& operator<<(std::ostream& stream, [[maybe_unused]] OrderSide value)
{
    if (value == OrderSide::buy) return stream << "buy";
    if (value == OrderSide::sell) return stream << "sell";
    if (value == OrderSide::tell) return stream << "tell";
    return stream << "<unknown>";
}

std::ostream& operator<<(std::ostream& stream, [[maybe_unused]] OrderType value)
{
    if (value == OrderType::market) return stream << "market";
    if (value == OrderType::limit) return stream << "limit";
    if (value == OrderType::stop) return stream << "stop";
    if (value == OrderType::stoplimit) return stream << "stoplimit";
    return stream << "<unknown>";
}

std::ostream& operator<<(std::ostream& stream, [[maybe_unused]] StateEx value)
{
    bool first = true;
    if ((value & StateEx::unknown) && ((value & StateEx::unknown) == StateEx::unknown))
    {
        stream << (first ? "" : "|") << "unknown";
        first = false;
    }
    if ((value & StateEx::invalid) && ((value & StateEx::invalid) == StateEx::invalid))
    {
        stream << (first ? "" : "|") << "invalid";
        first = false;
    }
    if ((value & StateEx::initialized) && ((value & StateEx::initialized) == StateEx::initialized))
    {
        stream << (first ? "" : "|") << "initialized";
        first = false;
    }
    if ((value & StateEx::calculated) && ((value & StateEx::calculated) == StateEx::calculated))
    {
        stream << (first ? "" : "|") << "calculated";
        first = false;
    }
    if ((value & StateEx::broken) && ((value & StateEx::broken) == StateEx::broken))
    {
        stream << (first ? "" : "|") << "broken";
        first = false;
    }
    if ((value & StateEx::happy) && ((value & StateEx::happy) == StateEx::happy))
    {
        stream << (first ? "" : "|") << "happy";
        first = false;
    }
    if ((value & StateEx::sad) && ((value & StateEx::sad) == StateEx::sad))
    {
        stream << (first ? "" : "|") << "sad";
        first = false;
    }
    if ((value & StateEx::good) && ((value & StateEx::good) == StateEx::good))
    {
        stream << (first ? "" : "|") << "good";
        first = false;
    }
    if ((value & StateEx::bad) && ((value & StateEx::bad) == StateEx::bad))
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
    , tp((double)10.0)
    , sl((double)-10.0)
{}

Order::Order(int32_t arg_id, const FBEString& arg_symbol, const ::protoex::OrderSide& arg_side, const ::protoex::OrderType& arg_type, double arg_price, double arg_volume, double arg_tp, double arg_sl)
    : id(arg_id)
    , symbol(arg_symbol)
    , side(arg_side)
    , type(arg_type)
    , price(arg_price)
    , volume(arg_volume)
    , tp(arg_tp)
    , sl(arg_sl)
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
        && (tp == other.tp)
        && (sl == other.sl)
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
    swap(tp, other.tp);
    swap(sl, other.sl);
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
    stream << ",tp="; stream << value.tp;
    stream << ",sl="; stream << value.sl;
    stream << ")";
    return stream;
}

Balance::Balance()
    : ::proto::Balance()
    , locked((double)0.0)
{}

Balance::Balance(const ::proto::Balance& base, double arg_locked)
    : ::proto::Balance(base)
    , locked(arg_locked)
{}

bool Balance::operator==([[maybe_unused]] const Balance& other) const noexcept
{
    return (
        ::proto::Balance::operator==(other)
        && (locked == other.locked)
        );
}

bool Balance::operator<([[maybe_unused]] const Balance& other) const noexcept
{
    if (::proto::Balance::operator<(other))
        return true;
    if (other.::proto::Balance::operator<(*this))
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
    ::proto::Balance::swap(other);
    swap(locked, other.locked);
}

std::ostream& operator<<(std::ostream& stream, [[maybe_unused]] const Balance& value)
{
    stream << "Balance(";
    stream << (const ::proto::Balance&)value;
    stream << ",locked="; stream << value.locked;
    stream << ")";
    return stream;
}

Account::Account()
    : id((int32_t)0ll)
    , name()
    , state(StateEx::initialized  |  StateEx::bad  |  StateEx::sad)
    , wallet()
    , asset()
    , orders()
{}

Account::Account(int32_t arg_id, const FBEString& arg_name, const ::protoex::StateEx& arg_state, const ::protoex::Balance& arg_wallet, const std::optional<::protoex::Balance>& arg_asset, const FastVec<::protoex::Order>& arg_orders)
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

OrderMessage::OrderMessage()
    : body()
{}

OrderMessage::OrderMessage(const ::protoex::Order& arg_body)
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

BalanceMessage::BalanceMessage(const ::protoex::Balance& arg_body)
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

AccountMessage::AccountMessage(const ::protoex::Account& arg_body)
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

} // namespace protoex
