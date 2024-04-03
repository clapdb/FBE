#include <cstdint>

#include "arena/arena.hpp"
#include "doctest/doctest.h"
#include "smoke_ptr_ptr_pmr.h"
#include "smoke_ptr_ptr_pmr_models.h"

using int128_t = __int128;
using stdb::memory::Arena;
using stdb::memory::arena_string;
using namespace smoke_ptr_pmr;
using namespace FBE::smoke_ptr_pmr;
using BufferT = pmr_buffer_t;

template <typename T>
struct TypeHelper
{};

template <>
struct TypeHelper<bool>
{
    using Type = Bool;
    using Model = BoolModel;
    static auto random_value(Arena& arena) -> bool { return true; }
};

template <>
struct TypeHelper<int8_t>
{
    using Type = Int8;
    using Model = Int8Model;
    static auto random_value(Arena& arena) -> int8_t { return 1; }
};

template <>
struct TypeHelper<int16_t>
{
    using Type = Int16;
    using Model = Int16Model;
    static auto random_value(Arena& arena) -> int16_t { return 1; }
};

template <>
struct TypeHelper<int32_t>
{
    using Type = Int32;
    using Model = Int32Model;
    static auto random_value(Arena& arena) -> int32_t { return 1; }
};

template <>
struct TypeHelper<int64_t>
{
    using Type = Int64;
    using Model = Int64Model;
    static auto random_value(Arena& arena) -> int64_t { return 1; }
};

template <>
struct TypeHelper<int128_t>
{
    using Type = Int128;
    using Model = Int128Model;
    static auto random_value(Arena& arena) -> int128_t { return 1; }
};

template <>
struct TypeHelper<arena_string>
{
    using Type = String;
    using Model = StringModel;
    static auto random_value(Arena& arena) -> arena_string {
        return {"asdfqwerzxcvasdfqwerzxcvasdfqwerzxcv", arena.get_memory_resource()};
    };
};

template <>
struct TypeHelper<BufferT>
{
    using Type = Bytes;
    using Model = BytesModel;
    static auto random_value(Arena& arena) -> BufferT {
        return BufferT{arena_string{"asdfqwer", arena.get_memory_resource()}};
    }
};

TEST_CASE_TEMPLATE("Smoke::serialize::ptr_pmr", T, bool, int8_t, int16_t, int32_t, int64_t, int128_t, arena_string) {
    using Helper = TypeHelper<T>;
    using Model = Helper::Model;
    using Type = Helper::Type;

    Arena arena{Arena::Options::GetDefaultOptions()};

    Type obj_1{arena.get_memory_resource()};
    obj_1.scalar = Helper::random_value(arena);
    obj_1.vec.push_back(Helper::random_value(arena));
    obj_1.vec.emplace_back(Helper::random_value(arena));
    obj_1.vec.emplace_back();
    obj_1.vec.back() = Helper::random_value(arena);
    obj_1.opt2.emplace(Helper::random_value(arena));
    obj_1.value = Helper::random_value(arena);

    // TODO(liuqi): FBE/modules/memory/arena/arena.hpp:685:20: runtime error: constructor call on misaligned address
    //              0x62100000a128 for type 'struct Block', which requires 16 byte alignment
    // obj_1.block1 = arena.Create<Block>(Helper::random_value(arena));
    // obj_1.block2 = arena.Create<Block>(Value{arena.Create<Block>(Helper::random_value(arena))});

    Model model_1{};
    model_1.serialize(obj_1, arena.get_memory_resource());

    Model model_2{};
    model_2.attach(model_1.buffer().data(), model_1.buffer().size());
    REQUIRE(model_2.verify());

    Type obj_2{arena.get_memory_resource()};
    model_2.deserialize(obj_2, arena.get_memory_resource());

    REQUIRE_EQ(obj_1, obj_2);
    for (uint64_t i = 0; i < obj_1.vec.size(); ++i) {
        CHECK_EQ(obj_1.vec[i], obj_2.vec[2]);
    }
    REQUIRE_EQ(obj_2.opt1, std::nullopt);
    REQUIRE_NE(obj_2.opt2, std::nullopt);
}

TEST_CASE("Smoke::serialize::ptr_pmr::nested") {
    Arena arena{Arena::Options::GetDefaultOptions()};

    Outer obj_1{arena.get_memory_resource()};
    obj_1.inner.value = 10;

    // attempting free on address which was not malloc()-ed
    // obj_1.inner_ptr = arena.Create<Inner>(11);

    // attempting free on address which was not malloc()-ed
    // obj_1.inner_ptr = arena.Create<Inner>(arena.get_memory_resource());
    // obj_1.inner_ptr->value = 11;

    // without deserialization: ok
    // with deserialization: attempting free on address which was not malloc()-ed
    // thread T0 obj_1.inner_ptr = new Inner{11};

    obj_1.inners.emplace_back(100);

    // attempting free on address which was not malloc()-ed
    // obj_1.inner_ptrs.emplace_back(arena.Create<Inner>(11));

    obj_1.values.emplace_back(10);
    obj_1.values.emplace_back(arena_string{"adsf", arena.get_memory_resource()});
    obj_1.values.emplace_back(Inner{12});

    obj_1.bytes_map.emplace(arena_string{"bytes", arena.get_memory_resource()},
                            BufferT{arena_string{"qwer", arena.get_memory_resource()}});

    obj_1.value_map.emplace(arena_string{"value", arena.get_memory_resource()},
                            arena_string{"", arena.get_memory_resource()});

    OuterModel model_1{};
    model_1.serialize(obj_1, arena.get_memory_resource());

    OuterModel model_2{};
    model_2.attach(model_1.buffer().data(), model_1.buffer().size());
    REQUIRE(model_2.verify());

    Outer obj_2{arena.get_memory_resource()};
    model_2.deserialize(obj_2, arena.get_memory_resource());

    REQUIRE_EQ(obj_1, obj_2);
}