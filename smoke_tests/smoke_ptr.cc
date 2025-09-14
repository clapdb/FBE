#include <cstdint>
#include <string>

#include "arena/arena.hpp"
#include "doctest/doctest.h"
#include "smoke_ptr_ptr.h"
#include "smoke_ptr_ptr_models.h"

using int128_t = __int128;
using arena::Arena;
using namespace smoke_ptr;
using namespace FBE::smoke_ptr;
using BufferT = buffer_t;

template <typename T>
struct TypeHelper
{};

template <>
struct TypeHelper<bool>
{
    using Type = Bool;
    using Model = BoolModel;
    static auto random_value() -> bool { return true; }
};

template <>
struct TypeHelper<int8_t>
{
    using Type = Int8;
    using Model = Int8Model;
    static auto random_value() -> int8_t { return 1; }
};

template <>
struct TypeHelper<int16_t>
{
    using Type = Int16;
    using Model = Int16Model;
    static auto random_value() -> int16_t { return 1; }
};

template <>
struct TypeHelper<int32_t>
{
    using Type = Int32;
    using Model = Int32Model;
    static auto random_value() -> int32_t { return 1; }
};

template <>
struct TypeHelper<int64_t>
{
    using Type = Int64;
    using Model = Int64Model;
    static auto random_value() -> int64_t { return 1; }
};

template <>
struct TypeHelper<int128_t>
{
    using Type = Int128;
    using Model = Int128Model;
    static auto random_value() -> int128_t { return 1; }
};

template <>
struct TypeHelper<std::string>
{
    using Type = String;
    using Model = StringModel;
    static auto random_value() -> std::string { return "asdf"; }
};

template <>
struct TypeHelper<BufferT>
{
    using Type = Bytes;
    using Model = BytesModel;
    static auto random_value() -> BufferT { return BufferT{"asdfqwer"}; }
};

TEST_CASE_TEMPLATE("Smoke::serialize::ptr", T, bool, int8_t, int16_t, int32_t, int64_t, int128_t, std::string) {
    using Helper = TypeHelper<T>;
    using Model = Helper::Model;
    using Type = Helper::Type;

    Arena arena{Arena::Options::GetDefaultOptions()};

    Type obj_1{};
    obj_1.scalar = Helper::random_value();
    obj_1.vec.push_back(Helper::random_value());
    obj_1.vec.emplace_back(Helper::random_value());
    obj_1.vec.emplace_back();
    obj_1.vec.back() = Helper::random_value();
    obj_1.opt2.emplace(Helper::random_value());
    obj_1.value = Helper::random_value();
    obj_1.block1 = new Block(Helper::random_value());

    // TODO(liuqi): ptr in variant can not be freed
    // obj_1.block2 = new Block(Value{new Block(Helper::random_value())});

    if constexpr (not std::is_same_v<T, BufferT>) {
        obj_1.map.emplace(Helper::random_value(), Helper::random_value());
    }

    Model model_1{};
    model_1.serialize(obj_1, arena.get_memory_resource());

    Model model{};
    model.attach(model_1.buffer().data(), model_1.buffer().size());
    REQUIRE(model.verify());

    Type obj{};
    model.deserialize(obj, arena.get_memory_resource());

    REQUIRE_EQ(obj_1, obj);
    for (uint64_t i = 0; i < obj_1.vec.size(); ++i) {
        CHECK_EQ(obj_1.vec[i], obj.vec[2]);
    }
    REQUIRE_EQ(obj.opt1, std::nullopt);
    REQUIRE_NE(obj.opt2, std::nullopt);
}

TEST_CASE("Smoke::serialize::ptr::nested") {
    Arena arena{Arena::Options::GetDefaultOptions()};

    Outer obj_1{};
    obj_1.inner.value = 10;
    obj_1.inner_ptr = new Inner{11};

    obj_1.inners.emplace_back(100);
    obj_1.inner_ptrs.emplace_back(new Inner{101});

    obj_1.values.emplace_back(10);
    obj_1.values.emplace_back("adsf");
    obj_1.values.emplace_back(Inner{12});

    obj_1.bytes_map.emplace("bytes", BufferT{"qwer"});

    obj_1.value_map.emplace("value", "");

    OuterModel model_1{};
    model_1.serialize(obj_1, arena.get_memory_resource());

    OuterModel model_2{};
    model_2.attach(model_1.buffer().data(), model_1.buffer().size());
    REQUIRE(model_2.verify());

    Outer obj_2{};
    model_2.deserialize(obj_2, arena.get_memory_resource());

    REQUIRE_EQ(obj_1, obj_2);
}