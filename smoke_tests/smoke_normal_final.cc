// TODO(liuqi): final should support variant
// fbe don't support variant in final mode by now
// this case will cause misalignment problem
// the variant with string/buffer_t will cause test failed

// #include <cstdint>
// #include <string>

// #include "arena/arena.hpp"
// #include "doctest/doctest.h"
// #include "smoke_normal.h"
// #include "smoke_normal_final_models.h"

// using int128_t = __int128;
// using stdb::memory::Arena;
// using namespace smoke_normal;
// using namespace FBE::smoke_normal;
// using BufferT = buffer_t;

// template <typename T>
// struct TypeHelper
// {};

// template <>
// struct TypeHelper<bool>
// {
//     using Type = Bool;
//     using Model = BoolFinalModel;
//     static auto random_value() -> bool { return true; }
// };

// template <>
// struct TypeHelper<int8_t>
// {
//     using Type = Int8;
//     using Model = Int8FinalModel;
//     static auto random_value() -> int8_t { return 1; }
// };

// template <>
// struct TypeHelper<int16_t>
// {
//     using Type = Int16;
//     using Model = Int16FinalModel;
//     static auto random_value() -> int16_t { return 1; }
// };

// template <>
// struct TypeHelper<int32_t>
// {
//     using Type = Int32;
//     using Model = Int32FinalModel;
//     static auto random_value() -> int32_t { return 1; }
// };

// template <>
// struct TypeHelper<int64_t>
// {
//     using Type = Int64;
//     using Model = Int64FinalModel;
//     static auto random_value() -> int64_t { return 1; }
// };

// template <>
// struct TypeHelper<int128_t>
// {
//     using Type = Int128;
//     using Model = Int128FinalModel;
//     static auto random_value() -> int128_t { return 1; }
// };

// template <>
// struct TypeHelper<std::string>
// {
//     using Type = String;
//     using Model = StringFinalModel;
//     static auto random_value() -> std::string { return "asdf"; }
// };

// template <>
// struct TypeHelper<BufferT>
// {
//     using Type = Bytes;
//     using Model = BytesFinalModel;
//     static auto random_value() -> BufferT { return BufferT{"asdfqwer"}; }
// };

// TEST_CASE_TEMPLATE("Smoke::serialize::normal_final", T, bool, int8_t, int16_t, int32_t, int64_t,
//                    int128_t /* std::string, BufferT*/) {
//     using Helper = TypeHelper<T>;
//     using Model = Helper::Model;
//     using Type = Helper::Type;

//     Arena arena{Arena::Options::GetDefaultOptions()};

//     Type obj_1{};
//     obj_1.scalar = Helper::random_value();
//     obj_1.vec.push_back(Helper::random_value());
//     obj_1.vec.emplace_back(Helper::random_value());
//     obj_1.vec.emplace_back();
//     obj_1.vec.back() = Helper::random_value();
//     obj_1.opt2.emplace(Helper::random_value());
//     obj_1.value = Helper::random_value();

//     if constexpr (not std::is_same_v<T, BufferT>) {
//         obj_1.map.emplace(Helper::random_value(), Helper::random_value());
//     }

//     Model model_1{};
//     model_1.serialize(obj_1);

//     Model model_2{};
//     model_2.attach(model_1.buffer().data(), model_1.buffer().size());
//     REQUIRE(model_2.verify());

//     Type obj_2{};
//     model_2.deserialize(obj_2);

//     REQUIRE_EQ(obj_1, obj_2);
//     for (uint64_t i = 0; i < obj_1.vec.size(); ++i) {
//         CHECK_EQ(obj_1.vec[i], obj_2.vec[2]);
//     }
//     REQUIRE_EQ(obj_2.opt1, std::nullopt);
//     REQUIRE_NE(obj_2.opt2, std::nullopt);
// }

// TEST_CASE("Smoke::serialize::normal_final::nested") {
//     Arena arena{Arena::Options::GetDefaultOptions()};

//     Outer obj_1{};
//     obj_1.inner.value = 10;

//     obj_1.inners.emplace_back(100);

//     obj_1.values.emplace_back(10);
//     // obj_1.values.emplace_back("adsf");
//     obj_1.values.emplace_back(Inner{12});

//     // obj_1.bytes_map.emplace("bytes", BufferT{"qwer"});

//     // obj_1.value_map.emplace("value", "");

//     OuterFinalModel model_1{};
//     model_1.serialize(obj_1);

//     OuterFinalModel model_2{};
//     model_2.attach(model_1.buffer().data(), model_1.buffer().size());
//     REQUIRE(model_2.verify());

//     Outer obj_2{};
//     model_2.deserialize(obj_2);

//     REQUIRE_EQ(obj_1, obj_2);
// }