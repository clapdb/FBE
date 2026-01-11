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
// Created by Ivan Shynkarenka on 07.05.2018
//

#include "fbe.h"
#include "test.h"

#include "../proto/proto_models.h"
#include "../proto/test_models.h"

TEST_CASE("Create & access", "[FBE]")
{
    // Create a new account using FBE model into the FBE stream
    FBE::proto::AccountModel writer;
    REQUIRE(writer.model.fbe_offset() == 4);
    size_t model_begin = writer.create_begin();
    size_t account_begin = writer.model.set_begin();
    writer.model.id.set(1, nullptr);
    writer.model.name.set(FBE::FBEString("Test"), nullptr);
    writer.model.state.set(proto::State::good, nullptr);
    size_t wallet_begin = writer.model.wallet.set_begin();
    writer.model.wallet.currency.set(FBE::FBEString("USD"), nullptr);
    writer.model.wallet.amount.set(1000.0, nullptr);
    writer.model.wallet.set_end(wallet_begin);
    size_t asset_begin = writer.model.asset.set_begin(true);
    size_t asset_wallet_begin = writer.model.asset.value.set_begin();
    writer.model.asset.value.currency.set(FBE::FBEString("EUR"), nullptr);
    writer.model.asset.value.amount.set(100.0, nullptr);
    writer.model.asset.set_end(asset_begin);
    writer.model.asset.value.set_end(asset_wallet_begin);
    auto order = writer.model.orders.resize(3);
    size_t order_begin = order.set_begin();
    order.id.set(1, nullptr);
    order.symbol.set(FBE::FBEString("EURUSD"), nullptr);
    order.side.set(proto::OrderSide::buy, nullptr);
    order.type.set(proto::OrderType::market, nullptr);
    order.price.set(1.23456, nullptr);
    order.volume.set(1000.0, nullptr);
    order.set_end(order_begin);
    order.fbe_shift(order.fbe_size());
    order_begin = order.set_begin();
    order.id.set(2, nullptr);
    order.symbol.set(FBE::FBEString("EURUSD"), nullptr);
    order.side.set(proto::OrderSide::sell, nullptr);
    order.type.set(proto::OrderType::limit, nullptr);
    order.price.set(1.0, nullptr);
    order.volume.set(100.0, nullptr);
    order.set_end(order_begin);
    order.fbe_shift(order.fbe_size());
    order_begin = order.set_begin();
    order.id.set(3, nullptr);
    order.symbol.set(FBE::FBEString("EURUSD"), nullptr);
    order.side.set(proto::OrderSide::buy, nullptr);
    order.type.set(proto::OrderType::stop, nullptr);
    order.price.set(1.5, nullptr);
    order.volume.set(10.0, nullptr);
    order.set_end(order_begin);
    order.fbe_shift(order.fbe_size());
    writer.model.set_end(account_begin);
    size_t serialized = writer.create_end(model_begin);
    REQUIRE(serialized == writer.buffer().size());
    REQUIRE(writer.verify());
    writer.next(serialized);
    REQUIRE(writer.model.fbe_offset() == (4 + writer.buffer().size()));

    // Check the serialized FBE size
    REQUIRE(writer.buffer().size() == 252);

    // Access the account model in the FBE stream
    FBE::proto::AccountModel reader;
    REQUIRE(reader.model.fbe_offset() == 4);
    reader.attach(writer.buffer());
    REQUIRE(reader.verify());

    int32_t id;
    FBE::FBEString name;
    proto::State state;
    FBE::FBEString wallet_currency;
    double wallet_amount;
    FBE::FBEString asset_wallet_currency;
    double asset_wallet_amount;

    account_begin = reader.model.get_begin();
    reader.model.id.get(id, nullptr);
    REQUIRE(id == 1);
    reader.model.name.get(name, nullptr);
    REQUIRE(name == "Test");
    reader.model.state.get(state, nullptr);
    REQUIRE((state | proto::State::good));

    wallet_begin = reader.model.wallet.get_begin();
    reader.model.wallet.currency.get(wallet_currency, nullptr);
    REQUIRE(wallet_currency == "USD");
    reader.model.wallet.amount.get(wallet_amount, nullptr);
    REQUIRE(wallet_amount == 1000.0);
    reader.model.wallet.get_end(wallet_begin);

    REQUIRE(reader.model.asset.has_value());
    asset_begin = reader.model.asset.get_begin();
    asset_wallet_begin = reader.model.asset.value.get_begin();
    reader.model.asset.value.currency.get(asset_wallet_currency, nullptr);
    REQUIRE(asset_wallet_currency == "EUR");
    reader.model.asset.value.amount.get(asset_wallet_amount, nullptr);
    REQUIRE(asset_wallet_amount == 100.0);
    reader.model.asset.value.get_end(asset_wallet_begin);
    reader.model.asset.get_end(asset_begin);

    REQUIRE(reader.model.orders.size() == 3);

    int order_id;
    FBE::FBEString order_symbol;
    proto::OrderSide order_side;
    proto::OrderType order_type;
    double order_price;
    double order_volume;

    auto o1 = reader.model.orders[0];
    order_begin = o1.get_begin();
    o1.id.get(order_id, nullptr);
    REQUIRE(order_id == 1);
    o1.symbol.get(order_symbol, nullptr);
    REQUIRE(order_symbol == "EURUSD");
    o1.side.get(order_side, nullptr);
    REQUIRE(order_side == proto::OrderSide::buy);
    o1.type.get(order_type, nullptr);
    REQUIRE(order_type == proto::OrderType::market);
    o1.price.get(order_price, nullptr);
    REQUIRE(order_price == 1.23456);
    o1.volume.get(order_volume, nullptr);
    REQUIRE(order_volume == 1000.0);
    o1.get_end(order_begin);

    auto o2 = reader.model.orders[1];
    order_begin = o2.get_begin();
    o2.id.get(order_id, nullptr);
    REQUIRE(order_id == 2);
    o2.symbol.get(order_symbol, nullptr);
    REQUIRE(order_symbol == "EURUSD");
    o2.side.get(order_side, nullptr);
    REQUIRE(order_side == proto::OrderSide::sell);
    o2.type.get(order_type, nullptr);
    REQUIRE(order_type == proto::OrderType::limit);
    o2.price.get(order_price, nullptr);
    REQUIRE(order_price == 1.0);
    o2.volume.get(order_volume, nullptr);
    REQUIRE(order_volume == 100.0);
    o1.get_end(order_begin);

    auto o3 = reader.model.orders[2];
    order_begin = o3.get_begin();
    o3.id.get(order_id, nullptr);
    REQUIRE(order_id == 3);
    o3.symbol.get(order_symbol, nullptr);
    REQUIRE(order_symbol == "EURUSD");
    o3.side.get(order_side, nullptr);
    REQUIRE(order_side == proto::OrderSide::buy);
    o3.type.get(order_type, nullptr);
    REQUIRE(order_type == proto::OrderType::stop);
    o3.price.get(order_price, nullptr);
    REQUIRE(order_price == 1.5);
    o3.volume.get(order_volume, nullptr);
    REQUIRE(order_volume == 10.0);
    o1.get_end(order_begin);

    reader.model.get_end(account_begin);
}
