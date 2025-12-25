/*
* Copyright (C) 2020 Beijing Jinyi Data Technology Co., Ltd. All rights reserved.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
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
