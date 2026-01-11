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
// Serialization benchmarks using simple timing
//

#include "test.h"

#include "../proto/proto_models.h"
#include "../proto/proto_final_models.h"
#include "../proto/test_models.h"
#include "../proto/test_final_models.h"

#include <chrono>
#include <iostream>
#include <iomanip>

// Helper to create a test account
static proto::Account CreateAccount()
{
    proto::Account account;
    account.id = 12345;
    account.name = "Test Account Name With Some Text";
    account.state = proto::State::good;
    account.wallet = { "USD", 1000.0 };
    account.asset = std::make_optional<proto::Balance>({ "EUR", 100.0 });
    for (int i = 0; i < 10; i++) {
        account.orders.emplace_back(i, "EURUSD", proto::OrderSide::buy, proto::OrderType::market, 1.23456 + i, 1000.0 + i);
    }
    return account;
}

// Helper to create a test struct with basic fields
static test::StructSimple CreateSimpleStruct()
{
    test::StructSimple s;
    s.id = 12345;
    s.f1 = true;
    s.f2 = 'A';
    s.f3 = 100;
    s.f4 = 50;
    s.f5 = 60;
    s.f6 = 70;
    s.f7 = 500;
    s.f8 = 600;
    s.f9 = 70;
    s.f10 = 80;
    s.f11 = 90;
    s.f12 = 100;
    s.f25 = 3.14f;
    s.f27 = 2.71828;
    return s;
}

static test::StructBytes CreateBytesStruct()
{
    test::StructBytes s;
    s.f1.assign(1024, 0xAB);  // 1KB of bytes
    return s;
}

template<typename Func>
double benchmark(const char* name, int iterations, Func&& func) {
    // Warmup
    for (int i = 0; i < 100; i++) {
        func();
    }

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; i++) {
        func();
    }
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    double ns_per_op = static_cast<double>(duration) / iterations;
    return ns_per_op;
}

TEST_CASE("Benchmark: Serialization Performance", "[FBE][Benchmark]")
{
    constexpr int ITERATIONS = 100000;

    std::cout << "\n=== FBE Serialization Benchmark ===" << std::endl;
    std::cout << "Iterations: " << ITERATIONS << std::endl;
    std::cout << std::fixed << std::setprecision(2);

    // Account serialize
    {
        proto::Account source = CreateAccount();
        FBE::proto::AccountModel writer;
        double ns = benchmark("Account serialize", ITERATIONS, [&]() {
            writer.reset();
            writer.serialize(source, nullptr);
        });
        std::cout << "Account serialize:         " << std::setw(10) << ns << " ns/op" << std::endl;
    }

    // Account deserialize
    {
        proto::Account source = CreateAccount();
        FBE::proto::AccountModel writer;
        writer.serialize(source, nullptr);
        FBE::proto::AccountModel reader;
        reader.attach(writer.buffer());
        proto::Account result;
        double ns = benchmark("Account deserialize", ITERATIONS, [&]() {
            reader.deserialize(result, nullptr);
        });
        std::cout << "Account deserialize:       " << std::setw(10) << ns << " ns/op" << std::endl;
    }

    // StructSimple serialize
    {
        test::StructSimple source = CreateSimpleStruct();
        FBE::test::StructSimpleModel writer;
        double ns = benchmark("StructSimple serialize", ITERATIONS, [&]() {
            writer.reset();
            writer.serialize(source, nullptr);
        });
        std::cout << "StructSimple serialize:    " << std::setw(10) << ns << " ns/op" << std::endl;
    }

    // StructSimple deserialize
    {
        test::StructSimple source = CreateSimpleStruct();
        FBE::test::StructSimpleModel writer;
        writer.serialize(source, nullptr);
        FBE::test::StructSimpleModel reader;
        reader.attach(writer.buffer());
        test::StructSimple result;
        double ns = benchmark("StructSimple deserialize", ITERATIONS, [&]() {
            reader.deserialize(result, nullptr);
        });
        std::cout << "StructSimple deserialize:  " << std::setw(10) << ns << " ns/op" << std::endl;
    }

    // StructBytes serialize (1KB)
    {
        test::StructBytes source = CreateBytesStruct();
        FBE::test::StructBytesModel writer;
        double ns = benchmark("StructBytes serialize", ITERATIONS, [&]() {
            writer.reset();
            writer.serialize(source, nullptr);
        });
        std::cout << "StructBytes serialize:     " << std::setw(10) << ns << " ns/op (1KB payload)" << std::endl;
    }

    // StructBytes deserialize (1KB)
    {
        test::StructBytes source = CreateBytesStruct();
        FBE::test::StructBytesModel writer;
        writer.serialize(source, nullptr);
        FBE::test::StructBytesModel reader;
        reader.attach(writer.buffer());
        test::StructBytes result;
        double ns = benchmark("StructBytes deserialize", ITERATIONS, [&]() {
            reader.deserialize(result, nullptr);
        });
        std::cout << "StructBytes deserialize:   " << std::setw(10) << ns << " ns/op (1KB payload)" << std::endl;
    }

    // StructNested serialize
    {
        test::StructNested source;
        source.f1 = true;
        source.f2 = 42;
        FBE::test::StructNestedModel writer;
        double ns = benchmark("StructNested serialize", ITERATIONS, [&]() {
            writer.reset();
            writer.serialize(source, nullptr);
        });
        std::cout << "StructNested serialize:    " << std::setw(10) << ns << " ns/op" << std::endl;
    }

    // StructNested deserialize
    {
        test::StructNested source;
        source.f1 = true;
        source.f2 = 42;
        FBE::test::StructNestedModel writer;
        writer.serialize(source, nullptr);
        FBE::test::StructNestedModel reader;
        reader.attach(writer.buffer());
        test::StructNested result;
        double ns = benchmark("StructNested deserialize", ITERATIONS, [&]() {
            reader.deserialize(result, nullptr);
        });
        std::cout << "StructNested deserialize:  " << std::setw(10) << ns << " ns/op" << std::endl;
    }

    // StructVector serialize (100 elements)
    {
        test::StructVector source;
        for (int i = 0; i < 100; i++) {
            source.f1.push_back((uint8_t)(i & 0xFF));
            source.f5.push_back(static_cast<test::EnumSimple>(i % 3));
            source.f9.push_back(CreateSimpleStruct());
        }
        FBE::test::StructVectorModel writer;
        double ns = benchmark("StructVector serialize", ITERATIONS / 10, [&]() {
            writer.reset();
            writer.serialize(source, nullptr);
        });
        std::cout << "StructVector serialize:    " << std::setw(10) << ns << " ns/op (100 elements)" << std::endl;
    }

    // StructVector deserialize (100 elements)
    {
        test::StructVector source;
        for (int i = 0; i < 100; i++) {
            source.f1.push_back((uint8_t)(i & 0xFF));
            source.f5.push_back(static_cast<test::EnumSimple>(i % 3));
            source.f9.push_back(CreateSimpleStruct());
        }
        FBE::test::StructVectorModel writer;
        writer.serialize(source, nullptr);
        FBE::test::StructVectorModel reader;
        reader.attach(writer.buffer());
        test::StructVector result;
        double ns = benchmark("StructVector deserialize", ITERATIONS / 10, [&]() {
            result.f1.clear();
            result.f5.clear();
            result.f9.clear();
            reader.deserialize(result, nullptr);
        });
        std::cout << "StructVector deserialize:  " << std::setw(10) << ns << " ns/op (100 elements)" << std::endl;
    }

    std::cout << "===================================" << std::endl;

    REQUIRE(true);  // Always pass - this is for benchmarking
}

TEST_CASE("Benchmark: Final Model Performance", "[FBE][Benchmark][Final]")
{
    constexpr int ITERATIONS = 100000;

    std::cout << "\n=== FBE Final Model Benchmark ===" << std::endl;
    std::cout << "Iterations: " << ITERATIONS << std::endl;
    std::cout << std::fixed << std::setprecision(2);

    // Account Final serialize
    {
        proto::Account source = CreateAccount();
        FBE::proto::AccountFinalModel writer;
        double ns = benchmark("Account Final serialize", ITERATIONS, [&]() {
            writer.reset();
            writer.serialize(source);
        });
        std::cout << "Account Final serialize:         " << std::setw(10) << ns << " ns/op" << std::endl;
    }

    // Account Final deserialize
    {
        proto::Account source = CreateAccount();
        FBE::proto::AccountFinalModel writer;
        writer.serialize(source);
        FBE::proto::AccountFinalModel reader;
        reader.attach(writer.buffer());
        proto::Account result;
        double ns = benchmark("Account Final deserialize", ITERATIONS, [&]() {
            reader.deserialize(result);
        });
        std::cout << "Account Final deserialize:       " << std::setw(10) << ns << " ns/op" << std::endl;
    }

    // StructSimple Final serialize
    {
        test::StructSimple source = CreateSimpleStruct();
        FBE::test::StructSimpleFinalModel writer;
        double ns = benchmark("StructSimple Final serialize", ITERATIONS, [&]() {
            writer.reset();
            writer.serialize(source);
        });
        std::cout << "StructSimple Final serialize:    " << std::setw(10) << ns << " ns/op" << std::endl;
    }

    // StructSimple Final deserialize
    {
        test::StructSimple source = CreateSimpleStruct();
        FBE::test::StructSimpleFinalModel writer;
        writer.serialize(source);
        FBE::test::StructSimpleFinalModel reader;
        reader.attach(writer.buffer());
        test::StructSimple result;
        double ns = benchmark("StructSimple Final deserialize", ITERATIONS, [&]() {
            reader.deserialize(result);
        });
        std::cout << "StructSimple Final deserialize:  " << std::setw(10) << ns << " ns/op" << std::endl;
    }

    // StructBytes Final serialize (1KB)
    {
        test::StructBytes source = CreateBytesStruct();
        FBE::test::StructBytesFinalModel writer;
        double ns = benchmark("StructBytes Final serialize", ITERATIONS, [&]() {
            writer.reset();
            writer.serialize(source);
        });
        std::cout << "StructBytes Final serialize:     " << std::setw(10) << ns << " ns/op (1KB payload)" << std::endl;
    }

    // StructBytes Final deserialize (1KB)
    {
        test::StructBytes source = CreateBytesStruct();
        FBE::test::StructBytesFinalModel writer;
        writer.serialize(source);
        FBE::test::StructBytesFinalModel reader;
        reader.attach(writer.buffer());
        test::StructBytes result;
        double ns = benchmark("StructBytes Final deserialize", ITERATIONS, [&]() {
            reader.deserialize(result);
        });
        std::cout << "StructBytes Final deserialize:   " << std::setw(10) << ns << " ns/op (1KB payload)" << std::endl;
    }

    // StructNested Final serialize
    {
        test::StructNested source;
        source.f1 = true;
        source.f2 = 42;
        FBE::test::StructNestedFinalModel writer;
        double ns = benchmark("StructNested Final serialize", ITERATIONS, [&]() {
            writer.reset();
            writer.serialize(source);
        });
        std::cout << "StructNested Final serialize:    " << std::setw(10) << ns << " ns/op" << std::endl;
    }

    // StructNested Final deserialize
    {
        test::StructNested source;
        source.f1 = true;
        source.f2 = 42;
        FBE::test::StructNestedFinalModel writer;
        writer.serialize(source);
        FBE::test::StructNestedFinalModel reader;
        reader.attach(writer.buffer());
        test::StructNested result;
        double ns = benchmark("StructNested Final deserialize", ITERATIONS, [&]() {
            reader.deserialize(result);
        });
        std::cout << "StructNested Final deserialize:  " << std::setw(10) << ns << " ns/op" << std::endl;
    }

    std::cout << "===================================" << std::endl;

    REQUIRE(true);  // Always pass - this is for benchmarking
}

TEST_CASE("Benchmark: Primitive Array Performance", "[FBE][Benchmark][PrimitiveArray]")
{
    constexpr int ITERATIONS = 1000000;
    constexpr size_t ARRAY_SIZE = 1000;

    std::cout << "\n=== FBE Primitive Array Benchmark (memcpy optimized) ===" << std::endl;
    std::cout << "Iterations: " << ITERATIONS << ", Array size: " << ARRAY_SIZE << std::endl;
    std::cout << std::fixed << std::setprecision(2);

    // Prepare source data
    FBE::FastVec<int32_t> int32_source;
    int32_source.reserve(ARRAY_SIZE);
    for (size_t i = 0; i < ARRAY_SIZE; i++) {
        int32_source.push_back(static_cast<int32_t>(i * 12345));
    }

    FBE::FastVec<double> double_source;
    double_source.reserve(ARRAY_SIZE);
    for (size_t i = 0; i < ARRAY_SIZE; i++) {
        double_source.push_back(static_cast<double>(i) * 3.14159);
    }

    // FinalModelVector<int32_t> serialize (memcpy)
    {
        FBE::FBEBuffer buffer;
        buffer.allocate(4 + ARRAY_SIZE * sizeof(int32_t) + 100);
        FBE::FinalModelVector<int32_t> model(buffer, 0);

        double ns = benchmark("int32[] Final serialize", ITERATIONS, [&]() {
            model.fbe_offset(0);
            model.set(int32_source);
        });
        std::cout << "int32[" << ARRAY_SIZE << "] serialize (memcpy):    " << std::setw(8) << ns << " ns/op" << std::endl;
    }

    // Pure memcpy comparison (no FBE overhead)
    {
        FBE::FBEBuffer buffer;
        buffer.allocate(4 + ARRAY_SIZE * sizeof(int32_t) + 100);
        uint8_t* data = buffer.data();

        double ns = benchmark("int32[] serialize (pure memcpy)", ITERATIONS, [&]() {
            *((uint32_t*)data) = (uint32_t)int32_source.size();
            memcpy(data + 4, int32_source.data(), ARRAY_SIZE * sizeof(int32_t));
        });
        std::cout << "int32[" << ARRAY_SIZE << "] serialize (memcpy raw):" << std::setw(8) << ns << " ns/op" << std::endl;
    }

    // Simulate old loop-based serialize for comparison
    {
        FBE::FBEBuffer buffer;
        buffer.allocate(4 + ARRAY_SIZE * sizeof(int32_t) + 100);
        uint8_t* data = buffer.data();

        double ns = benchmark("int32[] serialize (loop)", ITERATIONS, [&]() {
            *((uint32_t*)data) = (uint32_t)int32_source.size();
            int32_t* dest = (int32_t*)(data + 4);
            for (size_t i = 0; i < ARRAY_SIZE; ++i) {
                dest[i] = int32_source[i];
            }
        });
        std::cout << "int32[" << ARRAY_SIZE << "] serialize (loop):      " << std::setw(8) << ns << " ns/op" << std::endl;
    }

    // FinalModelVector<int32_t> deserialize (memcpy)
    {
        FBE::FBEBuffer buffer;
        buffer.allocate(4 + ARRAY_SIZE * sizeof(int32_t) + 100);
        FBE::FinalModelVector<int32_t> model(buffer, 0);
        model.set(int32_source);

        FBE::FastVec<int32_t> result;
        result.reserve(ARRAY_SIZE);
        double ns = benchmark("int32[] Final deserialize", ITERATIONS, [&]() {
            result.clear();
            model.fbe_offset(0);
            model.get(result);
        });
        std::cout << "int32[" << ARRAY_SIZE << "] deserialize (memcpy):  " << std::setw(8) << ns << " ns/op" << std::endl;
        REQUIRE(result.size() == ARRAY_SIZE);
        REQUIRE(result[0] == 0);
        REQUIRE(result[1] == 12345);
    }

    // Simulate old loop-based deserialize for comparison
    {
        FBE::FBEBuffer buffer;
        buffer.allocate(4 + ARRAY_SIZE * sizeof(int32_t) + 100);
        FBE::FinalModelVector<int32_t> model(buffer, 0);
        model.set(int32_source);
        uint8_t* data = buffer.data();

        FBE::FastVec<int32_t> result;
        result.reserve(ARRAY_SIZE);
        double ns = benchmark("int32[] deserialize (loop)", ITERATIONS, [&]() {
            result.clear();
            result.resize(ARRAY_SIZE);
            int32_t* src = (int32_t*)(data + 4);
            for (size_t i = 0; i < ARRAY_SIZE; ++i) {
                result[i] = src[i];
            }
        });
        std::cout << "int32[" << ARRAY_SIZE << "] deserialize (loop):    " << std::setw(8) << ns << " ns/op" << std::endl;
    }

    // FinalModelVector verify (optimized O(1) check)
    {
        FBE::FBEBuffer buffer;
        buffer.allocate(4 + ARRAY_SIZE * sizeof(int32_t) + 100);
        FBE::FinalModelVector<int32_t> model(buffer, 0);
        model.set(int32_source);

        double ns = benchmark("int32[] Final verify", ITERATIONS, [&]() {
            model.fbe_offset(0);
            auto size = model.verify();
            (void)size;
        });
        std::cout << "int32[" << ARRAY_SIZE << "] verify (O(1)):         " << std::setw(8) << ns << " ns/op" << std::endl;
    }

    // double array tests
    std::cout << std::endl;

    // FinalModelVector<double> serialize (memcpy)
    {
        FBE::FBEBuffer buffer;
        buffer.allocate(4 + ARRAY_SIZE * sizeof(double) + 100);
        FBE::FinalModelVector<double> model(buffer, 0);

        double ns = benchmark("double[] Final serialize", ITERATIONS, [&]() {
            model.fbe_offset(0);
            model.set(double_source);
        });
        std::cout << "double[" << ARRAY_SIZE << "] serialize (memcpy):   " << std::setw(8) << ns << " ns/op" << std::endl;
    }

    // FinalModelVector<double> deserialize (memcpy)
    {
        FBE::FBEBuffer buffer;
        buffer.allocate(4 + ARRAY_SIZE * sizeof(double) + 100);
        FBE::FinalModelVector<double> model(buffer, 0);
        model.set(double_source);

        FBE::FastVec<double> result;
        result.reserve(ARRAY_SIZE);
        double ns = benchmark("double[] Final deserialize", ITERATIONS, [&]() {
            result.clear();
            model.fbe_offset(0);
            model.get(result);
        });
        std::cout << "double[" << ARRAY_SIZE << "] deserialize (memcpy): " << std::setw(8) << ns << " ns/op" << std::endl;
        REQUIRE(result.size() == ARRAY_SIZE);
    }

    std::cout << "======================================" << std::endl;

    REQUIRE(true);
}

TEST_CASE("Benchmark: Complex Types Performance", "[FBE][Benchmark][Complex]")
{
    constexpr int ITERATIONS = 100000;

    std::cout << "\n=== FBE Complex Types Benchmark ===" << std::endl;
    std::cout << "Iterations: " << ITERATIONS << std::endl;
    std::cout << std::fixed << std::setprecision(2);

    // StructArray - Fixed arrays of primitives, enums, and structs
    {
        test::StructArray source;
        for (int i = 0; i < 2; i++) source.f1[i] = static_cast<uint8_t>(i);
        for (int i = 0; i < 2; i++) source.f2[i] = static_cast<uint8_t>(i + 10);
        for (int i = 0; i < 2; i++) source.f3[i] = FBE::buffer_t(100, static_cast<uint8_t>(i));
        for (int i = 0; i < 2; i++) source.f5[i] = static_cast<test::EnumSimple>(i % 3);
        for (int i = 0; i < 2; i++) source.f7[i] = test::FlagsSimple::FLAG_VALUE_1;
        test::StructSimple s1, s2;
        s1.id = 1; s2.id = 2;
        source.f9[0] = s1; source.f9[1] = s2;

        FBE::test::StructArrayModel writer;
        double ns_ser = benchmark("StructArray serialize", ITERATIONS, [&]() {
            writer.reset();
            writer.serialize(source, nullptr);
        });
        std::cout << "StructArray serialize:     " << std::setw(10) << ns_ser << " ns/op" << std::endl;

        FBE::test::StructArrayModel reader;
        reader.attach(writer.buffer());
        test::StructArray result;
        double ns_de = benchmark("StructArray deserialize", ITERATIONS, [&]() {
            reader.deserialize(result, nullptr);
        });
        std::cout << "StructArray deserialize:   " << std::setw(10) << ns_de << " ns/op" << std::endl;
    }

    // StructMap - Maps with int32_t keys and various value types
    {
        test::StructMap source;
        for (int i = 0; i < 50; i++) {
            source.f1[static_cast<int32_t>(i)] = static_cast<uint8_t>(i & 0xFF);
            source.f2[static_cast<int32_t>(i)] = static_cast<uint8_t>(i);
            source.f3[static_cast<int32_t>(i)] = FBE::buffer_t(100, static_cast<uint8_t>(i));
            source.f4[static_cast<int32_t>(i)] = FBE::buffer_t(50, static_cast<uint8_t>(i));
        }

        FBE::test::StructMapModel writer;
        double ns_ser = benchmark("StructMap serialize (50 entries)", ITERATIONS / 10, [&]() {
            writer.reset();
            writer.serialize(source, nullptr);
        });
        std::cout << "StructMap serialize:       " << std::setw(10) << ns_ser << " ns/op (50 entries)" << std::endl;

        FBE::test::StructMapModel reader;
        reader.attach(writer.buffer());
        test::StructMap result;
        double ns_de = benchmark("StructMap deserialize (50 entries)", ITERATIONS / 10, [&]() {
            result.f1.clear(); result.f2.clear(); result.f3.clear(); result.f4.clear();
            reader.deserialize(result, nullptr);
        });
        std::cout << "StructMap deserialize:     " << std::setw(10) << ns_de << " ns/op (50 entries)" << std::endl;
    }

    // StructHash - Hash maps with string keys and various value types
    {
        test::StructHash source;
        for (int i = 0; i < 50; i++) {
            std::string key = "key" + std::to_string(i);
            source.f1[key] = static_cast<uint8_t>(i & 0xFF);
            source.f2[key] = static_cast<uint8_t>(i);
            source.f3[key] = FBE::buffer_t(100, static_cast<uint8_t>(i));
            source.f4[key] = FBE::buffer_t(50, static_cast<uint8_t>(i));
        }

        FBE::test::StructHashModel writer;
        double ns_ser = benchmark("StructHash serialize (50 entries)", ITERATIONS / 10, [&]() {
            writer.reset();
            writer.serialize(source, nullptr);
        });
        std::cout << "StructHash serialize:      " << std::setw(10) << ns_ser << " ns/op (50 entries)" << std::endl;

        FBE::test::StructHashModel reader;
        reader.attach(writer.buffer());
        test::StructHash result;
        double ns_de = benchmark("StructHash deserialize (50 entries)", ITERATIONS / 10, [&]() {
            result.f1.clear(); result.f2.clear(); result.f3.clear(); result.f4.clear();
            reader.deserialize(result, nullptr);
        });
        std::cout << "StructHash deserialize:    " << std::setw(10) << ns_de << " ns/op (50 entries)" << std::endl;
    }

    // StructVector - Vector of primitives, enums, and structs
    {
        test::StructVector source;
        for (int i = 0; i < 100; i++) {
            source.f1.push_back(static_cast<uint8_t>(i & 0xFF));
            source.f5.push_back(static_cast<test::EnumSimple>(i % 3));
            test::StructSimple s;
            s.id = i;
            s.f1 = (i % 2) != 0;
            s.f2 = 'A' + (i % 26);
            s.f25 = static_cast<float>(i) * 1.5f;
            source.f9.push_back(s);
        }

        FBE::test::StructVectorModel writer;
        double ns_ser = benchmark("StructVector serialize (100 structs)", ITERATIONS / 10, [&]() {
            writer.reset();
            writer.serialize(source, nullptr);
        });
        std::cout << "StructVector (structs):    " << std::setw(10) << ns_ser << " ns/op serialize" << std::endl;

        FBE::test::StructVectorModel reader;
        reader.attach(writer.buffer());
        test::StructVector result;
        double ns_de = benchmark("StructVector deserialize (100 structs)", ITERATIONS / 10, [&]() {
            result.f1.clear(); result.f5.clear(); result.f9.clear();
            reader.deserialize(result, nullptr);
        });
        std::cout << "StructVector (structs):    " << std::setw(10) << ns_de << " ns/op deserialize" << std::endl;
    }

    // Deep nested structure
    {
        test::StructNested source;
        source.f1 = true;
        source.f2 = 42;
        // StructNested contains deeply nested StructOptional, StructSimple, etc.

        FBE::test::StructNestedModel writer;
        double ns_ser = benchmark("StructNested serialize", ITERATIONS, [&]() {
            writer.reset();
            writer.serialize(source, nullptr);
        });
        std::cout << "StructNested serialize:    " << std::setw(10) << ns_ser << " ns/op (deep nesting)" << std::endl;

        FBE::test::StructNestedModel reader;
        reader.attach(writer.buffer());
        test::StructNested result;
        double ns_de = benchmark("StructNested deserialize", ITERATIONS, [&]() {
            reader.deserialize(result, nullptr);
        });
        std::cout << "StructNested deserialize:  " << std::setw(10) << ns_de << " ns/op (deep nesting)" << std::endl;
    }

    // Compare primitive vector vs struct vector (same element count)
    {
        constexpr size_t N = 100;

        // Primitive vector
        test::StructVector prim_source;
        for (size_t i = 0; i < N; i++) {
            prim_source.f1.push_back(static_cast<uint8_t>(i));
        }
        FBE::test::StructVectorModel prim_writer;
        double prim_ns = benchmark("Primitive vector serialize", ITERATIONS, [&]() {
            prim_writer.reset();
            prim_writer.serialize(prim_source, nullptr);
        });
        std::cout << "\nVector comparison (100 elements):" << std::endl;
        std::cout << "  uint8_t[] serialize:     " << std::setw(10) << prim_ns << " ns/op" << std::endl;

        // Struct vector (same count)
        test::StructVector struct_source;
        for (size_t i = 0; i < N; i++) {
            test::StructSimple s;
            s.id = static_cast<int32_t>(i);
            struct_source.f9.push_back(s);
        }
        FBE::test::StructVectorModel struct_writer;
        double struct_ns = benchmark("Struct vector serialize", ITERATIONS / 10, [&]() {
            struct_writer.reset();
            struct_writer.serialize(struct_source, nullptr);
        });
        std::cout << "  StructSimple[] serialize:" << std::setw(10) << struct_ns << " ns/op" << std::endl;
        std::cout << "  Ratio (struct/primitive):" << std::setw(10) << (struct_ns / prim_ns) << "x" << std::endl;
    }

    std::cout << "======================================" << std::endl;

    REQUIRE(true);
}
