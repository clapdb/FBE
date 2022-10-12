//
// Created by Ivan Shynkarenka on 09.07.2018
//

#include "catch2/catch.hpp"
#include "enums.h"
#include "extra_ptr.h"
#include "osa.h"
#include "pkg_ptr.h"
#include "sa_ptr.h"
#include "variants.h"
#include "variants_ptr_ptr.h"
#include "test.h"
#include "template_variant_ptr.h"

#include "../proto/simple_ptr_models.h"
#include "../proto/extra_ptr_models.h"
#include "../proto/sa_ptr_models.h"
#include "../proto/osa_models.h"
#include "../proto/pkg_ptr_models.h"
#include "../proto/variants_ptr_ptr_models.h"
#include "../proto/template_variant_ptr_models.h"
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <utility>
#include <variant>
#include <iostream>
#include <vector>

TEST_CASE("Serialization (simple self-reference)", "[Ptr-based FBE]")
{
    std::vector<std::unique_ptr<::simple::Simple>> v;
    v.push_back(std::make_unique<::simple::Simple>(::simple::Simple(
        "v-info", nullptr, 1024, std::vector<std::unique_ptr<::simple::Simple>>(),
        std::vector<::simple::Simple>(),
        std::map<int32_t, std::unique_ptr<::simple::Simple>>(),
        std::map<int32_t, ::simple::Simple>())));

    auto simplep = std::make_unique<::simple::Simple>(
        ::simple::Simple(
            "single-info", nullptr, 2048,
            std::vector<std::unique_ptr<::simple::Simple>>(),
            std::vector<::simple::Simple>(),
            std::map<int32_t, std::unique_ptr<::simple::Simple>>(),
            std::map<int32_t, ::simple::Simple>()
        )
    );
    std::map<int32_t, std::unique_ptr<::simple::Simple>> pm;
    pm.emplace(std::make_pair(2048, std::move(simplep)));

    ::simple::Simple original = {
        "info",
        std::make_unique<::simple::Simple>(
            "info 1",
            std::make_unique<::simple::Simple>(::simple::Simple(
                "info 2",
                std::make_unique<::simple::Simple>(::simple::Simple(
                    "info 3",
                    std::make_unique<::simple::Simple>( 
                        "info 4", nullptr, 4, std::move(v),
                        std::vector<::simple::Simple>(),
                        std::map<int32_t, std::unique_ptr<::simple::Simple>>(),
                        std::map<int32_t, ::simple::Simple>()
                    ),
                    3, std::vector<std::unique_ptr<::simple::Simple>>(),
                    std::vector<::simple::Simple>(),
                    std::map<int32_t, std::unique_ptr<::simple::Simple>>(),
                    std::map<int32_t, ::simple::Simple>())),
                2, std::vector<std::unique_ptr<::simple::Simple>>(),
                std::vector<::simple::Simple>(),
                std::map<int32_t, std::unique_ptr<::simple::Simple>>(),
                std::map<int32_t, ::simple::Simple>())),
            1, std::vector<std::unique_ptr<::simple::Simple>>(),
            std::vector<::simple::Simple>(),
            std::map<int32_t, std::unique_ptr<::simple::Simple>>(),
            std::map<int32_t, ::simple::Simple>()),
        10,
        {},
        {},
        std::move(pm),
        std::map<int32_t, ::simple::Simple>()};

    ::simple::Simple simple = std::move(original);
    REQUIRE(original.simple == nullptr);

    FBE::simple::SimpleModel writer; 
    size_t serialized = writer.serialize(simple);
    REQUIRE(serialized == writer.buffer().size());
    REQUIRE(writer.verify());
    
    FBE::simple::SimpleModel reader;
    reader.attach(writer.buffer());
    REQUIRE(reader.verify());

    simple::Simple copy;
    size_t deserialized = reader.deserialize(copy);
    REQUIRE(deserialized == reader.buffer().size());

    REQUIRE(copy.info =="info");
    REQUIRE(copy.simple != nullptr);
    REQUIRE(copy.simple->info == "info 1");
    REQUIRE(copy.simple->simple != nullptr);
    REQUIRE(copy.simple->simple->info == "info 2");
    REQUIRE(copy.simple->simple->simple != nullptr);
    REQUIRE(copy.simple->simple->simple->info == "info 3");
    REQUIRE(copy.simple->simple->simple->simple != nullptr);
    REQUIRE(copy.simple->simple->simple->simple->info == "info 4");
    REQUIRE(copy.simple->simple->simple->simple->simple == nullptr);
    REQUIRE(copy.simple->simple->simple->simple->depth == 4);
    REQUIRE(copy.simple->simple->simple->simple->spv.size() == 1);
    REQUIRE(copy.simple->simple->simple->simple->spv[0]->info == "v-info");
    REQUIRE(copy.simple->simple->simple->simple->spv[0]->simple == nullptr);
    REQUIRE(copy.simple->simple->simple->simple->spv[0]->depth == 1024);
    REQUIRE(copy.simple->simple->simple->simple->spv[0]->spv.empty());
    REQUIRE(copy.simple->simple->simple->simple->spv[0]->sv.empty());
    REQUIRE(copy.simple->simple->simple->simple->spv[0]->spm.empty());
    REQUIRE(copy.simple->simple->simple->simple->spv[0]->sm.empty());
    REQUIRE(copy.simple->simple->simple->simple->sv.empty());
    REQUIRE(copy.simple->simple->simple->simple->spm.empty());
    REQUIRE(copy.simple->simple->simple->simple->sm.empty());
    REQUIRE(copy.simple->simple->simple->depth == 3);
    REQUIRE(copy.simple->simple->simple->spv.empty());
    REQUIRE(copy.simple->simple->simple->sv.empty());
    REQUIRE(copy.simple->simple->simple->spm.empty());
    REQUIRE(copy.simple->simple->simple->sm.empty());
    REQUIRE(copy.simple->simple->depth == 2);
    REQUIRE(copy.simple->simple->spv.empty());
    REQUIRE(copy.simple->simple->sv.empty());
    REQUIRE(copy.simple->simple->spm.empty());
    REQUIRE(copy.simple->simple->sm.empty());
    REQUIRE(copy.simple->depth == 1);
    REQUIRE(copy.simple->spv.empty());
    REQUIRE(copy.simple->sv.empty());
    REQUIRE(copy.simple->spm.empty());
    REQUIRE(copy.simple->sm.empty());
    REQUIRE(copy.depth == 10);
    REQUIRE(copy.spv.empty());
    REQUIRE(copy.sv.empty());
    REQUIRE(copy.spm.size() == 1);
    REQUIRE(copy.spm[2048]->info == "single-info");
    REQUIRE(copy.spm[2048]->simple == nullptr);
    REQUIRE(copy.spm[2048]->depth == 2048);
    REQUIRE(copy.spm[2048]->spv.empty());
    REQUIRE(copy.spm[2048]->sv.empty());
    REQUIRE(copy.spm[2048]->spm.empty());
    REQUIRE(copy.spm[2048]->sm.empty());
    REQUIRE(copy.sm.empty());
}

TEST_CASE("Serialization (extra circular-dependency)", "[Ptr-based FBE]")
{
    ::extra::Info &&embedded_info = {
        "embedded-struct-info3-data", nullptr, {}, {}};
    ::extra::Info &&embedded_info2 = {
        "embedded-struct-info3-data2", nullptr, {}, {}};
    ::extra::Info &&embedded_info3 = {
        "v-info-data",
        std::make_unique<::extra::Extra>(
            2, "v-extra-in-vector",
            std::make_unique<::extra::Info>(
                ::extra::Info("v-embedded-struct-info-data", nullptr, {}, {})),
            std::make_unique<::extra::Info>(
                ::extra::Info("v-embedded-struct-info2-data", nullptr, {}, {})),
            std::move(embedded_info), std::vector<::extra::Info>(),
            std::vector<std::unique_ptr<::extra::Info>>(),
            std::list<::extra::Info>(),
            std::list<std::unique_ptr<::extra::Info>>()),
        {},
        {}};
    std::vector<::extra::Info> v;
    std::vector<std::unique_ptr<::extra::Info>> pv;
    v.emplace_back(std::move(embedded_info3));
    pv.emplace_back(std::make_unique<::extra::Info>(::extra::Info(
        "pv-info-data",
        std::make_unique<::extra::Extra>(
            20, "pv-extra",
            std::make_unique<::extra::Info>(
                ::extra::Info("pv-embedded-struct-info-data", nullptr, {}, {})),
            std::make_unique<::extra::Info>(
                ::extra::Info("pv-embedded-struct-info2-data", nullptr, {}, {})),
            std::move(embedded_info2), std::vector<::extra::Info>(),
            std::vector<std::unique_ptr<::extra::Info>>(),
            std::list<::extra::Info>(),
            std::list<std::unique_ptr<::extra::Info>>()),
        {}, {})));

    ::extra::Extra extra = {
        1,
        "root-extra",
        std::make_unique<::extra::Info>(
            ::extra::Info{"info-data", nullptr, {}, {}}),
        std::make_unique<::extra::Info>(
            ::extra::Info{"info2-data", nullptr, {}, {}}),
        {"info3-data", nullptr, {}, {}},
        std::move(v),
        std::move(pv),
        std::list<::extra::Info>(),
        std::list<std::unique_ptr<::extra::Info>>(),
    };

    ::FBE::extra::ExtraModel writer;
    size_t serialized = writer.serialize(extra);
    REQUIRE(serialized == writer.buffer().size());
    REQUIRE(writer.verify());


    FBE::extra::ExtraModel reader;
    reader.attach(writer.buffer());
    REQUIRE(reader.verify());

    extra::Extra copy;
    size_t deserialized = reader.deserialize(copy);
    REQUIRE(deserialized == reader.buffer().size());
    assert(reader.verify());

    REQUIRE(copy.num == 1);
    REQUIRE(copy.data == "root-extra");
    REQUIRE(copy.info != nullptr);
    REQUIRE(copy.info->info == "info-data");
    REQUIRE(copy.info->extra == nullptr);
    REQUIRE(copy.info->extras.empty());
    REQUIRE(copy.info->extras1.empty());
    REQUIRE(copy.info2 != nullptr);
    REQUIRE(copy.info2->info == "info2-data");
    REQUIRE(copy.info2->extra == nullptr);
    REQUIRE(copy.info2->extras.empty());
    REQUIRE(copy.info2->extras1.empty());
    REQUIRE(copy.info3.info == "info3-data");
    REQUIRE(copy.info3.extra == nullptr);
    REQUIRE(copy.info3.extras.empty());
    REQUIRE(copy.info3.extras1.empty());
    REQUIRE(copy.infov.size() == 1);
    REQUIRE(copy.infov[0].info == "v-info-data");
    REQUIRE(copy.infov[0].extra != nullptr);
    REQUIRE(copy.infov[0].extra->num == 2);
    REQUIRE(copy.infov[0].extra->data == "v-extra-in-vector");
    REQUIRE(copy.infov[0].extra->info != nullptr);
    REQUIRE(copy.infov[0].extra->info->info == "v-embedded-struct-info-data");
    REQUIRE(copy.infov[0].extra->info2->info == "v-embedded-struct-info2-data");
    REQUIRE(copy.infov[0].extra->info2 != nullptr);
    REQUIRE(copy.infov[0].extra->info3.info == "embedded-struct-info3-data");
    REQUIRE(copy.infov[0].extra->info3.extra == nullptr);
    REQUIRE(copy.infov[0].extra->info3.extras.empty());
    REQUIRE(copy.infov[0].extra->info3.extras1.empty());
    REQUIRE(copy.infov[0].extra->infov.empty());
    REQUIRE(copy.infov[0].extra->infopv.empty());
    REQUIRE(copy.infov[0].extra->infol.empty());
    REQUIRE(copy.infov[0].extra->infopl.empty());
    REQUIRE(copy.infov[0].extras.empty());
    REQUIRE(copy.infov[0].extras1.empty());
    REQUIRE(copy.infopv.size() == 1);
    REQUIRE(copy.infopv[0]->info == "pv-info-data");
    REQUIRE(copy.infopv[0]->extra != nullptr);
    REQUIRE(copy.infopv[0]->extra->num == 20);
    REQUIRE(copy.infopv[0]->extra->data == "pv-extra");
    REQUIRE(copy.infopv[0]->extra->info != nullptr);
    REQUIRE(copy.infopv[0]->extra->info->info == "pv-embedded-struct-info-data");
    REQUIRE(copy.infopv[0]->extra->info2 != nullptr);
    REQUIRE(copy.infopv[0]->extra->info2->info == "pv-embedded-struct-info2-data");
    REQUIRE(copy.infopv[0]->extra->info3.info == "embedded-struct-info3-data2");
    REQUIRE(copy.infopv[0]->extra->info3.extra == nullptr);
    REQUIRE(copy.infopv[0]->extra->info3.extras.empty());
    REQUIRE(copy.infopv[0]->extra->info3.extras1.empty());
    REQUIRE(copy.infopv[0]->extra->infov.empty());
    REQUIRE(copy.infopv[0]->extra->infopv.empty());
    REQUIRE(copy.infopv[0]->extra->infol.empty());
    REQUIRE(copy.infopv[0]->extra->infopl.empty());
    REQUIRE(copy.infopv[0]->extras.empty());
    REQUIRE(copy.infopv[0]->extras1.empty());
    REQUIRE(copy.infol.empty());
    REQUIRE(copy.infopl.empty());
}

TEST_CASE("Serilization (identical with templated-based code without ptr)", "[Ptr-based FBE]")
{
    ::std::array<::osa::Extra, 1> osa_v = {::osa::Extra(stdb::memory::string("extra"), "detail", ::osa::Sex::male, ::osa::MyFLags::flag1)};
    ::osa::Simple osa = {
        "original",
        12,
        osa_v,
        ::osa::Sex::male,
    };

    FBE::osa::SimpleModel osa_writer;
    size_t serialized_osa = osa_writer.serialize(osa);
    REQUIRE(serialized_osa == osa_writer.buffer().size());
    REQUIRE(osa_writer.verify());

    ::std::array<::sa::Extra, 1> sa_v = {::sa::Extra(stdb::memory::string("extra"), "detail", ::sa::Sex::male, ::sa::MyFLags::flag1)};
    ::sa::Simple sa = {
        "original",
        12,
        std::move(sa_v),
        ::sa::Sex::male,
    };

    FBE::sa::SimpleModel sa_writer;
    size_t serialized_sa = sa_writer.serialize(sa);
    REQUIRE(serialized_sa == sa_writer.buffer().size());
    REQUIRE(sa_writer.verify());

    REQUIRE(serialized_osa == serialized_sa);

    FBE::sa::SimpleModel sa_reader;
    sa_reader.attach(sa_writer.buffer());
    REQUIRE(sa_reader.verify());

    ::sa::Simple sa_copy;
    size_t deserialized_sa = sa_reader.deserialize(sa_copy);
    REQUIRE(deserialized_sa == sa_reader.buffer().size());
    assert(sa_reader.verify());

    FBE::osa::SimpleModel osa_reader;
    osa_reader.attach(osa_writer.buffer());
    REQUIRE(osa_reader.verify());

    ::osa::Simple osa_copy;
    size_t deserialized_osa = osa_reader.deserialize(osa_copy);
    REQUIRE(deserialized_osa == osa_reader.buffer().size());
    assert(osa_reader.verify());

    REQUIRE(deserialized_osa == deserialized_sa);
    REQUIRE(deserialized_osa == osa_reader.buffer().size());
    REQUIRE(deserialized_sa == sa_reader.buffer().size());

    REQUIRE(sa_copy.name == osa_copy.name);
    REQUIRE(sa_copy.depth == osa_copy.depth);
    REQUIRE(sa_copy.sa[0].name == osa_copy.sa[0].name);
    REQUIRE(sa_copy.sa[0].name == "extra");
    REQUIRE(sa_copy.sa[0].detail == osa_copy.sa[0].detail);
    REQUIRE(sa_copy.sa[0].detail == "detail");
    REQUIRE(sa_copy.sa[0].sex == ::sa::Sex::male);
    REQUIRE(osa_copy.sa[0].flag == ::osa::MyFLags::flag1);
    REQUIRE(sa_copy.sa[0].flag == ::sa::MyFLags::flag1);
    REQUIRE(osa_copy.sa[0].sex == ::osa::Sex::male);
    REQUIRE(sa_copy.sex == sa::Sex::male);
    REQUIRE(osa_copy.sex == osa::Sex::male);

}
TEST_CASE("Serialization (optional)", "[Ptr-based FBE]") {
    ::sa::Complex c1 = {
        "test optional",
        std::make_optional<::sa::Sex>(::sa::Sex::male),
        std::make_optional<::sa::MyFLags>(::sa::MyFLags::flag1),
        std::nullopt,
        {123}
    };

    FBE::sa::ComplexModel c_writer;
    size_t serialized_c = c_writer.serialize(c1);
    REQUIRE(serialized_c == c_writer.buffer().size());
    REQUIRE(c_writer.verify());

    FBE::sa::ComplexModel c_reader;
    c_reader.attach(c_writer.buffer());
    REQUIRE(c_reader.verify());

    ::sa::Complex copy;
    size_t deserialized_c = c_reader.deserialize(copy);
    REQUIRE(deserialized_c == c_reader.buffer().size());
    assert(c_reader.verify());

    REQUIRE(copy.name == "test optional");
    REQUIRE(copy.sex.has_value());
    REQUIRE(copy.sex.value() == ::sa::Sex::male);
    REQUIRE(copy.flag.has_value());
    REQUIRE(copy.flag.value() == ::sa::MyFLags::flag1);
    REQUIRE(!copy.extra.has_value());

    c1.extra = std::make_optional<::sa::Extra>(::sa::Extra("extra", "detail",::sa::Sex::female, ::sa::MyFLags::flag2));

    c_writer.reset();
    c_reader.reset();
    size_t serialized_c1 = c_writer.serialize(c1);
    REQUIRE(serialized_c1 == c_writer.buffer().size());
    REQUIRE(c_writer.verify());
    REQUIRE(serialized_c1 > serialized_c);

    c_reader.attach(c_writer.buffer());

    ::sa::Complex c1_copy;
    size_t deserialized_c1 = c_reader.deserialize(c1_copy);
    REQUIRE(deserialized_c1 == c_reader.buffer().size());
    assert(c_reader.verify());

    REQUIRE(deserialized_c1 == serialized_c1);

    REQUIRE(c1_copy.name == "test optional");
    REQUIRE(c1_copy.sex.has_value());
    REQUIRE(c1_copy.sex.value() == ::sa::Sex::male);
    REQUIRE(c1_copy.flag.has_value());
    REQUIRE(c1_copy.flag.value() == ::sa::MyFLags::flag1);
    REQUIRE(c1_copy.extra.has_value());
    REQUIRE(c1_copy.extra->name == "extra");
    REQUIRE(c1_copy.extra->detail == "detail");
    REQUIRE(c1_copy.extra->sex == ::sa::Sex::female);
    REQUIRE(c1_copy.extra->flag == ::sa::MyFLags::flag2);
    REQUIRE(c1_copy.nums.size() == 1);
    REQUIRE(c1_copy.nums[0] == 123);
}
TEST_CASE("Serialization (ptr-based import templated-based fbe)", "[Ptr-based FBE]") {
    ::osa::Extra extra(stdb::memory::string("extra"), "detail", ::osa::Sex::male, ::osa::MyFLags::flag1);

    ::pkg::Info&& pkg_info = {
        "pkg_info", ::osa::Sex::male, ::osa::MyFLags::flag2,  std::move(extra)
    };

    FBE::pkg::InfoModel info_writer;
    size_t serialized_info = info_writer.serialize(pkg_info);
    REQUIRE(serialized_info == info_writer.buffer().size());
    REQUIRE(info_writer.verify());

    FBE::pkg::InfoModel info_reader;
    info_reader.attach(info_writer.buffer());
    REQUIRE(info_reader.verify());

    ::pkg::Info pkg_info_copy;
    size_t deserialized_info = info_reader.deserialize(pkg_info_copy);
    REQUIRE(deserialized_info == info_reader.buffer().size());
    assert(info_reader.verify());

    REQUIRE(pkg_info_copy.info == pkg_info.info);
    REQUIRE(pkg_info_copy.sex == pkg_info_copy.sex);
    REQUIRE(pkg_info_copy.flag == pkg_info.flag);
    REQUIRE(pkg_info_copy.extra.name == pkg_info.extra.name);
    REQUIRE(pkg_info_copy.extra.detail == pkg_info.extra.detail);
    REQUIRE(pkg_info_copy.extra.sex == pkg_info.extra.sex);
    REQUIRE(pkg_info_copy.extra.flag == pkg_info.extra.flag);

    ::pkg::Detail detail;
    detail.extram.emplace(1, std::move(pkg_info.extra));
    detail.extrav.emplace_back(std::move(pkg_info_copy.extra));

    FBE::pkg::DetailModel detail_writer;
    size_t serialized_detail = detail_writer.serialize(detail);
    REQUIRE(serialized_detail == detail_writer.buffer().size());
    REQUIRE(detail_writer.verify());

    FBE::pkg::DetailModel detail_reader;
    detail_reader.attach(detail_writer.buffer());
    REQUIRE(detail_reader.verify());

    ::pkg::Detail detail_copy;
    size_t deserialized_detail = detail_reader.deserialize(detail_copy);
    REQUIRE(deserialized_detail == detail_reader.buffer().size());
    assert(detail_reader.verify());

    REQUIRE(detail_copy.extram.size() == 1);
    REQUIRE(detail_copy.extram.at(1).name == "extra");
    REQUIRE(detail_copy.extram.at(1).detail == "detail");
    REQUIRE(detail_copy.extram.at(1).sex == ::osa::Sex::male);
    REQUIRE(detail_copy.extram.at(1).flag == ::osa::MyFLags::flag1);

    REQUIRE(detail_copy.extrav.size() == 1);
    REQUIRE(detail_copy.extrav.at(0).name == "extra");
    REQUIRE(detail_copy.extrav.at(0).detail == "detail");
    REQUIRE(detail_copy.extrav.at(0).sex == ::osa::Sex::male);
    REQUIRE(detail_copy.extrav.at(0).flag == ::osa::MyFLags::flag1);
}

TEST_CASE("Serialization (variant)", "[Ptr-based FBE]") {
    SECTION ("string") {
        ::variants_ptr::Value value;
        value.v.emplace<stdb::memory::string>("variant v");

        FBE::variants_ptr::ValueModel writer;
        size_t serialized = writer.serialize(value);
        REQUIRE(serialized == writer.buffer().size());
        REQUIRE(writer.verify());

        FBE::variants_ptr::ValueModel reader;
        reader.attach(writer.buffer());
        REQUIRE(reader.verify());

        ::variants_ptr::Value value_copy;
        size_t deserialized = reader.deserialize(value_copy);
        REQUIRE(deserialized == reader.buffer().size());

        REQUIRE(value_copy.v.index() == 1);
        REQUIRE(std::get<stdb::memory::string>(value_copy.v) == "variant v");
    }

    SECTION ("primitive type") {
        ::variants_ptr::Value value;
        value.v.emplace<int32_t>(42);

        FBE::variants_ptr::ValueModel writer;
        size_t serialized = writer.serialize(value);
        REQUIRE(serialized == writer.buffer().size());
        REQUIRE(writer.verify());

        FBE::variants_ptr::ValueModel reader;
        reader.attach(writer.buffer());
        REQUIRE(reader.verify());

        ::variants_ptr::Value value_copy;
        size_t deserialized = reader.deserialize(value_copy);
        REQUIRE(deserialized == reader.buffer().size());

        REQUIRE(value_copy.v.index() == 0);
        REQUIRE(std::get<int32_t>(value_copy.v) == 42);
    }

    SECTION ("struct") {
        ::variants_ptr::Value value;
        value.v.emplace<::variants_ptr::Simple>(::variants_ptr::Simple{"simple"});

        FBE::variants_ptr::ValueModel writer;
        size_t serialized = writer.serialize(value);
        REQUIRE(serialized == writer.buffer().size());
        REQUIRE(writer.verify());

        FBE::variants_ptr::ValueModel reader;
        reader.attach(writer.buffer());
        REQUIRE(reader.verify());

        ::variants_ptr::Value value_copy;
        size_t deserialized = reader.deserialize(value_copy);
        REQUIRE(deserialized == reader.buffer().size());

        REQUIRE(value_copy.v.index() == 3);
        REQUIRE(std::get<::variants_ptr::Simple>(value_copy.v).name == "simple");
    }

    SECTION ("pointer") {
        auto simple = std::make_unique<::variants_ptr::Simple>("simple");
        ::variants_ptr::Value value;
        value.v.emplace<::variants_ptr::Simple*>(simple.get());

        FBE::variants_ptr::ValueModel writer;
        size_t serialized = writer.serialize(value);
        REQUIRE(serialized == writer.buffer().size());
        REQUIRE(writer.verify());

        FBE::variants_ptr::ValueModel reader;
        reader.attach(writer.buffer());
        REQUIRE(reader.verify());

        ::variants_ptr::Value value_copy;
        size_t deserialized = reader.deserialize(value_copy);
        REQUIRE(deserialized == reader.buffer().size());

        REQUIRE(value_copy.v.index() == 4);
        REQUIRE(std::get<::variants_ptr::Simple*>(value_copy.v)->name == "simple");
    }

    SECTION ("vector of struct") {
        std::vector<::variants_ptr::Simple> v;
        v.emplace_back(::variants_ptr::Simple{"simple1"});
        v.emplace_back(::variants_ptr::Simple{"simple2"});

        ::variants_ptr::Value value;
        REQUIRE(value.v.index() == 0);
        value.v.emplace<5>(std::move(v));

        FBE::variants_ptr::ValueModel writer;
        size_t serialized = writer.serialize(value);
        REQUIRE(serialized == writer.buffer().size());
        REQUIRE(writer.verify());

        FBE::variants_ptr::ValueModel reader;
        reader.attach(writer.buffer());
        REQUIRE(reader.verify());

        ::variants_ptr::Value value_copy;
        size_t deserialized = reader.deserialize(value_copy);
        REQUIRE(deserialized == reader.buffer().size());

        REQUIRE(value_copy.v.index() == 5);
        auto& v_copy = std::get<5>(value_copy.v);
        REQUIRE(v_copy.size() == 2);
        REQUIRE(v_copy.at(0).name == "simple1");
        REQUIRE(v_copy.at(1).name == "simple2");
    }

    SECTION ("vector of primitive type") {
        std::vector<int32_t> v {1,2,3};

        ::variants_ptr::Value value;
        REQUIRE(value.v.index() == 0);
        value.v.emplace<6>(std::move(v));

        FBE::variants_ptr::ValueModel writer;
        size_t serialized = writer.serialize(value);
        REQUIRE(serialized == writer.buffer().size());
        REQUIRE(writer.verify());

        FBE::variants_ptr::ValueModel reader;
        reader.attach(writer.buffer());
        REQUIRE(reader.verify());

        ::variants_ptr::Value value_copy;
        size_t deserialized = reader.deserialize(value_copy);
        REQUIRE(deserialized == reader.buffer().size());

        REQUIRE(value_copy.v.index() == 6);
        auto& v_copy = std::get<6>(value_copy.v);
        REQUIRE(v_copy.size() ==3);
        REQUIRE(v_copy.at(0) == 1);
        REQUIRE(v_copy.at(1) == 2);
        REQUIRE(v_copy.at(2) == 3);
    }

    SECTION ("hash with primitive and struct") {
        std::unordered_map<int32_t, ::variants_ptr::Simple> m;
        m.emplace(1, ::variants_ptr::Simple{"simple1"});
        m.emplace(2, ::variants_ptr::Simple{"simple2"});

        ::variants_ptr::Value value;
        REQUIRE(value.v.index() == 0);
        value.v.emplace<7>(std::move(m));

        FBE::variants_ptr::ValueModel writer;
        size_t serialized = writer.serialize(value);
        REQUIRE(serialized == writer.buffer().size());
        REQUIRE(writer.verify());

        FBE::variants_ptr::ValueModel reader;
        reader.attach(writer.buffer());
        REQUIRE(reader.verify());

        ::variants_ptr::Value value_copy;
        size_t deserialized = reader.deserialize(value_copy);
        REQUIRE(deserialized == reader.buffer().size());

        REQUIRE(value_copy.v.index() == 7);
        auto& v_copy = std::get<7>(value_copy.v);
        REQUIRE(v_copy.size() == 2);
        REQUIRE(v_copy.at(1).name == "simple1");
        REQUIRE(v_copy.at(2).name == "simple2");
    }
    
    SECTION ("container of bytes") {
        std::vector<uint8_t> v {65, 66, 67, 68, 69};
        std::vector<FBE::buffer_t> bytes_v;
        bytes_v.emplace_back(FBE::buffer_t(v));

        ::variants_ptr::Value value;
        REQUIRE(value.v.index() == 0);
        value.v.emplace<8>(std::move(bytes_v));

        FBE::variants_ptr::ValueModel writer;
        size_t serialized = writer.serialize(value);
        REQUIRE(serialized == writer.buffer().size());
        REQUIRE(writer.verify());

        FBE::variants_ptr::ValueModel reader;
        reader.attach(writer.buffer());
        REQUIRE(reader.verify());

        ::variants_ptr::Value value_copy;
        size_t deserialized = reader.deserialize(value_copy);
        REQUIRE(deserialized == reader.buffer().size());

        REQUIRE(value_copy.v.index() == 8);
        auto& v_copy = std::get<8>(value_copy.v);
        REQUIRE(v_copy.size() == 1);
        REQUIRE(v_copy.at(0).string() == "ABCDE");
    }
    
    SECTION ("vector of string") {
        std::vector<stdb::memory::string> string_v {"hello", "world"};

        ::variants_ptr::Value value;
        REQUIRE(value.v.index() == 0);
        value.v.emplace<9>(std::move(string_v));

        FBE::variants_ptr::ValueModel writer;
        size_t serialized = writer.serialize(value);
        REQUIRE(serialized == writer.buffer().size());
        REQUIRE(writer.verify());

        FBE::variants_ptr::ValueModel reader;
        reader.attach(writer.buffer());
        REQUIRE(reader.verify());

        ::variants_ptr::Value value_copy;
        size_t deserialized = reader.deserialize(value_copy);
        REQUIRE(deserialized == reader.buffer().size());

        REQUIRE(value_copy.v.index() == 9);
        auto& v_copy = std::get<9>(value_copy.v);
        REQUIRE(v_copy.size() == 2);
        REQUIRE(v_copy.at(0) == "hello");
        REQUIRE(v_copy.at(1) == "world");
    }

    SECTION ("hash with primitive and bytes") {
        std::unordered_map<int32_t, FBE::buffer_t> m;
        std::vector<uint8_t> v {65, 66, 67, 68, 69};
        m.emplace(42, FBE::buffer_t(v));

        ::variants_ptr::Value value;
        REQUIRE(value.v.index() == 0);
        value.v.emplace<10>(std::move(m));

        FBE::variants_ptr::ValueModel writer;
        size_t serialized = writer.serialize(value);
        REQUIRE(serialized == writer.buffer().size());
        REQUIRE(writer.verify());

        FBE::variants_ptr::ValueModel reader;
        reader.attach(writer.buffer());
        REQUIRE(reader.verify());

        ::variants_ptr::Value value_copy;
        size_t deserialized = reader.deserialize(value_copy);
        REQUIRE(deserialized == reader.buffer().size());

        REQUIRE(value_copy.v.index() == 10);
        auto& v_copy = std::get<10>(value_copy.v);
        REQUIRE(v_copy.size() == 1);
        REQUIRE(v_copy.at(42).string() == "ABCDE");
    }

    SECTION ("hash with string and bytes") {
        std::unordered_map<stdb::memory::string, FBE::buffer_t> m;
        std::vector<uint8_t> v {65, 66, 67, 68, 69};
        m.emplace("hello world", FBE::buffer_t(v));

        ::variants_ptr::Value value;
        REQUIRE(value.v.index() == 0);
        value.v.emplace<11>(std::move(m));

        FBE::variants_ptr::ValueModel writer;
        size_t serialized = writer.serialize(value);
        REQUIRE(serialized == writer.buffer().size());
        REQUIRE(writer.verify());

        FBE::variants_ptr::ValueModel reader;
        reader.attach(writer.buffer());
        REQUIRE(reader.verify());

        ::variants_ptr::Value value_copy;
        size_t deserialized = reader.deserialize(value_copy);
        REQUIRE(deserialized == reader.buffer().size());

        REQUIRE(value_copy.v.index() == 11);
        auto& v_copy = std::get<11>(value_copy.v);
        REQUIRE(v_copy.size() == 1);
        REQUIRE(v_copy.at("hello world").string() == "ABCDE");
    }

    SECTION ("vector of pointer") {
        std::vector<::variants_ptr::Simple*> v;
        auto simple1 = std::make_unique<::variants_ptr::Simple>("simple1");
        auto simple2 = std::make_unique<::variants_ptr::Simple>("simple2");
        v.emplace_back(simple1.get());
        v.emplace_back(simple2.get());

        ::variants_ptr::Value value;
        REQUIRE(value.v.index() == 0);
        value.v.emplace<12>(std::move(v));

        FBE::variants_ptr::ValueModel writer;
        size_t serialized = writer.serialize(value);
        REQUIRE(serialized == writer.buffer().size());
        REQUIRE(writer.verify());

        FBE::variants_ptr::ValueModel reader;
        reader.attach(writer.buffer());
        REQUIRE(reader.verify());

        ::variants_ptr::Value value_copy;
        size_t deserialized = reader.deserialize(value_copy);
        REQUIRE(deserialized == reader.buffer().size());

        REQUIRE(value_copy.v.index() == 12);
        auto& v_copy = std::get<12>(value_copy.v);
        REQUIRE(v_copy.size() == 2);
        REQUIRE(v_copy.at(0)->name == "simple1");
        REQUIRE(v_copy.at(1)->name == "simple2");

        for (auto& s : v_copy) {
            delete s;
        }
    }

    SECTION ("variant used in container") {
        ::variants_ptr::ValueContainer value_container;
        value_container.vv.emplace_back(::variants_ptr::V{42});
        value_container.vv.emplace_back(::variants_ptr::V{stdb::memory::string("42")});
        value_container.vm.emplace(1, ::variants_ptr::V{42});
        value_container.vm.emplace(2, ::variants_ptr::V{stdb::memory::string("42")});

        FBE::variants_ptr::ValueContainerModel writer;
        size_t serialized = writer.serialize(value_container);
        REQUIRE(serialized == writer.buffer().size());
        REQUIRE(writer.verify());

        FBE::variants_ptr::ValueContainerModel reader;
        reader.attach(writer.buffer());
        REQUIRE(reader.verify());

        ::variants_ptr::ValueContainer value_container_copy;
        size_t deserialized = reader.deserialize(value_container_copy);
        REQUIRE(deserialized == reader.buffer().size());

        REQUIRE(value_container_copy.vv.size() == 2);
        REQUIRE(std::holds_alternative<int32_t>(value_container_copy.vv.at(0)));
        REQUIRE(std::get<int32_t>(value_container_copy.vv.at(0)) == 42);
        REQUIRE(std::holds_alternative<stdb::memory::string>(value_container_copy.vv.at(1)));
        REQUIRE(std::get<stdb::memory::string>(value_container_copy.vv.at(1)) == "42");
        REQUIRE(value_container_copy.vm.size() == 2);
        REQUIRE(std::holds_alternative<int32_t>(value_container_copy.vm.at(1)));
        REQUIRE(std::get<int32_t>(value_container_copy.vm.at(1)) == 42);
        REQUIRE(std::holds_alternative<stdb::memory::string>(value_container_copy.vm.at(2)));
        REQUIRE(std::get<stdb::memory::string>(value_container_copy.vm.at(2)) == "42");
    }

    SECTION ("variant of variant") {
        ::variants_ptr::Expr expr {"42"};
        ::variants_ptr::Value value;
        REQUIRE(value.v.index() == 0);
        value.v.emplace<::variants_ptr::Expr>(std::move(expr));
        REQUIRE_FALSE(value.vo.has_value());
        ::variants_ptr::V v {42};
        value.vo.emplace(std::move(v));
        REQUIRE_FALSE(value.vo2.has_value());
        value.vo2.emplace(::variants_ptr::V{::variants_ptr::Simple{"simple"}});


        FBE::variants_ptr::ValueModel writer;
        size_t serialized = writer.serialize(value);
        REQUIRE(serialized == writer.buffer().size());
        REQUIRE(writer.verify());

        FBE::variants_ptr::ValueModel reader;
        reader.attach(writer.buffer());
        REQUIRE(reader.verify());

        ::variants_ptr::Value value_copy;
        size_t deserialized = reader.deserialize(value_copy);
        REQUIRE(deserialized == reader.buffer().size());

        REQUIRE(value_copy.v.index() == 13);
        auto& v_copy_expr = std::get<::variants_ptr::Expr>(value_copy.v);
        REQUIRE(v_copy_expr.index() == 1);
        REQUIRE(std::get<1>(v_copy_expr) == "42");
        REQUIRE(value_copy.vo.has_value());
        REQUIRE(value_copy.vo->index() == 0);
        REQUIRE(std::get<int32_t>(value_copy.vo.value()) == 42);
        REQUIRE(value_copy.vo2.has_value());
        REQUIRE(value_copy.vo2->index() == 3);
        REQUIRE(std::get<::variants_ptr::Simple>(value_copy.vo2.value()).name == "simple");
    }
}


TEST_CASE("Serialization (import template)", "[Ptr-based FBE]") {
    SECTION("variant") {
        ::template_variant::Line line;
        // variant of variant
        ::variants::Expr expr {"42"};
        line.v.emplace<::variants::Expr>(std::move(expr));

        std::vector<::variants::Simple> vs;
        vs.emplace_back(::variants::Simple{"simple1"});
        vs.emplace_back(::variants::Simple{"simple2"});\
        ::variants::V v1;
        v1.emplace<std::vector<::variants::Simple>>(std::move(vs));
        line.vv.emplace_back(std::move(v1));
        ::variants::V v2 {stdb::memory::string("hello")};
        line.vv.emplace_back(std::move(v2));

        ::variants::V v3 {42};
        ::variants::V v4 {::variants::Simple("simple")};
        line.vm.emplace("key3", std::move(v3));
        line.vm.emplace("key4", std::move(v4));

        line.vo.emplace(::variants::V{::variants::Simple{"optional simple"}});

        FBE::template_variant::LineModel writer;
        size_t serialized = writer.serialize(line);
        REQUIRE(serialized == writer.buffer().size());
        REQUIRE(writer.verify());

        FBE::template_variant::LineModel reader;
        reader.attach(writer.buffer());
        REQUIRE(reader.verify());

        ::variants::Line line_copy;
        size_t deserialized = reader.deserialize(line_copy);
        REQUIRE(deserialized == reader.buffer().size());

        REQUIRE(std::holds_alternative<::variants::Expr>(line_copy.v));
        REQUIRE(line_copy.vv.size() == 2);
        REQUIRE(std::holds_alternative<std::vector<::variants::Simple>>(line_copy.vv.at(0)));
        REQUIRE(std::get<std::vector<::variants::Simple>>(line_copy.vv.at(0)).size() == 2);
        REQUIRE(std::get<std::vector<::variants::Simple>>(line_copy.vv.at(0)).at(0).name == "simple1");
        REQUIRE(std::get<std::vector<::variants::Simple>>(line_copy.vv.at(0)).at(1).name == "simple2");
        REQUIRE(std::holds_alternative<stdb::memory::string>(line_copy.vv.at(1)));
        REQUIRE(std::get<stdb::memory::string>(line_copy.vv.at(1)) == "hello");
        REQUIRE(std::holds_alternative<stdb::memory::string>(line_copy.vv.at(1)));
        REQUIRE(line_copy.vm.size() == 2);
        REQUIRE(std::holds_alternative<int32_t>(line_copy.vm.at("key3")));
        REQUIRE(std::get<int32_t>(line_copy.vm.at("key3")) == 42);
        REQUIRE(std::holds_alternative<::variants::Simple>(line_copy.vm.at("key4")));
        REQUIRE(std::get<::variants::Simple>(line_copy.vm.at("key4")).name == "simple");

        REQUIRE(line.vo.has_value());
        REQUIRE(std::holds_alternative<::variants::Simple>(line.vo.value()));
        REQUIRE(std::get<::variants::Simple>(line.vo.value()).name == "optional simple");
    }
    SECTION("enum and variant in a map") {
        ::template_variant::Line2 line;

        ::variants::V v3 {42};
        ::variants::V v4 {::variants::Simple("simple")};
        line.vm.emplace(::enums::EnumInt8::ENUM_VALUE_1, std::move(v3));
        line.vm.emplace(::enums::EnumInt8::ENUM_VALUE_2, std::move(v4));

        FBE::template_variant::Line2Model writer;
        size_t serialized = writer.serialize(line);
        REQUIRE(serialized == writer.buffer().size());
        REQUIRE(writer.verify());

        FBE::template_variant::Line2Model reader;
        reader.attach(writer.buffer());
        REQUIRE(reader.verify());

        ::variants::Line2 line_copy;
        size_t deserialized = reader.deserialize(line_copy);
        REQUIRE(deserialized == reader.buffer().size());

        REQUIRE(line_copy.vm.size() == 2);
        REQUIRE(std::holds_alternative<int32_t>(line_copy.vm.at(::enums::EnumInt8::ENUM_VALUE_1)));
        REQUIRE(std::get<int32_t>(line_copy.vm.at(::enums::EnumInt8::ENUM_VALUE_1)) == 42);
        REQUIRE(std::holds_alternative<::variants::Simple>(line_copy.vm.at(::enums::EnumInt8::ENUM_VALUE_2)));
        REQUIRE(std::get<::variants::Simple>(line_copy.vm.at(::enums::EnumInt8::ENUM_VALUE_2)).name == "simple");

    }
    SECTION("struct") {
        ::template_variant::Line3 line;

        std::vector<uint8_t> v {65, 66, 67, 68, 69};
        std::vector<FBE::buffer_t> bytes_v;
        bytes_v.emplace_back(FBE::buffer_t(v));

        REQUIRE(line.value.v.index() == 0);
        line.value.v.emplace<7>(std::move(bytes_v));

        FBE::template_variant::Line3Model writer;
        size_t serialized = writer.serialize(line);
        REQUIRE(serialized == writer.buffer().size());
        REQUIRE(writer.verify());

        FBE::template_variant::Line3Model reader;
        reader.attach(writer.buffer());
        REQUIRE(reader.verify());

        ::variants::Line3 line_copy;
        size_t deserialized = reader.deserialize(line_copy);
        REQUIRE(deserialized == reader.buffer().size());

        REQUIRE(line_copy.value.v.index() == 7);
        auto& v_copy = std::get<7>(line_copy.value.v);
        REQUIRE(v_copy.size() == 1);
        REQUIRE(v_copy.at(0).string() == "ABCDE");
    }
}
