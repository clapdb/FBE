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
// Created by Ivan Shynkarenka on 18.04.2018
//

#include "fbe.h"
#include "test.h"

#include "../proto/cpp_only_models.h"
#include "../proto/proto_models.h"
#include "../proto/test_models.h"
#include "../proto/variants_models.h"
#include "variants.h"
#include <cstdint>
#include <limits>
template <typename T> using FastVec = std::vector<T>;

TEST_CASE("Serialization: domain", "[FBE]") {
  // Create a new account with some orders
  proto::Account account1 = {1,
                             "Test",
                             proto::State::good,
                             {"USD", 1000.0},
                             std::make_optional<proto::Balance>({"EUR", 100.0}),
                             {}};
  account1.orders.emplace_back(1, "EURUSD", proto::OrderSide::buy,
                               proto::OrderType::market, 1.23456, 1000.0);
  account1.orders.emplace_back(2, "EURUSD", proto::OrderSide::sell,
                               proto::OrderType::limit, 1.0, 100.0);
  account1.orders.emplace_back(3, "EURUSD", proto::OrderSide::buy,
                               proto::OrderType::stop, 1.5, 10.0);

  // Serialize the account to the FBE stream
  FBE::proto::AccountModel writer;
  REQUIRE(writer.model.fbe_offset() == 4);
  size_t serialized = writer.serialize(account1, nullptr);
  REQUIRE(serialized == writer.buffer().size());
  REQUIRE(writer.verify());
  writer.next(serialized);
  REQUIRE(writer.model.fbe_offset() == (4 + writer.buffer().size()));

  // Check the serialized FBE size
  REQUIRE(writer.buffer().size() == 252);

  // Deserialize the account from the FBE stream
  proto::Account account2;
  FBE::proto::AccountModel reader;
  REQUIRE(reader.model.fbe_offset() == 4);
  reader.attach(writer.buffer());
  REQUIRE(reader.verify());
  size_t deserialized = reader.deserialize(account2, nullptr);
  REQUIRE(deserialized == reader.buffer().size());
  reader.next(deserialized);
  REQUIRE(reader.model.fbe_offset() == (4 + reader.buffer().size()));

  REQUIRE(account2.id == 1);
  REQUIRE(account2.name == "Test");
  REQUIRE((account2.state | proto::State::good));
  REQUIRE(FBE::FBEString(account2.wallet.currency) == "USD");
  REQUIRE(account2.wallet.amount == 1000.0);
  REQUIRE(account2.asset.has_value());
  REQUIRE(FBE::FBEString(account2.asset.value().currency) == "EUR");
  REQUIRE(account2.asset.value().amount == 100.0);
  REQUIRE(account2.orders.size() == 3);
  REQUIRE(account2.orders[0].id == 1);
  REQUIRE(FBE::FBEString(account2.orders[0].symbol) == "EURUSD");
  REQUIRE(account2.orders[0].side == proto::OrderSide::buy);
  REQUIRE(account2.orders[0].type == proto::OrderType::market);
  REQUIRE(account2.orders[0].price == 1.23456);
  REQUIRE(account2.orders[0].volume == 1000.0);
  REQUIRE(account2.orders[1].id == 2);
  REQUIRE(FBE::FBEString(account2.orders[1].symbol) == "EURUSD");
  REQUIRE(account2.orders[1].side == proto::OrderSide::sell);
  REQUIRE(account2.orders[1].type == proto::OrderType::limit);
  REQUIRE(account2.orders[1].price == 1.0);
  REQUIRE(account2.orders[1].volume == 100.0);
  REQUIRE(account2.orders[2].id == 3);
  REQUIRE(FBE::FBEString(account2.orders[2].symbol) == "EURUSD");
  REQUIRE(account2.orders[2].side == proto::OrderSide::buy);
  REQUIRE(account2.orders[2].type == proto::OrderType::stop);
  REQUIRE(account2.orders[2].price == 1.5);
  REQUIRE(account2.orders[2].volume == 10.0);
  REQUIRE(account1 == account2);
}

TEST_CASE("Serialization: struct simple", "[FBE]") {
  // Create a new struct
  test::StructSimple struct1;

  // Serialize the struct to the FBE stream
  FBE::test::StructSimpleModel writer;
  REQUIRE(writer.model.fbe_type() == 110);
  REQUIRE(writer.model.fbe_offset() == 4);
  size_t serialized = writer.serialize(struct1, nullptr);
  REQUIRE(serialized == writer.buffer().size());
  REQUIRE(writer.verify());
  writer.next(serialized);
  REQUIRE(writer.model.fbe_offset() == (4 + writer.buffer().size()));

  // Check the serialized FBE size
  REQUIRE(writer.buffer().size() == 392);

  // Deserialize the struct from the FBE stream
  test::StructSimple struct2;
  FBE::test::StructSimpleModel reader;
  REQUIRE(reader.model.fbe_type() == 110);
  REQUIRE(reader.model.fbe_offset() == 4);
  reader.attach(writer.buffer());
  REQUIRE(reader.verify());
  size_t deserialized = reader.deserialize(struct2, nullptr);
  REQUIRE(deserialized == reader.buffer().size());
  reader.next(deserialized);
  REQUIRE(reader.model.fbe_offset() == (4 + reader.buffer().size()));

  REQUIRE(struct2.f1 == false);
  REQUIRE(struct2.f2 == true);
  REQUIRE(struct2.f3 == 0);
  REQUIRE(struct2.f4 == 0xFF);
  REQUIRE(struct2.f5 == '\0');
  REQUIRE(struct2.f6 == '!');
  REQUIRE(struct2.f7 == 0);
  REQUIRE(struct2.f8 == 0x0444);
  REQUIRE(struct2.f9 == 0);
  REQUIRE(struct2.f10 == 127);
  REQUIRE(struct2.f11 == 0);
  REQUIRE(struct2.f12 == 0xFF);
  REQUIRE(struct2.f13 == 0);
  REQUIRE(struct2.f14 == 32767);
  REQUIRE(struct2.f15 == 0);
  REQUIRE(struct2.f16 == 0xFFFF);
  REQUIRE(struct2.f17 == 0);
  REQUIRE(struct2.f18 == 2147483647);
  REQUIRE(struct2.f19 == 0);
  REQUIRE(struct2.f20 == 0xFFFFFFFF);
  REQUIRE(struct2.f21 == 0);
  REQUIRE(struct2.f22 == 9223372036854775807ll);
  REQUIRE(struct2.f23 == 0);
  REQUIRE(struct2.f24 == 0xFFFFFFFFFFFFFFFFull);
  REQUIRE(struct2.f25 == 0.0f);
  REQUIRE(std::abs(struct2.f26 - 123.456f) < 0.0001);
  REQUIRE(struct2.f27 == 0.0);
  REQUIRE(std::abs(struct2.f28 - -123.567e+123) < 1e+123);
  REQUIRE(struct2.f29 == 0.0);
  REQUIRE(struct2.f30 == 123456.123456);
  REQUIRE(struct2.f31 == "");
  REQUIRE(struct2.f32 == "Initial string!");
  REQUIRE(struct2.f33 == 0);
  REQUIRE(struct2.f34 == 0);
  REQUIRE(struct2.f35 > 1514764800000000000);
  REQUIRE(struct2.f36 == FBE::uuid_t());
  REQUIRE(struct2.f37 != FBE::uuid_t());
  REQUIRE(struct2.f38 == FBE::uuid_t("123e4567-e89b-12d3-a456-426655440000"));

  REQUIRE(struct2.f1 == struct1.f1);
  REQUIRE(struct2.f2 == struct1.f2);
  REQUIRE(struct2.f3 == struct1.f3);
  REQUIRE(struct2.f4 == struct1.f4);
  REQUIRE(struct2.f5 == struct1.f5);
  REQUIRE(struct2.f6 == struct1.f6);
  REQUIRE(struct2.f7 == struct1.f7);
  REQUIRE(struct2.f8 == struct1.f8);
  REQUIRE(struct2.f9 == struct1.f9);
  REQUIRE(struct2.f10 == struct1.f10);
  REQUIRE(struct2.f11 == struct1.f11);
  REQUIRE(struct2.f12 == struct1.f12);
  REQUIRE(struct2.f13 == struct1.f13);
  REQUIRE(struct2.f14 == struct1.f14);
  REQUIRE(struct2.f15 == struct1.f15);
  REQUIRE(struct2.f16 == struct1.f16);
  REQUIRE(struct2.f17 == struct1.f17);
  REQUIRE(struct2.f18 == struct1.f18);
  REQUIRE(struct2.f19 == struct1.f19);
  REQUIRE(struct2.f20 == struct1.f20);
  REQUIRE(struct2.f21 == struct1.f21);
  REQUIRE(struct2.f22 == struct1.f22);
  REQUIRE(struct2.f23 == struct1.f23);
  REQUIRE(struct2.f24 == struct1.f24);
  REQUIRE(struct2.f25 == struct1.f25);
  REQUIRE(struct2.f26 == struct1.f26);
  REQUIRE(struct2.f27 == struct1.f27);
  REQUIRE(struct2.f28 == struct1.f28);
  REQUIRE(struct2.f29 == struct1.f29);
  REQUIRE(struct2.f30 == struct1.f30);
  REQUIRE(struct2.f31 == struct1.f31);
  REQUIRE(struct2.f32 == struct1.f32);
  REQUIRE(struct2.f33 == struct1.f33);
  REQUIRE(struct2.f34 == struct1.f34);
  REQUIRE(struct2.f35 == struct1.f35);
  REQUIRE(struct2.f36 == struct1.f36);
  REQUIRE(struct2.f37 == struct1.f37);
  REQUIRE(struct2.f38 == struct1.f38);
  REQUIRE(struct2.f39 == struct1.f39);
  REQUIRE(struct2.f40 == struct1.f40);
  REQUIRE(struct1 == struct2);
}

TEST_CASE("Serialization: struct optional", "[FBE]") {
  // Create a new struct
  test::StructOptional struct1;

  // Serialize the struct to the FBE stream
  FBE::test::StructOptionalModel writer;
  REQUIRE(writer.model.fbe_type() == 111);
  REQUIRE(writer.model.fbe_offset() == 4);
  size_t serialized = writer.serialize(struct1, nullptr);
  REQUIRE(serialized == writer.buffer().size());
  REQUIRE(writer.verify());
  writer.next(serialized);
  REQUIRE(writer.model.fbe_offset() == (4 + writer.buffer().size()));

  // Check the serialized FBE size
  REQUIRE(writer.buffer().size() == 834);

  // Deserialize the struct from the FBE stream
  test::StructOptional struct2;
  FBE::test::StructOptionalModel reader;
  REQUIRE(reader.model.fbe_type() == 111);
  REQUIRE(reader.model.fbe_offset() == 4);
  reader.attach(writer.buffer());
  REQUIRE(reader.verify());
  size_t deserialized = reader.deserialize(struct2, nullptr);
  REQUIRE(deserialized == reader.buffer().size());
  reader.next(deserialized);
  REQUIRE(reader.model.fbe_offset() == (4 + reader.buffer().size()));

  REQUIRE(struct2.f1 == false);
  REQUIRE(struct2.f2 == true);
  REQUIRE(struct2.f3 == 0);
  REQUIRE(struct2.f4 == 0xFF);
  REQUIRE(struct2.f5 == '\0');
  REQUIRE(struct2.f6 == '!');
  REQUIRE(struct2.f7 == 0);
  REQUIRE(struct2.f8 == 0x0444);
  REQUIRE(struct2.f9 == 0);
  REQUIRE(struct2.f10 == 127);
  REQUIRE(struct2.f11 == 0);
  REQUIRE(struct2.f12 == 0xFF);
  REQUIRE(struct2.f13 == 0);
  REQUIRE(struct2.f14 == 32767);
  REQUIRE(struct2.f15 == 0);
  REQUIRE(struct2.f16 == 0xFFFF);
  REQUIRE(struct2.f17 == 0);
  REQUIRE(struct2.f18 == 2147483647);
  REQUIRE(struct2.f19 == 0);
  REQUIRE(struct2.f20 == 0xFFFFFFFF);
  REQUIRE(struct2.f21 == 0);
  REQUIRE(struct2.f22 == 9223372036854775807ll);
  REQUIRE(struct2.f23 == 0);
  REQUIRE(struct2.f24 == 0xFFFFFFFFFFFFFFFFull);
  REQUIRE(struct2.f25 == 0.0f);
  REQUIRE(std::abs(struct2.f26 - 123.456f) < 0.0001);
  REQUIRE(struct2.f27 == 0.0);
  REQUIRE(std::abs(struct2.f28 - -123.567e+123) < 1e+123);
  REQUIRE(struct2.f29 == 0.0);
  REQUIRE(struct2.f30 == 123456.123456);
  REQUIRE(struct2.f31 == "");
  REQUIRE(struct2.f32 == "Initial string!");
  REQUIRE(struct2.f33 == 0);
  REQUIRE(struct2.f34 == 0);
  REQUIRE(struct2.f35 > 1514764800000000000);
  REQUIRE(struct2.f36 == FBE::uuid_t());
  REQUIRE(struct2.f37 != FBE::uuid_t());
  REQUIRE(struct2.f38 == FBE::uuid_t("123e4567-e89b-12d3-a456-426655440000"));

  REQUIRE(!struct2.f100.has_value());
  REQUIRE(struct2.f101.has_value());
  REQUIRE(struct2.f101.value() == true);
  REQUIRE(!struct2.f102.has_value());
  REQUIRE(!struct2.f103.has_value());
  REQUIRE(struct2.f104.has_value());
  REQUIRE(struct2.f104.value() == 0xFF);
  REQUIRE(!struct2.f105.has_value());
  REQUIRE(!struct2.f106.has_value());
  REQUIRE(struct2.f107.has_value());
  REQUIRE(struct2.f107.value() == '!');
  REQUIRE(!struct2.f108.has_value());
  REQUIRE(!struct2.f109.has_value());
  REQUIRE(struct2.f110.has_value());
  REQUIRE(struct2.f110.value() == 0x0444);
  REQUIRE(!struct2.f111.has_value());
  REQUIRE(!struct2.f112.has_value());
  REQUIRE(struct2.f113.has_value());
  REQUIRE(struct2.f113.value() == 127);
  REQUIRE(!struct2.f114.has_value());
  REQUIRE(!struct2.f115.has_value());
  REQUIRE(struct2.f116.has_value());
  REQUIRE(struct2.f116.value() == 0xFF);
  REQUIRE(!struct2.f117.has_value());
  REQUIRE(!struct2.f118.has_value());
  REQUIRE(struct2.f119.has_value());
  REQUIRE(struct2.f119.value() == 32767);
  REQUIRE(!struct2.f120.has_value());
  REQUIRE(!struct2.f121.has_value());
  REQUIRE(struct2.f122.has_value());
  REQUIRE(struct2.f122.value() == 0xFFFF);
  REQUIRE(!struct2.f123.has_value());
  REQUIRE(!struct2.f124.has_value());
  REQUIRE(struct2.f125.has_value());
  REQUIRE(struct2.f125.value() == 2147483647);
  REQUIRE(!struct2.f126.has_value());
  REQUIRE(!struct2.f127.has_value());
  REQUIRE(struct2.f128.has_value());
  REQUIRE(struct2.f128.value() == 0xFFFFFFFF);
  REQUIRE(!struct2.f129.has_value());
  REQUIRE(!struct2.f130.has_value());
  REQUIRE(struct2.f131.has_value());
  REQUIRE(struct2.f131.value() == 9223372036854775807ll);
  REQUIRE(!struct2.f132.has_value());
  REQUIRE(!struct2.f133.has_value());
  REQUIRE(struct2.f131.has_value());
  REQUIRE(struct2.f134.value() == 0xFFFFFFFFFFFFFFFFull);
  REQUIRE(!struct2.f135.has_value());
  REQUIRE(!struct2.f136.has_value());
  REQUIRE(struct2.f137.has_value());
  REQUIRE(std::abs(struct2.f137.value() - 123.456f) < 0.0001);
  REQUIRE(!struct2.f138.has_value());
  REQUIRE(!struct2.f139.has_value());
  REQUIRE(struct2.f140.has_value());
  REQUIRE(std::abs(struct2.f140.value() - -123.567e+123) < 1e+123);
  REQUIRE(!struct2.f141.has_value());
  REQUIRE(!struct2.f142.has_value());
  REQUIRE(struct2.f143.has_value());
  REQUIRE(struct2.f143.value() == 123456.123456);
  REQUIRE(!struct2.f144.has_value());
  REQUIRE(!struct2.f145.has_value());
  REQUIRE(struct2.f146.has_value());
  REQUIRE(struct2.f146.value() == "Initial string!");
  REQUIRE(!struct2.f147.has_value());
  REQUIRE(!struct2.f148.has_value());
  REQUIRE(struct2.f149.has_value());
  REQUIRE(struct2.f149.value() > 1514764800000000000);
  REQUIRE(!struct2.f150.has_value());
  REQUIRE(!struct2.f151.has_value());
  REQUIRE(struct2.f152.has_value());
  REQUIRE(struct2.f152.value() ==
          FBE::uuid_t("123e4567-e89b-12d3-a456-426655440000"));
  REQUIRE(!struct2.f153.has_value());
  REQUIRE(!struct2.f154.has_value());
  REQUIRE(!struct2.f155.has_value());
  REQUIRE(!struct2.f156.has_value());
  REQUIRE(!struct2.f157.has_value());
  REQUIRE(!struct2.f158.has_value());
  REQUIRE(!struct2.f159.has_value());
  REQUIRE(!struct2.f160.has_value());
  REQUIRE(!struct2.f161.has_value());
  REQUIRE(!struct2.f162.has_value());
  REQUIRE(!struct2.f163.has_value());
  REQUIRE(!struct2.f164.has_value());
  REQUIRE(!struct2.f165.has_value());

  REQUIRE(struct2.f1 == struct1.f1);
  REQUIRE(struct2.f2 == struct1.f2);
  REQUIRE(struct2.f3 == struct1.f3);
  REQUIRE(struct2.f4 == struct1.f4);
  REQUIRE(struct2.f5 == struct1.f5);
  REQUIRE(struct2.f6 == struct1.f6);
  REQUIRE(struct2.f7 == struct1.f7);
  REQUIRE(struct2.f8 == struct1.f8);
  REQUIRE(struct2.f9 == struct1.f9);
  REQUIRE(struct2.f10 == struct1.f10);
  REQUIRE(struct2.f11 == struct1.f11);
  REQUIRE(struct2.f12 == struct1.f12);
  REQUIRE(struct2.f13 == struct1.f13);
  REQUIRE(struct2.f14 == struct1.f14);
  REQUIRE(struct2.f15 == struct1.f15);
  REQUIRE(struct2.f16 == struct1.f16);
  REQUIRE(struct2.f17 == struct1.f17);
  REQUIRE(struct2.f18 == struct1.f18);
  REQUIRE(struct2.f19 == struct1.f19);
  REQUIRE(struct2.f20 == struct1.f20);
  REQUIRE(struct2.f21 == struct1.f21);
  REQUIRE(struct2.f22 == struct1.f22);
  REQUIRE(struct2.f23 == struct1.f23);
  REQUIRE(struct2.f24 == struct1.f24);
  REQUIRE(struct2.f25 == struct1.f25);
  REQUIRE(struct2.f26 == struct1.f26);
  REQUIRE(struct2.f27 == struct1.f27);
  REQUIRE(struct2.f28 == struct1.f28);
  REQUIRE(struct2.f29 == struct1.f29);
  REQUIRE(struct2.f30 == struct1.f30);
  REQUIRE(struct2.f31 == struct1.f31);
  REQUIRE(struct2.f32 == struct1.f32);
  REQUIRE(struct2.f33 == struct1.f33);
  REQUIRE(struct2.f34 == struct1.f34);
  REQUIRE(struct2.f35 == struct1.f35);
  REQUIRE(struct2.f36 == struct1.f36);
  REQUIRE(struct2.f37 == struct1.f37);
  REQUIRE(struct2.f38 == struct1.f38);
  REQUIRE(struct2.f39 == struct1.f39);
  REQUIRE(struct2.f40 == struct1.f40);

  REQUIRE(struct2.f100 == struct1.f100);
  REQUIRE(struct2.f101 == struct1.f101);
  REQUIRE(struct2.f102 == struct1.f102);
  REQUIRE(struct2.f103 == struct1.f103);
  REQUIRE(struct2.f104 == struct1.f104);
  REQUIRE(struct2.f105 == struct1.f105);
  REQUIRE(struct2.f106 == struct1.f106);
  REQUIRE(struct2.f107 == struct1.f107);
  REQUIRE(struct2.f108 == struct1.f108);
  REQUIRE(struct2.f109 == struct1.f109);
  REQUIRE(struct2.f110 == struct1.f110);
  REQUIRE(struct2.f111 == struct1.f111);
  REQUIRE(struct2.f112 == struct1.f112);
  REQUIRE(struct2.f113 == struct1.f113);
  REQUIRE(struct2.f114 == struct1.f114);
  REQUIRE(struct2.f115 == struct1.f115);
  REQUIRE(struct2.f116 == struct1.f116);
  REQUIRE(struct2.f117 == struct1.f117);
  REQUIRE(struct2.f118 == struct1.f118);
  REQUIRE(struct2.f119 == struct1.f119);
  REQUIRE(struct2.f120 == struct1.f120);
  REQUIRE(struct2.f121 == struct1.f121);
  REQUIRE(struct2.f122 == struct1.f122);
  REQUIRE(struct2.f123 == struct1.f123);
  REQUIRE(struct2.f124 == struct1.f124);
  REQUIRE(struct2.f125 == struct1.f125);
  REQUIRE(struct2.f126 == struct1.f126);
  REQUIRE(struct2.f127 == struct1.f127);
  REQUIRE(struct2.f128 == struct1.f128);
  REQUIRE(struct2.f129 == struct1.f129);
  REQUIRE(struct2.f130 == struct1.f130);
  REQUIRE(struct2.f131 == struct1.f131);
  REQUIRE(struct2.f132 == struct1.f132);
  REQUIRE(struct2.f133 == struct1.f133);
  REQUIRE(struct2.f134 == struct1.f134);
  REQUIRE(struct2.f135 == struct1.f135);
  REQUIRE(struct2.f136 == struct1.f136);
  REQUIRE(struct2.f137 == struct1.f137);
  REQUIRE(struct2.f138 == struct1.f138);
  REQUIRE(struct2.f139 == struct1.f139);
  REQUIRE(struct2.f140 == struct1.f140);
  REQUIRE(struct2.f141 == struct1.f141);
  REQUIRE(struct2.f142 == struct1.f142);
  REQUIRE(struct2.f143 == struct1.f143);
  REQUIRE(struct2.f144 == struct1.f144);
  REQUIRE(struct2.f145 == struct1.f145);
  REQUIRE(struct2.f146 == struct1.f146);
  REQUIRE(struct2.f147 == struct1.f147);
  REQUIRE(struct2.f148 == struct1.f148);
  REQUIRE(struct2.f149 == struct1.f149);
  REQUIRE(struct2.f150 == struct1.f150);
  REQUIRE(struct2.f151 == struct1.f151);
  REQUIRE(struct2.f152 == struct1.f152);
  REQUIRE(struct2.f153 == struct1.f153);
  REQUIRE(struct2.f154 == struct1.f154);
  REQUIRE(struct2.f155 == struct1.f155);
  REQUIRE(struct2.f156 == struct1.f156);
  REQUIRE(struct2.f157 == struct1.f157);
  REQUIRE(struct1 == struct2);
}

TEST_CASE("Serialization: struct nested", "[FBE]") {
  // Create a new struct
  test::StructNested struct1;

  // Serialize the struct to the FBE stream
  FBE::test::StructNestedModel writer;
  REQUIRE(writer.model.fbe_type() == 112);
  REQUIRE(writer.model.fbe_offset() == 4);
  size_t serialized = writer.serialize(struct1, nullptr);
  REQUIRE(serialized == writer.buffer().size());
  REQUIRE(writer.verify());
  writer.next(serialized);
  REQUIRE(writer.model.fbe_offset() == (4 + writer.buffer().size()));

  // Check the serialized FBE size
  REQUIRE(writer.buffer().size() == 2099);

  // Deserialize the struct from the FBE stream
  test::StructNested struct2;
  FBE::test::StructNestedModel reader;
  REQUIRE(reader.model.fbe_type() == 112);
  REQUIRE(reader.model.fbe_offset() == 4);
  reader.attach(writer.buffer());
  REQUIRE(reader.verify());
  size_t deserialized = reader.deserialize(struct2, nullptr);
  REQUIRE(deserialized == reader.buffer().size());
  reader.next(deserialized);
  REQUIRE(reader.model.fbe_offset() == (4 + reader.buffer().size()));

  REQUIRE(struct2.f1 == false);
  REQUIRE(struct2.f2 == true);
  REQUIRE(struct2.f3 == 0);
  REQUIRE(struct2.f4 == 0xFF);
  REQUIRE(struct2.f5 == '\0');
  REQUIRE(struct2.f6 == '!');
  REQUIRE(struct2.f7 == 0);
  REQUIRE(struct2.f8 == 0x0444);
  REQUIRE(struct2.f9 == 0);
  REQUIRE(struct2.f10 == 127);
  REQUIRE(struct2.f11 == 0);
  REQUIRE(struct2.f12 == 0xFF);
  REQUIRE(struct2.f13 == 0);
  REQUIRE(struct2.f14 == 32767);
  REQUIRE(struct2.f15 == 0);
  REQUIRE(struct2.f16 == 0xFFFF);
  REQUIRE(struct2.f17 == 0);
  REQUIRE(struct2.f18 == 2147483647);
  REQUIRE(struct2.f19 == 0);
  REQUIRE(struct2.f20 == 0xFFFFFFFF);
  REQUIRE(struct2.f21 == 0);
  REQUIRE(struct2.f22 == 9223372036854775807ll);
  REQUIRE(struct2.f23 == 0);
  REQUIRE(struct2.f24 == 0xFFFFFFFFFFFFFFFFull);
  REQUIRE(struct2.f25 == 0.0f);
  REQUIRE(std::abs(struct2.f26 - 123.456f) < 0.0001);
  REQUIRE(struct2.f27 == 0.0);
  REQUIRE(std::abs(struct2.f28 - -123.567e+123) < 1e+123);
  REQUIRE(struct2.f29 == 0.0);
  REQUIRE(struct2.f30 == 123456.123456);
  REQUIRE(struct2.f31 == "");
  REQUIRE(struct2.f32 == "Initial string!");
  REQUIRE(struct2.f33 == 0);
  REQUIRE(struct2.f34 == 0);
  REQUIRE(struct2.f35 > 1514764800000000000);
  REQUIRE(struct2.f36 == FBE::uuid_t());
  REQUIRE(struct2.f37 != FBE::uuid_t());
  REQUIRE(struct2.f38 == FBE::uuid_t("123e4567-e89b-12d3-a456-426655440000"));

  REQUIRE(!struct2.f100.has_value());
  REQUIRE(struct2.f101.has_value());
  REQUIRE(struct2.f101.value() == true);
  REQUIRE(!struct2.f102.has_value());
  REQUIRE(!struct2.f103.has_value());
  REQUIRE(struct2.f104.has_value());
  REQUIRE(struct2.f104.value() == 0xFF);
  REQUIRE(!struct2.f105.has_value());
  REQUIRE(!struct2.f106.has_value());
  REQUIRE(struct2.f107.has_value());
  REQUIRE(struct2.f107.value() == '!');
  REQUIRE(!struct2.f108.has_value());
  REQUIRE(!struct2.f109.has_value());
  REQUIRE(struct2.f110.has_value());
  REQUIRE(struct2.f110.value() == 0x0444);
  REQUIRE(!struct2.f111.has_value());
  REQUIRE(!struct2.f112.has_value());
  REQUIRE(struct2.f113.has_value());
  REQUIRE(struct2.f113.value() == 127);
  REQUIRE(!struct2.f114.has_value());
  REQUIRE(!struct2.f115.has_value());
  REQUIRE(struct2.f116.has_value());
  REQUIRE(struct2.f116.value() == 0xFF);
  REQUIRE(!struct2.f117.has_value());
  REQUIRE(!struct2.f118.has_value());
  REQUIRE(struct2.f119.has_value());
  REQUIRE(struct2.f119.value() == 32767);
  REQUIRE(!struct2.f120.has_value());
  REQUIRE(!struct2.f121.has_value());
  REQUIRE(struct2.f122.has_value());
  REQUIRE(struct2.f122.value() == 0xFFFF);
  REQUIRE(!struct2.f123.has_value());
  REQUIRE(!struct2.f124.has_value());
  REQUIRE(struct2.f125.has_value());
  REQUIRE(struct2.f125.value() == 2147483647);
  REQUIRE(!struct2.f126.has_value());
  REQUIRE(!struct2.f127.has_value());
  REQUIRE(struct2.f128.has_value());
  REQUIRE(struct2.f128.value() == 0xFFFFFFFF);
  REQUIRE(!struct2.f129.has_value());
  REQUIRE(!struct2.f130.has_value());
  REQUIRE(struct2.f131.has_value());
  REQUIRE(struct2.f131.value() == 9223372036854775807ll);
  REQUIRE(!struct2.f132.has_value());
  REQUIRE(!struct2.f133.has_value());
  REQUIRE(struct2.f131.has_value());
  REQUIRE(struct2.f134.value() == 0xFFFFFFFFFFFFFFFFull);
  REQUIRE(!struct2.f135.has_value());
  REQUIRE(!struct2.f136.has_value());
  REQUIRE(struct2.f137.has_value());
  REQUIRE(std::abs(struct2.f137.value() - 123.456f) < 0.0001);
  REQUIRE(!struct2.f138.has_value());
  REQUIRE(!struct2.f139.has_value());
  REQUIRE(struct2.f140.has_value());
  REQUIRE(std::abs(struct2.f140.value() - -123.567e+123) < 1e+123);
  REQUIRE(!struct2.f141.has_value());
  REQUIRE(!struct2.f142.has_value());
  REQUIRE(struct2.f143.has_value());
  REQUIRE(struct2.f143.value() == 123456.123456);
  REQUIRE(!struct2.f144.has_value());
  REQUIRE(!struct2.f145.has_value());
  REQUIRE(struct2.f146.has_value());
  REQUIRE(struct2.f146.value() == "Initial string!");
  REQUIRE(!struct2.f147.has_value());
  REQUIRE(!struct2.f148.has_value());
  REQUIRE(struct2.f149.has_value());
  REQUIRE(struct2.f149.value() > 1514764800000000000);
  REQUIRE(!struct2.f150.has_value());
  REQUIRE(!struct2.f151.has_value());
  REQUIRE(struct2.f152.has_value());
  REQUIRE(struct2.f152.value() ==
          FBE::uuid_t("123e4567-e89b-12d3-a456-426655440000"));
  REQUIRE(!struct2.f153.has_value());
  REQUIRE(!struct2.f154.has_value());
  REQUIRE(!struct2.f155.has_value());
  REQUIRE(!struct2.f156.has_value());
  REQUIRE(!struct2.f157.has_value());
  REQUIRE(!struct2.f158.has_value());
  REQUIRE(!struct2.f159.has_value());
  REQUIRE(!struct2.f160.has_value());
  REQUIRE(!struct2.f161.has_value());
  REQUIRE(!struct2.f162.has_value());
  REQUIRE(!struct2.f163.has_value());
  REQUIRE(!struct2.f164.has_value());
  REQUIRE(!struct2.f165.has_value());

  REQUIRE(struct2.f1000 == test::EnumSimple::ENUM_VALUE_0);
  REQUIRE(!struct2.f1001.has_value());
  REQUIRE(struct2.f1002 == test::EnumTyped::ENUM_VALUE_2);
  REQUIRE(!struct2.f1003.has_value());
  REQUIRE(struct2.f1004 == test::FlagsSimple::FLAG_VALUE_0);
  REQUIRE(!struct2.f1005.has_value());
  REQUIRE(struct2.f1006 ==
          (test::FlagsTyped::FLAG_VALUE_2 | test::FlagsTyped::FLAG_VALUE_4 |
           test::FlagsTyped::FLAG_VALUE_6));
  REQUIRE(!struct2.f1007.has_value());
  REQUIRE(!struct2.f1009.has_value());
  REQUIRE(!struct2.f1011.has_value());

  REQUIRE(struct2.f1 == struct1.f1);
  REQUIRE(struct2.f2 == struct1.f2);
  REQUIRE(struct2.f3 == struct1.f3);
  REQUIRE(struct2.f4 == struct1.f4);
  REQUIRE(struct2.f5 == struct1.f5);
  REQUIRE(struct2.f6 == struct1.f6);
  REQUIRE(struct2.f7 == struct1.f7);
  REQUIRE(struct2.f8 == struct1.f8);
  REQUIRE(struct2.f9 == struct1.f9);
  REQUIRE(struct2.f10 == struct1.f10);
  REQUIRE(struct2.f11 == struct1.f11);
  REQUIRE(struct2.f12 == struct1.f12);
  REQUIRE(struct2.f13 == struct1.f13);
  REQUIRE(struct2.f14 == struct1.f14);
  REQUIRE(struct2.f15 == struct1.f15);
  REQUIRE(struct2.f16 == struct1.f16);
  REQUIRE(struct2.f17 == struct1.f17);
  REQUIRE(struct2.f18 == struct1.f18);
  REQUIRE(struct2.f19 == struct1.f19);
  REQUIRE(struct2.f20 == struct1.f20);
  REQUIRE(struct2.f21 == struct1.f21);
  REQUIRE(struct2.f22 == struct1.f22);
  REQUIRE(struct2.f23 == struct1.f23);
  REQUIRE(struct2.f24 == struct1.f24);
  REQUIRE(struct2.f25 == struct1.f25);
  REQUIRE(struct2.f26 == struct1.f26);
  REQUIRE(struct2.f27 == struct1.f27);
  REQUIRE(struct2.f28 == struct1.f28);
  REQUIRE(struct2.f29 == struct1.f29);
  REQUIRE(struct2.f30 == struct1.f30);
  REQUIRE(struct2.f31 == struct1.f31);
  REQUIRE(struct2.f32 == struct1.f32);
  REQUIRE(struct2.f33 == struct1.f33);
  REQUIRE(struct2.f34 == struct1.f34);
  REQUIRE(struct2.f35 == struct1.f35);
  REQUIRE(struct2.f36 == struct1.f36);
  REQUIRE(struct2.f37 == struct1.f37);
  REQUIRE(struct2.f38 == struct1.f38);
  REQUIRE(struct2.f39 == struct1.f39);
  REQUIRE(struct2.f40 == struct1.f40);

  REQUIRE(struct2.f100 == struct1.f100);
  REQUIRE(struct2.f101 == struct1.f101);
  REQUIRE(struct2.f102 == struct1.f102);
  REQUIRE(struct2.f103 == struct1.f103);
  REQUIRE(struct2.f104 == struct1.f104);
  REQUIRE(struct2.f105 == struct1.f105);
  REQUIRE(struct2.f106 == struct1.f106);
  REQUIRE(struct2.f107 == struct1.f107);
  REQUIRE(struct2.f108 == struct1.f108);
  REQUIRE(struct2.f109 == struct1.f109);
  REQUIRE(struct2.f110 == struct1.f110);
  REQUIRE(struct2.f111 == struct1.f111);
  REQUIRE(struct2.f112 == struct1.f112);
  REQUIRE(struct2.f113 == struct1.f113);
  REQUIRE(struct2.f114 == struct1.f114);
  REQUIRE(struct2.f115 == struct1.f115);
  REQUIRE(struct2.f116 == struct1.f116);
  REQUIRE(struct2.f117 == struct1.f117);
  REQUIRE(struct2.f118 == struct1.f118);
  REQUIRE(struct2.f119 == struct1.f119);
  REQUIRE(struct2.f120 == struct1.f120);
  REQUIRE(struct2.f121 == struct1.f121);
  REQUIRE(struct2.f122 == struct1.f122);
  REQUIRE(struct2.f123 == struct1.f123);
  REQUIRE(struct2.f124 == struct1.f124);
  REQUIRE(struct2.f125 == struct1.f125);
  REQUIRE(struct2.f126 == struct1.f126);
  REQUIRE(struct2.f127 == struct1.f127);
  REQUIRE(struct2.f128 == struct1.f128);
  REQUIRE(struct2.f129 == struct1.f129);
  REQUIRE(struct2.f130 == struct1.f130);
  REQUIRE(struct2.f131 == struct1.f131);
  REQUIRE(struct2.f132 == struct1.f132);
  REQUIRE(struct2.f133 == struct1.f133);
  REQUIRE(struct2.f134 == struct1.f134);
  REQUIRE(struct2.f135 == struct1.f135);
  REQUIRE(struct2.f136 == struct1.f136);
  REQUIRE(struct2.f137 == struct1.f137);
  REQUIRE(struct2.f138 == struct1.f138);
  REQUIRE(struct2.f139 == struct1.f139);
  REQUIRE(struct2.f140 == struct1.f140);
  REQUIRE(struct2.f141 == struct1.f141);
  REQUIRE(struct2.f142 == struct1.f142);
  REQUIRE(struct2.f143 == struct1.f143);
  REQUIRE(struct2.f144 == struct1.f144);
  REQUIRE(struct2.f145 == struct1.f145);
  REQUIRE(struct2.f146 == struct1.f146);
  REQUIRE(struct2.f147 == struct1.f147);
  REQUIRE(struct2.f148 == struct1.f148);
  REQUIRE(struct2.f149 == struct1.f149);
  REQUIRE(struct2.f150 == struct1.f150);
  REQUIRE(struct2.f151 == struct1.f151);
  REQUIRE(struct2.f152 == struct1.f152);
  REQUIRE(struct2.f153 == struct1.f153);
  REQUIRE(struct2.f154 == struct1.f154);
  REQUIRE(struct2.f155 == struct1.f155);
  REQUIRE(struct2.f156 == struct1.f156);
  REQUIRE(struct2.f157 == struct1.f157);

  REQUIRE(struct2.f1000 == struct1.f1000);
  REQUIRE(struct2.f1001 == struct1.f1001);
  REQUIRE(struct2.f1002 == struct1.f1002);
  REQUIRE(struct2.f1003 == struct1.f1003);
  REQUIRE(struct2.f1004 == struct1.f1004);
  REQUIRE(struct2.f1005 == struct1.f1005);
  REQUIRE(struct2.f1006 == struct1.f1006);
  REQUIRE(struct2.f1007 == struct1.f1007);
  REQUIRE(struct1 == struct2);
}

TEST_CASE("Serialization: struct bytes", "[FBE]") {
  const char *f1 = "ABC";
  const char *f2 = "test";

  // Create a new struct
  test::StructBytes struct1;
  struct1.f1.assign((const uint8_t *)f1, (const uint8_t *)f1 + strlen(f1));
  struct1.f2.emplace();
  struct1.f2.value().assign((const uint8_t *)f2,
                            (const uint8_t *)f2 + strlen(f2));

  // Serialize the struct to the FBE stream
  FBE::test::StructBytesModel writer;
  REQUIRE(writer.model.fbe_type() == 120);
  REQUIRE(writer.model.fbe_offset() == 4);
  size_t serialized = writer.serialize(struct1, nullptr);
  REQUIRE(serialized == writer.buffer().size());
  REQUIRE(writer.verify());
  writer.next(serialized);
  REQUIRE(writer.model.fbe_offset() == (4 + writer.buffer().size()));

  // Check the serialized FBE size
  REQUIRE(writer.buffer().size() == 49);

  // Deserialize the struct from the FBE stream
  test::StructBytes struct2;
  FBE::test::StructBytesModel reader;
  REQUIRE(reader.model.fbe_type() == 120);
  REQUIRE(reader.model.fbe_offset() == 4);
  reader.attach(writer.buffer());
  REQUIRE(reader.verify());
  size_t deserialized = reader.deserialize(struct2, nullptr);
  REQUIRE(deserialized == reader.buffer().size());
  reader.next(deserialized);
  REQUIRE(reader.model.fbe_offset() == (4 + reader.buffer().size()));

  REQUIRE(struct2.f1.size() == 3);
  REQUIRE(memcmp(f1, struct2.f1.data(), struct2.f1.size()) == 0);
  REQUIRE(struct2.f2.has_value());
  REQUIRE(struct2.f2.value().size() == 4);
  REQUIRE(memcmp(f2, struct2.f2.value().data(), struct2.f2.value().size()) ==
          0);
  REQUIRE(!struct2.f3.has_value());
  REQUIRE(struct1 == struct2);
}

TEST_CASE("Serialization: struct array", "[FBE]") {
  // Create a new struct
  test::StructArray struct1;
  struct1.f1[0] = (uint8_t)48;
  struct1.f1[1] = (uint8_t)65;
  struct1.f2[0] = (uint8_t)97;
  struct1.f2[1] = std::nullopt;
  struct1.f3[0] = FastVec<uint8_t>(3, 48);
  struct1.f3[1] = FastVec<uint8_t>(3, 65);
  struct1.f4[0] = FastVec<uint8_t>(3, 97);
  struct1.f4[1] = std::nullopt;
  struct1.f5[0] = test::EnumSimple::ENUM_VALUE_1;
  struct1.f5[1] = test::EnumSimple::ENUM_VALUE_2;
  struct1.f6[0] = test::EnumSimple::ENUM_VALUE_1;
  struct1.f6[1] = std::nullopt;
  struct1.f7[0] =
      test::FlagsSimple::FLAG_VALUE_1 | test::FlagsSimple::FLAG_VALUE_2;
  struct1.f7[1] = test::FlagsSimple::FLAG_VALUE_1 |
                  test::FlagsSimple::FLAG_VALUE_2 |
                  test::FlagsSimple::FLAG_VALUE_3;
  struct1.f8[0] =
      test::FlagsSimple::FLAG_VALUE_1 | test::FlagsSimple::FLAG_VALUE_2;
  struct1.f8[1] = std::nullopt;
  struct1.f9[0] = test::StructSimple();
  struct1.f9[1] = test::StructSimple();
  struct1.f10[0] = test::StructSimple();
  struct1.f10[1] = std::nullopt;

  // Serialize the struct to the FBE stream
  FBE::test::StructArrayModel writer;
  REQUIRE(writer.model.fbe_type() == 125);
  REQUIRE(writer.model.fbe_offset() == 4);
  size_t serialized = writer.serialize(struct1, nullptr);
  REQUIRE(serialized == writer.buffer().size());
  REQUIRE(writer.verify());
  writer.next(serialized);
  REQUIRE(writer.model.fbe_offset() == (4 + writer.buffer().size()));

  // Check the serialized FBE size
  REQUIRE(writer.buffer().size() == 1290);

  // Deserialize the struct from the FBE stream
  test::StructArray struct2;
  FBE::test::StructArrayModel reader;
  REQUIRE(reader.model.fbe_type() == 125);
  REQUIRE(reader.model.fbe_offset() == 4);
  reader.attach(writer.buffer());
  REQUIRE(reader.verify());
  size_t deserialized = reader.deserialize(struct2, nullptr);
  REQUIRE(deserialized == reader.buffer().size());
  reader.next(deserialized);
  REQUIRE(reader.model.fbe_offset() == (4 + reader.buffer().size()));

  REQUIRE(struct2.f1.size() == 2);
  REQUIRE(struct2.f1[0] == 48);
  REQUIRE(struct2.f1[1] == 65);
  REQUIRE(struct2.f2.size() == 2);
  REQUIRE(struct2.f2[0].value() == 97);
  REQUIRE(struct2.f2[1] == std::nullopt);
  REQUIRE(struct2.f3.size() == 2);
  REQUIRE(struct2.f3[0].size() == 3);
  REQUIRE(struct2.f3[0][0] == 48);
  REQUIRE(struct2.f3[0][1] == 48);
  REQUIRE(struct2.f3[0][2] == 48);
  REQUIRE(struct2.f3[1].size() == 3);
  REQUIRE(struct2.f3[1][0] == 65);
  REQUIRE(struct2.f3[1][1] == 65);
  REQUIRE(struct2.f3[1][2] == 65);
  REQUIRE(struct2.f4.size() == 2);
  REQUIRE(struct2.f4[0].has_value());
  REQUIRE(struct2.f4[0].value().size() == 3);
  REQUIRE(struct2.f4[0].value()[0] == 97);
  REQUIRE(struct2.f4[0].value()[1] == 97);
  REQUIRE(struct2.f4[0].value()[2] == 97);
  REQUIRE(struct2.f4[1] == std::nullopt);
  REQUIRE(struct2.f5.size() == 2);
  REQUIRE(struct2.f5[0] == test::EnumSimple::ENUM_VALUE_1);
  REQUIRE(struct2.f5[1] == test::EnumSimple::ENUM_VALUE_2);
  REQUIRE(struct2.f6.size() == 2);
  REQUIRE(struct2.f6[0].value() == test::EnumSimple::ENUM_VALUE_1);
  REQUIRE(struct2.f6[1] == std::nullopt);
  REQUIRE(struct2.f7.size() == 2);
  REQUIRE(struct2.f7[0] ==
          (test::FlagsSimple::FLAG_VALUE_1 | test::FlagsSimple::FLAG_VALUE_2));
  REQUIRE(struct2.f7[1] ==
          (test::FlagsSimple::FLAG_VALUE_1 | test::FlagsSimple::FLAG_VALUE_2 |
           test::FlagsSimple::FLAG_VALUE_3));
  REQUIRE(struct2.f8.size() == 2);
  REQUIRE(struct2.f8[0].value() ==
          (test::FlagsSimple::FLAG_VALUE_1 | test::FlagsSimple::FLAG_VALUE_2));
  REQUIRE(struct2.f8[1] == std::nullopt);
  REQUIRE(struct2.f9.size() == 2);
  REQUIRE(struct2.f9[0].f2 == true);
  REQUIRE(struct2.f9[0].f12 == 255);
  REQUIRE(struct2.f9[0].f32 == "Initial string!");
  REQUIRE(struct2.f9[1].f2 == true);
  REQUIRE(struct2.f9[1].f12 == 255);
  REQUIRE(struct2.f9[1].f32 == "Initial string!");
  REQUIRE(struct2.f10.size() == 2);
  REQUIRE(struct2.f10[0].has_value());
  REQUIRE(struct2.f10[0].value().f2 == true);
  REQUIRE(struct2.f10[0].value().f12 == 255);
  REQUIRE(struct2.f10[0].value().f32 == "Initial string!");
  REQUIRE(struct2.f10[1] == std::nullopt);
  REQUIRE(struct1 == struct2);
}

TEST_CASE("Serialization: struct vector", "[FBE]") {
  // Create a new struct
  test::StructVector struct1;
  struct1.f1.emplace_back((uint8_t)48);
  struct1.f1.emplace_back((uint8_t)65);
  struct1.f2.emplace_back((uint8_t)97);
  struct1.f2.emplace_back(std::nullopt);
  struct1.f3.emplace_back(FastVec<uint8_t>(3, 48));
  struct1.f3.emplace_back(FastVec<uint8_t>(3, 65));
  struct1.f4.emplace_back(FastVec<uint8_t>(3, 97));
  struct1.f4.emplace_back(std::nullopt);
  struct1.f5.emplace_back(test::EnumSimple::ENUM_VALUE_1);
  struct1.f5.emplace_back(test::EnumSimple::ENUM_VALUE_2);
  struct1.f6.emplace_back(test::EnumSimple::ENUM_VALUE_1);
  struct1.f6.emplace_back(std::nullopt);
  struct1.f7.emplace_back(test::FlagsSimple::FLAG_VALUE_1 |
                          test::FlagsSimple::FLAG_VALUE_2);
  struct1.f7.emplace_back(test::FlagsSimple::FLAG_VALUE_1 |
                          test::FlagsSimple::FLAG_VALUE_2 |
                          test::FlagsSimple::FLAG_VALUE_3);
  struct1.f8.emplace_back(test::FlagsSimple::FLAG_VALUE_1 |
                          test::FlagsSimple::FLAG_VALUE_2);
  struct1.f8.emplace_back(std::nullopt);
  struct1.f9.emplace_back(test::StructSimple());
  struct1.f9.emplace_back(test::StructSimple());
  struct1.f10.emplace_back(test::StructSimple());
  struct1.f10.emplace_back(std::nullopt);

  // Serialize the struct to the FBE stream
  FBE::test::StructVectorModel writer;
  REQUIRE(writer.model.fbe_type() == 130);
  REQUIRE(writer.model.fbe_offset() == 4);
  size_t serialized = writer.serialize(struct1, nullptr);
  REQUIRE(serialized == writer.buffer().size());
  REQUIRE(writer.verify());
  writer.next(serialized);
  REQUIRE(writer.model.fbe_offset() == (4 + writer.buffer().size()));

  // Check the serialized FBE size
  REQUIRE(writer.buffer().size() == 1370);

  // Deserialize the struct from the FBE stream
  test::StructVector struct2;
  FBE::test::StructVectorModel reader;
  REQUIRE(reader.model.fbe_type() == 130);
  REQUIRE(reader.model.fbe_offset() == 4);
  reader.attach(writer.buffer());
  REQUIRE(reader.verify());
  size_t deserialized = reader.deserialize(struct2, nullptr);
  REQUIRE(deserialized == reader.buffer().size());
  reader.next(deserialized);
  REQUIRE(reader.model.fbe_offset() == (4 + reader.buffer().size()));

  REQUIRE(struct2.f1.size() == 2);
  REQUIRE(struct2.f1[0] == 48);
  REQUIRE(struct2.f1[1] == 65);
  REQUIRE(struct2.f2.size() == 2);
  REQUIRE(struct2.f2[0].value() == 97);
  REQUIRE(struct2.f2[1] == std::nullopt);
  REQUIRE(struct2.f3.size() == 2);
  REQUIRE(struct2.f3[0].size() == 3);
  REQUIRE(struct2.f3[0][0] == 48);
  REQUIRE(struct2.f3[0][1] == 48);
  REQUIRE(struct2.f3[0][2] == 48);
  REQUIRE(struct2.f3[1].size() == 3);
  REQUIRE(struct2.f3[1][0] == 65);
  REQUIRE(struct2.f3[1][1] == 65);
  REQUIRE(struct2.f3[1][2] == 65);
  REQUIRE(struct2.f4.size() == 2);
  REQUIRE(struct2.f4[0].has_value());
  REQUIRE(struct2.f4[0].value().size() == 3);
  REQUIRE(struct2.f4[0].value()[0] == 97);
  REQUIRE(struct2.f4[0].value()[1] == 97);
  REQUIRE(struct2.f4[0].value()[2] == 97);
  REQUIRE(struct2.f4[1] == std::nullopt);
  REQUIRE(struct2.f5.size() == 2);
  REQUIRE(struct2.f5[0] == test::EnumSimple::ENUM_VALUE_1);
  REQUIRE(struct2.f5[1] == test::EnumSimple::ENUM_VALUE_2);
  REQUIRE(struct2.f6.size() == 2);
  REQUIRE(struct2.f6[0].value() == test::EnumSimple::ENUM_VALUE_1);
  REQUIRE(struct2.f6[1] == std::nullopt);
  REQUIRE(struct2.f7.size() == 2);
  REQUIRE(struct2.f7[0] ==
          (test::FlagsSimple::FLAG_VALUE_1 | test::FlagsSimple::FLAG_VALUE_2));
  REQUIRE(struct2.f7[1] ==
          (test::FlagsSimple::FLAG_VALUE_1 | test::FlagsSimple::FLAG_VALUE_2 |
           test::FlagsSimple::FLAG_VALUE_3));
  REQUIRE(struct2.f8.size() == 2);
  REQUIRE(struct2.f8[0].value() ==
          (test::FlagsSimple::FLAG_VALUE_1 | test::FlagsSimple::FLAG_VALUE_2));
  REQUIRE(struct2.f8[1] == std::nullopt);
  REQUIRE(struct2.f9.size() == 2);
  REQUIRE(struct2.f9[0].f2 == true);
  REQUIRE(struct2.f9[0].f12 == 255);
  REQUIRE(struct2.f9[0].f32 == "Initial string!");
  REQUIRE(struct2.f9[1].f2 == true);
  REQUIRE(struct2.f9[1].f12 == 255);
  REQUIRE(struct2.f9[1].f32 == "Initial string!");
  REQUIRE(struct2.f10.size() == 2);
  REQUIRE(struct2.f10[0].has_value());
  REQUIRE(struct2.f10[0].value().f2 == true);
  REQUIRE(struct2.f10[0].value().f12 == 255);
  REQUIRE(struct2.f10[0].value().f32 == "Initial string!");
  REQUIRE(struct2.f10[1] == std::nullopt);
  REQUIRE(struct1 == struct2);
}

TEST_CASE("Serialization: struct list", "[FBE]") {
  // Create a new struct
  test::StructList struct1;
  struct1.f1.emplace_back((uint8_t)48);
  struct1.f1.emplace_back((uint8_t)65);
  struct1.f2.emplace_back((uint8_t)97);
  struct1.f2.emplace_back(std::nullopt);
  struct1.f3.emplace_back(FastVec<uint8_t>(3, 48));
  struct1.f3.emplace_back(FastVec<uint8_t>(3, 65));
  struct1.f4.emplace_back(FastVec<uint8_t>(3, 97));
  struct1.f4.emplace_back(std::nullopt);
  struct1.f5.emplace_back(test::EnumSimple::ENUM_VALUE_1);
  struct1.f5.emplace_back(test::EnumSimple::ENUM_VALUE_2);
  struct1.f6.emplace_back(test::EnumSimple::ENUM_VALUE_1);
  struct1.f6.emplace_back(std::nullopt);
  struct1.f7.emplace_back(test::FlagsSimple::FLAG_VALUE_1 |
                          test::FlagsSimple::FLAG_VALUE_2);
  struct1.f7.emplace_back(test::FlagsSimple::FLAG_VALUE_1 |
                          test::FlagsSimple::FLAG_VALUE_2 |
                          test::FlagsSimple::FLAG_VALUE_3);
  struct1.f8.emplace_back(test::FlagsSimple::FLAG_VALUE_1 |
                          test::FlagsSimple::FLAG_VALUE_2);
  struct1.f8.emplace_back(std::nullopt);
  struct1.f9.emplace_back(test::StructSimple());
  struct1.f9.emplace_back(test::StructSimple());
  struct1.f10.emplace_back(test::StructSimple());
  struct1.f10.emplace_back(std::nullopt);

  // Serialize the struct to the FBE stream
  FBE::test::StructListModel writer;
  REQUIRE(writer.model.fbe_type() == 131);
  REQUIRE(writer.model.fbe_offset() == 4);
  size_t serialized = writer.serialize(struct1, nullptr);
  REQUIRE(serialized == writer.buffer().size());
  REQUIRE(writer.verify());
  writer.next(serialized);
  REQUIRE(writer.model.fbe_offset() == (4 + writer.buffer().size()));

  // Check the serialized FBE size
  REQUIRE(writer.buffer().size() == 1370);

  // Deserialize the struct from the FBE stream
  test::StructList struct2;
  FBE::test::StructListModel reader;
  REQUIRE(reader.model.fbe_type() == 131);
  REQUIRE(reader.model.fbe_offset() == 4);
  reader.attach(writer.buffer());
  REQUIRE(reader.verify());
  size_t deserialized = reader.deserialize(struct2, nullptr);
  REQUIRE(deserialized == reader.buffer().size());
  reader.next(deserialized);
  REQUIRE(reader.model.fbe_offset() == (4 + reader.buffer().size()));

  REQUIRE(struct2.f1.size() == 2);
  REQUIRE(struct2.f1.front() == 48);
  REQUIRE(struct2.f1.back() == 65);
  REQUIRE(struct2.f2.size() == 2);
  REQUIRE(struct2.f2.front().value() == 97);
  REQUIRE(struct2.f2.back() == std::nullopt);
  REQUIRE(struct2.f3.size() == 2);
  REQUIRE(struct2.f3.front().size() == 3);
  REQUIRE(struct2.f3.front()[0] == 48);
  REQUIRE(struct2.f3.front()[1] == 48);
  REQUIRE(struct2.f3.front()[2] == 48);
  REQUIRE(struct2.f3.back().size() == 3);
  REQUIRE(struct2.f3.back()[0] == 65);
  REQUIRE(struct2.f3.back()[1] == 65);
  REQUIRE(struct2.f3.back()[2] == 65);
  REQUIRE(struct2.f4.size() == 2);
  REQUIRE(struct2.f4.front().has_value());
  REQUIRE(struct2.f4.front().value().size() == 3);
  REQUIRE(struct2.f4.front().value()[0] == 97);
  REQUIRE(struct2.f4.front().value()[1] == 97);
  REQUIRE(struct2.f4.front().value()[2] == 97);
  REQUIRE(struct2.f4.back() == std::nullopt);
  REQUIRE(struct2.f5.size() == 2);
  REQUIRE(struct2.f5.front() == test::EnumSimple::ENUM_VALUE_1);
  REQUIRE(struct2.f5.back() == test::EnumSimple::ENUM_VALUE_2);
  REQUIRE(struct2.f6.size() == 2);
  REQUIRE(struct2.f6.front().value() == test::EnumSimple::ENUM_VALUE_1);
  REQUIRE(struct2.f6.back() == std::nullopt);
  REQUIRE(struct2.f7.size() == 2);
  REQUIRE(struct2.f7.front() ==
          (test::FlagsSimple::FLAG_VALUE_1 | test::FlagsSimple::FLAG_VALUE_2));
  REQUIRE(struct2.f7.back() ==
          (test::FlagsSimple::FLAG_VALUE_1 | test::FlagsSimple::FLAG_VALUE_2 |
           test::FlagsSimple::FLAG_VALUE_3));
  REQUIRE(struct2.f8.size() == 2);
  REQUIRE(struct2.f8.front().value() ==
          (test::FlagsSimple::FLAG_VALUE_1 | test::FlagsSimple::FLAG_VALUE_2));
  REQUIRE(struct2.f8.back() == std::nullopt);
  REQUIRE(struct2.f9.size() == 2);
  REQUIRE(struct2.f9.front().f2 == true);
  REQUIRE(struct2.f9.front().f12 == 255);
  REQUIRE(struct2.f9.front().f32 == "Initial string!");
  REQUIRE(struct2.f9.back().f2 == true);
  REQUIRE(struct2.f9.back().f12 == 255);
  REQUIRE(struct2.f9.back().f32 == "Initial string!");
  REQUIRE(struct2.f10.size() == 2);
  REQUIRE(struct2.f10.front().has_value());
  REQUIRE(struct2.f10.front().value().f2 == true);
  REQUIRE(struct2.f10.front().value().f12 == 255);
  REQUIRE(struct2.f10.front().value().f32 == "Initial string!");
  REQUIRE(struct2.f10.back() == std::nullopt);
  REQUIRE(struct1 == struct2);
}

TEST_CASE("Serialization: struct set", "[FBE]") {
  // Create a new struct
  test::StructSet struct1;
  struct1.f1.emplace((uint8_t)48);
  struct1.f1.emplace((uint8_t)65);
  struct1.f1.emplace((uint8_t)97);
  struct1.f2.emplace(test::EnumSimple::ENUM_VALUE_1);
  struct1.f2.emplace(test::EnumSimple::ENUM_VALUE_2);
  struct1.f3.emplace(test::FlagsSimple::FLAG_VALUE_1 |
                     test::FlagsSimple::FLAG_VALUE_2);
  struct1.f3.emplace(test::FlagsSimple::FLAG_VALUE_1 |
                     test::FlagsSimple::FLAG_VALUE_2 |
                     test::FlagsSimple::FLAG_VALUE_3);
  test::StructSimple s1;
  s1.id = 48;
  struct1.f4.emplace(s1);
  test::StructSimple s2;
  s2.id = 65;
  struct1.f4.emplace(s2);

  // Serialize the struct to the FBE stream
  FBE::test::StructSetModel writer;
  REQUIRE(writer.model.fbe_type() == 132);
  REQUIRE(writer.model.fbe_offset() == 4);
  size_t serialized = writer.serialize(struct1, nullptr);
  REQUIRE(serialized == writer.buffer().size());
  REQUIRE(writer.verify());
  writer.next(serialized);
  REQUIRE(writer.model.fbe_offset() == (4 + writer.buffer().size()));

  // Check the serialized FBE size
  REQUIRE(writer.buffer().size() == 843);

  // Deserialize the struct from the FBE stream
  test::StructSet struct2;
  FBE::test::StructSetModel reader;
  REQUIRE(reader.model.fbe_type() == 132);
  REQUIRE(reader.model.fbe_offset() == 4);
  reader.attach(writer.buffer());
  REQUIRE(reader.verify());
  size_t deserialized = reader.deserialize(struct2, nullptr);
  REQUIRE(deserialized == reader.buffer().size());
  reader.next(deserialized);
  REQUIRE(reader.model.fbe_offset() == (4 + reader.buffer().size()));

  REQUIRE(struct2.f1.size() == 3);
  REQUIRE(struct2.f1.find((uint8_t)48) != struct2.f1.end());
  REQUIRE(struct2.f1.find((uint8_t)65) != struct2.f1.end());
  REQUIRE(struct2.f1.find((uint8_t)97) != struct2.f1.end());
  REQUIRE(struct2.f2.size() == 2);
  REQUIRE(struct2.f2.find(test::EnumSimple::ENUM_VALUE_1) != struct2.f2.end());
  REQUIRE(struct2.f2.find(test::EnumSimple::ENUM_VALUE_2) != struct2.f2.end());
  REQUIRE(struct2.f3.size() == 2);
  REQUIRE(struct2.f3.find(test::FlagsSimple::FLAG_VALUE_1 |
                          test::FlagsSimple::FLAG_VALUE_2) != struct2.f3.end());
  REQUIRE(struct2.f3.find(test::FlagsSimple::FLAG_VALUE_1 |
                          test::FlagsSimple::FLAG_VALUE_2 |
                          test::FlagsSimple::FLAG_VALUE_3) != struct2.f3.end());
  REQUIRE(struct2.f4.size() == 2);
  REQUIRE(struct2.f4.find(s1) != struct2.f4.end());
  REQUIRE(struct2.f4.find(s2) != struct2.f4.end());
  REQUIRE(struct1 == struct2);
}

TEST_CASE("Serialization: struct map", "[FBE]") {
  // Create a new struct
  test::StructMap struct1;
  struct1.f1.emplace(10, (uint8_t)48);
  struct1.f1.emplace(20, (uint8_t)65);
  struct1.f2.emplace(10, (uint8_t)97);
  struct1.f2.emplace(20, std::nullopt);
  struct1.f3.emplace(10, FastVec<uint8_t>(3, 48));
  struct1.f3.emplace(20, FastVec<uint8_t>(3, 65));
  struct1.f4.emplace(10, FastVec<uint8_t>(3, 97));
  struct1.f4.emplace(20, std::nullopt);
  struct1.f5.emplace(10, test::EnumSimple::ENUM_VALUE_1);
  struct1.f5.emplace(20, test::EnumSimple::ENUM_VALUE_2);
  struct1.f6.emplace(10, test::EnumSimple::ENUM_VALUE_1);
  struct1.f6.emplace(20, std::nullopt);
  struct1.f7.emplace(10, test::FlagsSimple::FLAG_VALUE_1 |
                             test::FlagsSimple::FLAG_VALUE_2);
  struct1.f7.emplace(20, test::FlagsSimple::FLAG_VALUE_1 |
                             test::FlagsSimple::FLAG_VALUE_2 |
                             test::FlagsSimple::FLAG_VALUE_3);
  struct1.f8.emplace(10, test::FlagsSimple::FLAG_VALUE_1 |
                             test::FlagsSimple::FLAG_VALUE_2);
  struct1.f8.emplace(20, std::nullopt);
  test::StructSimple s1;
  s1.id = 48;
  struct1.f9.emplace(10, s1);
  test::StructSimple s2;
  s2.id = 65;
  struct1.f9.emplace(20, s2);
  struct1.f10.emplace(10, s1);
  struct1.f10.emplace(20, std::nullopt);

  // Serialize the struct to the FBE stream
  FBE::test::StructMapModel writer;
  REQUIRE(writer.model.fbe_type() == 140);
  REQUIRE(writer.model.fbe_offset() == 4);
  size_t serialized = writer.serialize(struct1, nullptr);
  REQUIRE(serialized == writer.buffer().size());
  REQUIRE(writer.verify());
  writer.next(serialized);
  REQUIRE(writer.model.fbe_offset() == (4 + writer.buffer().size()));

  // Check the serialized FBE size
  REQUIRE(writer.buffer().size() == 1450);

  // Deserialize the struct from the FBE stream
  test::StructMap struct2;
  FBE::test::StructMapModel reader;
  REQUIRE(reader.model.fbe_type() == 140);
  REQUIRE(reader.model.fbe_offset() == 4);
  reader.attach(writer.buffer());
  REQUIRE(reader.verify());
  size_t deserialized = reader.deserialize(struct2, nullptr);
  REQUIRE(deserialized == reader.buffer().size());
  reader.next(deserialized);
  REQUIRE(reader.model.fbe_offset() == (4 + reader.buffer().size()));

  REQUIRE(struct2.f1.size() == 2);
  REQUIRE(struct2.f1.find(10)->second == 48);
  REQUIRE(struct2.f1.find(20)->second == 65);
  REQUIRE(struct2.f2.size() == 2);
  REQUIRE(struct2.f2.find(10)->second.value() == 97);
  REQUIRE(struct2.f2.find(20)->second == std::nullopt);
  REQUIRE(struct2.f3.size() == 2);
  REQUIRE(struct2.f3.find(10)->second.size() == 3);
  REQUIRE(struct2.f3.find(20)->second.size() == 3);
  REQUIRE(struct2.f4.size() == 2);
  REQUIRE(struct2.f4.find(10)->second.value().size() == 3);
  REQUIRE(struct2.f4.find(20)->second == std::nullopt);
  REQUIRE(struct2.f5.size() == 2);
  REQUIRE(struct2.f5.find(10)->second == test::EnumSimple::ENUM_VALUE_1);
  REQUIRE(struct2.f5.find(20)->second == test::EnumSimple::ENUM_VALUE_2);
  REQUIRE(struct2.f6.size() == 2);
  REQUIRE(struct2.f6.find(10)->second.value() ==
          test::EnumSimple::ENUM_VALUE_1);
  REQUIRE(struct2.f6.find(20)->second == std::nullopt);
  REQUIRE(struct2.f7.size() == 2);
  REQUIRE(struct2.f7.find(10)->second ==
          (test::FlagsSimple::FLAG_VALUE_1 | test::FlagsSimple::FLAG_VALUE_2));
  REQUIRE(struct2.f7.find(20)->second ==
          (test::FlagsSimple::FLAG_VALUE_1 | test::FlagsSimple::FLAG_VALUE_2 |
           test::FlagsSimple::FLAG_VALUE_3));
  REQUIRE(struct2.f8.size() == 2);
  REQUIRE(struct2.f8.find(10)->second.value() ==
          (test::FlagsSimple::FLAG_VALUE_1 | test::FlagsSimple::FLAG_VALUE_2));
  REQUIRE(struct2.f8.find(20)->second == std::nullopt);
  REQUIRE(struct2.f9.size() == 2);
  REQUIRE(struct2.f9.find(10)->second.id == 48);
  REQUIRE(struct2.f9.find(20)->second.id == 65);
  REQUIRE(struct2.f10.size() == 2);
  REQUIRE(struct2.f10.find(10)->second.value().id == 48);
  REQUIRE(struct2.f10.find(20)->second == std::nullopt);
  REQUIRE(struct1 == struct2);
}

TEST_CASE("Serialization: struct hash", "[FBE]") {
  // Create a new struct
  test::StructHash struct1;
  struct1.f1.emplace("10", (uint8_t)48);
  struct1.f1.emplace("20", (uint8_t)65);
  struct1.f2.emplace("10", (uint8_t)97);
  struct1.f2.emplace("20", std::nullopt);
  struct1.f3.emplace("10", FastVec<uint8_t>(3, 48));
  struct1.f3.emplace("20", FastVec<uint8_t>(3, 65));
  struct1.f4.emplace("10", FastVec<uint8_t>(3, 97));
  struct1.f4.emplace("20", std::nullopt);
  struct1.f5.emplace("10", test::EnumSimple::ENUM_VALUE_1);
  struct1.f5.emplace("20", test::EnumSimple::ENUM_VALUE_2);
  struct1.f6.emplace("10", test::EnumSimple::ENUM_VALUE_1);
  struct1.f6.emplace("20", std::nullopt);
  struct1.f7.emplace("10", test::FlagsSimple::FLAG_VALUE_1 |
                               test::FlagsSimple::FLAG_VALUE_2);
  struct1.f7.emplace("20", test::FlagsSimple::FLAG_VALUE_1 |
                               test::FlagsSimple::FLAG_VALUE_2 |
                               test::FlagsSimple::FLAG_VALUE_3);
  struct1.f8.emplace("10", test::FlagsSimple::FLAG_VALUE_1 |
                               test::FlagsSimple::FLAG_VALUE_2);
  struct1.f8.emplace("20", std::nullopt);
  test::StructSimple s1;
  s1.id = 48;
  struct1.f9.emplace("10", s1);
  test::StructSimple s2;
  s2.id = 65;
  struct1.f9.emplace("20", s2);
  struct1.f10.emplace("10", s1);
  struct1.f10.emplace("20", std::nullopt);

  // Serialize the struct to the FBE stream
  FBE::test::StructHashModel writer;
  REQUIRE(writer.model.fbe_type() == 141);
  REQUIRE(writer.model.fbe_offset() == 4);
  size_t serialized = writer.serialize(struct1, nullptr);
  REQUIRE(serialized == writer.buffer().size());
  REQUIRE(writer.verify());
  writer.next(serialized);
  REQUIRE(writer.model.fbe_offset() == (4 + writer.buffer().size()));

  // Check the serialized FBE size
  REQUIRE(writer.buffer().size() == 1570);

  // Deserialize the struct from the FBE stream
  test::StructHash struct2;
  FBE::test::StructHashModel reader;
  REQUIRE(reader.model.fbe_type() == 141);
  REQUIRE(reader.model.fbe_offset() == 4);
  reader.attach(writer.buffer());
  REQUIRE(reader.verify());
  size_t deserialized = reader.deserialize(struct2, nullptr);
  REQUIRE(deserialized == reader.buffer().size());
  reader.next(deserialized);
  REQUIRE(reader.model.fbe_offset() == (4 + reader.buffer().size()));

  REQUIRE(struct2.f1.size() == 2);
  REQUIRE(struct2.f1.find("10")->second == 48);
  REQUIRE(struct2.f1.find("20")->second == 65);
  REQUIRE(struct2.f2.size() == 2);
  REQUIRE(struct2.f2.find("10")->second.value() == 97);
  REQUIRE(struct2.f2.find("20")->second == std::nullopt);
  REQUIRE(struct2.f3.size() == 2);
  REQUIRE(struct2.f3.find("10")->second.size() == 3);
  REQUIRE(struct2.f3.find("20")->second.size() == 3);
  REQUIRE(struct2.f4.size() == 2);
  REQUIRE(struct2.f4.find("10")->second.value().size() == 3);
  REQUIRE(struct2.f4.find("20")->second == std::nullopt);
  REQUIRE(struct2.f5.size() == 2);
  REQUIRE(struct2.f5.find("10")->second == test::EnumSimple::ENUM_VALUE_1);
  REQUIRE(struct2.f5.find("20")->second == test::EnumSimple::ENUM_VALUE_2);
  REQUIRE(struct2.f6.size() == 2);
  REQUIRE(struct2.f6.find("10")->second.value() ==
          test::EnumSimple::ENUM_VALUE_1);
  REQUIRE(struct2.f6.find("20")->second == std::nullopt);
  REQUIRE(struct2.f7.size() == 2);
  REQUIRE(struct2.f7.find("10")->second ==
          (test::FlagsSimple::FLAG_VALUE_1 | test::FlagsSimple::FLAG_VALUE_2));
  REQUIRE(struct2.f7.find("20")->second ==
          (test::FlagsSimple::FLAG_VALUE_1 | test::FlagsSimple::FLAG_VALUE_2 |
           test::FlagsSimple::FLAG_VALUE_3));
  REQUIRE(struct2.f8.size() == 2);
  REQUIRE(struct2.f8.find("10")->second.value() ==
          (test::FlagsSimple::FLAG_VALUE_1 | test::FlagsSimple::FLAG_VALUE_2));
  REQUIRE(struct2.f8.find("20")->second == std::nullopt);
  REQUIRE(struct2.f9.size() == 2);
  REQUIRE(struct2.f9.find("10")->second.id == 48);
  REQUIRE(struct2.f9.find("20")->second.id == 65);
  REQUIRE(struct2.f10.size() == 2);
  REQUIRE(struct2.f10.find("10")->second.value().id == 48);
  REQUIRE(struct2.f10.find("20")->second == std::nullopt);
  REQUIRE(struct1 == struct2);
}

TEST_CASE("Serialization: struct hash extended", "[FBE]") {
  // Create a new struct
  test::StructHashEx struct1;
  test::StructSimple s1;
  s1.id = 48;
  struct1.f1.emplace(s1, test::StructNested());
  test::StructSimple s2;
  s2.id = 65;
  struct1.f1.emplace(s2, test::StructNested());
  struct1.f2.emplace(s1, test::StructNested());
  struct1.f2.emplace(s2, std::nullopt);

  // Serialize the struct to the FBE stream
  FBE::test::StructHashExModel writer;
  REQUIRE(writer.model.fbe_type() == 142);
  REQUIRE(writer.model.fbe_offset() == 4);
  size_t serialized = writer.serialize(struct1, nullptr);
  REQUIRE(serialized == writer.buffer().size());
  REQUIRE(writer.verify());
  writer.next(serialized);
  REQUIRE(writer.model.fbe_offset() == (4 + writer.buffer().size()));

  // Check the serialized FBE size
  REQUIRE(writer.buffer().size() == 7879);

  // Deserialize the struct from the FBE stream
  test::StructHashEx struct2;
  FBE::test::StructHashExModel reader;
  REQUIRE(reader.model.fbe_type() == 142);
  REQUIRE(reader.model.fbe_offset() == 4);
  reader.attach(writer.buffer());
  REQUIRE(reader.verify());
  size_t deserialized = reader.deserialize(struct2, nullptr);
  REQUIRE(deserialized == reader.buffer().size());
  reader.next(deserialized);
  REQUIRE(reader.model.fbe_offset() == (4 + reader.buffer().size()));

  REQUIRE(struct2.f1.size() == 2);
  REQUIRE(struct2.f1.find(s1)->second.f1002 == test::EnumTyped::ENUM_VALUE_2);
  REQUIRE(struct2.f1.find(s2)->second.f1002 == test::EnumTyped::ENUM_VALUE_2);
  REQUIRE(struct2.f2.size() == 2);
  REQUIRE(struct2.f2.find(s1)->second.value().f1002 ==
          test::EnumTyped::ENUM_VALUE_2);
  REQUIRE(struct2.f2.find(s2)->second == std::nullopt);
  REQUIRE(struct1 == struct2);
}

TEST_CASE("Serialization: variant", "[FBE]") {
  SECTION("monostate") {
    ::variants::Value value;
    REQUIRE(value.v.index() == 0); // default is monostate

    FBE::variants::ValueModel writer;
    size_t serialized = writer.serialize(value, nullptr);
    REQUIRE(serialized == writer.buffer().size());
    REQUIRE(writer.verify());

    FBE::variants::ValueModel reader;
    reader.attach(writer.buffer());
    REQUIRE(reader.verify());

    ::variants::Value value_copy;
    value_copy.v.emplace<int32_t>(999); // set to non-default first
    REQUIRE(value_copy.v.index() != 0);

    size_t deserialized = reader.deserialize(value_copy, nullptr);
    REQUIRE(deserialized == reader.buffer().size());

    REQUIRE(value_copy.v.index() == 0);
    REQUIRE(std::holds_alternative<std::monostate>(value_copy.v));
    REQUIRE(value == value_copy);
  }

  SECTION("string") {
    ::variants::Value value{FBE::FBEString("variant v")};

    FBE::variants::ValueModel writer;
    size_t serialized = writer.serialize(value, nullptr);
    REQUIRE(serialized == writer.buffer().size());
    REQUIRE(writer.verify());

    FBE::variants::ValueModel reader;
    reader.attach(writer.buffer());
    REQUIRE(reader.verify());

    ::variants::Value value_copy;
    size_t deserialized = reader.deserialize(value_copy, nullptr);
    REQUIRE(deserialized == reader.buffer().size());

    REQUIRE(value_copy.v.index() == 1);
    REQUIRE(std::get<FBE::FBEString>(value_copy.v) == "variant v");
    REQUIRE(value == value_copy);
  }

  SECTION("primitive type") {
    ::variants::Value value;
    value.v.emplace<int32_t>(42);

    FBE::variants::ValueModel writer;
    size_t serialized = writer.serialize(value, nullptr);
    REQUIRE(serialized == writer.buffer().size());
    REQUIRE(writer.verify());

    FBE::variants::ValueModel reader;
    reader.attach(writer.buffer());
    REQUIRE(reader.verify());

    ::variants::Value value_copy;
    size_t deserialized = reader.deserialize(value_copy, nullptr);
    REQUIRE(deserialized == reader.buffer().size());

    REQUIRE(value_copy.v.index() == 2);
    REQUIRE(std::get<int32_t>(value_copy.v) == 42);
    REQUIRE(value == value_copy);
  }

  SECTION("double") {
    ::variants::Value value;
    value.v.emplace<double>(3.14159265358979);

    FBE::variants::ValueModel writer;
    size_t serialized = writer.serialize(value, nullptr);
    REQUIRE(serialized == writer.buffer().size());
    REQUIRE(writer.verify());

    FBE::variants::ValueModel reader;
    reader.attach(writer.buffer());
    REQUIRE(reader.verify());

    ::variants::Value value_copy;
    size_t deserialized = reader.deserialize(value_copy, nullptr);
    REQUIRE(deserialized == reader.buffer().size());

    REQUIRE(value_copy.v.index() == 3);
    REQUIRE(std::get<double>(value_copy.v) == 3.14159265358979);
    REQUIRE(value == value_copy);
  }

  SECTION("struct") {
    ::variants::Simple simple{"simple"};
    ::variants::Value value(simple);

    FBE::variants::ValueModel writer;
    size_t serialized = writer.serialize(value, nullptr);
    REQUIRE(serialized == writer.buffer().size());
    REQUIRE(writer.verify());

    FBE::variants::ValueModel reader;
    reader.attach(writer.buffer());
    REQUIRE(reader.verify());

    ::variants::Value value_copy;
    size_t deserialized = reader.deserialize(value_copy, nullptr);
    REQUIRE(deserialized == reader.buffer().size());

    REQUIRE(value_copy.v.index() == 4);
    REQUIRE(std::get<::variants::Simple>(value_copy.v).name == "simple");
    REQUIRE(value == value_copy);
  }

  SECTION("vector of struct") {
    FastVec<::variants::Simple> v;
    v.emplace_back(::variants::Simple{"simple1"});
    v.emplace_back(::variants::Simple{"simple2"});

    ::variants::Value value;
    REQUIRE(value.v.index() == 0);
    value.v.emplace<5>(std::move(v));

    FBE::variants::ValueModel writer;
    size_t serialized = writer.serialize(value, nullptr);
    REQUIRE(serialized == writer.buffer().size());
    REQUIRE(writer.verify());

    FBE::variants::ValueModel reader;
    reader.attach(writer.buffer());
    REQUIRE(reader.verify());

    ::variants::Value value_copy;
    size_t deserialized = reader.deserialize(value_copy, nullptr);
    REQUIRE(deserialized == reader.buffer().size());

    REQUIRE(value_copy.v.index() == 5);
    auto &v_copy = std::get<5>(value_copy.v);
    REQUIRE(v_copy.size() == 2);
    REQUIRE(v_copy.at(0).name == "simple1");
    REQUIRE(v_copy.at(1).name == "simple2");
    REQUIRE(value == value_copy);
  }

  SECTION("vector of primitive type") {
    FastVec<int32_t> v{1, 2, 3};

    ::variants::Value value;
    REQUIRE(value.v.index() == 0);
    value.v.emplace<6>(std::move(v));

    FBE::variants::ValueModel writer;
    size_t serialized = writer.serialize(value, nullptr);
    REQUIRE(serialized == writer.buffer().size());
    REQUIRE(writer.verify());

    FBE::variants::ValueModel reader;
    reader.attach(writer.buffer());
    REQUIRE(reader.verify());

    ::variants::Value value_copy;
    size_t deserialized = reader.deserialize(value_copy, nullptr);
    REQUIRE(deserialized == reader.buffer().size());

    REQUIRE(value_copy.v.index() == 6);
    auto &v_copy = std::get<6>(value_copy.v);
    REQUIRE(v_copy.size() == 3);
    REQUIRE(v_copy.at(0) == 1);
    REQUIRE(v_copy.at(1) == 2);
    REQUIRE(v_copy.at(2) == 3);
    REQUIRE(value == value_copy);
  }

  SECTION("hash with primitive and struct") {
    std::unordered_map<int32_t, ::variants::Simple> m;
    m.emplace(1, ::variants::Simple{"simple1"});
    m.emplace(2, ::variants::Simple{"simple2"});

    ::variants::Value value;
    REQUIRE(value.v.index() == 0);
    value.v.emplace<7>(std::move(m));

    FBE::variants::ValueModel writer;
    size_t serialized = writer.serialize(value, nullptr);
    REQUIRE(serialized == writer.buffer().size());
    REQUIRE(writer.verify());

    FBE::variants::ValueModel reader;
    reader.attach(writer.buffer());
    REQUIRE(reader.verify());

    ::variants::Value value_copy;
    size_t deserialized = reader.deserialize(value_copy, nullptr);
    REQUIRE(deserialized == reader.buffer().size());

    REQUIRE(value_copy.v.index() == 7);
    auto &v_copy = std::get<7>(value_copy.v);
    REQUIRE(v_copy.size() == 2);
    REQUIRE(v_copy.at(1).name == "simple1");
    REQUIRE(v_copy.at(2).name == "simple2");
    REQUIRE(value == value_copy);
  }

  SECTION("container of bytes") {
    FastVec<uint8_t> v{65, 66, 67, 68, 69};
    FastVec<FBE::buffer_t> bytes_v;
    bytes_v.emplace_back(FBE::buffer_t(v));

    ::variants::Value value;
    REQUIRE(value.v.index() == 0);
    value.v.emplace<8>(std::move(bytes_v));

    FBE::variants::ValueModel writer;
    size_t serialized = writer.serialize(value, nullptr);
    REQUIRE(serialized == writer.buffer().size());
    REQUIRE(writer.verify());

    FBE::variants::ValueModel reader;
    reader.attach(writer.buffer());
    REQUIRE(reader.verify());

    ::variants::Value value_copy;
    size_t deserialized = reader.deserialize(value_copy, nullptr);
    REQUIRE(deserialized == reader.buffer().size());

    REQUIRE(value_copy.v.index() == 8);
    auto &v_copy = std::get<8>(value_copy.v);
    REQUIRE(v_copy.size() == 1);
    REQUIRE(v_copy.at(0).string() == "ABCDE");
    REQUIRE(value == value_copy);
  }

  SECTION("vector of string") {
    FastVec<FBE::FBEString> string_v{"hello", "world"};

    ::variants::Value value;
    REQUIRE(value.v.index() == 0);
    value.v.emplace<9>(std::move(string_v));

    FBE::variants::ValueModel writer;
    size_t serialized = writer.serialize(value, nullptr);
    REQUIRE(serialized == writer.buffer().size());
    REQUIRE(writer.verify());

    FBE::variants::ValueModel reader;
    reader.attach(writer.buffer());
    REQUIRE(reader.verify());

    ::variants::Value value_copy;
    size_t deserialized = reader.deserialize(value_copy, nullptr);
    REQUIRE(deserialized == reader.buffer().size());

    REQUIRE(value_copy.v.index() == 9);
    auto &v_copy = std::get<9>(value_copy.v);
    REQUIRE(v_copy.size() == 2);
    REQUIRE(v_copy.at(0) == "hello");
    REQUIRE(v_copy.at(1) == "world");
    REQUIRE(value == value_copy);
  }

  SECTION("hash with primitive and bytes") {
    std::unordered_map<int32_t, FBE::buffer_t> m;
    FastVec<uint8_t> v{65, 66, 67, 68, 69};
    m.emplace(42, FBE::buffer_t(v));

    ::variants::Value value;
    REQUIRE(value.v.index() == 0);
    value.v.emplace<10>(std::move(m));

    FBE::variants::ValueModel writer;
    size_t serialized = writer.serialize(value, nullptr);
    REQUIRE(serialized == writer.buffer().size());
    REQUIRE(writer.verify());

    FBE::variants::ValueModel reader;
    reader.attach(writer.buffer());
    REQUIRE(reader.verify());

    ::variants::Value value_copy;
    size_t deserialized = reader.deserialize(value_copy, nullptr);
    REQUIRE(deserialized == reader.buffer().size());

    REQUIRE(value_copy.v.index() == 10);
    auto &v_copy = std::get<10>(value_copy.v);
    REQUIRE(v_copy.size() == 1);
    REQUIRE(v_copy.at(42).string() == "ABCDE");
    REQUIRE(value == value_copy);
  }

  SECTION("hash with string and bytes") {
    std::unordered_map<FBE::FBEString, FBE::buffer_t> m;
    FastVec<uint8_t> v{65, 66, 67, 68, 69};
    m.emplace("hello world", FBE::buffer_t(v));

    ::variants::Value value;
    REQUIRE(value.v.index() == 0);
    value.v.emplace<11>(std::move(m));

    FBE::variants::ValueModel writer;
    size_t serialized = writer.serialize(value, nullptr);
    REQUIRE(serialized == writer.buffer().size());
    REQUIRE(writer.verify());

    FBE::variants::ValueModel reader;
    reader.attach(writer.buffer());
    REQUIRE(reader.verify());

    ::variants::Value value_copy;
    size_t deserialized = reader.deserialize(value_copy, nullptr);
    REQUIRE(deserialized == reader.buffer().size());

    REQUIRE(value_copy.v.index() == 11);
    auto &v_copy = std::get<11>(value_copy.v);
    REQUIRE(v_copy.size() == 1);
    REQUIRE(v_copy.at("hello world").string() == "ABCDE");
    REQUIRE(value == value_copy);
  }

  SECTION("variant of variant") {
    ::variants::Expr expr{"42"};
    ::variants::Value value;
    REQUIRE(value.v.index() == 0);
    value.v.emplace<::variants::Expr>(std::move(expr));

    FBE::variants::ValueModel writer;
    size_t serialized = writer.serialize(value, nullptr);
    REQUIRE(serialized == writer.buffer().size());
    REQUIRE(writer.verify());

    FBE::variants::ValueModel reader;
    reader.attach(writer.buffer());
    REQUIRE(reader.verify());

    ::variants::Value value_copy;
    size_t deserialized = reader.deserialize(value_copy, nullptr);
    REQUIRE(deserialized == reader.buffer().size());

    REQUIRE(value_copy.v.index() == 12);
    auto &v_copy_expr = std::get<::variants::Expr>(value_copy.v);
    REQUIRE(v_copy_expr.index() == 3);
    REQUIRE(std::get<3>(v_copy_expr) == "42");
    REQUIRE(value == value_copy);
  }
}

TEST_CASE("Serialization: int128 and uint128", "[FBE]") {
  // Create a new struct
  cpp_only::Struct128 struct1;
  struct1.f1 = std::numeric_limits<__int128_t>::min();
  struct1.f2 = std::numeric_limits<__int128_t>::min();
  struct1.f3 = std::numeric_limits<__uint128_t>::max();
  struct1.f4 = std::numeric_limits<__uint128_t>::max();

  // Serialize the struct to the FBE stream
  FBE::cpp_only::Struct128Model writer;
  size_t serialized = writer.serialize(struct1, nullptr);
  REQUIRE(serialized == writer.buffer().size());
  REQUIRE(writer.verify());
  writer.next(serialized);
  REQUIRE(writer.model.fbe_offset() == (4 + writer.buffer().size()));

  // Deserialize the struct from the FBE stream
  cpp_only::Struct128 struct2;
  FBE::cpp_only::Struct128Model reader;
  REQUIRE(reader.model.fbe_offset() == 4);
  reader.attach(writer.buffer());
  REQUIRE(reader.verify());
  size_t deserialized = reader.deserialize(struct2, nullptr);
  REQUIRE(deserialized == reader.buffer().size());
  reader.next(deserialized);
  REQUIRE(reader.model.fbe_offset() == (4 + reader.buffer().size()));

  REQUIRE(struct1.f1 == struct2.f1);
  REQUIRE(struct1.f2 == struct2.f2);
  REQUIRE(struct1.f3 == struct2.f3);
  REQUIRE(struct1.f4 == struct2.f4);
  REQUIRE(struct2.f1 < std::numeric_limits<int64_t>::min());
  REQUIRE(struct2.f2.value() < std::numeric_limits<int64_t>::min());
  REQUIRE(struct2.f3 > std::numeric_limits<uint64_t>::max());
  REQUIRE(struct2.f4.value() > std::numeric_limits<uint64_t>::max());
  REQUIRE(struct1 == struct2);
}

TEST_CASE("Serialization: empty vector", "[FBE][Vector]") {
  // Test empty vectors serialize and deserialize correctly
  test::StructVector struct1;
  // All vectors are empty by default

  FBE::test::StructVectorModel writer;
  size_t serialized = writer.serialize(struct1, nullptr);
  REQUIRE(serialized == writer.buffer().size());
  REQUIRE(writer.verify());

  test::StructVector struct2;
  FBE::test::StructVectorModel reader;
  reader.attach(writer.buffer());
  REQUIRE(reader.verify());
  size_t deserialized = reader.deserialize(struct2, nullptr);
  REQUIRE(deserialized == reader.buffer().size());

  REQUIRE(struct2.f1.empty());
  REQUIRE(struct2.f2.empty());
  REQUIRE(struct2.f3.empty());
  REQUIRE(struct2.f4.empty());
  REQUIRE(struct2.f5.empty());
  REQUIRE(struct2.f6.empty());
  REQUIRE(struct2.f7.empty());
  REQUIRE(struct2.f8.empty());
  REQUIRE(struct2.f9.empty());
  REQUIRE(struct2.f10.empty());
  REQUIRE(struct1 == struct2);
}

TEST_CASE("Serialization: large vector", "[FBE][Vector]") {
  // Test large vectors (stress test for emplace_back)
  test::StructVector struct1;

  // Add 1000 elements to byte vector
  for (int i = 0; i < 1000; ++i) {
    struct1.f1.emplace_back(static_cast<uint8_t>(i % 256));
  }

  // Add 500 elements to optional byte vector
  for (int i = 0; i < 500; ++i) {
    if (i % 3 == 0) {
      struct1.f2.emplace_back(std::nullopt);
    } else {
      struct1.f2.emplace_back(static_cast<uint8_t>(i % 256));
    }
  }

  // Add 100 structs
  for (int i = 0; i < 100; ++i) {
    test::StructSimple s;
    s.id = i;
    s.f32 = "String " + std::to_string(i);
    struct1.f9.emplace_back(std::move(s));
  }

  FBE::test::StructVectorModel writer;
  size_t serialized = writer.serialize(struct1, nullptr);
  REQUIRE(serialized == writer.buffer().size());
  REQUIRE(writer.verify());

  test::StructVector struct2;
  FBE::test::StructVectorModel reader;
  reader.attach(writer.buffer());
  REQUIRE(reader.verify());
  size_t deserialized = reader.deserialize(struct2, nullptr);
  REQUIRE(deserialized == reader.buffer().size());

  REQUIRE(struct2.f1.size() == 1000);
  for (int i = 0; i < 1000; ++i) {
    REQUIRE(struct2.f1[i] == static_cast<uint8_t>(i % 256));
  }

  REQUIRE(struct2.f2.size() == 500);
  for (int i = 0; i < 500; ++i) {
    if (i % 3 == 0) {
      REQUIRE(struct2.f2[i] == std::nullopt);
    } else {
      REQUIRE(struct2.f2[i].value() == static_cast<uint8_t>(i % 256));
    }
  }

  REQUIRE(struct2.f9.size() == 100);
  for (int i = 0; i < 100; ++i) {
    REQUIRE(struct2.f9[i].id == i);
    REQUIRE(struct2.f9[i].f32 == "String " + std::to_string(i));
  }

  REQUIRE(struct1 == struct2);
}

TEST_CASE("Serialization: vector with nested vectors", "[FBE][Vector]") {
  // Test vectors of bytes (nested structure)
  test::StructVector struct1;

  // Add varying size byte vectors
  for (int i = 0; i < 50; ++i) {
    FastVec<uint8_t> inner;
    for (int j = 0; j <= i; ++j) {
      inner.emplace_back(static_cast<uint8_t>((i + j) % 256));
    }
    struct1.f3.emplace_back(std::move(inner));
  }

  // Add optional byte vectors with nulls
  for (int i = 0; i < 30; ++i) {
    if (i % 4 == 0) {
      struct1.f4.emplace_back(std::nullopt);
    } else {
      FastVec<uint8_t> inner;
      for (int j = 0; j < i; ++j) {
        inner.emplace_back(static_cast<uint8_t>(j));
      }
      struct1.f4.emplace_back(std::move(inner));
    }
  }

  FBE::test::StructVectorModel writer;
  size_t serialized = writer.serialize(struct1, nullptr);
  REQUIRE(serialized == writer.buffer().size());
  REQUIRE(writer.verify());

  test::StructVector struct2;
  FBE::test::StructVectorModel reader;
  reader.attach(writer.buffer());
  REQUIRE(reader.verify());
  size_t deserialized = reader.deserialize(struct2, nullptr);
  REQUIRE(deserialized == reader.buffer().size());

  REQUIRE(struct2.f3.size() == 50);
  for (int i = 0; i < 50; ++i) {
    REQUIRE(struct2.f3[i].size() == static_cast<size_t>(i + 1));
    for (int j = 0; j <= i; ++j) {
      REQUIRE(struct2.f3[i][j] == static_cast<uint8_t>((i + j) % 256));
    }
  }

  REQUIRE(struct2.f4.size() == 30);
  for (int i = 0; i < 30; ++i) {
    if (i % 4 == 0) {
      REQUIRE(struct2.f4[i] == std::nullopt);
    } else {
      REQUIRE(struct2.f4[i].has_value());
      REQUIRE(struct2.f4[i].value().size() == static_cast<size_t>(i));
    }
  }

  REQUIRE(struct1 == struct2);
}

TEST_CASE("Serialization: multiple round trips", "[FBE][Vector]") {
  // Test that multiple serialize/deserialize cycles preserve data
  test::StructVector original;
  for (int i = 0; i < 100; ++i) {
    original.f1.emplace_back(static_cast<uint8_t>(i));
    test::StructSimple s;
    s.id = i * 10;
    s.f32 = "Test" + std::to_string(i);
    original.f9.emplace_back(std::move(s));
  }

  test::StructVector current = original;

  // Do 5 round trips
  for (int round = 0; round < 5; ++round) {
    FBE::test::StructVectorModel writer;
    size_t serialized = writer.serialize(current, nullptr);
    REQUIRE(writer.verify());

    test::StructVector next;
    FBE::test::StructVectorModel reader;
    reader.attach(writer.buffer());
    REQUIRE(reader.verify());
    size_t deserialized = reader.deserialize(next, nullptr);
    REQUIRE(deserialized == serialized);

    REQUIRE(current == next);
    current = std::move(next);
  }

  // Final result should equal original
  REQUIRE(original == current);
}

TEST_CASE("Serialization: map large", "[FBE][Map]") {
  // Test large maps
  test::StructMap struct1;

  for (int i = 0; i < 500; ++i) {
    struct1.f1.emplace(i, static_cast<uint8_t>(i % 256));
  }

  for (int i = 0; i < 200; ++i) {
    if (i % 5 == 0) {
      struct1.f2.emplace(i, std::nullopt);
    } else {
      struct1.f2.emplace(i, static_cast<uint8_t>(i % 256));
    }
  }

  for (int i = 0; i < 100; ++i) {
    test::StructSimple s;
    s.id = i;
    s.f32 = "MapValue" + std::to_string(i);
    struct1.f9.emplace(i, std::move(s));
  }

  FBE::test::StructMapModel writer;
  size_t serialized = writer.serialize(struct1, nullptr);
  REQUIRE(serialized == writer.buffer().size());
  REQUIRE(writer.verify());

  test::StructMap struct2;
  FBE::test::StructMapModel reader;
  reader.attach(writer.buffer());
  REQUIRE(reader.verify());
  size_t deserialized = reader.deserialize(struct2, nullptr);
  REQUIRE(deserialized == reader.buffer().size());

  REQUIRE(struct2.f1.size() == 500);
  for (int i = 0; i < 500; ++i) {
    REQUIRE(struct2.f1.at(i) == static_cast<uint8_t>(i % 256));
  }

  REQUIRE(struct2.f2.size() == 200);
  for (int i = 0; i < 200; ++i) {
    if (i % 5 == 0) {
      REQUIRE(struct2.f2.at(i) == std::nullopt);
    } else {
      REQUIRE(struct2.f2.at(i).value() == static_cast<uint8_t>(i % 256));
    }
  }

  REQUIRE(struct2.f9.size() == 100);
  for (int i = 0; i < 100; ++i) {
    REQUIRE(struct2.f9.at(i).id == i);
    REQUIRE(struct2.f9.at(i).f32 == "MapValue" + std::to_string(i));
  }

  REQUIRE(struct1 == struct2);
}

TEST_CASE("Serialization: hash large", "[FBE][Hash]") {
  // Test large hash maps
  test::StructHash struct1;

  for (int i = 0; i < 500; ++i) {
    struct1.f1.emplace("key" + std::to_string(i),
                       static_cast<uint8_t>(i % 256));
  }

  for (int i = 0; i < 100; ++i) {
    test::StructSimple s;
    s.id = i;
    s.f32 = "HashValue" + std::to_string(i);
    struct1.f9.emplace("struct" + std::to_string(i), std::move(s));
  }

  FBE::test::StructHashModel writer;
  size_t serialized = writer.serialize(struct1, nullptr);
  REQUIRE(serialized == writer.buffer().size());
  REQUIRE(writer.verify());

  test::StructHash struct2;
  FBE::test::StructHashModel reader;
  reader.attach(writer.buffer());
  REQUIRE(reader.verify());
  size_t deserialized = reader.deserialize(struct2, nullptr);
  REQUIRE(deserialized == reader.buffer().size());

  REQUIRE(struct2.f1.size() == 500);
  for (int i = 0; i < 500; ++i) {
    REQUIRE(struct2.f1.at("key" + std::to_string(i)) ==
            static_cast<uint8_t>(i % 256));
  }

  REQUIRE(struct2.f9.size() == 100);
  for (int i = 0; i < 100; ++i) {
    REQUIRE(struct2.f9.at("struct" + std::to_string(i)).id == i);
    REQUIRE(struct2.f9.at("struct" + std::to_string(i)).f32 ==
            "HashValue" + std::to_string(i));
  }

  REQUIRE(struct1 == struct2);
}

TEST_CASE("Serialization: list large", "[FBE][List]") {
  // Test large lists
  test::StructList struct1;

  for (int i = 0; i < 1000; ++i) {
    struct1.f1.emplace_back(static_cast<uint8_t>(i % 256));
  }

  for (int i = 0; i < 100; ++i) {
    test::StructSimple s;
    s.id = i;
    s.f32 = "ListValue" + std::to_string(i);
    struct1.f9.emplace_back(std::move(s));
  }

  FBE::test::StructListModel writer;
  size_t serialized = writer.serialize(struct1, nullptr);
  REQUIRE(serialized == writer.buffer().size());
  REQUIRE(writer.verify());

  test::StructList struct2;
  FBE::test::StructListModel reader;
  reader.attach(writer.buffer());
  REQUIRE(reader.verify());
  size_t deserialized = reader.deserialize(struct2, nullptr);
  REQUIRE(deserialized == reader.buffer().size());

  REQUIRE(struct2.f1.size() == 1000);
  int idx = 0;
  for (const auto &v : struct2.f1) {
    REQUIRE(v == static_cast<uint8_t>(idx % 256));
    ++idx;
  }

  REQUIRE(struct2.f9.size() == 100);
  idx = 0;
  for (const auto &s : struct2.f9) {
    REQUIRE(s.id == idx);
    REQUIRE(s.f32 == "ListValue" + std::to_string(idx));
    ++idx;
  }

  REQUIRE(struct1 == struct2);
}

TEST_CASE("Serialization: set large", "[FBE][Set]") {
  // Test large sets
  test::StructSet struct1;

  for (int i = 0; i < 200; ++i) {
    struct1.f1.emplace(static_cast<uint8_t>(i % 256));
  }

  for (int i = 0; i < 100; ++i) {
    test::StructSimple s;
    s.id = i; // id is the key
    s.f32 = "SetValue" + std::to_string(i);
    struct1.f4.emplace(std::move(s));
  }

  FBE::test::StructSetModel writer;
  size_t serialized = writer.serialize(struct1, nullptr);
  REQUIRE(serialized == writer.buffer().size());
  REQUIRE(writer.verify());

  test::StructSet struct2;
  FBE::test::StructSetModel reader;
  reader.attach(writer.buffer());
  REQUIRE(reader.verify());
  size_t deserialized = reader.deserialize(struct2, nullptr);
  REQUIRE(deserialized == reader.buffer().size());

  // Set of bytes has max 256 unique values
  REQUIRE(struct2.f1.size() == 200);

  REQUIRE(struct2.f4.size() == 100);
  for (int i = 0; i < 100; ++i) {
    test::StructSimple key;
    key.id = i;
    auto it = struct2.f4.find(key);
    REQUIRE(it != struct2.f4.end());
  }

  REQUIRE(struct1 == struct2);
}
