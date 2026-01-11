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
#include <string>

#include "../proto/arena_pmr_models.h"
#include "../proto/arena_ptr_ptr_pmr_models.h"
#include "arena/arena.hpp"
#include "arena_pmr.h"
#include "arena_ptr_ptr_pmr.h"
#include "catch2/catch.hpp"
#include "fbe.h"
#include "test.h"

using arena::Arena;
using arena::ArenaContainStatus;

class alloc_class {
public:
  alloc_class()
      : init(static_cast<char *>(malloc(4 * 1024 * 1024))), curr(init) {}
  virtual ~alloc_class() { free(init); }

  virtual auto alloc(uint64_t size) -> void * {
    alloc_sizes.push_back(size);
    void *ret = static_cast<void *>(curr);
    ptrs.push_back(ret);

    curr += size;
    return ret;
  }

  void dealloc(void *ptr) { free_ptrs.push_back(ptr); }

  void reset() {
    alloc_sizes.clear();
    ptrs.clear();
    free_ptrs.clear();
  }

  // private:
  std::vector<uint64_t> alloc_sizes;
  std::vector<void *> ptrs;
  std::vector<void *> free_ptrs;
  char *init{nullptr};
  char *curr{nullptr};
};

class ArenaTest {
public:
  Arena::Options ops_simple;

  thread_local static alloc_class *mock;

  static auto mock_alloc(std::size_t size) -> void * {
    return mock->alloc(size);
  }

  static void mock_dealloc(void *ptr) { return mock->dealloc(ptr); }

  ArenaTest() {
    // initialize the ops_simple
    ops_simple.block_alloc = &mock_alloc;
    ops_simple.block_dealloc = &mock_dealloc;
    ops_simple.normal_block_size = 1024ULL;
    ops_simple.suggested_init_block_size = 0ULL;
    ops_simple.huge_block_size = 0ULL;
  }

  template <typename Func> auto test_fn_with_allocs(Func &&func) {
    mock = new alloc_class;
    auto *arena = new Arena(ops_simple);

    REQUIRE(mock->ptrs.size() == 0);
    REQUIRE(mock->free_ptrs.size() == 0);

    func(*arena);

    delete arena;
    REQUIRE(mock->ptrs.size() != 0);
    REQUIRE(mock->free_ptrs.size() != 0);
    delete mock;
  }
};

thread_local alloc_class *ArenaTest::mock = nullptr;

// 1. template arena simple test
// 2. ptr arena simple test
// 2. ptr import template cstr
// 3. template import template cstr
// 4. ptr import ptr import

// bytes
// vector
// map,
// struct
// variant and so on

TEST_CASE_METHOD(ArenaTest, "Arena (template import template)",
                 "[template-based FBE]") {
  test_fn_with_allocs([](Arena &arena) {
    ::arena_common_pmr::Expression expr(arena.get_memory_resource());
    expr.keys.reserve(1);
    expr.aliases.reserve(1);

    FBE::ArenaString name("an alias name", arena.get_memory_resource());
    expr.keys.emplace_back(std::move(name));

    ::arena_common_pmr::Alias alias(arena.get_memory_resource());
    alias.name.assign("an alias name 2");
    alias.optr = ::arena_common_pmr::Optr::GE;
    expr.aliases.emplace_back(std::move(alias));

    ::arena_common_pmr::Alias alias2(arena.get_memory_resource());
    alias2.name.assign("an alias name 3");
    alias2.optr = ::arena_common_pmr::Optr::GT;
    expr.alias_int.emplace(24, std::move(alias2));

    auto *item = arena.Create<::arena_pmr::Item>();
    item->optr = ::arena_common_pmr::Optr::LE;
    item->alias.name.assign("an alias name for item");
    item->alias.optr = ::arena_common_pmr::Optr::LT;
    item->alias.expr.emplace<int32_t>(42);
    item->expressions.emplace_back(std::move(expr));

    REQUIRE(arena.check(reinterpret_cast<char *>(&item->optr)) ==
            ArenaContainStatus::BlockUsed);
    REQUIRE(arena.check(reinterpret_cast<char *>(item->alias.name.data())) ==
            ArenaContainStatus::BlockUsed);
    REQUIRE(arena.check(reinterpret_cast<char *>(&item->alias.optr)) ==
            ArenaContainStatus::BlockUsed);
    REQUIRE(arena.check(reinterpret_cast<char *>(&item->alias.expr)) ==
            ArenaContainStatus::BlockUsed);
    REQUIRE(arena.check(reinterpret_cast<char *>(&item->expressions)) ==
            ArenaContainStatus::BlockUsed);
    REQUIRE(arena.check(reinterpret_cast<char *>(&item->aliases_int)) ==
            ArenaContainStatus::BlockUsed);
    auto &item_expr = item->expressions.at(0);
    REQUIRE(arena.check(reinterpret_cast<char *>(
                item_expr.keys.at(0).data())) == ArenaContainStatus::BlockUsed);
    REQUIRE(arena.check(reinterpret_cast<char *>(
                item_expr.aliases.at(0).name.data())) ==
            ArenaContainStatus::BlockUsed);
    REQUIRE(
        arena.check(reinterpret_cast<char *>(&item_expr.aliases.at(0).optr)) ==
        ArenaContainStatus::BlockUsed);
    REQUIRE(arena.check(reinterpret_cast<char *>(
                item_expr.alias_int.at(24).name.data())) ==
            ArenaContainStatus::BlockUsed);
    REQUIRE(arena.check(
                reinterpret_cast<char *>(&item_expr.alias_int.at(24).optr)) ==
            ArenaContainStatus::BlockUsed);

    ::arena_common_pmr::Alias alias4(arena.get_memory_resource());
    alias4.name.assign("an alias name 4");
    alias4.optr = ::arena_common_pmr::Optr::EQ;
    item->aliases_int.emplace(42, std::move(alias4));

    FBE::arena_pmr::ItemModel item_writer;
    size_t serialized_info =
        item_writer.serialize(*item, arena.get_memory_resource());
    REQUIRE(serialized_info == item_writer.buffer().size());
    REQUIRE(item_writer.verify());

    FBE::arena_pmr::ItemModel item_reader;
    item_reader.attach(item_writer.buffer());
    REQUIRE(item_reader.verify());

    ::arena_pmr::Item copy;
    size_t deserialized_info =
        item_reader.deserialize(copy, arena.get_memory_resource());
    REQUIRE(deserialized_info == item_reader.buffer().size());

    REQUIRE(copy.optr == ::arena_common_pmr::Optr::LE);
    REQUIRE(copy.alias.name == "an alias name for item");
    REQUIRE(copy.alias.optr == ::arena_common_pmr::Optr::LT);
    REQUIRE(copy.alias.expr.index() == 2);
    REQUIRE(std::get<2>(copy.alias.expr) == 42);
    REQUIRE(copy.aliases_int.size() == 1);
    REQUIRE(copy.aliases_int.at(42).name == "an alias name 4");
    REQUIRE(copy.aliases_int.at(42).optr == ::arena_common_pmr::Optr::EQ);

    REQUIRE(copy.expressions.size() == 1);
    auto &expression = copy.expressions.at(0);
    REQUIRE(expression.keys.size() == 1);
    REQUIRE(expression.keys.at(0) == "an alias name");
    REQUIRE(expression.aliases.size() == 1);
    REQUIRE(expression.aliases.at(0).name == "an alias name 2");
    REQUIRE(expression.aliases.at(0).optr == ::arena_common_pmr::Optr::GE);
    REQUIRE(expression.alias_int.size() == 1);
    REQUIRE(expression.alias_int.at(24).name == "an alias name 3");
    REQUIRE(expression.alias_int.at(24).optr == ::arena_common_pmr::Optr::GT);
    REQUIRE(*item == copy);
  });
}

TEST_CASE_METHOD(ArenaTest, "Arena (ptr import template)", "[ptr-based FBE]") {
  test_fn_with_allocs([](Arena &arena) {
    auto *line = arena.Create<::arena_ptr_pmr::Line>();

    line->expression.keys.reserve(1);
    line->expression.aliases.reserve(1);

    FBE::ArenaString name("an alias name", arena.get_memory_resource());
    line->expression.keys.emplace_back(std::move(name));

    ::arena_common_pmr::Alias alias(arena.get_memory_resource());
    alias.name.assign("an alias name 2");
    alias.optr = ::arena_common_pmr::Optr::GE;
    line->expression.aliases.emplace_back(std::move(alias));

    ::arena_common_pmr::Alias alias2(arena.get_memory_resource());
    alias2.name.assign("an alias name 3");
    alias2.optr = ::arena_common_pmr::Optr::GT;
    line->expression.alias_int.emplace(24, std::move(alias2));

    ::arena_common_pmr::Alias alias4(arena.get_memory_resource());
#if defined(USING_BTREE_MAP)
    // Use short string (SSO) with btree_map to avoid memory leak.
    // btree_map's slot assignment doesn't propagate PMR allocators,
    // so long strings get allocated with default allocator and leak
    // when arena is destroyed (arena doesn't call destructors).
    alias4.name.assign("alias name 4");
#else
    alias4.name.assign("an alias name long enough to alloc on arena");
#endif
    alias4.optr = ::arena_common_pmr::Optr::EQ;
    line->expression.alias_int.emplace(42, std::move(alias4));

    REQUIRE(arena.check(reinterpret_cast<char *>(&line->expression)) ==
            ArenaContainStatus::BlockUsed);
    auto &line_expr = line->expression;
    REQUIRE(arena.check(reinterpret_cast<char *>(
                line_expr.keys.at(0).data())) == ArenaContainStatus::BlockUsed);
    REQUIRE(arena.check(reinterpret_cast<char *>(
                line_expr.aliases.at(0).name.data())) ==
            ArenaContainStatus::BlockUsed);
    REQUIRE(
        arena.check(reinterpret_cast<char *>(&line_expr.aliases.at(0).optr)) ==
        ArenaContainStatus::BlockUsed);
    // Short string uses SSO - data() points into the Alias object which is in
    // btree node (arena allocated)
    REQUIRE(arena.check(reinterpret_cast<char *>(
                line_expr.alias_int.at(24).name.data())) ==
            ArenaContainStatus::BlockUsed);
    REQUIRE(arena.check(
                reinterpret_cast<char *>(&line_expr.alias_int.at(24).optr)) ==
            ArenaContainStatus::BlockUsed);
#if defined(USING_BTREE_MAP)
    // With btree_map we use short string (SSO) - data() points into Alias
    // object in btree node
    REQUIRE(arena.check(reinterpret_cast<char *>(
                line_expr.alias_int.at(42).name.data())) ==
            ArenaContainStatus::BlockUsed);
#else
    // Long string exceeds SSO - data() points to heap buffer allocated from
    // arena
    REQUIRE(arena.check(reinterpret_cast<char *>(
                line_expr.alias_int.at(42).name.data())) ==
            ArenaContainStatus::BlockUsed);
#endif
    REQUIRE(arena.check(
                reinterpret_cast<char *>(&line_expr.alias_int.at(42).optr)) ==
            ArenaContainStatus::BlockUsed);

    FBE::arena_ptr_pmr::LineModel line_writer;
    size_t serialized_info =
        line_writer.serialize(*line, arena.get_memory_resource());
    REQUIRE(serialized_info == line_writer.buffer().size());
    REQUIRE(line_writer.verify());

    FBE::arena_ptr_pmr::LineModel line_reader;
    line_reader.attach(line_writer.buffer());
    REQUIRE(line_reader.verify());

    ::arena_ptr_pmr::Line line_copy;
    size_t deserialized_info =
        line_reader.deserialize(line_copy, arena.get_memory_resource());
    REQUIRE(deserialized_info == line_reader.buffer().size());

    auto &expression = line_copy.expression;
    REQUIRE(expression.keys.size() == 1);
    REQUIRE(expression.keys.at(0) == "an alias name");
    REQUIRE(expression.aliases.size() == 1);
    REQUIRE(expression.aliases.at(0).name == "an alias name 2");
    REQUIRE(expression.aliases.at(0).optr == ::arena_common_pmr::Optr::GE);
    REQUIRE(expression.alias_int.size() == 2);
    REQUIRE(expression.alias_int.at(24).name == "an alias name 3");
    REQUIRE(expression.alias_int.at(24).optr == ::arena_common_pmr::Optr::GT);
#if defined(USING_BTREE_MAP)
    REQUIRE(expression.alias_int.at(42).name == "alias name 4");
#else
    REQUIRE(expression.alias_int.at(42).name ==
            "an alias name long enough to alloc on arena");
#endif
    REQUIRE(expression.alias_int.at(42).optr == ::arena_common_pmr::Optr::EQ);
    REQUIRE(*line == line_copy);
  });
}

TEST_CASE_METHOD(ArenaTest, "Arena (pmr::bytes)", "[ptr-based FBE]") {
  test_fn_with_allocs([](Arena &arena) {
    auto *line2 = arena.Create<::arena_ptr_pmr::Line2>();

    std::pmr::vector<uint8_t> bytes_v{{65, 66, 67, 68, 69},
                                      arena.get_memory_resource()};
    FBE::pmr_buffer_t buffer(std::move(bytes_v));
    line2->bytes_v = std::move(buffer);

    REQUIRE(arena.check(reinterpret_cast<char *>(&line2->bytes_v)) ==
            ArenaContainStatus::BlockUsed);
    REQUIRE(arena.check(reinterpret_cast<char *>(line2->bytes_v.data())) ==
            ArenaContainStatus::BlockUsed);

    FBE::arena_ptr_pmr::Line2Model line_writer;
    size_t serialized_info =
        line_writer.serialize(*line2, arena.get_memory_resource());
    REQUIRE(serialized_info == line_writer.buffer().size());
    CHECK(line_writer.verify());

    FBE::arena_ptr_pmr::Line2Model line_reader;
    line_reader.attach(line_writer.buffer());
    CHECK(line_reader.verify());

    ::arena_ptr_pmr::Line2 copy_line;
    size_t deserialized_info =
        line_reader.deserialize(copy_line, arena.get_memory_resource());
    REQUIRE(deserialized_info == line_reader.buffer().size());

    REQUIRE(line2->bytes_v.string() == "ABCDE");
    REQUIRE(*line2 == copy_line);
  });
}

// ============================================================================
// Regular Mode + Arena + Variant Tests
// These tests verify FieldModel serialization with variants in Arena context
// ============================================================================

TEST_CASE_METHOD(ArenaTest, "Arena Regular with variant (monostate)",
                 "[template-based FBE]") {
  test_fn_with_allocs([](Arena &arena) {
    auto *alias = arena.Create<::arena_common_pmr::Alias>();
    alias->name.assign("test alias");
    alias->optr = ::arena_common_pmr::Optr::EQ;
    // expr defaults to monostate (index 0)
    REQUIRE(alias->expr.index() == 0);

    FBE::arena_common_pmr::AliasModel writer;
    size_t serialized = writer.serialize(*alias, arena.get_memory_resource());
    REQUIRE(serialized == writer.buffer().size());
    REQUIRE(writer.verify());

    FBE::arena_common_pmr::AliasModel reader;
    reader.attach(writer.buffer());
    REQUIRE(reader.verify());

    ::arena_common_pmr::Alias copy(arena.get_memory_resource());
    copy.expr.emplace<int32_t>(999); // set to non-default first
    size_t deserialized = reader.deserialize(copy, arena.get_memory_resource());
    REQUIRE(deserialized == reader.buffer().size());

    REQUIRE(copy.name == "test alias");
    REQUIRE(copy.optr == ::arena_common_pmr::Optr::EQ);
    REQUIRE(copy.expr.index() == 0);
    REQUIRE(std::holds_alternative<std::monostate>(copy.expr));
    REQUIRE(*alias == copy);
  });
}

TEST_CASE_METHOD(ArenaTest, "Arena Regular with variant (bool)",
                 "[template-based FBE]") {
  test_fn_with_allocs([](Arena &arena) {
    auto *alias = arena.Create<::arena_common_pmr::Alias>();
    alias->name.assign("bool test");
    alias->optr = ::arena_common_pmr::Optr::GT;
    alias->expr.emplace<bool>(true);

    FBE::arena_common_pmr::AliasModel writer;
    size_t serialized = writer.serialize(*alias, arena.get_memory_resource());
    REQUIRE(serialized == writer.buffer().size());
    REQUIRE(writer.verify());

    FBE::arena_common_pmr::AliasModel reader;
    reader.attach(writer.buffer());
    REQUIRE(reader.verify());

    ::arena_common_pmr::Alias copy(arena.get_memory_resource());
    size_t deserialized = reader.deserialize(copy, arena.get_memory_resource());
    REQUIRE(deserialized == reader.buffer().size());

    REQUIRE(copy.name == "bool test");
    REQUIRE(copy.optr == ::arena_common_pmr::Optr::GT);
    REQUIRE(copy.expr.index() == 1);
    REQUIRE(std::get<bool>(copy.expr) == true);
    REQUIRE(*alias == copy);
  });
}

TEST_CASE_METHOD(ArenaTest, "Arena Regular with variant (int32)",
                 "[template-based FBE]") {
  test_fn_with_allocs([](Arena &arena) {
    auto *alias = arena.Create<::arena_common_pmr::Alias>();
    alias->name.assign("int test");
    alias->optr = ::arena_common_pmr::Optr::LT;
    alias->expr.emplace<int32_t>(42);

    FBE::arena_common_pmr::AliasModel writer;
    size_t serialized = writer.serialize(*alias, arena.get_memory_resource());
    REQUIRE(serialized == writer.buffer().size());
    REQUIRE(writer.verify());

    FBE::arena_common_pmr::AliasModel reader;
    reader.attach(writer.buffer());
    REQUIRE(reader.verify());

    ::arena_common_pmr::Alias copy(arena.get_memory_resource());
    size_t deserialized = reader.deserialize(copy, arena.get_memory_resource());
    REQUIRE(deserialized == reader.buffer().size());

    REQUIRE(copy.name == "int test");
    REQUIRE(copy.optr == ::arena_common_pmr::Optr::LT);
    REQUIRE(copy.expr.index() == 2);
    REQUIRE(std::get<int32_t>(copy.expr) == 42);
    REQUIRE(*alias == copy);
  });
}

TEST_CASE_METHOD(ArenaTest, "Arena Regular with variant (ArenaString)",
                 "[template-based FBE]") {
  test_fn_with_allocs([](Arena &arena) {
    auto *alias = arena.Create<::arena_common_pmr::Alias>();
    alias->name.assign("string test");
    alias->optr = ::arena_common_pmr::Optr::GE;
    alias->expr.emplace<FBE::ArenaString>(
        FBE::ArenaString("variant string value", arena.get_memory_resource()));

    // Verify string is in arena
    REQUIRE(arena.check(
                reinterpret_cast<char *>(std::get<3>(alias->expr).data())) ==
            ArenaContainStatus::BlockUsed);

    FBE::arena_common_pmr::AliasModel writer;
    size_t serialized = writer.serialize(*alias, arena.get_memory_resource());
    REQUIRE(serialized == writer.buffer().size());
    REQUIRE(writer.verify());

    FBE::arena_common_pmr::AliasModel reader;
    reader.attach(writer.buffer());
    REQUIRE(reader.verify());

    ::arena_common_pmr::Alias copy(arena.get_memory_resource());
    size_t deserialized = reader.deserialize(copy, arena.get_memory_resource());
    REQUIRE(deserialized == reader.buffer().size());

    REQUIRE(copy.name == "string test");
    REQUIRE(copy.optr == ::arena_common_pmr::Optr::GE);
    REQUIRE(copy.expr.index() == 3);
    REQUIRE(std::get<3>(copy.expr) == "variant string value");
    REQUIRE(*alias == copy);
  });
}

// ============================================================================
// Final Mode + Arena Tests
// ============================================================================

#include "../proto/arena_common_final_pmr_models.h"
#include "../proto/arena_final_pmr_models.h"

TEST_CASE_METHOD(ArenaTest, "Arena Final (pmr::bytes)", "[final-based FBE]") {
  test_fn_with_allocs([](Arena &arena) {
    auto *item2 = arena.Create<::arena_pmr::Item2>();

    std::pmr::vector<uint8_t> bytes_v{{65, 66, 67, 68, 69},
                                      arena.get_memory_resource()};
    FBE::pmr_buffer_t buffer(std::move(bytes_v));
    item2->bytes_v = std::move(buffer);

    REQUIRE(arena.check(reinterpret_cast<char *>(&item2->bytes_v)) ==
            ArenaContainStatus::BlockUsed);
    REQUIRE(arena.check(reinterpret_cast<char *>(item2->bytes_v.data())) ==
            ArenaContainStatus::BlockUsed);

    // Serialize using Final model
    FBE::arena_pmr::Item2FinalModel writer;
    size_t serialized = writer.serialize(*item2);
    REQUIRE(serialized == writer.buffer().size());
    REQUIRE(writer.verify());
    writer.next(serialized);

    // Deserialize using Final model with memory resource
    FBE::arena_pmr::Item2FinalModel reader;
    reader.attach(writer.buffer());
    REQUIRE(reader.verify());

    ::arena_pmr::Item2 copy;
    size_t deserialized = reader.deserialize(copy, arena.get_memory_resource());
    REQUIRE(deserialized == reader.buffer().size());

    REQUIRE(item2->bytes_v.string() == "ABCDE");
    REQUIRE(copy.bytes_v.string() == "ABCDE");
    REQUIRE(*item2 == copy);
  });
}

TEST_CASE_METHOD(ArenaTest, "Arena Final vs Regular comparison (Item2)",
                 "[final-based FBE]") {
  // Test that Final and Regular mode produce equivalent results for Item2
  test_fn_with_allocs([](Arena &arena) {
    // Create test data
    auto *item2 = arena.Create<::arena_pmr::Item2>();
    std::pmr::vector<uint8_t> bytes_v{{72, 101, 108, 108, 111},
                                      arena.get_memory_resource()};
    item2->bytes_v = FBE::pmr_buffer_t(std::move(bytes_v));

    // Serialize with Final model
    FBE::arena_pmr::Item2FinalModel final_writer;
    size_t final_serialized = final_writer.serialize(*item2);
    REQUIRE(final_writer.verify());

    // Serialize with Regular model
    FBE::arena_pmr::Item2Model regular_writer;
    size_t regular_serialized =
        regular_writer.serialize(*item2, arena.get_memory_resource());
    REQUIRE(regular_writer.verify());

    // Deserialize from Final model
    FBE::arena_pmr::Item2FinalModel final_reader;
    final_reader.attach(final_writer.buffer());
    ::arena_pmr::Item2 final_copy;
    final_reader.deserialize(final_copy, arena.get_memory_resource());

    // Deserialize from Regular model
    FBE::arena_pmr::Item2Model regular_reader;
    regular_reader.attach(regular_writer.buffer());
    ::arena_pmr::Item2 regular_copy;
    regular_reader.deserialize(regular_copy, arena.get_memory_resource());

    // Both should equal the original
    REQUIRE(item2->bytes_v.string() == "Hello");
    REQUIRE(final_copy.bytes_v.string() == "Hello");
    REQUIRE(regular_copy.bytes_v.string() == "Hello");
    REQUIRE(*item2 == final_copy);
    REQUIRE(*item2 == regular_copy);
    REQUIRE(final_copy == regular_copy);

    // Final model should typically be more compact
    REQUIRE(final_serialized <= regular_serialized);
  });
}

TEST_CASE_METHOD(ArenaTest, "Arena Final empty bytes", "[final-based FBE]") {
  test_fn_with_allocs([](Arena &arena) {
    // Test with empty bytes
    auto *item2 = arena.Create<::arena_pmr::Item2>();
    // bytes_v is empty by default

    // Serialize
    FBE::arena_pmr::Item2FinalModel writer;
    [[maybe_unused]] size_t serialized = writer.serialize(*item2);
    REQUIRE(writer.verify());

    // Deserialize
    FBE::arena_pmr::Item2FinalModel reader;
    reader.attach(writer.buffer());
    REQUIRE(reader.verify());

    ::arena_pmr::Item2 copy;
    reader.deserialize(copy, arena.get_memory_resource());

    REQUIRE(copy.bytes_v.empty());
    REQUIRE(*item2 == copy);
  });
}

TEST_CASE_METHOD(ArenaTest, "Arena Final multiple Item2", "[final-based FBE]") {
  test_fn_with_allocs([](Arena &arena) {
    // Serialize multiple Item2 in sequence
    FBE::arena_pmr::Item2FinalModel writer;

    // First item
    auto *item1 = arena.Create<::arena_pmr::Item2>();
    std::pmr::vector<uint8_t> bytes1{{1, 2, 3}, arena.get_memory_resource()};
    item1->bytes_v = FBE::pmr_buffer_t(std::move(bytes1));
    size_t size1 = writer.serialize(*item1);
    REQUIRE(writer.verify());
    writer.next(size1);

    // Second item
    auto *item2 = arena.Create<::arena_pmr::Item2>();
    std::pmr::vector<uint8_t> bytes2{{4, 5, 6, 7, 8},
                                     arena.get_memory_resource()};
    item2->bytes_v = FBE::pmr_buffer_t(std::move(bytes2));
    size_t size2 = writer.serialize(*item2);
    REQUIRE(writer.verify());

    // Deserialize both
    FBE::arena_pmr::Item2FinalModel reader;
    reader.attach(writer.buffer());

    ::arena_pmr::Item2 copy1;
    size_t deser1 = reader.deserialize(copy1, arena.get_memory_resource());
    REQUIRE(deser1 == size1);
    reader.next(deser1);

    ::arena_pmr::Item2 copy2;
    size_t deser2 = reader.deserialize(copy2, arena.get_memory_resource());
    REQUIRE(deser2 == size2);

    REQUIRE(*item1 == copy1);
    REQUIRE(*item2 == copy2);
    REQUIRE(copy1.bytes_v.size() == 3);
    REQUIRE(copy2.bytes_v.size() == 5);
  });
}

TEST_CASE_METHOD(ArenaTest, "Arena Final large bytes", "[final-based FBE]") {
  test_fn_with_allocs([](Arena &arena) {
    // Test with larger bytes buffer
    auto *item2 = arena.Create<::arena_pmr::Item2>();

    // Create a larger buffer (1KB)
    std::pmr::vector<uint8_t> bytes_v(arena.get_memory_resource());
    bytes_v.reserve(1024);
    for (int i = 0; i < 1024; ++i) {
      bytes_v.push_back(static_cast<uint8_t>(i % 256));
    }
    item2->bytes_v = FBE::pmr_buffer_t(std::move(bytes_v));

    // Serialize
    FBE::arena_pmr::Item2FinalModel writer;
    size_t serialized = writer.serialize(*item2);
    REQUIRE(serialized == writer.buffer().size());
    REQUIRE(writer.verify());

    // Deserialize
    FBE::arena_pmr::Item2FinalModel reader;
    reader.attach(writer.buffer());
    REQUIRE(reader.verify());

    ::arena_pmr::Item2 copy;
    reader.deserialize(copy, arena.get_memory_resource());

    REQUIRE(copy.bytes_v.size() == 1024);
    REQUIRE(*item2 == copy);

    // Verify content
    for (int i = 0; i < 1024; ++i) {
      REQUIRE(copy.bytes_v[i] == static_cast<uint8_t>(i % 256));
    }
  });
}

// ============================================================================
// Final Mode + Arena + Variant Tests
// These tests use Item which contains variants (Alias.expr is a variant type)
// ============================================================================

TEST_CASE_METHOD(ArenaTest, "Arena Final with variant (int)",
                 "[final-based FBE]") {
  test_fn_with_allocs([](Arena &arena) {
    auto *item = arena.Create<::arena_pmr::Item>();
    item->optr = ::arena_common_pmr::Optr::LE;
    item->alias.name.assign("test alias");
    item->alias.optr = ::arena_common_pmr::Optr::LT;
    item->alias.expr.emplace<int32_t>(42); // variant with int

    // Serialize using Final model
    FBE::arena_pmr::ItemFinalModel writer;
    size_t serialized = writer.serialize(*item);
    REQUIRE(serialized == writer.buffer().size());
    REQUIRE(writer.verify());

    // Deserialize using Final model with memory resource
    FBE::arena_pmr::ItemFinalModel reader;
    reader.attach(writer.buffer());
    REQUIRE(reader.verify());

    ::arena_pmr::Item copy;
    size_t deserialized = reader.deserialize(copy, arena.get_memory_resource());
    REQUIRE(deserialized == reader.buffer().size());

    // Verify deserialized data
    REQUIRE(copy.optr == ::arena_common_pmr::Optr::LE);
    REQUIRE(copy.alias.name == "test alias");
    REQUIRE(copy.alias.optr == ::arena_common_pmr::Optr::LT);
    REQUIRE(copy.alias.expr.index() == 2); // int is at index 2
    REQUIRE(std::get<2>(copy.alias.expr) == 42);
    REQUIRE(*item == copy);
  });
}

TEST_CASE_METHOD(ArenaTest, "Arena Final with variant (bool)",
                 "[final-based FBE]") {
  test_fn_with_allocs([](Arena &arena) {
    auto *item = arena.Create<::arena_pmr::Item>();
    item->optr = ::arena_common_pmr::Optr::EQ;
    item->alias.name.assign("bool alias");
    item->alias.optr = ::arena_common_pmr::Optr::GE;
    item->alias.expr.emplace<bool>(true); // variant with bool

    // Serialize using Final model
    FBE::arena_pmr::ItemFinalModel writer;
    [[maybe_unused]] size_t serialized = writer.serialize(*item);
    REQUIRE(writer.verify());

    // Deserialize
    FBE::arena_pmr::ItemFinalModel reader;
    reader.attach(writer.buffer());
    REQUIRE(reader.verify());

    ::arena_pmr::Item copy;
    reader.deserialize(copy, arena.get_memory_resource());

    // Verify
    REQUIRE(copy.alias.expr.index() == 1); // bool is at index 1
    REQUIRE(std::get<1>(copy.alias.expr) == true);
    REQUIRE(*item == copy);
  });
}

TEST_CASE_METHOD(ArenaTest, "Arena Final with variant (ArenaString)",
                 "[final-based FBE]") {
  test_fn_with_allocs([](Arena &arena) {
    auto *item = arena.Create<::arena_pmr::Item>();
    item->optr = ::arena_common_pmr::Optr::GT;
    item->alias.name.assign("string alias");
    item->alias.optr = ::arena_common_pmr::Optr::GT;
    item->alias.expr.emplace<FBE::ArenaString>(
        FBE::ArenaString("variant string value", arena.get_memory_resource()));

    // Serialize using Final model
    FBE::arena_pmr::ItemFinalModel writer;
    [[maybe_unused]] size_t serialized = writer.serialize(*item);
    REQUIRE(writer.verify());

    // Deserialize
    FBE::arena_pmr::ItemFinalModel reader;
    reader.attach(writer.buffer());
    REQUIRE(reader.verify());

    ::arena_pmr::Item copy;
    reader.deserialize(copy, arena.get_memory_resource());

    // Verify - ArenaString is at index 3
    REQUIRE(copy.alias.expr.index() == 3);
    REQUIRE(std::get<3>(copy.alias.expr) == "variant string value");
    REQUIRE(*item == copy);
  });
}

TEST_CASE_METHOD(ArenaTest, "Arena Final with variant (monostate)",
                 "[final-based FBE]") {
  test_fn_with_allocs([](Arena &arena) {
    auto *item = arena.Create<::arena_pmr::Item>();
    item->optr = ::arena_common_pmr::Optr::EQ;
    item->alias.name.assign("monostate alias");
    item->alias.optr = ::arena_common_pmr::Optr::EQ;
    // Default is monostate (index 0)

    // Serialize using Final model
    FBE::arena_pmr::ItemFinalModel writer;
    [[maybe_unused]] size_t serialized = writer.serialize(*item);
    REQUIRE(writer.verify());

    // Deserialize
    FBE::arena_pmr::ItemFinalModel reader;
    reader.attach(writer.buffer());
    REQUIRE(reader.verify());

    ::arena_pmr::Item copy;
    reader.deserialize(copy, arena.get_memory_resource());

    // Verify monostate (index 0)
    REQUIRE(copy.alias.expr.index() == 0);
    REQUIRE(*item == copy);
  });
}

TEST_CASE_METHOD(ArenaTest,
                 "Arena Final with full Item (variants + containers)",
                 "[final-based FBE]") {
  test_fn_with_allocs([](Arena &arena) {
    ::arena_common_pmr::Expression expr(arena.get_memory_resource());
    expr.keys.reserve(2);
    expr.aliases.reserve(1);

    expr.keys.emplace_back(
        FBE::ArenaString("key1", arena.get_memory_resource()));
    expr.keys.emplace_back(
        FBE::ArenaString("key2", arena.get_memory_resource()));

    ::arena_common_pmr::Alias alias(arena.get_memory_resource());
    alias.name.assign("nested alias");
    alias.optr = ::arena_common_pmr::Optr::GE;
    alias.expr.emplace<int32_t>(100);
    expr.aliases.emplace_back(std::move(alias));

    ::arena_common_pmr::Alias alias2(arena.get_memory_resource());
    alias2.name.assign("map alias");
    alias2.optr = ::arena_common_pmr::Optr::GT;
    alias2.expr.emplace<bool>(false);
    expr.alias_int.emplace(24, std::move(alias2));

    auto *item = arena.Create<::arena_pmr::Item>();
    item->optr = ::arena_common_pmr::Optr::LE;
    item->alias.name.assign("main alias");
    item->alias.optr = ::arena_common_pmr::Optr::LT;
    item->alias.expr.emplace<int32_t>(42);
    item->expressions.emplace_back(std::move(expr));

    ::arena_common_pmr::Alias alias4(arena.get_memory_resource());
    alias4.name.assign("int map alias");
    alias4.optr = ::arena_common_pmr::Optr::EQ;
    alias4.expr.emplace<FBE::ArenaString>(
        FBE::ArenaString("string in map", arena.get_memory_resource()));
    item->aliases_int.emplace(42, std::move(alias4));

    // Serialize using Final model
    FBE::arena_pmr::ItemFinalModel writer;
    size_t serialized = writer.serialize(*item);
    REQUIRE(serialized == writer.buffer().size());
    REQUIRE(writer.verify());

    // Deserialize using Final model with memory resource
    FBE::arena_pmr::ItemFinalModel reader;
    reader.attach(writer.buffer());
    REQUIRE(reader.verify());

    ::arena_pmr::Item copy;
    size_t deserialized = reader.deserialize(copy, arena.get_memory_resource());
    REQUIRE(deserialized == reader.buffer().size());

    // Verify deserialized data
    REQUIRE(copy.optr == ::arena_common_pmr::Optr::LE);
    REQUIRE(copy.alias.name == "main alias");
    REQUIRE(copy.alias.optr == ::arena_common_pmr::Optr::LT);
    REQUIRE(copy.alias.expr.index() == 2);
    REQUIRE(std::get<2>(copy.alias.expr) == 42);
    REQUIRE(copy.aliases_int.size() == 1);
    REQUIRE(copy.aliases_int.at(42).name == "int map alias");
    REQUIRE(copy.aliases_int.at(42).expr.index() == 3);
    REQUIRE(std::get<3>(copy.aliases_int.at(42).expr) == "string in map");

    REQUIRE(copy.expressions.size() == 1);
    auto &expression = copy.expressions.at(0);
    REQUIRE(expression.keys.size() == 2);
    REQUIRE(expression.keys.at(0) == "key1");
    REQUIRE(expression.keys.at(1) == "key2");
    REQUIRE(expression.aliases.size() == 1);
    REQUIRE(expression.aliases.at(0).name == "nested alias");
    REQUIRE(expression.aliases.at(0).expr.index() == 2);
    REQUIRE(std::get<2>(expression.aliases.at(0).expr) == 100);
    REQUIRE(expression.alias_int.size() == 1);
    REQUIRE(expression.alias_int.at(24).name == "map alias");
    REQUIRE(expression.alias_int.at(24).expr.index() == 1);
    REQUIRE(std::get<1>(expression.alias_int.at(24).expr) == false);

    REQUIRE(*item == copy);
  });
}
