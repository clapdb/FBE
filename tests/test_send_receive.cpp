/*
 * Copyright (C) STDB Holdings Limited
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * This file is part of FBE (Fast Binary Encoding).
 *
 * FBE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FBE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with FBE. If not, see <https://www.gnu.org/licenses/>.
 */
//
// Created by Ivan Shynkarenka on 15.05.2018
//

#include "test.h"

#include "../proto/proto_protocol.h"
#include "../proto/test_protocol.h"

class MySender : public FBE::proto::Sender
{
protected:
    size_t onSend(const void* data, size_t size) override
    {
        // Send nothing...
        return 0;
    }
};

class MyReceiver : public FBE::proto::Receiver
{
public:
    MyReceiver() : _order(false), _balance(false), _account(false) {}

    bool check() const { return _order && _balance && _account; }

protected:
    void onReceive(const proto::OrderMessage& value) override { _order = true; }
    void onReceive(const proto::BalanceMessage& value) override { _balance = true; }
    void onReceive(const proto::AccountMessage& value) override { _account = true; }

private:
    bool _order;
    bool _balance;
    bool _account;
};

bool SendAndReceive(size_t index1, size_t index2)
{
    MySender sender;

    // Create and send a new order
    proto::OrderMessage order({ proto::Order({ 1, "EURUSD", proto::OrderSide::buy, proto::OrderType::market, 1.23456, 1000.0 }) });
    sender.send(order);

    // Create and send a new balance wallet
    proto::BalanceMessage balance({ proto::Balance({ "USD", 1000.0 }) });
    sender.send(balance);

    // Create and send a new account with some orders
    proto::AccountMessage account({ proto::Account({ 1, "Test", proto::State::good, { "USD", 1000.0 }, std::make_optional<proto::Balance>({ "EUR", 100.0 }), {} }) });
    account.body.orders.emplace_back(1, "EURUSD", proto::OrderSide::buy, proto::OrderType::market, 1.23456, 1000.0);
    account.body.orders.emplace_back(2, "EURUSD", proto::OrderSide::sell, proto::OrderType::limit, 1.0, 100.0);
    account.body.orders.emplace_back(3, "EURUSD", proto::OrderSide::buy, proto::OrderType::stop, 1.5, 10.0);
    sender.send(account);

    MyReceiver receiver;

    // Receive data from the sender
    index1 %= sender.buffer().size();
    index2 %= sender.buffer().size();
    index2 = std::max(index1, index2);
    receiver.receive(sender.buffer().data(), index1);
    receiver.receive(sender.buffer().data() + index1, index2 - index1);
    receiver.receive(sender.buffer().data() + index2, sender.buffer().size() - index2);
    return receiver.check();
}

// TEST_CASE("tend & Receive", "[FBE]")
// {
//     SendAndReceive(1, 56);

//     for (size_t i = 0; i < 1000; ++i)
//         for (size_t j = i; j < 1000; ++j)
//             REQUIRE(SendAndReceive(i, j));
// }
