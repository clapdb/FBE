// Benchmark: Regular vs Final vs Regular+PMR vs Final+PMR
// Using arena_common::Expression (complex struct with vectors, maps, variants)
//
// This is a standalone benchmark to avoid duplicate symbol conflicts with variants_models.cpp

#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>

// Arena
#include "arena/arena.hpp"
using Arena = arena::Arena;

// Include all 4 modes for arena_common and arena
#include "arena_common.h"
#include "arena_common_pmr.h"
#include "arena_common_models.h"
#include "arena_common_pmr_models.h"
#include "arena_common_final_models.h"
#include "arena_common_final_pmr_models.h"
#include "arena.h"
#include "arena_pmr.h"
#include "arena_models.h"
#include "arena_pmr_models.h"
#include "arena_final_models.h"
#include "arena_final_pmr_models.h"

template <typename Func>
double run_benchmark(const char *name, int iterations, Func &&func) {
  // Warmup
  for (int i = 0; i < 100; i++) {
    func();
  }

  auto start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < iterations; i++) {
    func();
  }
  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
  return static_cast<double>(duration) / iterations;
}

// Create complex Expression for non-PMR (Regular/Final) modes
static ::arena_common::Expression CreateComplexExpression() {
  ::arena_common::Expression expr;
  expr.keys.reserve(10);
  expr.aliases.reserve(5);

  for (int j = 0; j < 10; ++j) {
    expr.keys.emplace_back("key_" + std::to_string(j) + "_with_longer_name");
  }

  for (int j = 0; j < 5; ++j) {
    ::arena_common::Alias alias;
    alias.name = "nested_alias_" + std::to_string(j) + "_with_description";
    alias.optr = static_cast<::arena_common::Optr>(j % 6);

    switch (j % 4) {
      case 0: alias.expr.emplace<bool>(j % 2 == 0); break;
      case 1: alias.expr.emplace<int32_t>(j * 100); break;
      case 2: alias.expr.emplace<std::string>("string_variant_value_" + std::to_string(j)); break;
      case 3: break; // monostate
    }
    expr.aliases.emplace_back(std::move(alias));
  }

  for (int j = 0; j < 5; ++j) {
    ::arena_common::Alias map_alias;
    map_alias.name = "map_alias_" + std::to_string(j);
    map_alias.optr = ::arena_common::Optr::GT;
    map_alias.expr.emplace<int32_t>(j * 10);
    expr.alias_int.emplace(j * 100, std::move(map_alias));
  }

  return expr;
}

// Create deeply nested Item for non-PMR modes
static ::arena::Item CreateDeeplyNestedItem() {
  ::arena::Item item;
  item.optr = ::arena_common::Optr::LE;

  // Main alias with nested variant
  item.alias.name = "main_alias_with_long_name";
  item.alias.optr = ::arena_common::Optr::GT;
  item.alias.expr.emplace<std::string>("variant_string_value_in_alias");

  // Add multiple expressions (each with vectors, maps, and nested aliases)
  for (int i = 0; i < 10; ++i) {
    ::arena_common::Expression expr;
    expr.keys.reserve(20);
    expr.aliases.reserve(10);

    // 20 keys per expression
    for (int j = 0; j < 20; ++j) {
      expr.keys.emplace_back("key_" + std::to_string(i) + "_" + std::to_string(j) + "_long_name");
    }

    // 10 nested aliases per expression (with variants)
    for (int j = 0; j < 10; ++j) {
      ::arena_common::Alias alias;
      alias.name = "nested_alias_" + std::to_string(i) + "_" + std::to_string(j);
      alias.optr = static_cast<::arena_common::Optr>(j % 6);
      switch (j % 4) {
        case 0: alias.expr.emplace<bool>(j % 2 == 0); break;
        case 1: alias.expr.emplace<int32_t>(j * 100 + i); break;
        case 2: alias.expr.emplace<std::string>("variant_str_" + std::to_string(j)); break;
        case 3: break;
      }
      expr.aliases.emplace_back(std::move(alias));
    }

    // 5 map entries per expression
    for (int j = 0; j < 5; ++j) {
      ::arena_common::Alias map_alias;
      map_alias.name = "map_alias_" + std::to_string(j);
      map_alias.optr = ::arena_common::Optr::EQ;
      map_alias.expr.emplace<int32_t>(j * 10);
      expr.alias_int.emplace(i * 100 + j, std::move(map_alias));
    }

    item.expressions.emplace_back(std::move(expr));
  }

  // 10 top-level map entries
  for (int i = 0; i < 10; ++i) {
    ::arena_common::Alias alias;
    alias.name = "top_level_alias_" + std::to_string(i);
    alias.optr = ::arena_common::Optr::GE;
    alias.expr.emplace<std::string>("top_level_string_" + std::to_string(i));
    item.aliases_int.emplace(i, std::move(alias));
  }

  return item;
}

// Create deeply nested Item for PMR modes
static ::arena_pmr::Item CreateDeeplyNestedItemPmr(std::pmr::memory_resource *resource) {
  ::arena_pmr::Item item(resource);
  item.optr = ::arena_common_pmr::Optr::LE;

  // Main alias with nested variant
  item.alias.name.assign("main_alias_with_long_name");
  item.alias.optr = ::arena_common_pmr::Optr::GT;
  item.alias.expr.emplace<FBE::ArenaString>(
      FBE::ArenaString("variant_string_value_in_alias", resource));

  // Add multiple expressions (each with vectors, maps, and nested aliases)
  for (int i = 0; i < 10; ++i) {
    ::arena_common_pmr::Expression expr(resource);
    expr.keys.reserve(20);
    expr.aliases.reserve(10);

    // 20 keys per expression
    for (int j = 0; j < 20; ++j) {
      std::string key = "key_" + std::to_string(i) + "_" + std::to_string(j) + "_long_name";
      expr.keys.emplace_back(FBE::ArenaString(key.c_str(), resource));
    }

    // 10 nested aliases per expression (with variants)
    for (int j = 0; j < 10; ++j) {
      ::arena_common_pmr::Alias alias(resource);
      std::string name = "nested_alias_" + std::to_string(i) + "_" + std::to_string(j);
      alias.name.assign(name.c_str());
      alias.optr = static_cast<::arena_common_pmr::Optr>(j % 6);
      switch (j % 4) {
        case 0: alias.expr.emplace<bool>(j % 2 == 0); break;
        case 1: alias.expr.emplace<int32_t>(j * 100 + i); break;
        case 2: {
          std::string val = "variant_str_" + std::to_string(j);
          alias.expr.emplace<FBE::ArenaString>(FBE::ArenaString(val.c_str(), resource));
          break;
        }
        case 3: break;
      }
      expr.aliases.emplace_back(std::move(alias));
    }

    // 5 map entries per expression
    for (int j = 0; j < 5; ++j) {
      ::arena_common_pmr::Alias map_alias(resource);
      std::string name = "map_alias_" + std::to_string(j);
      map_alias.name.assign(name.c_str());
      map_alias.optr = ::arena_common_pmr::Optr::EQ;
      map_alias.expr.emplace<int32_t>(j * 10);
      expr.alias_int.emplace(i * 100 + j, std::move(map_alias));
    }

    item.expressions.emplace_back(std::move(expr));
  }

  // 10 top-level map entries
  for (int i = 0; i < 10; ++i) {
    ::arena_common_pmr::Alias alias(resource);
    std::string name = "top_level_alias_" + std::to_string(i);
    alias.name.assign(name.c_str());
    alias.optr = ::arena_common_pmr::Optr::GE;
    std::string val = "top_level_string_" + std::to_string(i);
    alias.expr.emplace<FBE::ArenaString>(FBE::ArenaString(val.c_str(), resource));
    item.aliases_int.emplace(i, std::move(alias));
  }

  return item;
}

// Create complex Expression for PMR modes
static ::arena_common_pmr::Expression
CreateComplexExpressionPmr(std::pmr::memory_resource *resource) {
  ::arena_common_pmr::Expression expr(resource);
  expr.keys.reserve(10);
  expr.aliases.reserve(5);

  for (int j = 0; j < 10; ++j) {
    std::string key = "key_" + std::to_string(j) + "_with_longer_name";
    expr.keys.emplace_back(FBE::ArenaString(key.c_str(), resource));
  }

  for (int j = 0; j < 5; ++j) {
    ::arena_common_pmr::Alias alias(resource);
    std::string name = "nested_alias_" + std::to_string(j) + "_with_description";
    alias.name.assign(name.c_str());
    alias.optr = static_cast<::arena_common_pmr::Optr>(j % 6);

    switch (j % 4) {
      case 0: alias.expr.emplace<bool>(j % 2 == 0); break;
      case 1: alias.expr.emplace<int32_t>(j * 100); break;
      case 2: {
        std::string val = "string_variant_value_" + std::to_string(j);
        alias.expr.emplace<FBE::ArenaString>(FBE::ArenaString(val.c_str(), resource));
        break;
      }
      case 3: break; // monostate
    }
    expr.aliases.emplace_back(std::move(alias));
  }

  for (int j = 0; j < 5; ++j) {
    ::arena_common_pmr::Alias map_alias(resource);
    std::string name = "map_alias_" + std::to_string(j);
    map_alias.name.assign(name.c_str());
    map_alias.optr = ::arena_common_pmr::Optr::GT;
    map_alias.expr.emplace<int32_t>(j * 10);
    expr.alias_int.emplace(j * 100, std::move(map_alias));
  }

  return expr;
}

int main() {
  constexpr int ITERATIONS = 10000;

  // Create Arena for PMR modes
  Arena::Options arena_opts;
  arena_opts.normal_block_size = 64 * 1024;
  arena_opts.huge_block_size = 1024 * 1024;
  arena_opts.suggested_init_block_size = 64 * 1024;
  arena_opts.block_alloc = [](std::size_t size) -> void* { return std::malloc(size); };
  arena_opts.block_dealloc = [](void* ptr) { std::free(ptr); };
  Arena arena(arena_opts);

  // Create test data for all modes
  auto expr = CreateComplexExpression();
  auto expr_pmr = CreateComplexExpressionPmr(arena.get_memory_resource());

  // ==================== SERIALIZE BENCHMARKS ====================
  // Note: Serialize doesn't allocate memory, so Arena doesn't affect performance
  // We use the same pre-created arena for PMR modes

  // Regular serialize
  double regular_serialize = run_benchmark("Regular serialize", ITERATIONS, [&]() {
    FBE::arena_common::ExpressionModel writer;
    writer.serialize(expr, nullptr);
  });

  // Final serialize
  double final_serialize = run_benchmark("Final serialize", ITERATIONS, [&]() {
    FBE::arena_common::ExpressionFinalModel writer;
    writer.serialize(expr);
  });

  // Regular+PMR serialize (same source data, Arena not involved in serialization)
  double regular_pmr_serialize = run_benchmark("Regular+PMR serialize", ITERATIONS, [&]() {
    FBE::arena_common_pmr::ExpressionModel writer;
    writer.serialize(expr_pmr, nullptr);  // nullptr is fine for serialize
  });

  // Final+PMR serialize (same source data, Arena not involved in serialization)
  double final_pmr_serialize = run_benchmark("Final+PMR serialize", ITERATIONS, [&]() {
    FBE::arena_common_pmr::ExpressionFinalModel writer;
    writer.serialize(expr_pmr);
  });

  // Create serialized buffers for deserialize benchmarks
  FBE::arena_common::ExpressionModel regular_writer;
  regular_writer.serialize(expr, nullptr);
  size_t regular_size = regular_writer.buffer().size();

  FBE::arena_common::ExpressionFinalModel final_writer;
  final_writer.serialize(expr);
  size_t final_size = final_writer.buffer().size();

  FBE::arena_common_pmr::ExpressionModel regular_pmr_writer;
  regular_pmr_writer.serialize(expr_pmr, arena.get_memory_resource());
  size_t regular_pmr_size = regular_pmr_writer.buffer().size();

  FBE::arena_common_pmr::ExpressionFinalModel final_pmr_writer;
  final_pmr_writer.serialize(expr_pmr);
  size_t final_pmr_size = final_pmr_writer.buffer().size();

  // ==================== DESERIALIZE BENCHMARKS ====================
  // Result objects created OUTSIDE the loop (like test_benchmark.cpp)
  // This measures pure deserialization, not object construction + deserialization

  // Regular deserialize
  FBE::arena_common::ExpressionModel regular_reader;
  regular_reader.attach(regular_writer.buffer());
  ::arena_common::Expression regular_result;
  double regular_deserialize = run_benchmark("Regular deserialize", ITERATIONS, [&]() {
    regular_reader.deserialize(regular_result, nullptr);
  });

  // Final deserialize
  FBE::arena_common::ExpressionFinalModel final_reader;
  final_reader.attach(final_writer.buffer());
  ::arena_common::Expression final_result;
  double final_deserialize = run_benchmark("Final deserialize", ITERATIONS, [&]() {
    final_reader.deserialize(final_result);
  });

  // Regular+PMR deserialize with Arena
  Arena deser_arena_regular(arena_opts);
  FBE::arena_common_pmr::ExpressionModel regular_pmr_reader;
  regular_pmr_reader.attach(regular_pmr_writer.buffer());
  ::arena_common_pmr::Expression regular_pmr_result(deser_arena_regular.get_memory_resource());
  double regular_pmr_deserialize = run_benchmark("Regular+PMR deserialize", ITERATIONS, [&]() {
    regular_pmr_reader.deserialize(regular_pmr_result, deser_arena_regular.get_memory_resource());
  });

  // Final+PMR deserialize with Arena
  Arena deser_arena_final(arena_opts);
  FBE::arena_common_pmr::ExpressionFinalModel final_pmr_reader;
  final_pmr_reader.attach(final_pmr_writer.buffer());
  ::arena_common_pmr::Expression final_pmr_result(deser_arena_final.get_memory_resource());
  double final_pmr_deserialize = run_benchmark("Final+PMR deserialize", ITERATIONS, [&]() {
    final_pmr_reader.deserialize(final_pmr_result, deser_arena_final.get_memory_resource());
  });

  // ==================== PRINT RESULTS ====================
  std::cout << "\n";
  std::cout << "================================================================================\n";
  std::cout << "Benchmark: Regular vs Final vs Regular+PMR vs Final+PMR\n";
  std::cout << "Schema: arena_common::Expression (" << ITERATIONS << " iterations)\n";
  std::cout << "--------------------------------------------------------------------------------\n";
  std::cout << "Expression structure:\n";
  std::cout << "  - 10 keys (strings)\n";
  std::cout << "  - 5 aliases (with variant types: bool/int/string/monostate)\n";
  std::cout << "  - 5 alias_int map entries\n";
  std::cout << "================================================================================\n";
  std::cout << std::fixed << std::setprecision(2);

  std::cout << "\n--- Serialized Size (bytes) ---\n";
  std::cout << "                   Size      vs Regular\n";
  std::cout << "Regular:         " << std::setw(6) << regular_size << "      (baseline)\n";
  std::cout << "Final:           " << std::setw(6) << final_size << "      " << std::setw(6) << (100.0 * (double)final_size / regular_size) << "%\n";
  std::cout << "Regular+PMR:     " << std::setw(6) << regular_pmr_size << "      " << std::setw(6) << (100.0 * (double)regular_pmr_size / regular_size) << "%\n";
  std::cout << "Final+PMR:       " << std::setw(6) << final_pmr_size << "      " << std::setw(6) << (100.0 * (double)final_pmr_size / regular_size) << "%\n";

  std::cout << "\n--- Serialize (ns/op) ---\n";
  std::cout << "                   Time        vs Regular\n";
  std::cout << "Regular:         " << std::setw(8) << regular_serialize << "    (baseline)\n";
  std::cout << "Final:           " << std::setw(8) << final_serialize << "    " << std::setw(5) << (regular_serialize / final_serialize) << "x\n";
  std::cout << "Regular+PMR:     " << std::setw(8) << regular_pmr_serialize << "    " << std::setw(5) << (regular_serialize / regular_pmr_serialize) << "x\n";
  std::cout << "Final+PMR:       " << std::setw(8) << final_pmr_serialize << "    " << std::setw(5) << (regular_serialize / final_pmr_serialize) << "x\n";

  std::cout << "\n--- Deserialize (ns/op) ---\n";
  std::cout << "                   Time        vs Regular\n";
  std::cout << "Regular:         " << std::setw(8) << regular_deserialize << "    (baseline)\n";
  std::cout << "Final:           " << std::setw(8) << final_deserialize << "    " << std::setw(5) << (regular_deserialize / final_deserialize) << "x\n";
  std::cout << "Regular+PMR:     " << std::setw(8) << regular_pmr_deserialize << "    " << std::setw(5) << (regular_deserialize / regular_pmr_deserialize) << "x\n";
  std::cout << "Final+PMR:       " << std::setw(8) << final_pmr_deserialize << "    " << std::setw(5) << (regular_deserialize / final_pmr_deserialize) << "x\n";

  std::cout << "\n================================================================================\n";
  std::cout << "Summary: Speedup Matrix (row vs column)\n";
  std::cout << "================================================================================\n";
  std::cout << "\nSerialize speedup:\n";
  std::cout << "                   Regular    Final      Reg+PMR    Final+PMR\n";
  std::cout << "Regular:           1.00x      " << std::setw(5) << (regular_serialize / final_serialize) << "x     "
            << std::setw(5) << (regular_serialize / regular_pmr_serialize) << "x     "
            << std::setw(5) << (regular_serialize / final_pmr_serialize) << "x\n";
  std::cout << "Final:             " << std::setw(5) << (final_serialize / regular_serialize) << "x     1.00x      "
            << std::setw(5) << (final_serialize / regular_pmr_serialize) << "x     "
            << std::setw(5) << (final_serialize / final_pmr_serialize) << "x\n";
  std::cout << "Regular+PMR:       " << std::setw(5) << (regular_pmr_serialize / regular_serialize) << "x     "
            << std::setw(5) << (regular_pmr_serialize / final_serialize) << "x     1.00x      "
            << std::setw(5) << (regular_pmr_serialize / final_pmr_serialize) << "x\n";
  std::cout << "Final+PMR:         " << std::setw(5) << (final_pmr_serialize / regular_serialize) << "x     "
            << std::setw(5) << (final_pmr_serialize / final_serialize) << "x     "
            << std::setw(5) << (final_pmr_serialize / regular_pmr_serialize) << "x     1.00x\n";

  std::cout << "\nDeserialize speedup:\n";
  std::cout << "                   Regular    Final      Reg+PMR    Final+PMR\n";
  std::cout << "Regular:           1.00x      " << std::setw(5) << (regular_deserialize / final_deserialize) << "x     "
            << std::setw(5) << (regular_deserialize / regular_pmr_deserialize) << "x     "
            << std::setw(5) << (regular_deserialize / final_pmr_deserialize) << "x\n";
  std::cout << "Final:             " << std::setw(5) << (final_deserialize / regular_deserialize) << "x     1.00x      "
            << std::setw(5) << (final_deserialize / regular_pmr_deserialize) << "x     "
            << std::setw(5) << (final_deserialize / final_pmr_deserialize) << "x\n";
  std::cout << "Regular+PMR:       " << std::setw(5) << (regular_pmr_deserialize / regular_deserialize) << "x     "
            << std::setw(5) << (regular_pmr_deserialize / final_deserialize) << "x     1.00x      "
            << std::setw(5) << (regular_pmr_deserialize / final_pmr_deserialize) << "x\n";
  std::cout << "Final+PMR:         " << std::setw(5) << (final_pmr_deserialize / regular_deserialize) << "x     "
            << std::setw(5) << (final_pmr_deserialize / final_deserialize) << "x     "
            << std::setw(5) << (final_pmr_deserialize / regular_pmr_deserialize) << "x     1.00x\n";

  std::cout << "\n================================================================================\n";
  std::cout << "Key Comparisons:\n";
  std::cout << "================================================================================\n";
  std::cout << "1. Final vs Regular (effect of Final mode):\n";
  std::cout << "   Serialize:    " << std::setw(5) << (regular_serialize / final_serialize) << "x faster\n";
  std::cout << "   Deserialize:  " << std::setw(5) << (regular_deserialize / final_deserialize) << "x faster\n";
  std::cout << "   Size:         " << std::setw(5) << (100.0 - 100.0 * (double)final_size / regular_size) << "% smaller\n";

  std::cout << "\n2. Regular+PMR vs Regular (effect of PMR on Regular mode):\n";
  std::cout << "   Serialize:    " << std::setw(5) << (regular_serialize / regular_pmr_serialize) << "x\n";
  std::cout << "   Deserialize:  " << std::setw(5) << (regular_deserialize / regular_pmr_deserialize) << "x\n";

  std::cout << "\n3. Final+PMR vs Final (effect of PMR on Final mode):\n";
  std::cout << "   Serialize:    " << std::setw(5) << (final_serialize / final_pmr_serialize) << "x\n";
  std::cout << "   Deserialize:  " << std::setw(5) << (final_deserialize / final_pmr_deserialize) << "x\n";

  std::cout << "\n4. Final+PMR vs Regular (best vs baseline):\n";
  std::cout << "   Serialize:    " << std::setw(5) << (regular_serialize / final_pmr_serialize) << "x faster\n";
  std::cout << "   Deserialize:  " << std::setw(5) << (regular_deserialize / final_pmr_deserialize) << "x faster\n";
  std::cout << "   Size:         " << std::setw(5) << (100.0 - 100.0 * (double)final_pmr_size / regular_size) << "% smaller\n";
  std::cout << "================================================================================\n";

  // ==========================================================================
  // PART 2: DEEPLY NESTED ITEM BENCHMARK
  // ==========================================================================
  std::cout << "\n\n";
  std::cout << "================================================================================\n";
  std::cout << "Benchmark: DEEPLY NESTED Item struct\n";
  std::cout << "Schema: arena::Item (" << ITERATIONS << " iterations)\n";
  std::cout << "--------------------------------------------------------------------------------\n";
  std::cout << "Item structure (approximately 310+ nested objects):\n";
  std::cout << "  - 1 Alias (with variant)\n";
  std::cout << "  - 10 Expressions, each containing:\n";
  std::cout << "    - 20 keys (strings)\n";
  std::cout << "    - 10 aliases (with variants)\n";
  std::cout << "    - 5 alias_int map entries\n";
  std::cout << "  - 10 aliases_int top-level map entries\n";
  std::cout << "================================================================================\n";

  // Create Arenas for nested benchmarks
  Arena nested_arena(arena_opts);

  // Create test data for nested Item
  auto item = CreateDeeplyNestedItem();
  auto item_pmr = CreateDeeplyNestedItemPmr(nested_arena.get_memory_resource());

  // Nested Item - Serialize benchmarks
  double item_regular_serialize = run_benchmark("Item Regular serialize", ITERATIONS, [&]() {
    FBE::arena::ItemModel writer;
    writer.serialize(item, nullptr);
  });

  double item_final_serialize = run_benchmark("Item Final serialize", ITERATIONS, [&]() {
    FBE::arena::ItemFinalModel writer;
    writer.serialize(item);
  });

  double item_regular_pmr_serialize = run_benchmark("Item Regular+PMR serialize", ITERATIONS, [&]() {
    FBE::arena_pmr::ItemModel writer;
    writer.serialize(item_pmr, nullptr);
  });

  double item_final_pmr_serialize = run_benchmark("Item Final+PMR serialize", ITERATIONS, [&]() {
    FBE::arena_pmr::ItemFinalModel writer;
    writer.serialize(item_pmr);
  });

  // Create serialized buffers for Item deserialize benchmarks
  FBE::arena::ItemModel item_regular_writer;
  item_regular_writer.serialize(item, nullptr);
  size_t item_regular_size = item_regular_writer.buffer().size();

  FBE::arena::ItemFinalModel item_final_writer;
  item_final_writer.serialize(item);
  size_t item_final_size = item_final_writer.buffer().size();

  FBE::arena_pmr::ItemModel item_regular_pmr_writer;
  item_regular_pmr_writer.serialize(item_pmr, nested_arena.get_memory_resource());
  size_t item_regular_pmr_size = item_regular_pmr_writer.buffer().size();

  FBE::arena_pmr::ItemFinalModel item_final_pmr_writer;
  item_final_pmr_writer.serialize(item_pmr);
  size_t item_final_pmr_size = item_final_pmr_writer.buffer().size();

  // Nested Item - Deserialize benchmarks
  // Result objects created OUTSIDE the loop (like test_benchmark.cpp)
  FBE::arena::ItemModel item_regular_reader;
  item_regular_reader.attach(item_regular_writer.buffer());
  ::arena::Item item_regular_result;
  double item_regular_deserialize = run_benchmark("Item Regular deserialize", ITERATIONS, [&]() {
    item_regular_reader.deserialize(item_regular_result, nullptr);
  });

  FBE::arena::ItemFinalModel item_final_reader;
  item_final_reader.attach(item_final_writer.buffer());
  ::arena::Item item_final_result;
  double item_final_deserialize = run_benchmark("Item Final deserialize", ITERATIONS, [&]() {
    item_final_reader.deserialize(item_final_result);
  });

  // Regular+PMR deserialize
  Arena item_deser_arena_regular(arena_opts);
  FBE::arena_pmr::ItemModel item_regular_pmr_reader;
  item_regular_pmr_reader.attach(item_regular_pmr_writer.buffer());
  ::arena_pmr::Item item_regular_pmr_result(item_deser_arena_regular.get_memory_resource());
  double item_regular_pmr_deserialize = run_benchmark("Item Regular+PMR deserialize", ITERATIONS, [&]() {
    item_regular_pmr_reader.deserialize(item_regular_pmr_result, item_deser_arena_regular.get_memory_resource());
  });

  // Final+PMR deserialize
  Arena item_deser_arena_final(arena_opts);
  FBE::arena_pmr::ItemFinalModel item_final_pmr_reader;
  item_final_pmr_reader.attach(item_final_pmr_writer.buffer());
  ::arena_pmr::Item item_final_pmr_result(item_deser_arena_final.get_memory_resource());
  double item_final_pmr_deserialize = run_benchmark("Item Final+PMR deserialize", ITERATIONS, [&]() {
    item_final_pmr_reader.deserialize(item_final_pmr_result, item_deser_arena_final.get_memory_resource());
  });

  // Print Nested Item Results
  std::cout << "\n--- Serialized Size (bytes) ---\n";
  std::cout << "                   Size      vs Regular\n";
  std::cout << "Regular:         " << std::setw(6) << item_regular_size << "      (baseline)\n";
  std::cout << "Final:           " << std::setw(6) << item_final_size << "      " << std::setw(6) << (100.0 * (double)item_final_size / item_regular_size) << "%\n";
  std::cout << "Regular+PMR:     " << std::setw(6) << item_regular_pmr_size << "      " << std::setw(6) << (100.0 * (double)item_regular_pmr_size / item_regular_size) << "%\n";
  std::cout << "Final+PMR:       " << std::setw(6) << item_final_pmr_size << "      " << std::setw(6) << (100.0 * (double)item_final_pmr_size / item_regular_size) << "%\n";

  std::cout << "\n--- Serialize (ns/op) ---\n";
  std::cout << "                   Time        vs Regular\n";
  std::cout << "Regular:         " << std::setw(8) << item_regular_serialize << "    (baseline)\n";
  std::cout << "Final:           " << std::setw(8) << item_final_serialize << "    " << std::setw(5) << (item_regular_serialize / item_final_serialize) << "x\n";
  std::cout << "Regular+PMR:     " << std::setw(8) << item_regular_pmr_serialize << "    " << std::setw(5) << (item_regular_serialize / item_regular_pmr_serialize) << "x\n";
  std::cout << "Final+PMR:       " << std::setw(8) << item_final_pmr_serialize << "    " << std::setw(5) << (item_regular_serialize / item_final_pmr_serialize) << "x\n";

  std::cout << "\n--- Deserialize (ns/op) ---\n";
  std::cout << "                   Time        vs Regular\n";
  std::cout << "Regular:         " << std::setw(8) << item_regular_deserialize << "    (baseline)\n";
  std::cout << "Final:           " << std::setw(8) << item_final_deserialize << "    " << std::setw(5) << (item_regular_deserialize / item_final_deserialize) << "x\n";
  std::cout << "Regular+PMR:     " << std::setw(8) << item_regular_pmr_deserialize << "    " << std::setw(5) << (item_regular_deserialize / item_regular_pmr_deserialize) << "x\n";
  std::cout << "Final+PMR:       " << std::setw(8) << item_final_pmr_deserialize << "    " << std::setw(5) << (item_regular_deserialize / item_final_pmr_deserialize) << "x\n";

  std::cout << "\n================================================================================\n";
  std::cout << "Key Comparisons for Deeply Nested Item:\n";
  std::cout << "================================================================================\n";
  std::cout << "1. Final vs Regular (effect of Final mode):\n";
  std::cout << "   Serialize:    " << std::setw(5) << (item_regular_serialize / item_final_serialize) << "x faster\n";
  std::cout << "   Deserialize:  " << std::setw(5) << (item_regular_deserialize / item_final_deserialize) << "x faster\n";
  std::cout << "   Size:         " << std::setw(5) << (100.0 - 100.0 * (double)item_final_size / item_regular_size) << "% smaller\n";

  std::cout << "\n2. Regular+PMR vs Regular (effect of PMR on Regular mode):\n";
  std::cout << "   Serialize:    " << std::setw(5) << (item_regular_serialize / item_regular_pmr_serialize) << "x\n";
  std::cout << "   Deserialize:  " << std::setw(5) << (item_regular_deserialize / item_regular_pmr_deserialize) << "x\n";

  std::cout << "\n3. Final+PMR vs Final (effect of PMR on Final mode):\n";
  std::cout << "   Serialize:    " << std::setw(5) << (item_final_serialize / item_final_pmr_serialize) << "x\n";
  std::cout << "   Deserialize:  " << std::setw(5) << (item_final_deserialize / item_final_pmr_deserialize) << "x\n";

  std::cout << "\n4. Final+PMR vs Regular (best vs baseline):\n";
  std::cout << "   Serialize:    " << std::setw(5) << (item_regular_serialize / item_final_pmr_serialize) << "x faster\n";
  std::cout << "   Deserialize:  " << std::setw(5) << (item_regular_deserialize / item_final_pmr_deserialize) << "x faster\n";
  std::cout << "   Size:         " << std::setw(5) << (100.0 - 100.0 * (double)item_final_pmr_size / item_regular_size) << "% smaller\n";
  std::cout << "================================================================================\n";

  // ==========================================================================
  // PART 3: BATCH PROCESSING WITH ARENA.RESET()
  // This demonstrates the real advantage of PMR: bulk deallocation
  // ==========================================================================
  std::cout << "\n\n";
  std::cout << "================================================================================\n";
  std::cout << "Benchmark: BATCH PROCESSING with Arena.Reset()\n";
  std::cout << "--------------------------------------------------------------------------------\n";
  std::cout << "Simulates real-world batch processing:\n";
  std::cout << "  - Process BATCH_SIZE objects per batch\n";
  std::cout << "  - Reset arena after each batch (bulk deallocation)\n";
  std::cout << "  - Compare: Regular (malloc/free each) vs PMR (bulk reset)\n";
  std::cout << "================================================================================\n";

  constexpr int BATCH_SIZE = 1000;  // Larger batch to show O(1) Reset advantage
  constexpr int NUM_BATCHES = 100;

  // Regular mode: store objects in vector, then clear (triggers destructors)
  double regular_batch = run_benchmark("Regular batch", NUM_BATCHES, [&]() {
    std::vector<::arena::Item> items;
    items.reserve(BATCH_SIZE);
    for (int i = 0; i < BATCH_SIZE; ++i) {
      items.emplace_back();
      item_regular_reader.deserialize(items.back(), nullptr);
    }
    items.clear();  // Triggers destructor for each item -> free() calls
  });

  // Final mode: store objects in vector, then clear (triggers destructors)
  double final_batch = run_benchmark("Final batch", NUM_BATCHES, [&]() {
    std::vector<::arena::Item> items;
    items.reserve(BATCH_SIZE);
    for (int i = 0; i < BATCH_SIZE; ++i) {
      items.emplace_back();
      item_final_reader.deserialize(items.back());
    }
    items.clear();  // Triggers destructor for each item -> free() calls
  });

  // Regular+PMR mode with Arena.Reset(): bulk deallocation
  // Use raw pointers to avoid destructor calls - this is the correct Arena pattern
  Arena batch_arena_regular(arena_opts);
  double regular_pmr_batch = run_benchmark("Regular+PMR batch", NUM_BATCHES, [&]() {
    auto* resource = batch_arena_regular.get_memory_resource();
    // Allocate array of Items directly from arena (no vector overhead)
    auto* items = static_cast<::arena_pmr::Item*>(
        resource->allocate(sizeof(::arena_pmr::Item) * BATCH_SIZE, alignof(::arena_pmr::Item)));
    for (int i = 0; i < BATCH_SIZE; ++i) {
      new (&items[i]) ::arena_pmr::Item(resource);  // placement new
      item_regular_pmr_reader.deserialize(items[i], resource);
    }
    // NO destructor calls! Just reset the arena.
    batch_arena_regular.Reset();  // O(1) bulk deallocation
  });

  // Final+PMR mode with Arena.Reset(): bulk deallocation
  Arena batch_arena_final(arena_opts);
  double final_pmr_batch = run_benchmark("Final+PMR batch", NUM_BATCHES, [&]() {
    auto* resource = batch_arena_final.get_memory_resource();
    auto* items = static_cast<::arena_pmr::Item*>(
        resource->allocate(sizeof(::arena_pmr::Item) * BATCH_SIZE, alignof(::arena_pmr::Item)));
    for (int i = 0; i < BATCH_SIZE; ++i) {
      new (&items[i]) ::arena_pmr::Item(resource);  // placement new
      item_final_pmr_reader.deserialize(items[i], resource);
    }
    // NO destructor calls! Just reset the arena.
    batch_arena_final.Reset();  // O(1) bulk deallocation
  });

  std::cout << "\n--- Batch Processing: " << BATCH_SIZE << " Items per batch, " << NUM_BATCHES << " batches ---\n";
  std::cout << "                   Time (ns/batch)   Per-item (ns)   vs Regular\n";
  std::cout << "Regular:         " << std::setw(12) << regular_batch << "   " << std::setw(10) << (regular_batch / BATCH_SIZE) << "      (baseline)\n";
  std::cout << "Final:           " << std::setw(12) << final_batch << "   " << std::setw(10) << (final_batch / BATCH_SIZE) << "      " << std::setw(5) << (regular_batch / final_batch) << "x\n";
  std::cout << "Regular+PMR:     " << std::setw(12) << regular_pmr_batch << "   " << std::setw(10) << (regular_pmr_batch / BATCH_SIZE) << "      " << std::setw(5) << (regular_batch / regular_pmr_batch) << "x\n";
  std::cout << "Final+PMR:       " << std::setw(12) << final_pmr_batch << "   " << std::setw(10) << (final_pmr_batch / BATCH_SIZE) << "      " << std::setw(5) << (regular_batch / final_pmr_batch) << "x\n";

  std::cout << "\n================================================================================\n";
  std::cout << "Key Comparisons for Batch Processing:\n";
  std::cout << "================================================================================\n";
  std::cout << "1. Final vs Regular:\n";
  std::cout << "   Batch time:   " << std::setw(5) << (regular_batch / final_batch) << "x faster\n";

  std::cout << "\n2. Regular+PMR vs Regular (PMR advantage with Reset):\n";
  std::cout << "   Batch time:   " << std::setw(5) << (regular_batch / regular_pmr_batch) << "x faster\n";

  std::cout << "\n3. Final+PMR vs Final (PMR advantage with Reset):\n";
  std::cout << "   Batch time:   " << std::setw(5) << (final_batch / final_pmr_batch) << "x faster\n";

  std::cout << "\n4. Final+PMR vs Regular (best vs baseline):\n";
  std::cout << "   Batch time:   " << std::setw(5) << (regular_batch / final_pmr_batch) << "x faster\n";
  std::cout << "================================================================================\n";

  return 0;
}
