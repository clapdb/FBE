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
#ifndef GENERATOR_CPP_FIXTURE_H
#define GENERATOR_CPP_FIXTURE_H

#include <regex>
#include <string>

namespace FBE {

class GeneratorCPPFixture {
public:
  static std::string GenerateImports() {
    return R"CODE(
#include <array>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstring>
#include <cctype>
#include <future>
#include <iomanip>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <vector>
#if defined(__APPLE__) && (__clang__)
#include <experimental/memory_resource>
#include <experimental/vector>
#include <experimental/list>
#include <experimental/map>
#include <experimental/unordered_map>
#include <experimental/string>
#include <experimental/set>
namespace std { namespace pmr = ::std::experimental::pmr; }
#else
#include <memory_resource>
#endif
#include <utility>
#include <variant>
#if defined(USING_BTREE_MAP)
#include "container/btree_map.hpp"
#endif

#if defined(USING_SEASTAR_STRING)
#include <seastar/core/sstring.hh>
#elif defined(USING_SMALL_STRING)
#include "smallstring.hpp"
#endif

#if defined(USING_SMALL_ARENA_STRING)
#include "smallstring.hpp"
#endif

namespace FBE {
    template <typename T>
    using FastVec = std::vector<T>;

    #if defined(USING_SEASTAR_STRING)
    using FBEString = seastar::sstring;
    #elif defined(USING_SMALL_STRING)
    using FBEString = small::small_byte_string;
    #else
    using FBEString = std::string;
    #endif

    #if defined(USING_SMALL_ARENA_STRING)
    using ArenaString = small::pmr::small_byte_string;
    #else
    using ArenaString = std::pmr::string;
    #endif

    template <typename Key, typename Compare = std::less<Key>, typename Allocator = std::allocator<Key>>
    #if defined(USING_BTREE_MAP)
    // Use fixed 256-byte node size to support forward-declared types (ptr-based FBE)
    using set = stdb::container::btree_set<Key, Compare, Allocator, 256>;
    #else
    using set = std::set<Key, Compare, Allocator>;
    #endif

    template <typename Key, typename Value, typename Compare = std::less<Key>, typename Allocator = std::allocator<std::pair<const Key, Value>>>
    #if defined(USING_BTREE_MAP)
    // Use fixed 256-byte node size to support forward-declared types (ptr-based FBE)
    using map = stdb::container::btree_map<Key, Value, Compare, Allocator, 256>;
    #else
    using map = std::map<Key, Value, Compare, Allocator>;
    #endif

    // PMR namespace for arena allocator support
    namespace pmr {
        template <typename Key, typename Compare = std::less<Key>>
        #if defined(USING_BTREE_MAP)
        using set = stdb::pmr::btree_set_compact<Key, Compare>;
        #else
        using set = std::pmr::set<Key, Compare>;
        #endif

        template <typename Key, typename Value, typename Compare = std::less<Key>>
        #if defined(USING_BTREE_MAP)
        using map = stdb::pmr::btree_map_compact<Key, Value, Compare>;
        #else
        using map = std::pmr::map<Key, Value, Compare>;
        #endif
    } // namespace pmr

    // Type trait to detect primitive types that can be bulk-copied with memcpy
    // These types have fixed size and trivial copy semantics in FinalModel
    // Note: bool is excluded because std::vector<bool> is a special template that doesn't support .data()
    template <typename T>
    struct is_fbe_final_primitive : std::false_type {};

    template <> struct is_fbe_final_primitive<char> : std::true_type {};
    template <> struct is_fbe_final_primitive<wchar_t> : std::true_type {};
    template <> struct is_fbe_final_primitive<int8_t> : std::true_type {};
    template <> struct is_fbe_final_primitive<uint8_t> : std::true_type {};
    template <> struct is_fbe_final_primitive<int16_t> : std::true_type {};
    template <> struct is_fbe_final_primitive<uint16_t> : std::true_type {};
    template <> struct is_fbe_final_primitive<int32_t> : std::true_type {};
    template <> struct is_fbe_final_primitive<uint32_t> : std::true_type {};
    template <> struct is_fbe_final_primitive<int64_t> : std::true_type {};
    template <> struct is_fbe_final_primitive<uint64_t> : std::true_type {};
    template <> struct is_fbe_final_primitive<float> : std::true_type {};
    template <> struct is_fbe_final_primitive<double> : std::true_type {};

    template <typename T>
    inline constexpr bool is_fbe_final_primitive_v = is_fbe_final_primitive<T>::value;
} // namespace FBE

#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
#include <time.h>
#include <uuid/uuid.h>
#undef HOST_NOT_FOUND
#elif defined(_WIN32) || defined(_WIN64)
#undef DELETE
#undef ERROR
#undef HOST_NOT_FOUND
#undef Yield
#undef min
#undef max
#undef uuid_t
#endif
)CODE";
  }

  static std::string GenerateImportsSource() {
    return R"CODE(
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <rpc.h>
#undef DELETE
#undef ERROR
#undef HOST_NOT_FOUND
#undef Yield
#undef min
#undef max
#undef uuid_t
#endif
)CODE";
  }

  static std::string GenerateBufferWrapper_Header() {
    return R"CODE(
//! Bytes buffer type
/*!
    Represents bytes buffer which is a lightweight wrapper around FastVec<uint8_t>
    with similar interface.
*/
class buffer_t
{
public:
    using iterator = FastVec<uint8_t>::iterator;
    using const_iterator = FastVec<uint8_t>::const_iterator;
    using reverse_iterator = FastVec<uint8_t>::reverse_iterator;
    using const_reverse_iterator = FastVec<uint8_t>::const_reverse_iterator;

    buffer_t() = default;
    buffer_t(size_t capacity) { reserve(capacity); }
    buffer_t(const std::string& str) { assign(str); }
    buffer_t(size_t size, uint8_t value) { assign(size, value); }
    buffer_t(const uint8_t* data, size_t size) { assign(data, size); }
    buffer_t(const FastVec<uint8_t>& other) : _data(other) {}
    buffer_t(FastVec<uint8_t>&& other) : _data(std::move(other)) {}
    buffer_t(const buffer_t& other) = default;
    buffer_t(buffer_t&& other) = default;
    ~buffer_t() = default;

    buffer_t& operator=(const std::string& str) { assign(str); return *this; }
    buffer_t& operator=(const FastVec<uint8_t>& other) { _data = other; return *this; }
    buffer_t& operator=(FastVec<uint8_t>&& other) { _data = std::move(other); return *this; }
    buffer_t& operator=(const buffer_t& other) = default;
    buffer_t& operator=(buffer_t&& other) = default;

    bool operator==(const buffer_t& other) const noexcept { return buffer() == other.buffer(); }
    bool operator!=(const buffer_t& other) const noexcept { return !operator==(other); }

    uint8_t& operator[](size_t index) { return _data[index]; }
    const uint8_t& operator[](size_t index) const { return _data[index]; }

    bool empty() const { return _data.empty(); }
    size_t capacity() const { return _data.capacity(); }
    size_t size() const { return _data.size(); }
    size_t max_size() const { return _data.max_size(); }

    FastVec<uint8_t>& buffer() noexcept { return _data; }
    const FastVec<uint8_t>& buffer() const noexcept { return _data; }
    uint8_t* data() noexcept { return _data.data(); }
    const uint8_t* data() const noexcept { return _data.data(); }
    uint8_t& at(size_t index) { return _data.at(index); }
    const uint8_t& at(size_t index) const { return _data.at(index); }
    uint8_t& front() { return _data.front(); }
    const uint8_t& front() const { return _data.front(); }
    uint8_t& back() { return _data.back(); }
    const uint8_t& back() const { return _data.back(); }

    void reserve(size_t capacity) { _data.reserve(capacity); }
    void resize(size_t size, uint8_t value = 0) { _data.resize(size, value); }
    void shrink_to_fit() { _data.shrink_to_fit(); }

    void assign(const std::string& str) { assign((const uint8_t*)str.c_str(), str.size()); }
    void assign(const FastVec<uint8_t>& vec) { assign(vec.begin(), vec.end()); }
    void assign(size_t size, uint8_t value) { _data.assign(size, value); }
    void assign(const uint8_t* data, size_t size) { _data.assign(data, data + size); }
    template <class InputIterator>
    void assign(InputIterator first, InputIterator last) { _data.assign(first, last); }
    iterator insert(const_iterator position, uint8_t value) { return _data.insert(position, value); }
    iterator insert(const_iterator position, const std::string& str) { return insert(position, (const uint8_t*)str.c_str(), str.size()); }
    iterator insert(const_iterator position, const FastVec<uint8_t>& vec) { return insert(position, vec.begin(), vec.end()); }
    iterator insert(const_iterator position, size_t size, uint8_t value) { return _data.insert(position, size, value); }
    iterator insert(const_iterator position, const uint8_t* data, size_t size) { return _data.insert(position, data, data + size); }
    template <class InputIterator>
    iterator insert(const_iterator position, InputIterator first, InputIterator last) { return _data.insert(position, first, last); }
    iterator erase(const_iterator position) { return _data.erase(position); }
    iterator erase(const_iterator first, const_iterator last) { return _data.erase(first, last); }
    void clear() noexcept { _data.clear(); }

    void push_back(uint8_t value) { _data.push_back(value); }
    void pop_back() { _data.pop_back(); }

    template <class... Args>
    iterator emplace(const_iterator position, Args&&... args) { return _data.emplace(position, args...); }
    template <class... Args>
    void emplace_back(Args&&... args) { _data.emplace_back(args...); }

    iterator begin() noexcept { return _data.begin(); }
    const_iterator begin() const noexcept { return _data.begin(); }
    const_iterator cbegin() const noexcept { return _data.cbegin(); }
    reverse_iterator rbegin() noexcept { return _data.rbegin(); }
    const_reverse_iterator rbegin() const noexcept { return _data.rbegin(); }
    const_reverse_iterator crbegin() const noexcept { return _data.crbegin(); }
    iterator end() noexcept { return _data.end(); }
    const_iterator end() const noexcept { return _data.end(); }
    const_iterator cend() const noexcept { return _data.cend(); }
    reverse_iterator rend() noexcept { return _data.rend(); }
    const_reverse_iterator rend() const noexcept { return _data.rend(); }
    const_reverse_iterator crend() const noexcept { return _data.crend(); }

    //! Get the string equivalent from the bytes buffer
    std::string string() const { return std::string(_data.begin(), _data.end()); }

    //! Encode the Base64 string from the bytes buffer
    std::string base64encode() const;
    //! Decode the bytes buffer from the Base64 string
    static buffer_t base64decode(const std::string& str);

    //! Swap two instances
    void swap(buffer_t& value) noexcept
    { using std::swap; swap(_data, value._data); }
    friend void swap(buffer_t& value1, buffer_t& value2) noexcept
    { value1.swap(value2); }

private:
    FastVec<uint8_t> _data;
};
)CODE";
  }

  static std::string GenerateBufferWrapper_Source() {
    return R"CODE(
std::string buffer_t::base64encode() const
{
    const char base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string result;

    int val = 0;
    int valb = -6;
    for (auto c : _data)
    {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0)
        {
            result.push_back(base64[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }

    if (valb > -6)
        result.push_back(base64[((val << 8) >> (valb + 8)) & 0x3F]);

    while (result.size() % 4)
        result.push_back('=');

    return result;
}

buffer_t buffer_t::base64decode(const std::string& str)
{
    const char base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    buffer_t result;

    std::vector<int> pattern(256, -1);
    for (int i = 0; i < 64; ++i)
        pattern[base64[i]] = i;

    int val = 0;
    int valb = -8;
    for (auto c : str)
    {
        if (pattern[c] == -1)
            break;

        val = (val << 6) + pattern[c];
        valb += 6;

        if (valb >= 0)
        {
            result.push_back((uint8_t)((val >> valb) & 0xFF));
            valb -= 8;
        }
    }

    return result;
}
)CODE";
  }

  static std::string GeneratePMRBufferWrapper_Header() {
    return R"CODE(
//! PMR bytes buffer type
/*!
    Represents pmr bytes buffer which is a lightweight wrapper around std::pmr::vector<uint8_t>
    with similar interface.
*/
class pmr_buffer_t
{
public:
    using iterator = std::pmr::vector<uint8_t>::iterator;
    using const_iterator = std::pmr::vector<uint8_t>::const_iterator;
    using reverse_iterator = std::pmr::vector<uint8_t>::reverse_iterator;
    using const_reverse_iterator = std::pmr::vector<uint8_t>::const_reverse_iterator;
    using allocator_type = std::pmr::polymorphic_allocator<char>;

    pmr_buffer_t() = default;
    explicit pmr_buffer_t(allocator_type alloc): _data(alloc) {}
    explicit pmr_buffer_t(size_t capacity) { reserve(capacity); }
    explicit pmr_buffer_t(const ArenaString& str) { assign(str); }
    pmr_buffer_t(size_t size, uint8_t value) { assign(size, value); }
    pmr_buffer_t(const uint8_t* data, size_t size) { assign(data, size); }
    explicit pmr_buffer_t(const std::pmr::vector<uint8_t>& other) : _data(other) {}
    explicit pmr_buffer_t(std::pmr::vector<uint8_t>&& other) : _data(std::move(other)) {}
    pmr_buffer_t(const pmr_buffer_t& other) = default;
    explicit pmr_buffer_t(pmr_buffer_t&& other) = default;
    pmr_buffer_t(const pmr_buffer_t& other, allocator_type alloc): _data(alloc) {
        for (const uint8_t v: other.buffer()) {
            _data.emplace_back(v);
        }
    };
    ~pmr_buffer_t() = default;

    pmr_buffer_t& operator=(const ArenaString& str) { assign(str); return *this; }
    pmr_buffer_t& operator=(const std::pmr::vector<uint8_t>& other) { _data = other; return *this; }
    pmr_buffer_t& operator=(std::pmr::vector<uint8_t>&& other) { _data = std::move(other); return *this; }
    pmr_buffer_t& operator=(const pmr_buffer_t& other) = default;
    pmr_buffer_t& operator=(pmr_buffer_t&& other) = default;

    bool operator==(const pmr_buffer_t& other) const noexcept { return buffer() == other.buffer(); }
    bool operator!=(const pmr_buffer_t& other) const noexcept { return !operator==(other); }

    uint8_t& operator[](size_t index) { return _data[index]; }
    const uint8_t& operator[](size_t index) const { return _data[index]; }

    bool empty() const { return _data.empty(); }
    size_t capacity() const { return _data.capacity(); }
    size_t size() const { return _data.size(); }
    size_t max_size() const { return _data.max_size(); }

    std::pmr::vector<uint8_t>& buffer() noexcept { return _data; }
    const std::pmr::vector<uint8_t>& buffer() const noexcept { return _data; }
    uint8_t* data() noexcept { return _data.data(); }
    const uint8_t* data() const noexcept { return _data.data(); }
    uint8_t& at(size_t index) { return _data.at(index); }
    const uint8_t& at(size_t index) const { return _data.at(index); }
    uint8_t& front() { return _data.front(); }
    const uint8_t& front() const { return _data.front(); }
    uint8_t& back() { return _data.back(); }
    const uint8_t& back() const { return _data.back(); }

    void reserve(size_t capacity) { _data.reserve(capacity); }
    void resize(size_t size, uint8_t value = 0) { _data.resize(size, value); }
    void shrink_to_fit() { _data.shrink_to_fit(); }

    void assign(const ArenaString& str) { assign((const uint8_t*)str.data(), str.size()); }
    void assign(const std::pmr::vector<uint8_t>& vec) { assign(vec.begin(), vec.end()); }
    void assign(size_t size, uint8_t value) { _data.assign(size, value); }
    void assign(const uint8_t* data, size_t size) { _data.assign(data, data + size); }
    template <class InputIterator>
    void assign(InputIterator first, InputIterator last) { _data.assign(first, last); }
    iterator insert(const_iterator position, uint8_t value) { return _data.insert(position, value); }
    iterator insert(const_iterator position, const ArenaString& str) { return insert(position, (const uint8_t*)str.data(), str.size()); }
    iterator insert(const_iterator position, const std::pmr::vector<uint8_t>& vec) { return insert(position, vec.begin(), vec.end()); }
    iterator insert(const_iterator position, size_t size, uint8_t value) { return _data.insert(position, size, value); }
    iterator insert(const_iterator position, const uint8_t* data, size_t size) { return _data.insert(position, data, data + size); }
    template <class InputIterator>
    iterator insert(const_iterator position, InputIterator first, InputIterator last) { return _data.insert(position, first, last); }
    iterator erase(const_iterator position) { return _data.erase(position); }
    iterator erase(const_iterator first, const_iterator last) { return _data.erase(first, last); }
    void clear() noexcept { _data.clear(); }

    void push_back(uint8_t value) { _data.push_back(value); }
    void pop_back() { _data.pop_back(); }

    template <class... Args>
    iterator emplace(const_iterator position, Args&&... args) { return _data.emplace(position, args...); }
    template <class... Args>
    void emplace_back(Args&&... args) { _data.emplace_back(args...); }

    iterator begin() noexcept { return _data.begin(); }
    const_iterator begin() const noexcept { return _data.begin(); }
    const_iterator cbegin() const noexcept { return _data.cbegin(); }
    reverse_iterator rbegin() noexcept { return _data.rbegin(); }
    const_reverse_iterator rbegin() const noexcept { return _data.rbegin(); }
    const_reverse_iterator crbegin() const noexcept { return _data.crbegin(); }
    iterator end() noexcept { return _data.end(); }
    const_iterator end() const noexcept { return _data.end(); }
    const_iterator cend() const noexcept { return _data.cend(); }
    reverse_iterator rend() noexcept { return _data.rend(); }
    const_reverse_iterator rend() const noexcept { return _data.rend(); }
    const_reverse_iterator crend() const noexcept { return _data.crend(); }

    //! Get the string equivalent from the bytes buffer
    std::string string() const { return std::string(_data.begin(), _data.end()); }

    //! Encode the Base64 string from the bytes buffer
    std::string base64encode() const;
    //! Decode the bytes buffer from the Base64 string
    static buffer_t base64decode(const std::string& str);

    //! Swap two instances
    void swap(pmr_buffer_t& value) noexcept
    { using std::swap; swap(_data, value._data); }
    friend void swap(pmr_buffer_t& value1, pmr_buffer_t& value2) noexcept
    { value1.swap(value2); }

    //! Output instance into the given output stream
    friend std::ostream& operator<<(std::ostream& os, const pmr_buffer_t& value)
    { os << value.string(); return os; }

private:
    std::pmr::vector<uint8_t> _data;
    
};
)CODE";
  }

  static std::string GeneratePMRBufferWrapper_Source() {
    return R"CODE(
std::string pmr_buffer_t::base64encode() const
{
    const char base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string result;

    int val = 0;
    int valb = -6;
    for (auto c : _data)
    {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0)
        {
            result.push_back(base64[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }

    if (valb > -6)
        result.push_back(base64[((val << 8) >> (valb + 8)) & 0x3F]);

    while (result.size() % 4)
        result.push_back('=');

    return result;
}

buffer_t pmr_buffer_t::base64decode(const std::string& str)
{
    const char base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    buffer_t result;

    std::vector<int> pattern(256, -1);
    for (int i = 0; i < 64; ++i)
        pattern[base64[i]] = i;

    int val = 0;
    int valb = -8;
    for (auto c : str)
    {
        if (pattern[c] == -1)
            break;

        val = (val << 6) + pattern[c];
        valb += 6;

        if (valb >= 0)
        {
            result.push_back((uint8_t)((val >> valb) & 0xFF));
            valb -= 8;
        }
    }

    return result;
}
)CODE";
  }

  static std::string GenerateDecimalWrapper_Header() {
    return R"CODE(
//! Decimal type
/*!
    Represents decimal type using double and provides basic arithmetic operations.
*/
class decimal_t
{
public:
    decimal_t() noexcept { _value = 0.0; }
    decimal_t(int8_t value) noexcept { _value = (double)value; }
    decimal_t(uint8_t value) noexcept { _value = (double)value; }
    decimal_t(int16_t value) noexcept { _value = (double)value; }
    decimal_t(uint16_t value) noexcept { _value = (double)value; }
    decimal_t(int32_t value) noexcept { _value = (double)value; }
    decimal_t(uint32_t value) noexcept { _value = (double)value; }
    decimal_t(int64_t value) noexcept { _value = (double)value; }
    decimal_t(uint64_t value) noexcept { _value = (double)value; }
    decimal_t(float value) noexcept { _value = (double)value; }
    decimal_t(double value) noexcept { _value = value; }
    template <typename T> requires (!std::same_as<T, std::pmr::memory_resource*>)
    explicit decimal_t(const T& value) noexcept { _value = (double)value; }
    decimal_t(const decimal_t& value) noexcept = default;
    decimal_t(decimal_t&& value) noexcept = default;
    ~decimal_t() noexcept = default;

    template <typename T> requires (!std::same_as<T, std::pmr::memory_resource*>)
    decimal_t& operator=(const T& value) noexcept { _value = (double)value; return *this; }
    decimal_t& operator=(const decimal_t& value) noexcept = default;
    decimal_t& operator=(decimal_t&& value) noexcept = default;

    // Arithmetic operators
    decimal_t operator+() const noexcept { return decimal_t(_value); }
    decimal_t operator-() const noexcept { return decimal_t(-_value); }

    decimal_t& operator++() noexcept { return *this += 1; }
    decimal_t operator++(int) noexcept { decimal_t temp(*this); ++*this; return temp; }
    decimal_t& operator--() noexcept { return *this -= 1; }
    decimal_t operator--(int) noexcept { decimal_t temp(*this); --*this; return temp; }

    decimal_t& operator+=(const decimal_t& value) noexcept { return *this = *this + value; }
    decimal_t& operator-=(const decimal_t& value) noexcept { return *this = *this - value; }
    decimal_t& operator*=(const decimal_t& value) noexcept { return *this = *this * value; }
    decimal_t& operator/=(const decimal_t& value) { return *this = *this / value; }

    template <typename T>
    decimal_t& operator+=(const T& value) noexcept { return *this = *this + decimal_t(value); }
    template <typename T>
    decimal_t& operator-=(const T& value) noexcept { return *this = *this - decimal_t(value); }
    template <typename T>
    decimal_t& operator*=(const T& value) noexcept { return *this = *this * decimal_t(value); }
    template <typename T>
    decimal_t& operator/=(const T& value) { return *this = *this / decimal_t(value); }

    template <typename T>
    friend T& operator+=(T& value1, const decimal_t& value2) noexcept { return value1 = (T)(decimal_t(value1) + value2); }
    template <typename T>
    friend T& operator-=(T& value1, const decimal_t& value2) noexcept { return value1 = (T)(decimal_t(value1) - value2); }
    template <typename T>
    friend T& operator*=(T& value1, const decimal_t& value2) noexcept { return value1 = (T)(decimal_t(value1) * value2); }
    template <typename T>
    friend T& operator/=(T& value1, const decimal_t& value2) { return value1 = (T)(decimal_t(value1) / value2); }

    template <typename T>
    friend decimal_t operator+(const T& value1, const decimal_t& value2) noexcept { return decimal_t(value1) + value2; }
    template <typename T>
    friend decimal_t operator+(const decimal_t& value1, const T& value2) noexcept { return value1 + decimal_t(value2); }
    friend decimal_t operator+(const decimal_t& value1, const decimal_t& value2) noexcept { return decimal_t(value1._value + value2._value); }

    template <typename T>
    friend decimal_t operator-(const T& value1, const decimal_t& value2) noexcept { return decimal_t(value1) - value2; }
    template <typename T>
    friend decimal_t operator-(const decimal_t& value1, const T& value2) noexcept { return value1 - decimal_t(value2); }
    friend decimal_t operator-(const decimal_t& value1, const decimal_t& value2) noexcept { return decimal_t(value1._value - value2._value); }

    template <typename T>
    friend decimal_t operator*(const T& value1, const decimal_t& value2) noexcept { return decimal_t(value1) * value2; }
    template <typename T>
    friend decimal_t operator*(const decimal_t& value1, const T& value2) noexcept { return value1 * decimal_t(value2); }
    friend decimal_t operator*(const decimal_t& value1, const decimal_t& value2) noexcept { return decimal_t(value1._value * value2._value); }

    template <typename T>
    friend decimal_t operator/(const T& value1, const decimal_t& value2) { return decimal_t(value1) / value2; }
    template <typename T>
    friend decimal_t operator/(const decimal_t& value1, const T& value2) { return value1 / decimal_t(value2); }
    friend decimal_t operator/(const decimal_t& value1, const decimal_t& value2) { return decimal_t(value1._value / value2._value); }

    // Comparison operators
    template <typename T>
    friend bool operator==(const T& value1, const decimal_t& value2) noexcept { return decimal_t(value1) == value2; }
    template <typename T>
    friend bool operator==(const decimal_t& value1, const T& value2) noexcept { return value1 == decimal_t(value2); }
    friend bool operator==(const decimal_t& value1, const decimal_t& value2) noexcept { return value1._value == value2._value; }

    template <typename T>
    friend bool operator!=(const T& value1, const decimal_t& value2) noexcept { return decimal_t(value1) != value2; }
    template <typename T>
    friend bool operator!=(const decimal_t& value1, const T& value2) noexcept { return value1 != decimal_t(value2); }
    friend bool operator!=(const decimal_t& value1, const decimal_t& value2) noexcept { return value1._value != value2._value; }

    template <typename T>
    friend bool operator<(const T& value1, const decimal_t& value2) noexcept { return decimal_t(value1) < value2; }
    template <typename T>
    friend bool operator<(const decimal_t& value1, const T& value2) noexcept { return value1 < decimal_t(value2); }
    friend bool operator<(const decimal_t& value1, const decimal_t& value2) noexcept { return value1._value < value2._value; }

    template <typename T>
    friend bool operator>(const T& value1, const decimal_t& value2) noexcept { return decimal_t(value1) > value2; }
    template <typename T>
    friend bool operator>(const decimal_t& value1, const T& value2) noexcept { return value1 > decimal_t(value2); }
    friend bool operator>(const decimal_t& value1, const decimal_t& value2) noexcept { return value1._value > value2._value; }

    template <typename T>
    friend bool operator<=(const T& value1, const decimal_t& value2) noexcept { return decimal_t(value1) <= value2; }
    template <typename T>
    friend bool operator<=(const decimal_t& value1, const T& value2) noexcept { return value1 <= decimal_t(value2); }
    friend bool operator<=(const decimal_t& value1, const decimal_t& value2) noexcept { return value1._value <= value2._value; }

    template <typename T>
    friend bool operator>=(const T& value1, const decimal_t& value2) noexcept { return decimal_t(value1) >= value2; }
    template <typename T>
    friend bool operator>=(const decimal_t& value1, const T& value2) noexcept { return value1 >= decimal_t(value2); }
    friend bool operator>=(const decimal_t& value1, const decimal_t& value2) noexcept { return value1._value >= value2._value; }

    // Type cast
    operator bool() const noexcept { return (_value != 0.0); }
    operator uint8_t() const noexcept { return (uint8_t)_value; }
    operator uint16_t() const noexcept { return (uint16_t)_value; }
    operator uint32_t() const noexcept { return (uint32_t)_value; }
    operator uint64_t() const noexcept { return (uint64_t)_value; }
    operator float() const noexcept { return (float)_value; }
    operator double() const noexcept { return (double)_value; }

    //! Get string from the current decimal value
    std::string string() const { return std::to_string(_value); }

    //! Input instance from the given input stream
    friend std::istream& operator>>(std::istream& is, decimal_t& value)
    { is >> value._value; return is; }
    //! Output instance into the given output stream
    friend std::ostream& operator<<(std::ostream& os, const decimal_t& value)
    { os << value.string(); return os; }

#if defined(LOGGING_PROTOCOL)
    //! Store logging format
    friend CppLogging::Record& operator<<(CppLogging::Record& record, const decimal_t& value)
    { return record.StoreCustom(value._value); }
#endif

    //! Swap two instances
    void swap(decimal_t& value) noexcept
    { using std::swap; swap(_value, value._value); }
    friend void swap(decimal_t& value1, decimal_t& value2) noexcept
    { value1.swap(value2); }

private:
    double _value;
};

} // namespace FBE

template <>
struct std::hash<FBE::decimal_t>
{
    typedef FBE::decimal_t argument_type;
    typedef size_t result_type;

    result_type operator() (const argument_type& value) const
    {
        result_type result = 17;
        result = result * 31 + std::hash<double>()((double)value);
        return result;
    }
};

namespace FBE {
)CODE";
  }

  static std::string GenerateFlagsWrapper_Header() {
    return R"CODE(
// Register a new enum-based flags macro
#define FBE_ENUM_FLAGS(type)\
inline FBE::Flags<type> operator|(type f1, type f2) noexcept { return FBE::Flags<type>(f1) | FBE::Flags<type>(f2); }\
inline FBE::Flags<type> operator&(type f1, type f2) noexcept { return FBE::Flags<type>(f1) & FBE::Flags<type>(f2); }\
inline FBE::Flags<type> operator^(type f1, type f2) noexcept { return FBE::Flags<type>(f1) ^ FBE::Flags<type>(f2); }

// Enum-based flags
template <typename TEnum>
class Flags
{
    // Enum underlying type
    typedef typename std::make_unsigned<typename std::underlying_type<TEnum>::type>::type type;

public:
    Flags() noexcept : _value(0) {}
    explicit Flags(type value) noexcept : _value(value) {}
    explicit Flags(TEnum value) noexcept : _value((type)value) {}
    Flags(const Flags&) noexcept = default;
    Flags(Flags&&) noexcept = default;
    ~Flags() noexcept = default;

    Flags& operator=(type value) noexcept
    { _value = value; return *this; }
    Flags& operator=(TEnum value) noexcept
    { _value = (type)value; return *this; }
    Flags& operator=(const Flags&) noexcept = default;
    Flags& operator=(Flags&&) noexcept = default;

    // Is any flag set?
    explicit operator bool() const noexcept { return isset(); }

    // Is no flag set?
    bool operator!() const noexcept { return !isset(); }

    // Reverse all flags
    Flags operator~() const noexcept { return Flags(~_value); }

    // Flags logical assign operators
    Flags& operator&=(const Flags& flags) noexcept
    { _value &= flags._value; return *this; }
    Flags& operator|=(const Flags& flags) noexcept
    { _value |= flags._value; return *this; }
    Flags& operator^=(const Flags& flags) noexcept
    { _value ^= flags._value; return *this; }

    // Flags logical friend operators
    friend Flags operator&(const Flags& flags1, const Flags& flags2) noexcept
    { return Flags(flags1._value & flags2._value); }
    friend Flags operator|(const Flags& flags1, const Flags& flags2) noexcept
    { return Flags(flags1._value | flags2._value); }
    friend Flags operator^(const Flags& flags1, const Flags& flags2) noexcept
    { return Flags(flags1._value ^ flags2._value); }

    // Flags comparison
    friend bool operator==(const Flags& flags1, const Flags& flags2) noexcept
    { return flags1._value == flags2._value; }
    friend bool operator!=(const Flags& flags1, const Flags& flags2) noexcept
    { return flags1._value != flags2._value; }

    // Convert to the enum value
    operator TEnum() const noexcept { return (TEnum)_value; }

    //! Is any flag set?
    bool isset() const noexcept { return (_value != 0); }
    //! Is the given flag set?
    bool isset(type value) const noexcept { return (_value & value) != 0; }
    //! Is the given flag set?
    bool isset(TEnum value) const noexcept { return (_value & (type)value) != 0; }

    // Get the enum value
    TEnum value() const noexcept { return (TEnum)_value; }
    // Get the underlying enum value
    type underlying() const noexcept { return _value; }
    // Get the bitset value
    std::bitset<sizeof(type) * 8> bitset() const noexcept { return {_value}; }

    // Swap two instances
    void swap(Flags& flags) noexcept { using std::swap; swap(_value, flags._value); }
    template <typename UEnum>
    friend void swap(Flags<UEnum>& flags1, Flags<UEnum>& flags2) noexcept;

private:
    type _value;
};

template <typename TEnum>
inline void swap(Flags<TEnum>& flags1, Flags<TEnum>& flags2) noexcept
{
    flags1.swap(flags2);
}
)CODE";
  }

  static std::string GenerateTimeWrapper_Header() {
    return R"CODE(
// Get Epoch timestamp
inline uint64_t epoch() { return 0ull; }
// Get UTC timestamp
uint64_t utc();
)CODE";
  }

  static std::string GenerateTimeWrapper_Source() {
    return R"CODE(
uint64_t utc()
{
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    struct timespec timestamp;
    if (clock_gettime(CLOCK_REALTIME, &timestamp) != 0)
        throw std::runtime_error("Cannot get value of CLOCK_REALTIME timer!");
    return (timestamp.tv_sec * 1000000000) + timestamp.tv_nsec;
#elif defined(_WIN32) || defined(_WIN64)
    FILETIME ft;
    GetSystemTimePreciseAsFileTime(&ft);

    ULARGE_INTEGER result;
    result.LowPart = ft.dwLowDateTime;
    result.HighPart = ft.dwHighDateTime;
    return (result.QuadPart - 116444736000000000ull) * 100;
#endif
}
)CODE";
  }

  static std::string GenerateUUIDWrapper_Header() {
    return R"CODE(
//! Universally unique identifier (UUID)
/*!
    A universally unique identifier (UUID) is an identifier standard used
    in software construction. This implementation generates the following
    UUID types:
    - Nil UUID0 (all bits set to zero)
    - Sequential UUID1 (time based version)
    - Random UUID4 (randomly or pseudo-randomly generated version)

    A UUID is simply a 128-bit value: "123e4567-e89b-12d3-a456-426655440000"

    https://en.wikipedia.org/wiki/Universally_unique_identifier
    https://www.ietf.org/rfc/rfc4122.txt
*/
class uuid_t
{
public:
    //! Default constructor
    uuid_t() : _data() { _data.fill(0); }
    //! Initialize UUID with a given string
    /*!
        \param uuid - UUID string
    */
    explicit uuid_t(const std::string& uuid);
    //! Initialize UUID with a given 16 bytes data buffer
    /*!
        \param data - UUID 16 bytes data buffer
    */
    explicit uuid_t(const std::array<uint8_t, 16>& data) : _data(data) {}
    uuid_t(const uuid_t&) = default;
    uuid_t(uuid_t&&) noexcept = default;
    ~uuid_t() = default;

    uuid_t& operator=(const std::string& uuid)
    { _data = uuid_t(uuid).data(); return *this; }
    uuid_t& operator=(const std::array<uint8_t, 16>& data)
    { _data = data; return *this; }
    uuid_t& operator=(const uuid_t&) = default;
    uuid_t& operator=(uuid_t&&) noexcept = default;

    // UUID comparison
    friend bool operator==(const uuid_t& uuid1, const uuid_t& uuid2)
    { return uuid1._data == uuid2._data; }
    friend bool operator!=(const uuid_t& uuid1, const uuid_t& uuid2)
    { return uuid1._data != uuid2._data; }
    friend bool operator<(const uuid_t& uuid1, const uuid_t& uuid2)
    { return uuid1._data < uuid2._data; }
    friend bool operator>(const uuid_t& uuid1, const uuid_t& uuid2)
    { return uuid1._data > uuid2._data; }
    friend bool operator<=(const uuid_t& uuid1, const uuid_t& uuid2)
    { return uuid1._data <= uuid2._data; }
    friend bool operator>=(const uuid_t& uuid1, const uuid_t& uuid2)
    { return uuid1._data >= uuid2._data; }

    //! Check if the UUID is nil UUID0 (all bits set to zero)
    explicit operator bool() const noexcept { return *this != nil(); }

    //! Get the UUID data buffer
    std::array<uint8_t, 16>& data() noexcept { return _data; }
    //! Get the UUID data buffer
    const std::array<uint8_t, 16>& data() const noexcept { return _data; }

    //! Get string from the current UUID in format "00000000-0000-0000-0000-000000000000"
    std::string string() const;

    //! Generate nil UUID0 (all bits set to zero)
    static uuid_t nil() { return uuid_t(); }
    //! Generate sequential UUID1 (time based version)
    static uuid_t sequential();
    //! Generate random UUID4 (randomly or pseudo-randomly generated version)
    static uuid_t random();

    //! Output instance into the given output stream
    friend std::ostream& operator<<(std::ostream& os, const uuid_t& uuid)
    { os << uuid.string(); return os; }

#if defined(LOGGING_PROTOCOL)
    //! Store logging format
    friend CppLogging::Record& operator<<(CppLogging::Record& record, const uuid_t& uuid);
#endif

    //! Swap two instances
    void swap(uuid_t& uuid) noexcept
    { using std::swap; swap(_data, uuid._data); }
    friend void swap(uuid_t& uuid1, uuid_t& uuid2) noexcept
    { uuid1.swap(uuid2); }

private:
    std::array<uint8_t, 16> _data;
};

} // namespace FBE

template <>
struct std::hash<FBE::uuid_t>
{
    typedef FBE::uuid_t argument_type;
    typedef size_t result_type;

    result_type operator() (const argument_type& value) const
    {
        result_type result = 17;
        std::hash<uint8_t> hasher;
        for (size_t i = 0; i < value.data().size(); ++i)
            result = result * 31 + hasher(value.data()[i]);
        return result;
    }
};

namespace FBE {
)CODE";
  }

  static std::string GenerateUUIDWrapper_Source() {
    return R"CODE(
uint8_t unhex(char ch)
{
    if ((ch >= '0') && (ch <= '9'))
        return ch - '0';
    else if ((ch >= 'a') && (ch <= 'f'))
        return 10 + ch - 'a';
    else if ((ch >= 'A') && (ch <= 'F'))
        return 10 + ch - 'A';
    else
        return 255;
}

uuid_t::uuid_t(const std::string& uuid)
{
    char v1 = 0;
    char v2 = 0;
    bool pack = false;
    size_t index = 0;

    // Parse UUID string
    for (auto ch : uuid)
    {
        if ((ch == '-') || (ch == '{') || (ch == '}'))
            continue;

        if (pack)
        {
            v2 = ch;
            pack = false;
            uint8_t ui1 = unhex(v1);
            uint8_t ui2 = unhex(v2);
            if ((ui1 > 15) || (ui2 > 15))
                throw std::invalid_argument("Invalid UUID string: " + uuid);
            _data[index++] = ui1 * 16 + ui2;
            if (index >= 16)
                break;
        }
        else
        {
            v1 = ch;
            pack = true;
        }
    }

    // Fill remaining data with zeros
    for (; index < 16; ++index)
        _data[index++] = 0;
}

std::string uuid_t::string() const
{
    const char* digits = "0123456789abcdef";

    std::string result(36, '0');

    int index = 0;
    for (auto value : _data)
    {
        result[index++] = digits[(value >> 4) & 0x0F];
        result[index++] = digits[(value >> 0) & 0x0F];
        if ((index == 8) || (index == 13) || (index == 18) || (index == 23))
            result[index++] = '-';
    }

    return result;
}

uuid_t uuid_t::sequential()
{
    uuid_t result;
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    ::uuid_t uuid;
    uuid_generate_time(uuid);
    result._data[0] = uuid[0];
    result._data[1] = uuid[1];
    result._data[2] = uuid[2];
    result._data[3] = uuid[3];
    result._data[4] = uuid[4];
    result._data[5] = uuid[5];
    result._data[6] = uuid[6];
    result._data[7] = uuid[7];
    result._data[8] = uuid[8];
    result._data[9] = uuid[9];
    result._data[10] = uuid[10];
    result._data[11] = uuid[11];
    result._data[12] = uuid[12];
    result._data[13] = uuid[13];
    result._data[14] = uuid[14];
    result._data[15] = uuid[15];
#elif defined(_WIN32) || defined(_WIN64)
    ::UUID uuid;
    if (UuidCreateSequential(&uuid) != RPC_S_OK)
        throw std::runtime_error("Cannot generate sequential UUID!");

    result._data[0] = (uuid.Data1 >> 24) & 0xFF;
    result._data[1] = (uuid.Data1 >> 16) & 0xFF;
    result._data[2] = (uuid.Data1 >>  8) & 0xFF;
    result._data[3] = (uuid.Data1 >>  0) & 0xFF;
    result._data[4] = (uuid.Data2 >>  8) & 0xFF;
    result._data[5] = (uuid.Data2 >>  0) & 0xFF;

    result._data[6] = (uuid.Data3 >>  8) & 0xFF;
    result._data[7] = (uuid.Data3 >>  0) & 0xFF;

    result._data[8] = uuid.Data4[0];
    result._data[9] = uuid.Data4[1];

    result._data[10] = uuid.Data4[2];
    result._data[11] = uuid.Data4[3];
    result._data[12] = uuid.Data4[4];
    result._data[13] = uuid.Data4[5];
    result._data[14] = uuid.Data4[6];
    result._data[15] = uuid.Data4[7];
#endif
    return result;
}

uuid_t uuid_t::random()
{
    uuid_t result;
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    ::uuid_t uuid;
    uuid_generate_random(uuid);
    result._data[0] = uuid[0];
    result._data[1] = uuid[1];
    result._data[2] = uuid[2];
    result._data[3] = uuid[3];
    result._data[4] = uuid[4];
    result._data[5] = uuid[5];
    result._data[6] = uuid[6];
    result._data[7] = uuid[7];
    result._data[8] = uuid[8];
    result._data[9] = uuid[9];
    result._data[10] = uuid[10];
    result._data[11] = uuid[11];
    result._data[12] = uuid[12];
    result._data[13] = uuid[13];
    result._data[14] = uuid[14];
    result._data[15] = uuid[15];
#elif defined(_WIN32) || defined(_WIN64)
    ::UUID uuid;
    if (UuidCreate(&uuid) != RPC_S_OK)
        throw std::runtime_error("Cannot generate random UUID!");

    result._data[0] = (uuid.Data1 >> 24) & 0xFF;
    result._data[1] = (uuid.Data1 >> 16) & 0xFF;
    result._data[2] = (uuid.Data1 >>  8) & 0xFF;
    result._data[3] = (uuid.Data1 >>  0) & 0xFF;
    result._data[4] = (uuid.Data2 >>  8) & 0xFF;
    result._data[5] = (uuid.Data2 >>  0) & 0xFF;

    result._data[6] = (uuid.Data3 >>  8) & 0xFF;
    result._data[7] = (uuid.Data3 >>  0) & 0xFF;

    result._data[8] = uuid.Data4[0];
    result._data[9] = uuid.Data4[1];

    result._data[10] = uuid.Data4[2];
    result._data[11] = uuid.Data4[3];
    result._data[12] = uuid.Data4[4];
    result._data[13] = uuid.Data4[5];
    result._data[14] = uuid.Data4[6];
    result._data[15] = uuid.Data4[7];
#endif
    return result;
}

#if defined(LOGGING_PROTOCOL)
CppLogging::Record& operator<<(CppLogging::Record& record, const uuid_t& uuid)
{
    const char* digits = "0123456789abcdef";

    std::array<char, 36> result;

    int index = 0;
    for (auto value : uuid.data())
    {
        result[index++] = digits[(value >> 4) & 0x0F];
        result[index++] = digits[(value >> 0) & 0x0F];
        if ((index == 8) || (index == 13) || (index == 18) || (index == 23))
            result[index++] = '-';
    }

    return record.StoreCustom(std::string_view(result.data(), result.size()));
}
#endif
)CODE";
  }

  static std::string GenerateFBEBuffer_Header() {
    return R"CODE(
// Fast Binary Encoding buffer based on the dynamic byte buffer
class FBEBuffer
{
public:
    FBEBuffer() : _data(nullptr), _capacity(0), _size(0), _offset(0) {}
    // Initialize the read buffer with the given byte buffer and offset
    explicit FBEBuffer(const void* data, size_t size, size_t offset = 0) { attach(data, size, offset); }
    // Initialize the read buffer with the given byte vector and offset
    explicit FBEBuffer(const FastVec<uint8_t>& buffer, size_t offset = 0) { attach(buffer, offset); }
    // Initialize the read buffer with another buffer and offset
    explicit FBEBuffer(const FBEBuffer& buffer, size_t offset = 0) { attach(buffer.data(), buffer.size(), offset); }
    // Initialize the write buffer with the given capacity
    explicit FBEBuffer(size_t capacity) : FBEBuffer() { reserve(capacity); }
    FBEBuffer(const FBEBuffer&) = delete;
    FBEBuffer(FBEBuffer&&) noexcept = delete;
    ~FBEBuffer() { if (_capacity > 0) std::free(_data); }

    FBEBuffer& operator=(const FBEBuffer&) = delete;
    FBEBuffer& operator=(FBEBuffer&&) noexcept = delete;

    bool empty() const noexcept { return (_data == nullptr) || (_size == 0); }
    const uint8_t* data() const noexcept { return _data; }
    uint8_t* data() noexcept { return _data; }
    size_t capacity() const noexcept { return _capacity; }
    size_t size() const noexcept { return _size; }
    size_t offset() const noexcept { return _offset; }

    // Attach the given buffer with a given offset to the current read buffer
    void attach(const void* data, size_t size, size_t offset = 0);
    // Attach the given byte vector with a given offset to the current read buffer
    void attach(const FastVec<uint8_t>& buffer, size_t offset = 0);

    // Clone the given buffer with a given offset to the current buffer
    void clone(const void* data, size_t size, size_t offset = 0);
    // Clone the given vector with a given offset to the current buffer
    void clone(const FastVec<uint8_t>& buffer, size_t offset = 0);

    // Allocate memory in the current write buffer and return offset to the allocated memory block
    size_t allocate(size_t size);
    // Remove some memory of the given size from the current write buffer
    void remove(size_t offset, size_t size);
    // Reserve memory of the given capacity in the current write buffer
    void reserve(size_t capacity);
    // Resize the current write buffer
    void resize(size_t size);
    // Reset the current write buffer and its offset
    void reset();

    // Shift the current write buffer offset
    void shift(size_t offset) { _offset += offset; }
    // Unshift the current write buffer offset
    void unshift(size_t offset) { _offset -= offset; }

private:
    uint8_t* _data;
    size_t _capacity;
    size_t _size;
    size_t _offset;
};
)CODE";
  }

  static std::string GenerateFBEBuffer_Source() {
    return R"CODE(
void FBEBuffer::attach(const void* data, size_t size, size_t offset)
{
    assert((data != nullptr) && "Invalid buffer!");
    if (data == nullptr)
        throw std::invalid_argument("Invalid buffer!");
    assert((size > 0) && "Invalid size!");
    if (size == 0)
        throw std::invalid_argument("Invalid size!");
    assert((offset <= size) && "Invalid offset!");
    if (offset > size)
        throw std::invalid_argument("Invalid offset!");

    _data = (uint8_t*)data;
    _capacity = 0;
    _size = size;
    _offset = offset;
}

void FBEBuffer::attach(const FastVec<uint8_t>& buffer, size_t offset)
{
    assert((buffer.data() != nullptr) && "Invalid buffer!");
    if (buffer.data() == nullptr)
        throw std::invalid_argument("Invalid buffer!");
    assert((buffer.size() > 0) && "Invalid size!");
    if (buffer.size() == 0)
        throw std::invalid_argument("Invalid size!");
    assert((offset <= buffer.size()) && "Invalid offset!");
    if (offset > buffer.size())
        throw std::invalid_argument("Invalid offset!");

    _data = (uint8_t*)buffer.data();
    _capacity = 0;
    _size = buffer.size();
    _offset = offset;
}

void FBEBuffer::clone(const void* data, size_t size, size_t offset)
{
    assert((offset <= size) && "Invalid offset!");
    if (offset > size)
        throw std::invalid_argument("Invalid offset!");

    reserve(size);
    std::memcpy(_data, data, size);
    _capacity = size;
    _size = size;
    _offset = offset;
}

void FBEBuffer::clone(const FastVec<uint8_t>& buffer, size_t offset)
{
    assert((offset <= buffer.size()) && "Invalid offset!");
    if (offset > buffer.size())
        throw std::invalid_argument("Invalid offset!");

    size_t size = buffer.size();

    reserve(size);
    std::memcpy(_data, buffer.data(), size);
    _capacity = size;
    _size = size;
    _offset = offset;
}

size_t FBEBuffer::allocate(size_t size)
{
    size_t offset = _size;

    // Calculate a new buffer size
    size_t total = _size + size;

    if (total <= _capacity)
    {
        _size = total;
        return offset;
    }

    _capacity = std::max(total, 2 * _capacity);
    void* data = nullptr;
    int err = posix_memalign(&data, 2 * sizeof(uint64_t), _capacity);
    if (err == 0)
    {
        if (_data != nullptr) {
            std::memcpy(data, _data, _size);
            std::free(_data);
        }

        _data = (uint8_t*)data;
        _size = total;
        return offset;
    } else {
        throw std::bad_alloc();
    }
}

void FBEBuffer::remove(size_t offset, size_t size)
{
    assert(((offset + size) <= _size) && "Invalid offset & size!");
    if ((offset + size) > _size)
        throw std::invalid_argument("Invalid offset & size!");

    std::memcpy(_data + offset, _data + offset + size, _size - size - offset);
    _size -= size;
    if (_offset >= (offset + size))
        _offset -= size;
    else if (_offset >= offset)
    {
        _offset -= _offset - offset;
        if (_offset > _size)
            _offset = _size;
    }
}

void FBEBuffer::reserve(size_t capacity)
{
    if (capacity > _capacity)
    {
        _capacity = std::max(capacity, 2 * _capacity);
        void* data = nullptr;
        int err = posix_memalign(&data, 2 * sizeof(uint64_t), _capacity);
        if (err == 0) {
            if (_data != nullptr) {
                std::memcpy(data, _data, _size);
                std::free(_data);
            }
            _data = (uint8_t*)data;
        } else {
            throw std::bad_alloc();
        }
    }
}

void FBEBuffer::resize(size_t size)
{
    reserve(size);
    _size = size;
    if (_offset > _size)
        _offset = _size;
}

void FBEBuffer::reset()
{
    _size = 0;
    _offset = 0;
}
)CODE";
  }

  static std::string GenerateFBEBaseModel_Header() {
    return R"CODE(
// Fast Binary Encoding base model
class Model
{
public:
    Model() : Model(nullptr) {}
    Model(const std::shared_ptr<FBEBuffer>& buffer) { _buffer = buffer ? buffer : std::make_shared<FBEBuffer>(); }
    Model(const Model&) = default;
    Model(Model&&) noexcept = default;
    ~Model() = default;

    Model& operator=(const Model&) = default;
    Model& operator=(Model&&) noexcept = default;

    // Get the model buffer
    FBEBuffer& buffer() noexcept { return *_buffer; }
    const FBEBuffer& buffer() const noexcept { return *_buffer; }

    // Attach the model buffer
    void attach(const void* data, size_t size, size_t offset = 0) { _buffer->attach(data, size, offset); }
    void attach(const FastVec<uint8_t>& buffer, size_t offset = 0) { _buffer->attach(buffer, offset); }
    void attach(const FBEBuffer& buffer, size_t offset = 0) { _buffer->attach(buffer.data(), buffer.size(), offset); }

    // Model buffer operations
    size_t allocate(size_t size) { return _buffer->allocate(size); }
    void remove(size_t offset, size_t size) { _buffer->remove(offset, size); }
    void reserve(size_t capacity) { _buffer->reserve(capacity); }
    void resize(size_t size) { _buffer->resize(size); }
    void reset() { _buffer->reset(); }
    void shift(size_t offset) { _buffer->shift(offset); }
    void unshift(size_t offset) { _buffer->unshift(offset); }

private:
    std::shared_ptr<FBEBuffer> _buffer;
};
)CODE";
  }

  static std::string GenerateFBEFieldModel_Header() {
    return R"CODE(
// Fast Binary Encoding base field model
template <typename T, typename TBase = T>
class FieldModelBase
{
public:
    FieldModelBase(FBEBuffer& buffer, size_t offset) noexcept : _buffer(buffer), _offset(offset) {}

    // Get the field offset
    size_t fbe_offset() const noexcept { return _offset; }
    // Get the field size
    size_t fbe_size() const noexcept { return sizeof(TBase); }
    // Get the field extra size
    size_t fbe_extra() const noexcept { return 0; }

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept { _offset -= size; }

    // Check if the value is valid
    bool verify() const noexcept { return true; }

    // Get the field value
    void get(T& value, std::pmr::memory_resource* resource, T defaults = (T)0) const noexcept;
    // Set the field value
    void set(T value, std::pmr::memory_resource* resource) noexcept;

private:
    FBEBuffer& _buffer;
    size_t _offset;
};

// Fast Binary Encoding field model
template <typename T>
class FieldModel : public FieldModelBase<T>
{
public:
    using FieldModelBase<T>::FieldModelBase;
};

// Fast Binary Encoding field model bool specialization
template <>
class FieldModel<bool> : public FieldModelBase<bool, uint8_t>
{
public:
    using FieldModelBase<bool, uint8_t>::FieldModelBase;
};

// Fast Binary Encoding field model char specialization
template <>
class FieldModel<char> : public FieldModelBase<char, uint8_t>
{
public:
    using FieldModelBase<char, uint8_t>::FieldModelBase;
};

// Fast Binary Encoding field model wchar specialization
template <>
class FieldModel<wchar_t> : public FieldModelBase<wchar_t, uint32_t>
{
public:
    using FieldModelBase<wchar_t, uint32_t>::FieldModelBase;
};

template<typename V, typename T>
auto variant_emplace_value(V& fbe_value, std::pmr::memory_resource* resource) {
    if constexpr (std::is_integral_v<T> or std::is_floating_point_v<T> or std::is_enum_v<T> or std::is_same_v<T, std::string> or not std::is_constructible_v<T, std::pmr::memory_resource*>) {
        fbe_value.template emplace<T>();
    } else {
        fbe_value.template emplace<T>(resource);
    }
}

template<typename M, typename T>
auto variant_get_value(M& model, T& value, std::pmr::memory_resource* resource) {
    if constexpr (std::is_integral_v<T> or std::is_floating_point_v<T> or std::is_enum_v<T> or std::is_same_v<T, std::string> or not (std::is_constructible_v<T, std::pmr::memory_resource*> or is_variant_v<T>)) {
        model.get(value, nullptr);
    } else {
        model.get(value, resource);
    }
}

template<typename M, typename T>
auto variant_get_value(M& model, T** value, std::pmr::memory_resource* resource) {
    if constexpr (std::is_integral_v<T> or std::is_floating_point_v<T> or std::is_enum_v<T> or std::is_same_v<T, std::string> or not (std::is_constructible_v<T, std::pmr::memory_resource*> or is_variant_v<T>)) {
        model.get(value, nullptr);
    } else {
        model.get(value, resource);
    }
}

template<typename M, typename T>
auto variant_set_value(M* model, T& value, std::pmr::memory_resource* resource) {
    if constexpr (std::is_integral_v<T> or std::is_floating_point_v<T> or std::is_enum_v<T> or std::is_same_v<T, std::string> or not (std::is_constructible_v<T, std::pmr::memory_resource*> or is_variant_v<T>)) {
        model->set(value, nullptr);
    } else {
        model->set(value, resource);
    }
}


)CODE";
  }

  static std::string GenerateFBEFieldModel_Inline() {
    return R"CODE(
template <typename T, typename TBase>
inline void FieldModelBase<T, TBase>::get(T& value, std::pmr::memory_resource* resource, T defaults) const noexcept
{
    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    if ((fbe_full_offset + fbe_size()) > _buffer.size())
    {
        value = defaults;
        return;
    }

    value = unaligned_load<T>(_buffer.data() + fbe_full_offset);
}

template <typename T, typename TBase>
inline void FieldModelBase<T, TBase>::set(T value, std::pmr::memory_resource* resource) noexcept
{
    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    assert(((fbe_full_offset + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((fbe_full_offset + fbe_size()) > _buffer.size())
        return;

    unaligned_store<TBase>(_buffer.data() + fbe_full_offset, (TBase)value);
}
)CODE";
  }

  static std::string GenerateFBEFieldModelDecimal_Header() {
    return R"CODE(
// Fast Binary Encoding field model decimal specialization
template <>
class FieldModel<decimal_t>
{
public:
    FieldModel(FBEBuffer& buffer, size_t offset) noexcept : _buffer(buffer), _offset(offset) {}

    // Get the field offset
    size_t fbe_offset() const noexcept { return _offset; }
    // Get the field size
    size_t fbe_size() const noexcept { return 16; }
    // Get the field extra size
    size_t fbe_extra() const noexcept { return 0; }

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept { _offset -= size; }

    // Check if the decimal value is valid
    bool verify() const noexcept { return true; }

    // Get the decimal value
    void get(decimal_t& value, std::pmr::memory_resource* resource, decimal_t defaults = decimal_t()) const noexcept;
    // Set the decimal value
    void set(decimal_t value, std::pmr::memory_resource* resource) noexcept;

private:
    FBEBuffer& _buffer;
    size_t _offset;

    static uint64_t extract(double a) noexcept;
    static uint64_t uint32x32(uint32_t a, uint32_t b) noexcept;
    static void uint64x64(uint64_t a, uint64_t b, uint64_t& low64, uint32_t& high32) noexcept;
};
)CODE";
  }

  static std::string GenerateFBEFieldModelDecimal_Source() {
    return R"CODE(
// Lookup table for powers of 10 (10^0 to 10^28)
static constexpr double kPow10Table[] = {
    1e0,  1e1,  1e2,  1e3,  1e4,  1e5,  1e6,  1e7,  1e8,  1e9,
    1e10, 1e11, 1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19,
    1e20, 1e21, 1e22, 1e23, 1e24, 1e25, 1e26, 1e27, 1e28
};

// Lookup table for powers of 10 as uint64_t (10^0 to 10^19)
static constexpr uint64_t kPow10TableU64[] = {
    1ULL,
    10ULL,
    100ULL,
    1000ULL,
    10000ULL,
    100000ULL,
    1000000ULL,
    10000000ULL,
    100000000ULL,
    1000000000ULL,
    10000000000ULL,
    100000000000ULL,
    1000000000000ULL,
    10000000000000ULL,
    100000000000000ULL,
    1000000000000000ULL,
    10000000000000000ULL,
    100000000000000000ULL,
    1000000000000000000ULL,
    10000000000000000000ULL
};

uint64_t FieldModel<decimal_t>::extract(double a) noexcept
{
    uint64_t result;
    std::memcpy(&result, &a, sizeof(double));
    return result;
}

uint64_t FieldModel<decimal_t>::uint32x32(uint32_t a, uint32_t b) noexcept
{
    return (uint64_t)a * (uint64_t)b;
}

void FieldModel<decimal_t>::uint64x64(uint64_t a, uint64_t b, uint64_t& low64, uint32_t& high32) noexcept
{
    uint64_t low = uint32x32((uint32_t)a, (uint32_t)b);
    uint64_t mid = uint32x32((uint32_t)a, (uint32_t)(b >> 32));
    uint64_t high = uint32x32((uint32_t)(a >> 32), (uint32_t)(b >> 32));
    high += (mid >> 32);
    low += (mid <<= 32);
    // Test for carry
    if (low < mid)
        high++;

    mid = uint32x32((uint32_t)(a >> 32), (uint32_t)b);
    high += (mid >> 32);
    low += (mid <<= 32);
    // Test for carry
    if (low < mid)
        high++;

    if (high > 0xFFFFFFFFu)
    {
        low64 = 0;
        high32 = 0;
    }
    low64 = low;
    high32 = (uint32_t)high;
}

void FieldModel<decimal_t>::get(decimal_t& value, std::pmr::memory_resource* resource, decimal_t defaults) const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
    {
        value = defaults;
        return;
    }

    // Value taken via reverse engineering the double that corresponds to 2^64
    const double ds2to64 = 1.8446744073709552e+019;

    // Read decimal parts
    uint64_t low = unaligned_load<uint64_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    uint32_t high = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset() + 8);
    uint32_t flags = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset() + 12);

    // Calculate decimal value
    uint8_t scale = (uint8_t)(flags >> 16);
    double dValue = ((double)low + (double)high * ds2to64) / kPow10Table[scale];
    if (flags & 0x80000000)
        dValue = -dValue;

    value = dValue;
}

void FieldModel<decimal_t>::set(decimal_t value, std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    // The most we can scale by is 10^28, which is just slightly more
    // than 2^93.  So a float with an exponent of -94 could just
    // barely reach 0.5, but smaller exponents will always round to zero.
    const uint32_t DBLBIAS = 1022;

    // Get exponent value
    double dValue = (double)value;
    int32_t iExp = (int32_t)(((uint32_t)(extract(dValue) >> 52) & 0x7FFu) - DBLBIAS);
    if ((iExp < -94) || (iExp > 96))
    {
        // Value too big for .NET Decimal (exponent is limited to [-94, 96])
        memset((uint8_t*)(_buffer.data() + _buffer.offset() + fbe_offset()), 0, 16);
        return;
    }

    uint32_t flags = 0;
    if (dValue < 0)
    {
        dValue = -dValue;
        flags = 0x80000000;
    }

    // Round the input to a 15-digit integer.  The R8 format has
    // only 15 digits of precision, and we want to keep garbage digits
    // out of the Decimal were making.

    // Calculate max power of 10 input value could have by multiplying
    // the exponent by log10(2).  Using scaled integer multiplcation,
    // log10(2) * 2 ^ 16 = .30103 * 65536 = 19728.3.
    int32_t iPower = 14 - ((iExp * 19728) >> 16);

    // iPower is between -14 and 43
    if (iPower >= 0)
    {
        // We have less than 15 digits, scale input up.
        if (iPower > 28)
            iPower = 28;

        dValue *= kPow10Table[iPower];
    }
    else
    {
        if ((iPower != -1) || (dValue >= 1E15))
            dValue /= kPow10Table[-iPower];
        else
            iPower = 0; // didn't scale it
    }

    assert(dValue < 1E15);
    if ((dValue < 1E14) && (iPower < 28))
    {
        dValue *= 10;
        iPower++;
        assert(dValue >= 1E14);
    }

    // Round to int64
    uint64_t ulMant;
    ulMant = (uint64_t)(int64_t)dValue;
    dValue -= (int64_t)ulMant; // difference between input & integer
    if ((dValue > 0.5) || ((dValue == 0.5) && ((ulMant & 1) != 0)))
        ulMant++;

    if (ulMant == 0)
    {
        // Mantissa is 0
        memset((uint8_t*)(_buffer.data() + _buffer.offset() + fbe_offset()), 0, 16);
        return;
    }

    if (iPower < 0)
    {
        // Add -iPower factors of 10, -iPower <= (29 - 15) = 14
        iPower = -iPower;
        if (iPower < 10)
        {
            uint64_t pow10 = kPow10TableU64[iPower];
            uint64_t low64 = uint32x32((uint32_t)ulMant, (uint32_t)pow10);
            uint64_t high64 = uint32x32((uint32_t)(ulMant >> 32), (uint32_t)pow10);
            unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset(), (uint32_t)low64);
            high64 += low64 >> 32;
            unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset() + 4, (uint32_t)high64);
            high64 >>= 32;
            unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset() + 8, (uint32_t)high64);
        }
        else
        {
            // Have a big power of 10.
            assert(iPower <= 14);
            uint64_t low64;
            uint32_t high32;
            uint64x64(ulMant, kPow10TableU64[iPower], low64, high32);
            unaligned_store<uint64_t>(_buffer.data() + _buffer.offset() + fbe_offset(), low64);
            unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset() + 8, high32);
        }
    }
    else
    {
        // Factor out powers of 10 to reduce the scale, if possible.
        // The maximum number we could factor out would be 14.  This
        // comes from the fact we have a 15-digit number, and the
        // MSD must be non-zero -- but the lower 14 digits could be
        // zero.  Note also the scale factor is never negative, so
        // we can't scale by any more than the power we used to
        // get the integer.
        int lmax = iPower;
        if (lmax > 14)
            lmax = 14;

        if ((((uint8_t)ulMant) == 0) && (lmax >= 8))
        {
            const uint32_t den = 100000000;
            uint64_t div = ulMant / den;
            if ((uint32_t)ulMant == (uint32_t)(div * den))
            {
                ulMant = div;
                iPower -= 8;
                lmax -= 8;
            }
        }

        if ((((uint32_t)ulMant & 0xF) == 0) && (lmax >= 4))
        {
            const uint32_t den = 10000;
            uint64_t div = ulMant / den;
            if ((uint32_t)ulMant == (uint32_t)(div * den))
            {
                ulMant = div;
                iPower -= 4;
                lmax -= 4;
            }
        }

        if ((((uint32_t)ulMant & 3) == 0) && (lmax >= 2))
        {
            const uint32_t den = 100;
            uint64_t div = ulMant / den;
            if ((uint32_t)ulMant == (uint32_t)(div * den))
            {
                ulMant = div;
                iPower -= 2;
                lmax -= 2;
            }
        }

        if ((((uint32_t)ulMant & 1) == 0) && (lmax >= 1))
        {
            const uint32_t den = 10;
            uint64_t div = ulMant / den;
            if ((uint32_t)ulMant == (uint32_t)(div * den))
            {
                ulMant = div;
                iPower--;
            }
        }

        flags |= (uint32_t)iPower << 16;

        unaligned_store<uint64_t>(_buffer.data() + _buffer.offset() + fbe_offset(), ulMant);
        unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset() + 8, 0);
    }

    unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset() + 12, flags);
}
)CODE";
  }

  static std::string GenerateFBEFieldModelUUID_Header() {
    return R"CODE(
// Fast Binary Encoding field model UUID specialization
template <>
class FieldModel<uuid_t>
{
public:
    FieldModel(FBEBuffer& buffer, size_t offset) noexcept : _buffer(buffer), _offset(offset) {}

    // Get the field offset
    size_t fbe_offset() const noexcept { return _offset; }
    // Get the field size
    size_t fbe_size() const noexcept { return 16; }
    // Get the field extra size
    size_t fbe_extra() const noexcept { return 0; }

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept { _offset -= size; }

    // Check if the UUID value is valid
    bool verify() const noexcept { return true; }

    // Get the UUID value
    void get(uuid_t& value, std::pmr::memory_resource* resource, uuid_t defaults = uuid_t::nil()) const noexcept;
    // Set the UUID value
    void set(uuid_t value, std::pmr::memory_resource* resource) noexcept;

private:
    FBEBuffer& _buffer;
    size_t _offset;
};
)CODE";
  }

  static std::string GenerateFBEFieldModelUUID_Source() {
    return R"CODE(
void FieldModel<uuid_t>::get(uuid_t& value, std::pmr::memory_resource* resource, uuid_t defaults) const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
    {
        value = defaults;
        return;
    }

    std::memcpy(value.data().data(), (const uint8_t*)(_buffer.data() + _buffer.offset() + fbe_offset()), fbe_size());
}

void FieldModel<uuid_t>::set(uuid_t value, std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    std::memcpy((uint8_t*)(_buffer.data() + _buffer.offset() + fbe_offset()), value.data().data(), fbe_size());
}
)CODE";
  }

  static std::string GenerateFBEFieldModelBytes_Header() {
    return R"CODE(
// Fast Binary Encoding field model bytes specialization
template <>
class FieldModel<buffer_t>
{
public:
    FieldModel(FBEBuffer& buffer, size_t offset) noexcept : _buffer(buffer), _offset(offset) {}

    // Get the field offset
    size_t fbe_offset() const noexcept { return _offset; }
    // Get the field size
    size_t fbe_size() const noexcept { return 4; }
    // Get the field extra size
    size_t fbe_extra() const noexcept;

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept { _offset -= size; }

    // Check if the bytes value is valid
    bool verify() const noexcept;

    // Get the bytes value
    size_t get(void* data, size_t size, std::pmr::memory_resource* resource) const noexcept;
    // Get the bytes value
    template <size_t N>
    size_t get(uint8_t (&data)[N], std::pmr::memory_resource* resource) const noexcept { return get(data, N, resource); }
    // Get the bytes value
    template <size_t N>
    size_t get(std::array<uint8_t, N>& data, std::pmr::memory_resource* resource) const noexcept { return get(data.data(), data.size(), resource); }
    // Get the bytes value
    void get(FastVec<uint8_t>& value, std::pmr::memory_resource* resource) const noexcept;
    // Get the bytes value
    void get(buffer_t& value, std::pmr::memory_resource* resource) const noexcept { get(value.buffer(), resource); }

    // Set the bytes value
    void set(const void* data, size_t size, std::pmr::memory_resource* resource);
    // Set the bytes value
    template <size_t N>
    void set(const uint8_t (&data)[N], std::pmr::memory_resource* resource) { set(data, N, resource); }
    // Set the bytes value
    template <size_t N>
    void set(const std::array<uint8_t, N>& data, std::pmr::memory_resource* resource) { set(data.data(), data.size(), resource); }
    // Set the bytes value
    void set(const FastVec<uint8_t>& value, std::pmr::memory_resource* resource) { set(value.data(), value.size(), resource); }
    // Set the bytes value
    void set(const buffer_t& value, std::pmr::memory_resource* resource) { set(value.buffer(), resource); }

private:
    FBEBuffer& _buffer;
    size_t _offset;
};
)CODE";
  }

  static std::string GenerateFBEFieldModelBytes_Source() {
    return R"CODE(
size_t FieldModel<buffer_t>::fbe_extra() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return 0;

    uint32_t fbe_bytes_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    if ((fbe_bytes_offset == 0) || ((_buffer.offset() + fbe_bytes_offset + 4) > _buffer.size()))
        return 0;

    uint32_t fbe_bytes_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_bytes_offset);
    return (size_t)(4 + fbe_bytes_size);
}

bool FieldModel<buffer_t>::verify() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return true;

    uint32_t fbe_bytes_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    if (fbe_bytes_offset == 0)
        return true;

    if ((_buffer.offset() + fbe_bytes_offset + 4) > _buffer.size())
        return false;

    uint32_t fbe_bytes_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_bytes_offset);
    if ((_buffer.offset() + fbe_bytes_offset + 4 + fbe_bytes_size) > _buffer.size())
        return false;

    return true;
}

size_t FieldModel<buffer_t>::get(void* data, size_t size, std::pmr::memory_resource* resource) const noexcept
{
    assert(((size == 0) || (data != nullptr)) && "Invalid buffer!");
    if ((size > 0) && (data == nullptr))
        return 0;

    size_t buffer_offset = _buffer.offset();
    const uint8_t* buffer_data = _buffer.data();
    size_t buffer_size = _buffer.size();

    if ((buffer_offset + fbe_offset() + fbe_size()) > buffer_size)
        return 0;

    uint32_t fbe_bytes_offset = unaligned_load<uint32_t>(buffer_data + buffer_offset + fbe_offset());
    if (fbe_bytes_offset == 0)
        return 0;

    size_t bytes_base = buffer_offset + fbe_bytes_offset;
    assert(((bytes_base + 4) <= buffer_size) && "Model is broken!");
    if ((bytes_base + 4) > buffer_size)
        return 0;

    uint32_t fbe_bytes_size = unaligned_load<uint32_t>(buffer_data + bytes_base);
    assert(((bytes_base + 4 + fbe_bytes_size) <= buffer_size) && "Model is broken!");
    if ((bytes_base + 4 + fbe_bytes_size) > buffer_size)
        return 0;

    size_t result = std::min(size, (size_t)fbe_bytes_size);
    if (result > 0)
        memcpy(data, buffer_data + bytes_base + 4, result);
    return result;
}

void FieldModel<buffer_t>::get(FastVec<uint8_t>& value, std::pmr::memory_resource* resource) const noexcept
{
    value.clear();

    size_t buffer_offset = _buffer.offset();
    const uint8_t* buffer_data = _buffer.data();
    size_t buffer_size = _buffer.size();

    if ((buffer_offset + fbe_offset() + fbe_size()) > buffer_size)
        return;

    uint32_t fbe_bytes_offset = unaligned_load<uint32_t>(buffer_data + buffer_offset + fbe_offset());
    if (fbe_bytes_offset == 0)
        return;

    size_t bytes_base = buffer_offset + fbe_bytes_offset;
    assert(((bytes_base + 4) <= buffer_size) && "Model is broken!");
    if ((bytes_base + 4) > buffer_size)
        return;

    uint32_t fbe_bytes_size = unaligned_load<uint32_t>(buffer_data + bytes_base);
    assert(((bytes_base + 4 + fbe_bytes_size) <= buffer_size) && "Model is broken!");
    if ((bytes_base + 4 + fbe_bytes_size) > buffer_size)
        return;

    if (fbe_bytes_size > 0) {
        const char* fbe_bytes = (const char*)(buffer_data + bytes_base + 4);
        value.assign(fbe_bytes, fbe_bytes + fbe_bytes_size);
    }
}

void FieldModel<buffer_t>::set(const void* data, size_t size, std::pmr::memory_resource* resource)
{
    assert(((size == 0) || (data != nullptr)) && "Invalid buffer!");
    if ((size > 0) && (data == nullptr))
        return;

    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    uint32_t fbe_bytes_size = (uint32_t)size;
    uint32_t fbe_bytes_offset = (uint32_t)(_buffer.allocate(4 + fbe_bytes_size) - _buffer.offset());
    assert(((fbe_bytes_offset > 0) && ((_buffer.offset() + fbe_bytes_offset + 4 + fbe_bytes_size) <= _buffer.size())) && "Model is broken!");
    if ((fbe_bytes_offset == 0) || ((_buffer.offset() + fbe_bytes_offset + 4 + fbe_bytes_size) > _buffer.size()))
        return;

    unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset(), fbe_bytes_offset);
    unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_bytes_offset, fbe_bytes_size);

    if (fbe_bytes_size > 0)
        memcpy((char*)(_buffer.data() + _buffer.offset() + fbe_bytes_offset + 4), data, fbe_bytes_size);
}
)CODE";
  }

  static std::string GenerateFBEFieldModelPMRBytes_Header() {
    return R"CODE(
// Fast Binary Encoding field model bytes specialization
template <>
class FieldModel<pmr_buffer_t>
{
public:
    FieldModel(FBEBuffer& buffer, size_t offset) noexcept : _buffer(buffer), _offset(offset) {}

    // Get the field offset
    size_t fbe_offset() const noexcept { return _offset; }
    // Get the field size
    size_t fbe_size() const noexcept { return 4; }
    // Get the field extra size
    size_t fbe_extra() const noexcept;

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept { _offset -= size; }

    // Check if the bytes value is valid
    bool verify() const noexcept;

    // Get the bytes value
    size_t get(void* data, size_t size) const noexcept;
    // Get the bytes value
    template <size_t N>
    size_t get(uint8_t (&data)[N], std::pmr::memory_resource* resource) const noexcept { return get(data, N); }
    // Get the bytes value
    void get(std::pmr::vector<uint8_t>& value, std::pmr::memory_resource* resource) const noexcept;
    // Get the bytes value
    void get(pmr_buffer_t& value, std::pmr::memory_resource* resource) const noexcept { get(value.buffer(), resource); }

    // Set the bytes value
    void set(const void* data, size_t size, std::pmr::memory_resource* resource);
    // Set the bytes value
    template <size_t N>
    void set(const uint8_t (&data)[N], std::pmr::memory_resource* resource) { set(data, N, resource); }
    // Set the bytes value
    void set(const std::pmr::vector<uint8_t>& value, std::pmr::memory_resource* resource) { set(value.data(), value.size(), resource); }
    // Set the bytes value
    void set(const pmr_buffer_t& value, std::pmr::memory_resource* resource) { set(value.buffer(), resource); }

private:
    FBEBuffer& _buffer;
    size_t _offset;
};
)CODE";
  }

  static std::string GenerateFBEFieldModelPMRBytes_Source() {
    return R"CODE(
size_t FieldModel<pmr_buffer_t>::fbe_extra() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return 0;

    uint32_t fbe_bytes_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    if ((fbe_bytes_offset == 0) || ((_buffer.offset() + fbe_bytes_offset + 4) > _buffer.size()))
        return 0;

    uint32_t fbe_bytes_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_bytes_offset);
    return (size_t)(4 + fbe_bytes_size);
}

bool FieldModel<pmr_buffer_t>::verify() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return true;

    uint32_t fbe_bytes_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    if (fbe_bytes_offset == 0)
        return true;

    if ((_buffer.offset() + fbe_bytes_offset + 4) > _buffer.size())
        return false;

    uint32_t fbe_bytes_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_bytes_offset);
    if ((_buffer.offset() + fbe_bytes_offset + 4 + fbe_bytes_size) > _buffer.size())
        return false;

    return true;
}

size_t FieldModel<pmr_buffer_t>::get(void* data, size_t size) const noexcept
{
    assert(((size == 0) || (data != nullptr)) && "Invalid buffer!");
    if ((size > 0) && (data == nullptr))
        return 0;

    size_t buffer_offset = _buffer.offset();
    const uint8_t* buffer_data = _buffer.data();
    size_t buffer_size = _buffer.size();

    if ((buffer_offset + fbe_offset() + fbe_size()) > buffer_size)
        return 0;

    uint32_t fbe_bytes_offset = unaligned_load<uint32_t>(buffer_data + buffer_offset + fbe_offset());
    if (fbe_bytes_offset == 0)
        return 0;

    size_t bytes_base = buffer_offset + fbe_bytes_offset;
    assert(((bytes_base + 4) <= buffer_size) && "Model is broken!");
    if ((bytes_base + 4) > buffer_size)
        return 0;

    uint32_t fbe_bytes_size = unaligned_load<uint32_t>(buffer_data + bytes_base);
    assert(((bytes_base + 4 + fbe_bytes_size) <= buffer_size) && "Model is broken!");
    if ((bytes_base + 4 + fbe_bytes_size) > buffer_size)
        return 0;

    size_t result = std::min(size, (size_t)fbe_bytes_size);
    memcpy(data, buffer_data + bytes_base + 4, result);
    return result;
}

void FieldModel<pmr_buffer_t>::get(std::pmr::vector<uint8_t>& value, std::pmr::memory_resource* resource) const noexcept
{
    value.clear();

    size_t buffer_offset = _buffer.offset();
    const uint8_t* buffer_data = _buffer.data();
    size_t buffer_size = _buffer.size();

    if ((buffer_offset + fbe_offset() + fbe_size()) > buffer_size)
        return;

    uint32_t fbe_bytes_offset = unaligned_load<uint32_t>(buffer_data + buffer_offset + fbe_offset());
    if (fbe_bytes_offset == 0)
        return;

    size_t bytes_base = buffer_offset + fbe_bytes_offset;
    assert(((bytes_base + 4) <= buffer_size) && "Model is broken!");
    if ((bytes_base + 4) > buffer_size)
        return;

    uint32_t fbe_bytes_size = unaligned_load<uint32_t>(buffer_data + bytes_base);
    assert(((bytes_base + 4 + fbe_bytes_size) <= buffer_size) && "Model is broken!");
    if ((bytes_base + 4 + fbe_bytes_size) > buffer_size)
        return;

    const char* fbe_bytes = (const char*)(buffer_data + bytes_base + 4);
    value.assign(fbe_bytes, fbe_bytes + fbe_bytes_size);
}

void FieldModel<pmr_buffer_t>::set(const void* data, size_t size, std::pmr::memory_resource* resource)
{
    assert(((size == 0) || (data != nullptr)) && "Invalid buffer!");
    if ((size > 0) && (data == nullptr))
        return;

    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    uint32_t fbe_bytes_size = (uint32_t)size;
    uint32_t fbe_bytes_offset = (uint32_t)(_buffer.allocate(4 + fbe_bytes_size) - _buffer.offset());
    assert(((fbe_bytes_offset > 0) && ((_buffer.offset() + fbe_bytes_offset + 4 + fbe_bytes_size) <= _buffer.size())) && "Model is broken!");
    if ((fbe_bytes_offset == 0) || ((_buffer.offset() + fbe_bytes_offset + 4 + fbe_bytes_size) > _buffer.size()))
        return;

    unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset(), fbe_bytes_offset);
    unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_bytes_offset, fbe_bytes_size);

    if (fbe_bytes_size > 0)
        memcpy((char*)(_buffer.data() + _buffer.offset() + fbe_bytes_offset + 4), data, fbe_bytes_size);
}
)CODE";
  }

  static std::string GenerateFBEFieldModelFBEString_Header() {
    return R"CODE(
// Fast Binary Encoding field model string specialization
template <>
class FieldModel<FBEString>
{
public:
    FieldModel(FBEBuffer& buffer, size_t offset) noexcept : _buffer(buffer), _offset(offset) {}

    // Get the field offset
    size_t fbe_offset() const noexcept { return _offset; }
    // Get the field size
    size_t fbe_size() const noexcept { return 4; }
    // Get the field extra size
    size_t fbe_extra() const noexcept;

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept { _offset -= size; }

    // Check if the string value is valid
    bool verify() const noexcept;

    // Get the string value
    size_t get(char* data, size_t size) const noexcept;
    // Get the string value
    template <size_t N>
    size_t get(char (&data)[N], std::pmr::memory_resource* resource) const noexcept { return get(data, N); }
    // Get the string value
    template <size_t N>
    size_t get(std::array<char, N>& data, std::pmr::memory_resource* resource) const noexcept { return get(data.data(), data.size()); }
    // Get the pmr string value
    void get(FBEString& value, std::pmr::memory_resource* resource) const noexcept;
    // Get the pmr string value
    void get(FBEString& value, std::pmr::memory_resource* resource, const FBEString& defaults) const noexcept;

    // Set the string value
    void set(const char* data, size_t size, std::pmr::memory_resource* resource);
    // Set the string value
    template <size_t N>
    void set(const char (&data)[N], std::pmr::memory_resource* resource) { set(data, N); }
    // Set the string value
    template <size_t N>
    void set(const std::array<char, N>& data, std::pmr::memory_resource* resource) { set(data.data(), data.size()); }
    // Set the string value
    void set(const FBEString& value, std::pmr::memory_resource* resource);

private:
    FBEBuffer& _buffer;
    size_t _offset;
};
)CODE";
  }

  static std::string GenerateFBEFieldModeArenaString_Header() {
    return R"CODE(
// Fast Binary Encoding field model string specialization
template <>
class FieldModel<ArenaString>
{
public:
    FieldModel(FBEBuffer& buffer, size_t offset) noexcept : _buffer(buffer), _offset(offset) {}

    // Get the field offset
    size_t fbe_offset() const noexcept { return _offset; }
    // Get the field size
    size_t fbe_size() const noexcept { return 4; }
    // Get the field extra size
    size_t fbe_extra() const noexcept;

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept { _offset -= size; }

    // Check if the string value is valid
    bool verify() const noexcept;

    // Get the string value
    size_t get(char* data, size_t size) const noexcept;
    // Get the string value
    template <size_t N>
    size_t get(char (&data)[N], std::pmr::memory_resource* resource) const noexcept { return get(data, N); }
    // Get the string value
    template <size_t N>
    size_t get(std::array<char, N>& data, std::pmr::memory_resource* resource) const noexcept { return get(data.data(), data.size()); }
    // Get the pmr string value
    void get(ArenaString& value, std::pmr::memory_resource* resource) const noexcept;
    // Get the pmr string value
    void get(ArenaString& value, std::pmr::memory_resource* resource, const ArenaString& defaults) const noexcept;

    // Set the string value
    void set(const char* data, size_t size, std::pmr::memory_resource* resource);
    // Set the string value
    template <size_t N>
    void set(const char (&data)[N], std::pmr::memory_resource* resource) { set(data, N); }
    // Set the string value
    template <size_t N>
    void set(const std::array<char, N>& data, std::pmr::memory_resource* resource) { set(data.data(), data.size()); }
    // Set the string value
    void set(const ArenaString& value, std::pmr::memory_resource* resource);

private:
    FBEBuffer& _buffer;
    size_t _offset;
};
)CODE";
  }

  static std::string GenerateFBEFieldModelFBEString_Source() {
    return R"CODE(
size_t FieldModel<FBEString>::fbe_extra() const noexcept
{
    size_t buffer_offset = _buffer.offset();
    size_t buffer_size = _buffer.size();
    if ((buffer_offset + fbe_offset() + fbe_size()) > buffer_size)
        return 0;

    const uint8_t* buffer_data = _buffer.data();
    uint32_t fbe_string_offset = unaligned_load<uint32_t>(buffer_data + buffer_offset + fbe_offset());
    if ((fbe_string_offset == 0) || ((buffer_offset + fbe_string_offset + 4) > buffer_size))
        return 0;

    uint32_t fbe_string_size = unaligned_load<uint32_t>(buffer_data + buffer_offset + fbe_string_offset);
    return (size_t)(4 + fbe_string_size);
}

bool FieldModel<FBEString>::verify() const noexcept
{
    size_t buffer_offset = _buffer.offset();
    size_t buffer_size = _buffer.size();
    if ((buffer_offset + fbe_offset() + fbe_size()) > buffer_size)
        return true;

    const uint8_t* buffer_data = _buffer.data();
    uint32_t fbe_string_offset = unaligned_load<uint32_t>(buffer_data + buffer_offset + fbe_offset());
    if (fbe_string_offset == 0)
        return true;

    size_t string_base = buffer_offset + fbe_string_offset;
    if ((string_base + 4) > buffer_size)
        return false;

    uint32_t fbe_string_size = unaligned_load<uint32_t>(buffer_data + string_base);
    if ((string_base + 4 + fbe_string_size) > buffer_size)
        return false;

    return true;
}

size_t FieldModel<FBEString>::get(char* data, size_t size) const noexcept
{
    assert(((size == 0) || (data != nullptr)) && "Invalid buffer!");
    if ((size > 0) && (data == nullptr))
        return 0;

    size_t buffer_offset = _buffer.offset();
    const uint8_t* buffer_data = _buffer.data();
    size_t buffer_size = _buffer.size();

    if ((buffer_offset + fbe_offset() + fbe_size()) > buffer_size)
        return 0;

    uint32_t fbe_string_offset = unaligned_load<uint32_t>(buffer_data + buffer_offset + fbe_offset());
    if (fbe_string_offset == 0)
        return 0;

    size_t string_base = buffer_offset + fbe_string_offset;
    assert(((string_base + 4) <= buffer_size) && "Model is broken!");
    if ((string_base + 4) > buffer_size)
        return 0;

    uint32_t fbe_string_size = unaligned_load<uint32_t>(buffer_data + string_base);
    assert(((string_base + 4 + fbe_string_size) <= buffer_size) && "Model is broken!");
    if ((string_base + 4 + fbe_string_size) > buffer_size)
        return 0;

    size_t result = std::min(size, (size_t)fbe_string_size);
    memcpy(data, (const char*)(buffer_data + string_base + 4), result);
    return result;
}

void FieldModel<FBEString>::get(FBEString& value, std::pmr::memory_resource* resource) const noexcept
{
    #if !defined(USING_SEASTAR_STRING)
    value.clear();
    #endif

    size_t buffer_offset = _buffer.offset();
    const uint8_t* buffer_data = _buffer.data();
    size_t buffer_size = _buffer.size();

    if ((buffer_offset + fbe_offset() + fbe_size()) > buffer_size)
        return;

    uint32_t fbe_string_offset = unaligned_load<uint32_t>(buffer_data + buffer_offset + _offset);
    if (fbe_string_offset == 0)
        return;

    size_t string_base = buffer_offset + fbe_string_offset;
    assert(((string_base + 4) <= buffer_size) && "Model is broken!");
    if ((string_base + 4) > buffer_size)
        return;

    uint32_t fbe_string_size = unaligned_load<uint32_t>(buffer_data + string_base);
    assert(((string_base + 4 + fbe_string_size) <= buffer_size) && "Model is broken!");
    if ((string_base + 4 + fbe_string_size) > buffer_size)
        return;

    const char* string_data = (const char*)(buffer_data + string_base + 4);
    #if defined(USING_SEASTAR_STRING)
    value.resize(fbe_string_size);
    memcpy(value.data(), string_data, fbe_string_size);
    #else
    value.assign(string_data, fbe_string_size);
    #endif
}

void FieldModel<FBEString>::get(FBEString& value, std::pmr::memory_resource* resource, const FBEString& defaults) const noexcept
{
    value = defaults;

    size_t buffer_offset = _buffer.offset();
    const uint8_t* buffer_data = _buffer.data();
    size_t buffer_size = _buffer.size();

    if ((buffer_offset + fbe_offset() + fbe_size()) > buffer_size)
        return;

    uint32_t fbe_string_offset = unaligned_load<uint32_t>(buffer_data + buffer_offset + _offset);
    if (fbe_string_offset == 0)
        return;

    size_t string_base = buffer_offset + fbe_string_offset;
    assert(((string_base + 4) <= buffer_size) && "Model is broken!");
    if ((string_base + 4) > buffer_size)
        return;

    uint32_t fbe_string_size = unaligned_load<uint32_t>(buffer_data + string_base);
    assert(((string_base + 4 + fbe_string_size) <= buffer_size) && "Model is broken!");
    if ((string_base + 4 + fbe_string_size) > buffer_size)
        return;

    const char* string_data = (const char*)(buffer_data + string_base + 4);
    #if defined(USING_SEASTAR_STRING)
    value.resize(fbe_string_size);
    memcpy(value.data(), string_data, fbe_string_size);
    #else
    value.assign(string_data, fbe_string_size);
    #endif
}

void FieldModel<FBEString>::set(const char* data, size_t size, std::pmr::memory_resource* resource)
{
    assert(((size == 0) || (data != nullptr)) && "Invalid buffer!");
    if ((size > 0) && (data == nullptr))
        return;

    size_t buffer_offset = _buffer.offset();
    assert(((buffer_offset + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((buffer_offset + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    uint32_t fbe_string_size = (uint32_t)size;
    uint32_t fbe_string_offset = (uint32_t)(_buffer.allocate(4 + fbe_string_size) - buffer_offset);
    assert(((fbe_string_offset > 0) && ((buffer_offset + fbe_string_offset + 4 + fbe_string_size) <= _buffer.size())) && "Model is broken!");
    if ((fbe_string_offset == 0) || ((buffer_offset + fbe_string_offset + 4 + fbe_string_size) > _buffer.size()))
        return;

    uint8_t* buffer_data = _buffer.data();
    size_t string_base = buffer_offset + fbe_string_offset;
    unaligned_store<uint32_t>(buffer_data + buffer_offset + fbe_offset(), fbe_string_offset);
    unaligned_store<uint32_t>(buffer_data + string_base, fbe_string_size);

    if (fbe_string_size > 0)
        memcpy((char*)(buffer_data + string_base + 4), data, fbe_string_size);
}

void FieldModel<FBEString>::set(const FBEString& value, std::pmr::memory_resource* resource)
{
    size_t buffer_offset = _buffer.offset();
    assert(((buffer_offset + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((buffer_offset + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    uint32_t fbe_string_size = (uint32_t)value.size();
    uint32_t fbe_string_offset = (uint32_t)(_buffer.allocate(4 + fbe_string_size) - buffer_offset);
    assert(((fbe_string_offset > 0) && ((buffer_offset + fbe_string_offset + 4 + fbe_string_size) <= _buffer.size())) && "Model is broken!");
    if ((fbe_string_offset == 0) || ((buffer_offset + fbe_string_offset + 4 + fbe_string_size) > _buffer.size()))
        return;

    uint8_t* buffer_data = _buffer.data();
    size_t string_base = buffer_offset + fbe_string_offset;
    unaligned_store<uint32_t>(buffer_data + buffer_offset + fbe_offset(), fbe_string_offset);
    unaligned_store<uint32_t>(buffer_data + string_base, fbe_string_size);

    if (fbe_string_size > 0)
        memcpy((char*)(buffer_data + string_base + 4), value.data(), fbe_string_size);
}
)CODE";
  }

  static std::string GenerateFBEFieldModelMemoryArenaString_Source() {
    return R"CODE(
size_t FieldModel<ArenaString>::fbe_extra() const noexcept
{
    size_t buffer_offset = _buffer.offset();
    size_t buffer_size = _buffer.size();
    if ((buffer_offset + fbe_offset() + fbe_size()) > buffer_size)
        return 0;

    const uint8_t* buffer_data = _buffer.data();
    uint32_t fbe_string_offset = unaligned_load<uint32_t>(buffer_data + buffer_offset + fbe_offset());
    if ((fbe_string_offset == 0) || ((buffer_offset + fbe_string_offset + 4) > buffer_size))
        return 0;

    uint32_t fbe_string_size = unaligned_load<uint32_t>(buffer_data + buffer_offset + fbe_string_offset);
    return (size_t)(4 + fbe_string_size);
}

bool FieldModel<ArenaString>::verify() const noexcept
{
    size_t buffer_offset = _buffer.offset();
    size_t buffer_size = _buffer.size();
    if ((buffer_offset + fbe_offset() + fbe_size()) > buffer_size)
        return true;

    const uint8_t* buffer_data = _buffer.data();
    uint32_t fbe_string_offset = unaligned_load<uint32_t>(buffer_data + buffer_offset + fbe_offset());
    if (fbe_string_offset == 0)
        return true;

    size_t string_base = buffer_offset + fbe_string_offset;
    if ((string_base + 4) > buffer_size)
        return false;

    uint32_t fbe_string_size = unaligned_load<uint32_t>(buffer_data + string_base);
    if ((string_base + 4 + fbe_string_size) > buffer_size)
        return false;

    return true;
}

size_t FieldModel<ArenaString>::get(char* data, size_t size) const noexcept
{
    assert(((size == 0) || (data != nullptr)) && "Invalid buffer!");
    if ((size > 0) && (data == nullptr))
        return 0;

    size_t buffer_offset = _buffer.offset();
    const uint8_t* buffer_data = _buffer.data();
    size_t buffer_size = _buffer.size();

    if ((buffer_offset + fbe_offset() + fbe_size()) > buffer_size)
        return 0;

    uint32_t fbe_string_offset = unaligned_load<uint32_t>(buffer_data + buffer_offset + fbe_offset());
    if (fbe_string_offset == 0)
        return 0;

    size_t string_base = buffer_offset + fbe_string_offset;
    assert(((string_base + 4) <= buffer_size) && "Model is broken!");
    if ((string_base + 4) > buffer_size)
        return 0;

    uint32_t fbe_string_size = unaligned_load<uint32_t>(buffer_data + string_base);
    assert(((string_base + 4 + fbe_string_size) <= buffer_size) && "Model is broken!");
    if ((string_base + 4 + fbe_string_size) > buffer_size)
        return 0;

    size_t result = std::min(size, (size_t)fbe_string_size);
    memcpy(data, (const char*)(buffer_data + string_base + 4), result);
    return result;
}

void FieldModel<ArenaString>::get(ArenaString& value, std::pmr::memory_resource* resource) const noexcept
{
    value.clear();

    size_t buffer_offset = _buffer.offset();
    const uint8_t* buffer_data = _buffer.data();
    size_t buffer_size = _buffer.size();

    if ((buffer_offset + fbe_offset() + fbe_size()) > buffer_size)
        return;

    uint32_t fbe_string_offset = unaligned_load<uint32_t>(buffer_data + buffer_offset + _offset);
    if (fbe_string_offset == 0)
        return;

    size_t string_base = buffer_offset + fbe_string_offset;
    assert(((string_base + 4) <= buffer_size) && "Model is broken!");
    if ((string_base + 4) > buffer_size)
        return;

    uint32_t fbe_string_size = unaligned_load<uint32_t>(buffer_data + string_base);
    assert(((string_base + 4 + fbe_string_size) <= buffer_size) && "Model is broken!");
    if ((string_base + 4 + fbe_string_size) > buffer_size)
        return;

    value.assign((const char*)(buffer_data + string_base + 4), fbe_string_size);
}

void FieldModel<ArenaString>::get(ArenaString& value, std::pmr::memory_resource* resource, const ArenaString& defaults) const noexcept
{
    value = defaults;

    size_t buffer_offset = _buffer.offset();
    const uint8_t* buffer_data = _buffer.data();
    size_t buffer_size = _buffer.size();

    if ((buffer_offset + fbe_offset() + fbe_size()) > buffer_size)
        return;

    uint32_t fbe_string_offset = unaligned_load<uint32_t>(buffer_data + buffer_offset + _offset);
    if (fbe_string_offset == 0)
        return;

    size_t string_base = buffer_offset + fbe_string_offset;
    assert(((string_base + 4) <= buffer_size) && "Model is broken!");
    if ((string_base + 4) > buffer_size)
        return;

    uint32_t fbe_string_size = unaligned_load<uint32_t>(buffer_data + string_base);
    assert(((string_base + 4 + fbe_string_size) <= buffer_size) && "Model is broken!");
    if ((string_base + 4 + fbe_string_size) > buffer_size)
        return;

    value.assign((const char*)(buffer_data + string_base + 4), fbe_string_size);
}

void FieldModel<ArenaString>::set(const char* data, size_t size, std::pmr::memory_resource* resource)
{
    assert(((size == 0) || (data != nullptr)) && "Invalid buffer!");
    if ((size > 0) && (data == nullptr))
        return;

    size_t buffer_offset = _buffer.offset();
    assert(((buffer_offset + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((buffer_offset + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    uint32_t fbe_string_size = (uint32_t)size;
    uint32_t fbe_string_offset = (uint32_t)(_buffer.allocate(4 + fbe_string_size) - buffer_offset);
    assert(((fbe_string_offset > 0) && ((buffer_offset + fbe_string_offset + 4 + fbe_string_size) <= _buffer.size())) && "Model is broken!");
    if ((fbe_string_offset == 0) || ((buffer_offset + fbe_string_offset + 4 + fbe_string_size) > _buffer.size()))
        return;

    uint8_t* buffer_data = _buffer.data();
    size_t string_base = buffer_offset + fbe_string_offset;
    unaligned_store<uint32_t>(buffer_data + buffer_offset + fbe_offset(), fbe_string_offset);
    unaligned_store<uint32_t>(buffer_data + string_base, fbe_string_size);

    if (fbe_string_size > 0)
        memcpy((char*)(buffer_data + string_base + 4), data, fbe_string_size);
}

void FieldModel<ArenaString>::set(const ArenaString& value, std::pmr::memory_resource* resource)
{
    size_t buffer_offset = _buffer.offset();
    assert(((buffer_offset + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((buffer_offset + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    uint32_t fbe_string_size = (uint32_t)value.size();
    uint32_t fbe_string_offset = (uint32_t)(_buffer.allocate(4 + fbe_string_size) - buffer_offset);
    assert(((fbe_string_offset > 0) && ((buffer_offset + fbe_string_offset + 4 + fbe_string_size) <= _buffer.size())) && "Model is broken!");
    if ((fbe_string_offset == 0) || ((buffer_offset + fbe_string_offset + 4 + fbe_string_size) > _buffer.size()))
        return;

    uint8_t* buffer_data = _buffer.data();
    size_t string_base = buffer_offset + fbe_string_offset;
    unaligned_store<uint32_t>(buffer_data + buffer_offset + fbe_offset(), fbe_string_offset);
    unaligned_store<uint32_t>(buffer_data + string_base, fbe_string_size);

    if (fbe_string_size > 0)
        memcpy((char*)(buffer_data + string_base + 4), value.data(), fbe_string_size);
}
)CODE";
  }

  static std::string GenerateFBEFieldModelOptional_Header() {
    return R"CODE(
// Fast Binary Encoding field model optional specialization
template <typename T>
class FieldModel<std::optional<T>>
{
public:
    FieldModel(FBEBuffer& buffer, size_t offset) noexcept : _buffer(buffer), _offset(offset), value(buffer, 0) {}

    // Get the field offset
    size_t fbe_offset() const noexcept { return _offset; }
    // Get the field size
    size_t fbe_size() const noexcept { return 1 + 4; }
    // Get the field extra size
    size_t fbe_extra() const noexcept;

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept { _offset -= size; }

    //! Is the value present?
    explicit operator bool() const noexcept { return has_value(); }

    // Checks if the object contains a value
    bool has_value() const noexcept;

    // Check if the optional value is valid
    bool verify() const noexcept;

    // Get the optional value (being phase)
    size_t get_begin() const noexcept;
    // Get the optional value (end phase)
    void get_end(size_t fbe_begin) const noexcept;

    // Get the optional value
    void get(std::optional<T>& opt, std::pmr::memory_resource* resource, const std::optional<T>& defaults) const noexcept;
    // Get the optional value
    void get(std::optional<T>& opt, std::pmr::memory_resource* resource) const noexcept;

    // Set the optional value (begin phase)
    size_t set_begin(bool has_value);
    // Set the optional value (end phase)
    void set_end(size_t fbe_begin);

    // Set the optional value
    void set(const std::optional<T>& opt, std::pmr::memory_resource* resource);

private:
    FBEBuffer& _buffer;
    size_t _offset;

public:
    // Base field model value
    FieldModel<T> value;
};
)CODE";
  }

  static std::string GenerateFBEFieldModelOptional_Inline() {
    return R"CODE(
template <typename T>
inline size_t FieldModel<std::optional<T>>::fbe_extra() const noexcept
{
    if (!has_value())
        return 0;

    uint32_t fbe_optional_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset() + 1);
    if ((fbe_optional_offset == 0) || ((_buffer.offset() + fbe_optional_offset + 4) > _buffer.size()))
        return 0;

    _buffer.shift(fbe_optional_offset);
    size_t fbe_result = value.fbe_size() + value.fbe_extra();
    _buffer.unshift(fbe_optional_offset);
    return fbe_result;
}

template <typename T>
inline bool FieldModel<std::optional<T>>::has_value() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return false;

    uint8_t fbe_has_value = *((const uint8_t*)(_buffer.data() + _buffer.offset() + fbe_offset()));
    return (fbe_has_value != 0);
}

template <typename T>
inline bool FieldModel<std::optional<T>>::verify() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return true;

    uint8_t fbe_has_value = *((const uint8_t*)(_buffer.data() + _buffer.offset() + fbe_offset()));
    if (fbe_has_value == 0)
        return true;

    uint32_t fbe_optional_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset() + 1);
    if (fbe_optional_offset == 0)
        return false;

    _buffer.shift(fbe_optional_offset);
    bool fbe_result = value.verify();
    _buffer.unshift(fbe_optional_offset);
    return fbe_result;
}

template <typename T>
inline size_t FieldModel<std::optional<T>>::get_begin() const noexcept
{
    if (!has_value())
        return 0;

    uint32_t fbe_optional_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset() + 1);
    assert((fbe_optional_offset > 0) && "Model is broken!");
    if (fbe_optional_offset == 0)
        return 0;

    _buffer.shift(fbe_optional_offset);
    return fbe_optional_offset;
}

template <typename T>
inline void FieldModel<std::optional<T>>::get_end(size_t fbe_begin) const noexcept
{
    _buffer.unshift(fbe_begin);
}

template <typename T>
inline void FieldModel<std::optional<T>>::get(std::optional<T>& opt, std::pmr::memory_resource* resource, const std::optional<T>& defaults) const noexcept
{
    opt = defaults;

    size_t fbe_begin = get_begin();
    if (fbe_begin == 0)
        return;
    if constexpr(std::is_constructible_v<T, std::pmr::memory_resource*> and not is_variant_v<T>) {
        T temp = T(resource);

        value.get(temp, resource);
        opt.emplace(std::move(temp));

        get_end(fbe_begin);
    } else {
        T temp{};

        value.get(temp, resource);
        opt.emplace(std::move(temp));

        get_end(fbe_begin);
    }
}

template <typename T>
inline void FieldModel<std::optional<T>>::get(std::optional<T>& opt, std::pmr::memory_resource* resource) const noexcept
{
    size_t fbe_begin = get_begin();
    if (fbe_begin == 0)
        return;

    if constexpr(std::is_constructible_v<T, std::pmr::memory_resource*> and not is_variant_v<T>) {
        T temp = T(resource);

        value.get(temp, resource);
        opt.emplace(std::move(temp));

        get_end(fbe_begin);
    } else {
        T temp = T();

        value.get(temp, resource);
        opt.emplace(std::move(temp));

        get_end(fbe_begin);
    }
}

template <typename T>
inline size_t FieldModel<std::optional<T>>::set_begin(bool has_value)
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return 0;

    uint8_t fbe_has_value = has_value ? 1 : 0;
    *((uint8_t*)(_buffer.data() + _buffer.offset() + fbe_offset())) = fbe_has_value;
    if (fbe_has_value == 0)
        return 0;

    uint32_t fbe_optional_size = (uint32_t)value.fbe_size();
    uint32_t fbe_optional_offset = (uint32_t)(_buffer.allocate(fbe_optional_size) - _buffer.offset());
    assert(((fbe_optional_offset > 0) && ((_buffer.offset() + fbe_optional_offset + fbe_optional_size) <= _buffer.size())) && "Model is broken!");
    if ((fbe_optional_offset == 0) || ((_buffer.offset() + fbe_optional_offset + fbe_optional_size) > _buffer.size()))
        return 0;

    unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset() + 1, fbe_optional_offset);

    _buffer.shift(fbe_optional_offset);
    return fbe_optional_offset;
}

template <typename T>
inline void FieldModel<std::optional<T>>::set_end(size_t fbe_begin)
{
    _buffer.unshift(fbe_begin);
}

template <typename T>
inline void FieldModel<std::optional<T>>::set(const std::optional<T>& opt, std::pmr::memory_resource* resource)
{
    size_t fbe_begin = set_begin(opt.has_value());
    if (fbe_begin == 0)
        return;

    if (opt)
        value.set(opt.value(), resource);

    set_end(fbe_begin);
}
)CODE";
  }

  static std::string GenerateFBEFieldModelArray_Header() {
    return R"CODE(
// Fast Binary Encoding field model array
template <typename T, size_t N>
class FieldModelArray
{
public:
    FieldModelArray(FBEBuffer& buffer, size_t offset) noexcept : _buffer(buffer), _offset(offset), _model(buffer, offset) {}

    // Get the field offset
    size_t fbe_offset() const noexcept { return _offset; }
    // Get the field size
    size_t fbe_size() const noexcept { return N * _model.fbe_size(); }
    // Get the field extra size
    size_t fbe_extra() const noexcept { return 0; }

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept { _offset -= size; }

    // Get the array
    const uint8_t* data() const noexcept;
    // Get the array
    uint8_t* data() noexcept;
    // Get the array offset
    size_t offset() const noexcept { return 0; }
    // Get the array size
    size_t size() const noexcept { return N; }

    // Array index operator
    FieldModel<T> operator[](size_t index) const noexcept;

    // Check if the array is valid
    bool verify() const noexcept;

    // Get the array as C-array
    template <size_t S>
    void get(T (&values)[S], std::pmr::memory_resource* resource = nullptr) const noexcept;
    // Get the array as std::array
    template <size_t S>
    void get(std::array<T, S>& values, std::pmr::memory_resource* resource = nullptr) const noexcept;
    // Get the array as FastVec
    void get(FastVec<T>& values, std::pmr::memory_resource* resource = nullptr) const noexcept;

    // Set the array as C-array
    template <size_t S>
    void set(const T (&values)[S], std::pmr::memory_resource* resource) noexcept;
    // Set the array as std::array
    template <size_t S>
    void set(const std::array<T, S>& values, std::pmr::memory_resource* resource) noexcept;
    // Set the array as FastVec
    void set(const FastVec<T>& values, std::pmr::memory_resource* resource) noexcept;

private:
    FBEBuffer& _buffer;
    size_t _offset;
    FieldModel<T> _model;
};
)CODE";
  }

  static std::string GenerateFBEFieldModelArray_Inline() {
    return R"CODE(
template <typename T, size_t N>
inline const uint8_t* FieldModelArray<T, N>::data() const noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    return _buffer.data() + _buffer.offset() + fbe_offset();
}

template <typename T, size_t N>
inline uint8_t* FieldModelArray<T, N>::data() noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    return _buffer.data() + _buffer.offset() + fbe_offset();
}

template <typename T, size_t N>
inline FieldModel<T> FieldModelArray<T, N>::operator[](size_t index) const noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    assert((index < N) && "Index is out of bounds!");

    FieldModel<T> fbe_model(_buffer, fbe_offset());
    fbe_model.fbe_shift(index * fbe_model.fbe_size());
    return fbe_model;
}

template <typename T, size_t N>
inline bool FieldModelArray<T, N>::verify() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return false;

    if constexpr (is_fbe_final_primitive_v<T>) {
        // Primitive types always verify successfully if buffer size is sufficient
        return true;
    } else {
        FieldModel<T> fbe_model(_buffer, fbe_offset());
        for (size_t i = 0; i < N; ++i)
        {
            if (!fbe_model.verify())
                return false;
            fbe_model.fbe_shift(fbe_model.fbe_size());
        }
        return true;
    }
}

template <typename T, size_t N>
template <size_t S>
inline void FieldModelArray<T, N>::get(T (&values)[S], std::pmr::memory_resource* resource) const noexcept
{
    constexpr size_t count = (S < N) ? S : N;

    if constexpr (is_fbe_final_primitive_v<T>) {
        // Bulk copy for primitive types
        const uint8_t* src = _buffer.data() + _buffer.offset() + fbe_offset();
        memcpy(values, src, count * sizeof(T));
    } else {
        auto fbe_model = (*this)[0];
        for (size_t i = 0; i < count; ++i)
        {
            fbe_model.get(values[i], resource);
            fbe_model.fbe_shift(fbe_model.fbe_size());
        }
    }
}

template <typename T, size_t N>
template <size_t S>
inline void FieldModelArray<T, N>::get(std::array<T, S>& values, std::pmr::memory_resource* resource) const noexcept
{
    constexpr size_t count = (S < N) ? S : N;

    if constexpr (is_fbe_final_primitive_v<T>) {
        // Bulk copy for primitive types
        const uint8_t* src = _buffer.data() + _buffer.offset() + fbe_offset();
        memcpy(values.data(), src, count * sizeof(T));
    } else {
        auto fbe_model = (*this)[0];
        for (size_t i = 0; i < count; ++i)
        {
            fbe_model.get(values[i], resource);
            fbe_model.fbe_shift(fbe_model.fbe_size());
        }
    }
}

template <typename T, size_t N>
inline void FieldModelArray<T, N>::get(FastVec<T>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();
    values.reserve(N);

    if constexpr (is_fbe_final_primitive_v<T>) {
        // Bulk copy for primitive types
        const uint8_t* src = _buffer.data() + _buffer.offset() + fbe_offset();
        values.resize(N);
        memcpy(values.data(), src, N * sizeof(T));
    } else {
        auto fbe_model = (*this)[0];
        for (size_t i = 0; i < N; ++i)
        {
            T value = T();
            fbe_model.get(value, resource);
            #if defined(USING_STD_VECTOR)
            values.emplace_back(std::move(value));
            #else
            values.template emplace_back(std::move(value));
            #endif
            fbe_model.fbe_shift(fbe_model.fbe_size());
        }
    }
}

template <typename T, size_t N>
template <size_t S>
inline void FieldModelArray<T, N>::set(const T (&values)[S], std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    constexpr size_t count = (S < N) ? S : N;

    if constexpr (is_fbe_final_primitive_v<T>) {
        // Bulk copy for primitive types
        uint8_t* dest = _buffer.data() + _buffer.offset() + fbe_offset();
        memcpy(dest, values, count * sizeof(T));
    } else {
        auto fbe_model = (*this)[0];
        for (size_t i = 0; i < count; ++i)
        {
            fbe_model.set(values[i], resource);
            fbe_model.fbe_shift(fbe_model.fbe_size());
        }
    }
}

template <typename T, size_t N>
template <size_t S>
inline void FieldModelArray<T, N>::set(const std::array<T, S>& values, std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    constexpr size_t count = (S < N) ? S : N;

    if constexpr (is_fbe_final_primitive_v<T>) {
        // Bulk copy for primitive types
        uint8_t* dest = _buffer.data() + _buffer.offset() + fbe_offset();
        memcpy(dest, values.data(), count * sizeof(T));
    } else {
        auto fbe_model = (*this)[0];
        for (size_t i = 0; i < count; ++i)
        {
            fbe_model.set(values[i], resource);
            fbe_model.fbe_shift(fbe_model.fbe_size());
        }
    }
}

template <typename T, size_t N>
inline void FieldModelArray<T, N>::set(const FastVec<T>& values, std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    const size_t count = std::min(values.size(), N);

    if constexpr (is_fbe_final_primitive_v<T>) {
        // Bulk copy for primitive types
        uint8_t* dest = _buffer.data() + _buffer.offset() + fbe_offset();
        memcpy(dest, values.data(), count * sizeof(T));
    } else {
        auto fbe_model = (*this)[0];
        for (size_t i = 0; i < count; ++i)
        {
            fbe_model.set(values[i], resource);
            fbe_model.fbe_shift(fbe_model.fbe_size());
        }
    }
}
)CODE";
  }

  static std::string GenerateFBEFieldModelVector_Header() {
    return R"CODE(
// Fast Binary Encoding field model vector
template <typename T>
class FieldModelVector
{
public:
    FieldModelVector(FBEBuffer& buffer, size_t offset) noexcept : _buffer(buffer), _offset(offset) {}

    // Get the field offset
    size_t fbe_offset() const noexcept { return _offset; }
    // Get the field size
    size_t fbe_size() const noexcept { return 4; }
    // Get the field extra size
    size_t fbe_extra() const noexcept;

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept { _offset -= size; }

    // Get the vector offset
    size_t offset() const noexcept;
    // Get the vector size
    size_t size() const noexcept;

    // Vector index operator
    FieldModel<T> operator[](size_t index) const noexcept;

    // Resize the vector and get its first model
    FieldModel<T> resize(size_t size);

    // Check if the vector is valid
    bool verify() const noexcept;

    // Get the vector as FastVec
    void get(FastVec<T>& values, std::pmr::memory_resource* resource) const noexcept;
    // Get the vector as std::list
    void get(std::list<T>& values, std::pmr::memory_resource* resource) const noexcept;
    // Get the vector as std::set
    void get(std::set<T>& values, std::pmr::memory_resource* resource) const noexcept;

    // Get the vector as std::pmr::vector
    void get(std::pmr::vector<T>& values, std::pmr::memory_resource* resource) const noexcept;
    // Get the vector as std::pmr::list
    void get(std::pmr::list<T>& values, std::pmr::memory_resource* resource) const noexcept;
    // Get the vector as std::pmr::set
    void get(std::pmr::set<T>& values, std::pmr::memory_resource* resource) const noexcept;

    // Set the vector as FastVec
    void set(const FastVec<T>& values, std::pmr::memory_resource* resource) noexcept;
    // Set the vector as std::list
    void set(const std::list<T>& values, std::pmr::memory_resource* resource) noexcept;
    // Set the vector as std::set
    void set(const std::set<T>& values, std::pmr::memory_resource* resource) noexcept;

    // Set the vector as std::pmr::vector
    void set(const std::pmr::vector<T>& values, std::pmr::memory_resource* resource) noexcept;
    // Set the vector as std::pmr::list
    void set(const std::pmr::list<T>& values, std::pmr::memory_resource* resource) noexcept;
    // Set the vector as std::pmr::set
    void set(const std::pmr::set<T>& values, std::pmr::memory_resource* resource) noexcept;

#if defined(USING_BTREE_MAP)
    // Get the vector as FBE::set (btree_set with std allocator)
    void get(FBE::set<T>& values, std::pmr::memory_resource* resource) const noexcept;
    // Get the vector as FBE::pmr::set (btree_set with pmr allocator)
    void get(FBE::pmr::set<T>& values, std::pmr::memory_resource* resource) const noexcept;
    // Set the vector as FBE::set (btree_set with std allocator)
    void set(const FBE::set<T>& values, std::pmr::memory_resource* resource) noexcept;
    // Set the vector as FBE::pmr::set (btree_set with pmr allocator)
    void set(const FBE::pmr::set<T>& values, std::pmr::memory_resource* resource) noexcept;
#endif

private:
    FBEBuffer& _buffer;
    size_t _offset;
};
)CODE";
  }

  static std::string GenerateFBEFieldModelVector_Inline() {
    return R"CODE(
template <typename T>
inline size_t FieldModelVector<T>::fbe_extra() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return 0;

    uint32_t fbe_vector_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    if ((fbe_vector_offset == 0) || ((_buffer.offset() + fbe_vector_offset + 4) > _buffer.size()))
        return 0;

    uint32_t fbe_vector_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_vector_offset);

    size_t fbe_result = 4;
    FieldModel<T> fbe_model(_buffer, fbe_vector_offset + 4);
    for (size_t i = fbe_vector_size; i-- > 0;)
    {
        fbe_result += fbe_model.fbe_size() + fbe_model.fbe_extra();
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
    return fbe_result;
}

template <typename T>
inline size_t FieldModelVector<T>::offset() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return 0;

    uint32_t fbe_vector_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    return fbe_vector_offset;
}

template <typename T>
inline size_t FieldModelVector<T>::size() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return 0;

    uint32_t fbe_vector_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    if ((fbe_vector_offset == 0) || ((_buffer.offset() + fbe_vector_offset + 4) > _buffer.size()))
        return 0;

    uint32_t fbe_vector_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_vector_offset);
    return fbe_vector_size;
}

template <typename T>
inline FieldModel<T> FieldModelVector<T>::operator[](size_t index) const noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");

    uint32_t fbe_vector_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    assert(((fbe_vector_offset > 0) && ((_buffer.offset() + fbe_vector_offset + 4) <= _buffer.size())) && "Model is broken!");

    [[maybe_unused]] uint32_t fbe_vector_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_vector_offset);
    assert((index < fbe_vector_size) && "Index is out of bounds!");

    FieldModel<T> fbe_model(_buffer, fbe_vector_offset + 4);
    fbe_model.fbe_shift(index * fbe_model.fbe_size());
    return fbe_model;
}

template <typename T>
inline FieldModel<T> FieldModelVector<T>::resize(size_t size)
{
    FieldModel<T> fbe_model(_buffer, fbe_offset());

    uint32_t fbe_vector_size = (uint32_t)(size * fbe_model.fbe_size());
    uint32_t fbe_vector_offset = (uint32_t)(_buffer.allocate(4 + fbe_vector_size) - _buffer.offset());
    assert(((fbe_vector_offset > 0) && ((_buffer.offset() + fbe_vector_offset + 4) <= _buffer.size())) && "Model is broken!");

    unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset(), fbe_vector_offset);
    unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_vector_offset, size);

    memset((char*)(_buffer.data() + _buffer.offset() + fbe_vector_offset + 4), 0, fbe_vector_size);

    return FieldModel<T>(_buffer, fbe_vector_offset + 4);
}

template <typename T>
inline bool FieldModelVector<T>::verify() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return true;

    uint32_t fbe_vector_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    if (fbe_vector_offset == 0)
        return true;

    if ((_buffer.offset() + fbe_vector_offset + 4) > _buffer.size())
        return false;

    uint32_t fbe_vector_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_vector_offset);

    if constexpr (is_fbe_final_primitive_v<T>) {
        // Bulk verification for primitive types - just check total size
        size_t total_size = fbe_vector_size * sizeof(T);
        return ((_buffer.offset() + fbe_vector_offset + 4 + total_size) <= _buffer.size());
    } else {
        FieldModel<T> fbe_model(_buffer, fbe_vector_offset + 4);
        for (size_t i = 0; i < fbe_vector_size; ++i)
        {
            if (!fbe_model.verify())
                return false;
            fbe_model.fbe_shift(fbe_model.fbe_size());
        }
        return true;
    }
}

template <typename T>
inline void FieldModelVector<T>::get(FastVec<T>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();

    size_t fbe_vector_size = size();
    if (fbe_vector_size == 0)
        return;

    if constexpr (is_fbe_final_primitive_v<T>) {
        // Bulk copy for primitive types
        uint32_t fbe_vector_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
        const uint8_t* src = _buffer.data() + _buffer.offset() + fbe_vector_offset + 4;
        values.resize(fbe_vector_size);
        memcpy(values.data(), src, fbe_vector_size * sizeof(T));
    } else if constexpr (std::is_same_v<T, bool>) {
        // Special case for vector<bool> - uses proxy objects, can't pass by reference
        values.resize(fbe_vector_size);
        auto fbe_model = (*this)[0];
        for (size_t i = 0; i < fbe_vector_size; ++i)
        {
            T temp{};
            fbe_model.get(temp, resource);
            values[i] = temp;
            fbe_model.fbe_shift(fbe_model.fbe_size());
        }
    } else {
        // Pre-allocate and deserialize directly into elements (avoids temporaries)
        values.resize(fbe_vector_size);
        auto fbe_model = (*this)[0];
        for (size_t i = 0; i < fbe_vector_size; ++i)
        {
            fbe_model.get(values[i], resource);
            fbe_model.fbe_shift(fbe_model.fbe_size());
        }
    }
}

template <typename T>
inline void FieldModelVector<T>::get(std::list<T>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();

    size_t fbe_vector_size = size();
    if (fbe_vector_size == 0)
        return;

    auto fbe_model = (*this)[0];
    for (size_t i = 0; i < fbe_vector_size; ++i)
    {
        T value = T();
        fbe_model.get(value, resource);
        values.emplace_back(std::move(value));
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T>
inline void FieldModelVector<T>::get(std::set<T>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();

    size_t fbe_vector_size = size();
    if (fbe_vector_size == 0)
        return;

    auto fbe_model = (*this)[0];
    // Use hint-based insertion for O(1) amortized insertion (data is already sorted)
    auto hint = values.end();
    for (size_t i = 0; i < fbe_vector_size; ++i)
    {
        T value = T();
        fbe_model.get(value, resource);
        hint = values.emplace_hint(hint, std::move(value));
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T>
inline void FieldModelVector<T>::get(std::pmr::vector<T>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();

    size_t fbe_vector_size = size();
    if (fbe_vector_size == 0)
        return;

    if constexpr (is_fbe_final_primitive_v<T>) {
        // Bulk copy for primitive types
        uint32_t fbe_vector_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
        const uint8_t* src = _buffer.data() + _buffer.offset() + fbe_vector_offset + 4;
        values.resize(fbe_vector_size);
        memcpy(values.data(), src, fbe_vector_size * sizeof(T));
    } else if constexpr (std::is_same_v<T, bool>) {
        // Special case for vector<bool> - uses proxy objects, can't pass by reference
        values.resize(fbe_vector_size);
        auto fbe_model = (*this)[0];
        for (size_t i = 0; i < fbe_vector_size; ++i)
        {
            T temp{};
            fbe_model.get(temp, resource);
            values[i] = temp;
            fbe_model.fbe_shift(fbe_model.fbe_size());
        }
    } else {
        // Pre-allocate and deserialize directly into elements
        values.resize(fbe_vector_size);
        auto fbe_model = (*this)[0];
        for (size_t i = 0; i < fbe_vector_size; ++i)
        {
            fbe_model.get(values[i], resource);
            fbe_model.fbe_shift(fbe_model.fbe_size());
        }
    }
}

template <typename T>
inline void FieldModelVector<T>::get(std::pmr::list<T>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();

    size_t fbe_vector_size = size();
    if (fbe_vector_size == 0)
        return;

    auto fbe_model = (*this)[0];
    for (size_t i = 0; i < fbe_vector_size; ++i)
    {
        if constexpr (std::is_constructible_v<T, std::pmr::polymorphic_allocator<char>> and not is_variant_v<T>) {
            T value{resource};
            fbe_model.get(value, resource);
            values.emplace_back(std::move(value));
        } else {
            T value = T();
            fbe_model.get(value, resource);
            values.emplace_back(std::move(value));
        }
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T>
inline void FieldModelVector<T>::get(std::pmr::set<T>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();

    size_t fbe_vector_size = size();
    if (fbe_vector_size == 0)
        return;

    auto fbe_model = (*this)[0];
    // Use hint-based insertion for O(1) amortized insertion (data is already sorted)
    auto hint = values.end();
    for (size_t i = 0; i < fbe_vector_size; ++i)
    {
        if constexpr (std::is_constructible_v<T, std::pmr::polymorphic_allocator<char>> and not is_variant_v<T>) {
            T value{resource};
            fbe_model.get(value, resource);
            hint = values.emplace_hint(hint, std::move(value));
        } else {
            T value = T();
            fbe_model.get(value, resource);
            hint = values.emplace_hint(hint, std::move(value));
        }
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T>
inline void FieldModelVector<T>::set(const FastVec<T>& values, std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    if constexpr (is_fbe_final_primitive_v<T>) {
        // Bulk copy for primitive types - resize() already zeroed the buffer
        // fbe_model points to fbe_vector_offset + 4
        uint8_t* dest = _buffer.data() + _buffer.offset() + fbe_model.fbe_offset();
        memcpy(dest, values.data(), values.size() * sizeof(T));
    } else {
        for (const auto& value : values)
        {
            fbe_model.set(value, resource);
            fbe_model.fbe_shift(fbe_model.fbe_size());
        }
    }
}

template <typename T>
inline void FieldModelVector<T>::set(const std::list<T>& values, std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.set(value, resource);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T>
inline void FieldModelVector<T>::set(const std::set<T>& values, std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.set(value, resource);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T>
inline void FieldModelVector<T>::set(const std::pmr::vector<T>& values, std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.set(value, resource);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T>
inline void FieldModelVector<T>::set(const std::pmr::list<T>& values, std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.set(value, resource);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T>
inline void FieldModelVector<T>::set(const std::pmr::set<T>& values, std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.set(value, resource);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

#if defined(USING_BTREE_MAP)
template <typename T>
inline void FieldModelVector<T>::get(FBE::set<T>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();

    size_t fbe_vector_size = size();
    if (fbe_vector_size == 0)
        return;

    auto fbe_model = (*this)[0];
    // Use hint-based insertion for O(1) amortized insertion (data is already sorted)
    auto hint = values.end();
    for (size_t i = 0; i < fbe_vector_size; ++i)
    {
        T value = T();
        fbe_model.get(value, resource);
        hint = values.emplace_hint(hint, std::move(value));
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T>
inline void FieldModelVector<T>::get(FBE::pmr::set<T>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();

    size_t fbe_vector_size = size();
    if (fbe_vector_size == 0)
        return;

    auto fbe_model = (*this)[0];
    // Use hint-based insertion for O(1) amortized insertion (data is already sorted)
    auto hint = values.end();
    for (size_t i = 0; i < fbe_vector_size; ++i)
    {
        T value = T();
        fbe_model.get(value, resource);
        hint = values.emplace_hint(hint, std::move(value));
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T>
inline void FieldModelVector<T>::set(const FBE::set<T>& values, std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.set(value, resource);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T>
inline void FieldModelVector<T>::set(const FBE::pmr::set<T>& values, std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.set(value, resource);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}
#endif
)CODE";
  }

  static std::string GenerateFBEFieldModelMap_Header() {
    return R"CODE(
// Fast Binary Encoding field model map
template <typename TKey, typename TValue>
class FieldModelMap
{
public:
    FieldModelMap(FBEBuffer& buffer, size_t offset) noexcept : _buffer(buffer), _offset(offset) {}

    // Get the field offset
    size_t fbe_offset() const noexcept { return _offset; }
    // Get the field size
    size_t fbe_size() const noexcept { return 4; }
    // Get the field extra size
    size_t fbe_extra() const noexcept;

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept { _offset -= size; }

    // Get the map offset
    size_t offset() const noexcept;
    // Get the map size
    size_t size() const noexcept;

    // Map index operator
    std::pair<FieldModel<TKey>, FieldModel<TValue>> operator[](size_t index) const noexcept;

    // Resize the map and get its first model
    std::pair<FieldModel<TKey>, FieldModel<TValue>> resize(size_t size);

    // Check if the map is valid
    bool verify() const noexcept;

    // Get the map as std::map
    void get(std::map<TKey, TValue>& values, std::pmr::memory_resource* resource = nullptr) const noexcept;
    // Get the map as std::unordered_map
    void get(std::unordered_map<TKey, TValue>& values, std::pmr::memory_resource* resource = nullptr) const noexcept;

    // Get the map as std::pmr::map
    void get(std::pmr::map<TKey, TValue>& values, std::pmr::memory_resource* resource) const noexcept;
    // Get the map as std::pmr::unordered_map
    void get(std::pmr::unordered_map<TKey, TValue>& values, std::pmr::memory_resource* resource) const noexcept;

#if defined(USING_BTREE_MAP)
    // Get the map as FBE::map (btree_map with std allocator)
    void get(FBE::map<TKey, TValue>& values, std::pmr::memory_resource* resource) const noexcept;
    // Get the map as FBE::pmr::map (btree_map with pmr allocator)
    void get(FBE::pmr::map<TKey, TValue>& values, std::pmr::memory_resource* resource) const noexcept;
#endif

    // Set the map as std::map
    void set(const std::map<TKey, TValue>& values, std::pmr::memory_resource* resource) noexcept;
    // Set the map as std::unordered_map
    void set(const std::unordered_map<TKey, TValue>& values, std::pmr::memory_resource* resource) noexcept;

    // Set the map as std::pmr::map
    void set(const std::pmr::map<TKey, TValue>& values, std::pmr::memory_resource* resource) noexcept;
    // Set the map as std::pmr::unordered_map
    void set(const std::pmr::unordered_map<TKey, TValue>& values, std::pmr::memory_resource* resource) noexcept;

#if defined(USING_BTREE_MAP)
    // Set the map as FBE::map (btree_map with std allocator)
    void set(const FBE::map<TKey, TValue>& values, std::pmr::memory_resource* resource) noexcept;
    // Set the map as FBE::pmr::map (btree_map with pmr allocator)
    void set(const FBE::pmr::map<TKey, TValue>& values, std::pmr::memory_resource* resource) noexcept;
#endif

private:
    FBEBuffer& _buffer;
    size_t _offset;
};
)CODE";
  }

  static std::string GenerateFBEFieldModelMap_Inline() {
    return R"CODE(
template <typename TKey, typename TValue>
inline size_t FieldModelMap<TKey, TValue>::fbe_extra() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return 0;

    uint32_t fbe_map_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    if ((fbe_map_offset == 0) || ((_buffer.offset() + fbe_map_offset + 4) > _buffer.size()))
        return 0;

    uint32_t fbe_map_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_map_offset);

    size_t fbe_result = 4;
    FieldModel<TKey> fbe_model_key(_buffer, fbe_map_offset + 4);
    FieldModel<TValue> fbe_model_value(_buffer, fbe_map_offset + 4 + fbe_model_key.fbe_size());
    for (size_t i = fbe_map_size; i-- > 0;)
    {
        fbe_result += fbe_model_key.fbe_size() + fbe_model_key.fbe_extra();
        fbe_model_key.fbe_shift(fbe_model_key.fbe_size() + fbe_model_value.fbe_size());
        fbe_result += fbe_model_value.fbe_size() + fbe_model_value.fbe_extra();
        fbe_model_value.fbe_shift(fbe_model_key.fbe_size() + fbe_model_value.fbe_size());
    }
    return fbe_result;
}

template <typename TKey, typename TValue>
inline size_t FieldModelMap<TKey, TValue>::offset() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return 0;

    uint32_t fbe_map_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    return fbe_map_offset;
}

template <typename TKey, typename TValue>
inline size_t FieldModelMap<TKey, TValue>::size() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return 0;

    uint32_t fbe_map_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    if ((fbe_map_offset == 0) || ((_buffer.offset() + fbe_map_offset + 4) > _buffer.size()))
        return 0;

    uint32_t fbe_map_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_map_offset);
    return fbe_map_size;
}

template <typename TKey, typename TValue>
inline std::pair<FieldModel<TKey>, FieldModel<TValue>> FieldModelMap<TKey, TValue>::operator[](size_t index) const noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");

    uint32_t fbe_map_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    assert(((fbe_map_offset > 0) && ((_buffer.offset() + fbe_map_offset + 4) <= _buffer.size())) && "Model is broken!");

    [[maybe_unused]] uint32_t fbe_map_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_map_offset);
    assert((index < fbe_map_size) && "Index is out of bounds!");

    FieldModel<TKey> fbe_model_key(_buffer, fbe_map_offset + 4);
    FieldModel<TValue> fbe_model_value(_buffer, fbe_map_offset + 4 + fbe_model_key.fbe_size());
    fbe_model_key.fbe_shift(index * (fbe_model_key.fbe_size() + fbe_model_value.fbe_size()));
    fbe_model_value.fbe_shift(index * (fbe_model_key.fbe_size() + fbe_model_value.fbe_size()));
    return std::make_pair(fbe_model_key, fbe_model_value);
}

template <typename TKey, typename TValue>
inline std::pair<FieldModel<TKey>, FieldModel<TValue>> FieldModelMap<TKey, TValue>::resize(size_t size)
{
    FieldModel<TKey> fbe_model_key(_buffer, fbe_offset());
    FieldModel<TValue> fbe_model_value(_buffer, fbe_offset() + fbe_model_key.fbe_size());

    uint32_t fbe_map_size = (uint32_t)(size * (fbe_model_key.fbe_size() + fbe_model_value.fbe_size()));
    uint32_t fbe_map_offset = (uint32_t)(_buffer.allocate(4 + fbe_map_size) - _buffer.offset());
    assert(((fbe_map_offset > 0) && ((_buffer.offset() + fbe_map_offset + 4 + fbe_map_size) <= _buffer.size())) && "Model is broken!");

    unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset(), fbe_map_offset);
    unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_map_offset, size);


    memset((char*)(_buffer.data() + _buffer.offset() + fbe_map_offset + 4), 0, fbe_map_size);

    return std::make_pair(FieldModel<TKey>(_buffer, fbe_map_offset + 4), FieldModel<TValue>(_buffer, fbe_map_offset + 4 + fbe_model_key.fbe_size()));
}

template <typename TKey, typename TValue>
inline bool FieldModelMap<TKey, TValue>::verify() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return true;

    uint32_t fbe_map_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    if (fbe_map_offset == 0)
        return true;

    if ((_buffer.offset() + fbe_map_offset + 4) > _buffer.size())
        return false;

    uint32_t fbe_map_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_map_offset);

    FieldModel<TKey> fbe_model_key(_buffer, fbe_map_offset + 4);
    FieldModel<TValue> fbe_model_value(_buffer, fbe_map_offset + 4 + fbe_model_key.fbe_size());
    for (size_t i = fbe_map_size; i-- > 0;)
    {
        if (!fbe_model_key.verify())
            return false;
        fbe_model_key.fbe_shift(fbe_model_key.fbe_size() + fbe_model_value.fbe_size());
        if (!fbe_model_value.verify())
            return false;
        fbe_model_value.fbe_shift(fbe_model_key.fbe_size() + fbe_model_value.fbe_size());
    }

    return true;
}

template <typename TKey, typename TValue>
inline void FieldModelMap<TKey, TValue>::get(std::map<TKey, TValue>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();

    size_t fbe_map_size = size();
    if (fbe_map_size == 0)
        return;

    auto fbe_model = (*this)[0];
    size_t fbe_model_stride = fbe_model.first.fbe_size() + fbe_model.second.fbe_size();
    // Use hint-based insertion: serialized data is already sorted (from std::map),
    // so inserting at end() is O(1) amortized instead of O(log n)
    auto hint = values.end();
    for (size_t i = 0; i < fbe_map_size; ++i)
    {
        TKey key;
        TValue value;
        fbe_model.first.get(key, resource);
        fbe_model.second.get(value, resource);
        hint = values.emplace_hint(hint, std::move(key), std::move(value));
        fbe_model.first.fbe_shift(fbe_model_stride);
        fbe_model.second.fbe_shift(fbe_model_stride);
    }
}

template <typename TKey, typename TValue>
inline void FieldModelMap<TKey, TValue>::get(std::unordered_map<TKey, TValue>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();

    size_t fbe_map_size = size();
    if (fbe_map_size == 0)
        return;

    values.reserve(fbe_map_size);
    auto fbe_model = (*this)[0];
    size_t fbe_model_stride = fbe_model.first.fbe_size() + fbe_model.second.fbe_size();
    for (size_t i = 0; i < fbe_map_size; ++i)
    {
        TKey key;
        TValue value;
        fbe_model.first.get(key, resource);
        fbe_model.second.get(value, resource);
        values.emplace(std::move(key), std::move(value));
        fbe_model.first.fbe_shift(fbe_model_stride);
        fbe_model.second.fbe_shift(fbe_model_stride);
    }
}

template <typename TKey, typename TValue>
inline void FieldModelMap<TKey, TValue>::get(std::pmr::map<TKey, TValue>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();

    size_t fbe_map_size = size();
    if (fbe_map_size == 0)
        return;

    auto fbe_model = (*this)[0];
    size_t fbe_model_stride = fbe_model.first.fbe_size() + fbe_model.second.fbe_size();
    // Use hint-based insertion for O(1) amortized insertion
    auto hint = values.end();
    for (size_t i = 0; i < fbe_map_size; ++i)
    {
        TKey key;
        TValue value;
        fbe_model.first.get(key, resource);
        fbe_model.second.get(value, resource);
        hint = values.emplace_hint(hint, std::move(key), std::move(value));
        fbe_model.first.fbe_shift(fbe_model_stride);
        fbe_model.second.fbe_shift(fbe_model_stride);
    }
}

template <typename TKey, typename TValue>
inline void FieldModelMap<TKey, TValue>::get(std::pmr::unordered_map<TKey, TValue>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();

    size_t fbe_map_size = size();
    if (fbe_map_size == 0)
        return;

    values.reserve(fbe_map_size);
    auto fbe_model = (*this)[0];
    size_t fbe_model_stride = fbe_model.first.fbe_size() + fbe_model.second.fbe_size();
    for (size_t i = 0; i < fbe_map_size; ++i)
    {
        TKey key;
        TValue value;
        fbe_model.first.get(key, resource);
        fbe_model.second.get(value, resource);
        values.emplace(std::move(key), std::move(value));
        fbe_model.first.fbe_shift(fbe_model_stride);
        fbe_model.second.fbe_shift(fbe_model_stride);
    }
}

#if defined(USING_BTREE_MAP)
template <typename TKey, typename TValue>
inline void FieldModelMap<TKey, TValue>::get(FBE::map<TKey, TValue>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();

    size_t fbe_map_size = size();
    if (fbe_map_size == 0)
        return;

    auto fbe_model = (*this)[0];
    size_t fbe_model_stride = fbe_model.first.fbe_size() + fbe_model.second.fbe_size();
    // Use hint-based insertion for O(1) amortized insertion
    auto hint = values.end();
    for (size_t i = 0; i < fbe_map_size; ++i)
    {
        TKey key;
        TValue value;
        fbe_model.first.get(key, resource);
        fbe_model.second.get(value, resource);
        hint = values.emplace_hint(hint, std::move(key), std::move(value));
        fbe_model.first.fbe_shift(fbe_model_stride);
        fbe_model.second.fbe_shift(fbe_model_stride);
    }
}

template <typename TKey, typename TValue>
inline void FieldModelMap<TKey, TValue>::get(FBE::pmr::map<TKey, TValue>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();

    size_t fbe_map_size = size();
    if (fbe_map_size == 0)
        return;

    auto fbe_model = (*this)[0];
    size_t fbe_model_stride = fbe_model.first.fbe_size() + fbe_model.second.fbe_size();
    // Use hint-based insertion for O(1) amortized insertion
    auto hint = values.end();
    for (size_t i = 0; i < fbe_map_size; ++i)
    {
        TKey key;
        TValue value;
        fbe_model.first.get(key, resource);
        fbe_model.second.get(value, resource);
        hint = values.emplace_hint(hint, std::move(key), std::move(value));
        fbe_model.first.fbe_shift(fbe_model_stride);
        fbe_model.second.fbe_shift(fbe_model_stride);
    }
}
#endif

template <typename TKey, typename TValue>
inline void FieldModelMap<TKey, TValue>::set(const std::map<TKey, TValue>& values, std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.first.set(value.first, resource);
        fbe_model.first.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
        fbe_model.second.set(value.second, resource);
        fbe_model.second.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
    }
}

template <typename TKey, typename TValue>
inline void FieldModelMap<TKey, TValue>::set(const std::unordered_map<TKey, TValue>& values, std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.first.set(value.first, resource);
        fbe_model.first.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
        fbe_model.second.set(value.second, resource);
        fbe_model.second.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
    }
}

template <typename TKey, typename TValue>
inline void FieldModelMap<TKey, TValue>::set(const std::pmr::map<TKey, TValue>& values, std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.first.set(value.first, resource);
        fbe_model.first.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
        fbe_model.second.set(value.second, resource);
        fbe_model.second.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
    }
}

template <typename TKey, typename TValue>
inline void FieldModelMap<TKey, TValue>::set(const std::pmr::unordered_map<TKey, TValue>& values, std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.first.set(value.first, resource);
        fbe_model.first.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
        fbe_model.second.set(value.second, resource);
        fbe_model.second.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
    }
}

#if defined(USING_BTREE_MAP)
template <typename TKey, typename TValue>
inline void FieldModelMap<TKey, TValue>::set(const FBE::map<TKey, TValue>& values, std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.first.set(value.first, resource);
        fbe_model.first.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
        fbe_model.second.set(value.second, resource);
        fbe_model.second.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
    }
}

template <typename TKey, typename TValue>
inline void FieldModelMap<TKey, TValue>::set(const FBE::pmr::map<TKey, TValue>& values, std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.first.set(value.first, resource);
        fbe_model.first.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
        fbe_model.second.set(value.second, resource);
        fbe_model.second.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
    }
}
#endif
)CODE";
  }

  static std::string GenerateFBEFinalModel_Header() {
    return R"CODE(
// Fast Binary Encoding base final model
template <typename T, typename TBase = T>
class FinalModelBase
{
public:
    FinalModelBase(FBEBuffer& buffer, size_t offset) noexcept : _buffer(buffer), _offset(offset) {}

    // Get the allocation size
    size_t fbe_allocation_size(T value) const noexcept { return fbe_size(); }

    // Get the field offset
    size_t fbe_offset() const noexcept { return _offset; }
    // Set the field offset
    size_t fbe_offset(size_t offset) const noexcept { return _offset = offset; }

    // Get the final size
    size_t fbe_size() const noexcept { return sizeof(TBase); }

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept { _offset -= size; }

    // Check if the value is valid
    size_t verify() const noexcept;

    // Get the field value
    size_t get(T& value) const noexcept;
    // Get the field value (with memory resource, ignored for primitives)
    size_t get(T& value, [[maybe_unused]] std::pmr::memory_resource* resource) const noexcept { return get(value); }
    // Set the field value
    size_t set(T value) noexcept;

private:
    FBEBuffer& _buffer;
    mutable size_t _offset;
};

// Fast Binary Encoding final model
template <typename T>
class FinalModel : public FinalModelBase<T>
{
public:
    using FinalModelBase<T>::FinalModelBase;
};

// Fast Binary Encoding final model bool specialization
template <>
class FinalModel<bool> : public FinalModelBase<bool, uint8_t>
{
public:
    using FinalModelBase<bool, uint8_t>::FinalModelBase;
};

// Fast Binary Encoding final model char specialization
template <>
class FinalModel<char> : public FinalModelBase<char, uint8_t>
{
public:
    using FinalModelBase<char, uint8_t>::FinalModelBase;
};

// Fast Binary Encoding final model wchar specialization
template <>
class FinalModel<wchar_t> : public FinalModelBase<wchar_t, uint32_t>
{
public:
    using FinalModelBase<wchar_t, uint32_t>::FinalModelBase;
};

// Helper function for PMR-aware variant value construction (used by FinalModel for variants)
template<typename V, typename T>
auto final_variant_emplace_value(V& fbe_value, std::pmr::memory_resource* resource) {
    if constexpr (std::is_integral_v<T> or std::is_floating_point_v<T> or std::is_enum_v<T> or std::is_same_v<T, std::string> or not std::is_constructible_v<T, std::pmr::memory_resource*>) {
        fbe_value.template emplace<T>();
    } else {
        fbe_value.template emplace<T>(resource);
    }
}
)CODE";
  }

  static std::string GenerateFBEFinalModel_Inline() {
    return R"CODE(
template <typename T, typename TBase>
inline size_t FinalModelBase<T, TBase>::verify() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return std::numeric_limits<std::size_t>::max();

    return fbe_size();
}

template <typename T, typename TBase>
inline size_t FinalModelBase<T, TBase>::get(T& value) const noexcept
{
    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    if ((fbe_full_offset + fbe_size()) > _buffer.size())
        return 0;

    value = unaligned_load<T>(_buffer.data() + fbe_full_offset);

    return fbe_size();
}

template <typename T, typename TBase>
inline size_t FinalModelBase<T, TBase>::set(T value) noexcept
{
    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    assert(((fbe_full_offset + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((fbe_full_offset + fbe_size()) > _buffer.size())
        return 0;

    unaligned_store<TBase>(_buffer.data() + fbe_full_offset, (TBase)value);
    return fbe_size();
}
)CODE";
  }

  static std::string GenerateFBEFinalModelDecimal_Header() {
    return R"CODE(
// Fast Binary Encoding final model decimal specialization
template <>
class FinalModel<decimal_t>
{
public:
    FinalModel(FBEBuffer& buffer, size_t offset) noexcept : _buffer(buffer), _offset(offset) {}

    // Get the allocation size
    size_t fbe_allocation_size(decimal_t value) const noexcept { return fbe_size(); }

    // Get the field offset
    size_t fbe_offset() const noexcept { return _offset; }
    // Set the field offset
    size_t fbe_offset(size_t offset) const noexcept { return _offset = offset; }

    // Get the final size
    size_t fbe_size() const noexcept { return 16; }

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept { _offset -= size; }

    // Check if the decimal value is valid
    size_t verify() const noexcept;

    // Get the decimal value
    size_t get(decimal_t& value) const noexcept;
    // Set the decimal value
    size_t set(decimal_t value) noexcept;

private:
    FBEBuffer& _buffer;
    mutable size_t _offset;

    static uint64_t extract(double a) noexcept;
    static uint64_t uint32x32(uint32_t a, uint32_t b) noexcept;
    static void uint64x64(uint64_t a, uint64_t b, uint64_t& low64, uint32_t& high32) noexcept;
};
)CODE";
  }

  static std::string GenerateFBEFinalModelDecimal_Source() {
    return R"CODE(
// Lookup table for powers of 10 (10^0 to 10^28) - for FinalModel
static constexpr double kFinalPow10Table[] = {
    1e0,  1e1,  1e2,  1e3,  1e4,  1e5,  1e6,  1e7,  1e8,  1e9,
    1e10, 1e11, 1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19,
    1e20, 1e21, 1e22, 1e23, 1e24, 1e25, 1e26, 1e27, 1e28
};

// Lookup table for powers of 10 as uint64_t (10^0 to 10^19) - for FinalModel
static constexpr uint64_t kFinalPow10TableU64[] = {
    1ULL,
    10ULL,
    100ULL,
    1000ULL,
    10000ULL,
    100000ULL,
    1000000ULL,
    10000000ULL,
    100000000ULL,
    1000000000ULL,
    10000000000ULL,
    100000000000ULL,
    1000000000000ULL,
    10000000000000ULL,
    100000000000000ULL,
    1000000000000000ULL,
    10000000000000000ULL,
    100000000000000000ULL,
    1000000000000000000ULL,
    10000000000000000000ULL
};

uint64_t FinalModel<decimal_t>::extract(double a) noexcept
{
    uint64_t result;
    std::memcpy(&result, &a, sizeof(double));
    return result;
}

uint64_t FinalModel<decimal_t>::uint32x32(uint32_t a, uint32_t b) noexcept
{
    return (uint64_t)a * (uint64_t)b;
}

void FinalModel<decimal_t>::uint64x64(uint64_t a, uint64_t b, uint64_t& low64, uint32_t& high32) noexcept
{
    uint64_t low = uint32x32((uint32_t)a, (uint32_t)b);
    uint64_t mid = uint32x32((uint32_t)a, (uint32_t)(b >> 32));
    uint64_t high = uint32x32((uint32_t)(a >> 32), (uint32_t)(b >> 32));
    high += (mid >> 32);
    low += (mid <<= 32);
    // Test for carry
    if (low < mid)
        high++;

    mid = uint32x32((uint32_t)(a >> 32), (uint32_t)b);
    high += (mid >> 32);
    low += (mid <<= 32);
    // Test for carry
    if (low < mid)
        high++;

    if (high > 0xFFFFFFFFu)
    {
        low64 = 0;
        high32 = 0;
    }
    low64 = low;
    high32 = (uint32_t)high;
}

size_t FinalModel<decimal_t>::verify() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return std::numeric_limits<std::size_t>::max();

    return fbe_size();
}

size_t FinalModel<decimal_t>::get(decimal_t& value) const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return 0;

    // Value taken via reverse engineering the double that corresponds to 2^64
    const double ds2to64 = 1.8446744073709552e+019;

    // Read decimal parts
    uint64_t low = unaligned_load<uint64_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    uint32_t high = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset() + 8);
    uint32_t flags = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset() + 12);

    // Calculate decimal value
    uint8_t scale = (uint8_t)(flags >> 16);
    double dValue = ((double)low + (double)high * ds2to64) / kFinalPow10Table[scale];
    if (flags & 0x80000000)
        dValue = -dValue;

    value = dValue;
    return fbe_size();
}

size_t FinalModel<decimal_t>::set(decimal_t value) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return 0;

    // The most we can scale by is 10^28, which is just slightly more
    // than 2^93.  So a float with an exponent of -94 could just
    // barely reach 0.5, but smaller exponents will always round to zero.
    const uint32_t DBLBIAS = 1022;

    // Get exponent value
    double dValue = (double)value;
    int32_t iExp = (int32_t)(((uint32_t)(extract(dValue) >> 52) & 0x7FFu) - DBLBIAS);
    if ((iExp < -94) || (iExp > 96))
    {
        // Value too big for .NET Decimal (exponent is limited to [-94, 96])
        memset((uint8_t*)(_buffer.data() + _buffer.offset() + fbe_offset()), 0, 16);
        return fbe_size();
    }

    uint32_t flags = 0;
    if (dValue < 0)
    {
        dValue = -dValue;
        flags = 0x80000000;
    }

    // Round the input to a 15-digit integer.  The R8 format has
    // only 15 digits of precision, and we want to keep garbage digits
    // out of the Decimal were making.

    // Calculate max power of 10 input value could have by multiplying
    // the exponent by log10(2).  Using scaled integer multiplcation,
    // log10(2) * 2 ^ 16 = .30103 * 65536 = 19728.3.
    int32_t iPower = 14 - ((iExp * 19728) >> 16);

    // iPower is between -14 and 43
    if (iPower >= 0)
    {
        // We have less than 15 digits, scale input up.
        if (iPower > 28)
            iPower = 28;

        dValue *= kFinalPow10Table[iPower];
    }
    else
    {
        if ((iPower != -1) || (dValue >= 1E15))
            dValue /= kFinalPow10Table[-iPower];
        else
            iPower = 0; // didn't scale it
    }

    assert(dValue < 1E15);
    if ((dValue < 1E14) && (iPower < 28))
    {
        dValue *= 10;
        iPower++;
        assert(dValue >= 1E14);
    }

    // Round to int64
    uint64_t ulMant;
    ulMant = (uint64_t)(int64_t)dValue;
    dValue -= (int64_t)ulMant; // difference between input & integer
    if ((dValue > 0.5) || ((dValue == 0.5) && ((ulMant & 1) != 0)))
        ulMant++;

    if (ulMant == 0)
    {
        // Mantissa is 0
        memset((uint8_t*)(_buffer.data() + _buffer.offset() + fbe_offset()), 0, 16);
        return fbe_size();
    }

    if (iPower < 0)
    {
        // Add -iPower factors of 10, -iPower <= (29 - 15) = 14
        iPower = -iPower;
        if (iPower < 10)
        {
            uint64_t pow10 = kFinalPow10TableU64[iPower];
            uint64_t low64 = uint32x32((uint32_t)ulMant, (uint32_t)pow10);
            uint64_t high64 = uint32x32((uint32_t)(ulMant >> 32), (uint32_t)pow10);
            unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset(), (uint32_t)low64);
            high64 += low64 >> 32;
            unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset() + 4, (uint32_t)high64);
            high64 >>= 32;
            unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset() + 8, (uint32_t)high64);
        }
        else
        {
            // Have a big power of 10.
            assert(iPower <= 14);
            uint64_t low64;
            uint32_t high32;
            uint64x64(ulMant, kFinalPow10TableU64[iPower], low64, high32);
            unaligned_store<uint64_t>(_buffer.data() + _buffer.offset() + fbe_offset(), low64);
            unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset() + 8, high32);
        }
    }
    else
    {
        // Factor out powers of 10 to reduce the scale, if possible.
        // The maximum number we could factor out would be 14.  This
        // comes from the fact we have a 15-digit number, and the
        // MSD must be non-zero -- but the lower 14 digits could be
        // zero.  Note also the scale factor is never negative, so
        // we can't scale by any more than the power we used to
        // get the integer.
        int lmax = iPower;
        if (lmax > 14)
            lmax = 14;

        if ((((uint8_t)ulMant) == 0) && (lmax >= 8))
        {
            const uint32_t den = 100000000;
            uint64_t div = ulMant / den;
            if ((uint32_t)ulMant == (uint32_t)(div * den))
            {
                ulMant = div;
                iPower -= 8;
                lmax -= 8;
            }
        }

        if ((((uint32_t)ulMant & 0xF) == 0) && (lmax >= 4))
        {
            const uint32_t den = 10000;
            uint64_t div = ulMant / den;
            if ((uint32_t)ulMant == (uint32_t)(div * den))
            {
                ulMant = div;
                iPower -= 4;
                lmax -= 4;
            }
        }

        if ((((uint32_t)ulMant & 3) == 0) && (lmax >= 2))
        {
            const uint32_t den = 100;
            uint64_t div = ulMant / den;
            if ((uint32_t)ulMant == (uint32_t)(div * den))
            {
                ulMant = div;
                iPower -= 2;
                lmax -= 2;
            }
        }

        if ((((uint32_t)ulMant & 1) == 0) && (lmax >= 1))
        {
            const uint32_t den = 10;
            uint64_t div = ulMant / den;
            if ((uint32_t)ulMant == (uint32_t)(div * den))
            {
                ulMant = div;
                iPower--;
            }
        }

        flags |= (uint32_t)iPower << 16;

        unaligned_store<uint64_t>(_buffer.data() + _buffer.offset() + fbe_offset(), ulMant);
        unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset() + 8, (uint32_t)0);
    }

    unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset() + 12, flags);
    return fbe_size();
}
)CODE";
  }

  static std::string GenerateFBEFinalModelUUID_Header() {
    return R"CODE(
// Fast Binary Encoding final model UUID specialization
template <>
class FinalModel<uuid_t>
{
public:
    FinalModel(FBEBuffer& buffer, size_t offset) noexcept : _buffer(buffer), _offset(offset) {}

    // Get the allocation size
    size_t fbe_allocation_size(uuid_t value) const noexcept { return fbe_size(); }

    // Get the field offset
    size_t fbe_offset() const noexcept { return _offset; }
    // Set the field offset
    size_t fbe_offset(size_t offset) const noexcept { return _offset = offset; }

    // Get the final size
    size_t fbe_size() const noexcept { return 16; }

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept { _offset -= size; }

    // Check if the UUID value is valid
    size_t verify() const noexcept;

    // Get the UUID value
    size_t get(uuid_t& value) const noexcept;
    // Set the UUID value
    size_t set(uuid_t value) noexcept;

private:
    FBEBuffer& _buffer;
    mutable size_t _offset;
};
)CODE";
  }

  static std::string GenerateFBEFinalModelUUID_Source() {
    return R"CODE(
size_t FinalModel<uuid_t>::verify() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return std::numeric_limits<std::size_t>::max();

    return fbe_size();
}

size_t FinalModel<uuid_t>::get(uuid_t& value) const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return 0;

    std::memcpy(value.data().data(), (const uint8_t*)(_buffer.data() + _buffer.offset() + fbe_offset()), fbe_size());
    return fbe_size();
}

size_t FinalModel<uuid_t>::set(uuid_t value) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return 0;

    std::memcpy((uint8_t*)(_buffer.data() + _buffer.offset() + fbe_offset()), value.data().data(), fbe_size());
    return fbe_size();
}
)CODE";
  }

  static std::string GenerateFBEFinalModelBytes_Header() {
    return R"CODE(
// Fast Binary Encoding final model bytes specialization
template <>
class FinalModel<buffer_t>
{
public:
    FinalModel(FBEBuffer& buffer, size_t offset) noexcept : _buffer(buffer), _offset(offset) {}

    // Get the allocation size
    size_t fbe_allocation_size(const void* data, size_t size) const noexcept { return 4 + size; }
    template <size_t N>
    size_t fbe_allocation_size(const uint8_t (&data)[N]) const noexcept { return 4 + N; }
    template <size_t N>
    size_t fbe_allocation_size(const std::array<uint8_t, N>& data) const noexcept { return 4 + N; }
    size_t fbe_allocation_size(const FastVec<uint8_t>& value) const noexcept { return 4 + value.size(); }
    size_t fbe_allocation_size(const buffer_t& value) const noexcept { return 4 + value.size(); }

    // Get the field offset
    size_t fbe_offset() const noexcept { return _offset; }
    // Set the field offset
    size_t fbe_offset(size_t offset) const noexcept { return _offset = offset; }

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept { _offset -= size; }

    // Check if the bytes value is valid
    size_t verify() const noexcept;

    // Get the bytes value
    size_t get(void* data, size_t size) const noexcept;
    // Get the bytes value
    template <size_t N>
    size_t get(uint8_t (&data)[N]) const noexcept { return get(data, N); }
    // Get the bytes value
    template <size_t N>
    size_t get(std::array<uint8_t, N>& data) const noexcept { return get(data.data(), data.size()); }
    // Get the bytes value
    size_t get(FastVec<uint8_t>& value) const noexcept;
    // Get the bytes value
    size_t get(buffer_t& value) const noexcept { return get(value.buffer()); }

    // Set the bytes value
    size_t set(const void* data, size_t size);
    // Set the bytes value
    template <size_t N>
    size_t set(const uint8_t (&data)[N]) { return set(data, N); }
    // Set the bytes value
    template <size_t N>
    size_t set(const std::array<uint8_t, N>& data) { return set(data.data(), data.size()); }
    // Set the bytes value
    size_t set(const FastVec<uint8_t>& value) { return set(value.data(), value.size()); }
    // Set the bytes value
    size_t set(const buffer_t& value) { return set(value.buffer()); }

private:
    FBEBuffer& _buffer;
    mutable size_t _offset;
};
)CODE";
  }

  static std::string GenerateFBEFinalModelBytes_Source() {
    return R"CODE(
size_t FinalModel<buffer_t>::verify() const noexcept
{
    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    size_t buffer_size = _buffer.size();
    if ((fbe_full_offset + 4) > buffer_size)
        return std::numeric_limits<std::size_t>::max();

    uint32_t fbe_bytes_size = unaligned_load<uint32_t>(_buffer.data() + fbe_full_offset);
    if ((fbe_full_offset + 4 + fbe_bytes_size) > buffer_size)
        return std::numeric_limits<std::size_t>::max();

    return 4 + fbe_bytes_size;
}

size_t FinalModel<buffer_t>::get(void* data, size_t size) const noexcept
{
    assert(((size == 0) || (data != nullptr)) && "Invalid buffer!");
    if ((size > 0) && (data == nullptr))
        return 0;

    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    const uint8_t* buffer_data = _buffer.data();
    size_t buffer_size = _buffer.size();

    assert(((fbe_full_offset + 4) <= buffer_size) && "Model is broken!");
    if ((fbe_full_offset + 4) > buffer_size)
        return 0;

    uint32_t fbe_bytes_size = unaligned_load<uint32_t>(buffer_data + fbe_full_offset);
    assert(((fbe_full_offset + 4 + fbe_bytes_size) <= buffer_size) && "Model is broken!");
    if ((fbe_full_offset + 4 + fbe_bytes_size) > buffer_size)
        return 4;

    size_t result = std::min(size, (size_t)fbe_bytes_size);
    memcpy(data, buffer_data + fbe_full_offset + 4, result);
    return 4 + fbe_bytes_size;
}

size_t FinalModel<buffer_t>::get(FastVec<uint8_t>& value) const noexcept
{
    value.clear();

    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    const uint8_t* buffer_data = _buffer.data();
    size_t buffer_size = _buffer.size();

    assert(((fbe_full_offset + 4) <= buffer_size) && "Model is broken!");
    if ((fbe_full_offset + 4) > buffer_size)
        return 0;

    uint32_t fbe_bytes_size = unaligned_load<uint32_t>(buffer_data + fbe_full_offset);
    assert(((fbe_full_offset + 4 + fbe_bytes_size) <= buffer_size) && "Model is broken!");
    if ((fbe_full_offset + 4 + fbe_bytes_size) > buffer_size)
        return 4;

    const char* fbe_bytes = (const char*)(buffer_data + fbe_full_offset + 4);
    value.assign(fbe_bytes, fbe_bytes + fbe_bytes_size);
    return 4 + fbe_bytes_size;
}

size_t FinalModel<buffer_t>::set(const void* data, size_t size)
{
    assert(((size == 0) || (data != nullptr)) && "Invalid buffer!");
    if ((size > 0) && (data == nullptr))
        return 0;

    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    uint8_t* buffer_data = _buffer.data();
    size_t buffer_size = _buffer.size();

    assert(((fbe_full_offset + 4) <= buffer_size) && "Model is broken!");
    if ((fbe_full_offset + 4) > buffer_size)
        return 0;

    uint32_t fbe_bytes_size = (uint32_t)size;
    assert(((fbe_full_offset + 4 + fbe_bytes_size) <= buffer_size) && "Model is broken!");
    if ((fbe_full_offset + 4 + fbe_bytes_size) > buffer_size)
        return 4;

    unaligned_store<uint32_t>(buffer_data + fbe_full_offset, fbe_bytes_size);

    if (fbe_bytes_size > 0)
        memcpy((char*)(buffer_data + fbe_full_offset + 4), data, fbe_bytes_size);
    return 4 + fbe_bytes_size;
}
)CODE";
  }

  static std::string GenerateFBEFinalModelPMRBytes_Header() {
    return R"CODE(
// Fast Binary Encoding final model pmr_buffer_t specialization
template <>
class FinalModel<pmr_buffer_t>
{
public:
    FinalModel(FBEBuffer& buffer, size_t offset) noexcept : _buffer(buffer), _offset(offset) {}

    // Get the allocation size
    size_t fbe_allocation_size(const void* data, size_t size) const noexcept { return 4 + size; }
    template <size_t N>
    size_t fbe_allocation_size(const uint8_t (&data)[N]) const noexcept { return 4 + N; }
    template <size_t N>
    size_t fbe_allocation_size(const std::array<uint8_t, N>& data) const noexcept { return 4 + N; }
    size_t fbe_allocation_size(const std::pmr::vector<uint8_t>& value) const noexcept { return 4 + value.size(); }
    size_t fbe_allocation_size(const pmr_buffer_t& value) const noexcept { return 4 + value.size(); }

    // Get the field offset
    size_t fbe_offset() const noexcept { return _offset; }
    // Set the field offset
    size_t fbe_offset(size_t offset) const noexcept { return _offset = offset; }

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept { _offset -= size; }

    // Check if the bytes value is valid
    size_t verify() const noexcept;

    // Get the bytes value
    size_t get(void* data, size_t size) const noexcept;
    // Get the bytes value
    template <size_t N>
    size_t get(uint8_t (&data)[N]) const noexcept { return get(data, N); }
    // Get the bytes value
    template <size_t N>
    size_t get(std::array<uint8_t, N>& data) const noexcept { return get(data.data(), data.size()); }
    // Get the bytes value (with optional memory resource)
    size_t get(std::pmr::vector<uint8_t>& value) const noexcept { return get(value, nullptr); }
    size_t get(std::pmr::vector<uint8_t>& value, std::pmr::memory_resource* resource) const noexcept;
    // Get the bytes value (with optional memory resource)
    size_t get(pmr_buffer_t& value) const noexcept { return get(value.buffer(), nullptr); }
    size_t get(pmr_buffer_t& value, std::pmr::memory_resource* resource) const noexcept { return get(value.buffer(), resource); }

    // Set the bytes value
    size_t set(const void* data, size_t size);
    // Set the bytes value
    template <size_t N>
    size_t set(const uint8_t (&data)[N]) { return set(data, N); }
    // Set the bytes value
    template <size_t N>
    size_t set(const std::array<uint8_t, N>& data) { return set(data.data(), data.size()); }
    // Set the bytes value
    size_t set(const std::pmr::vector<uint8_t>& value) { return set(value.data(), value.size()); }
    // Set the bytes value
    size_t set(const pmr_buffer_t& value) { return set(value.buffer()); }

private:
    FBEBuffer& _buffer;
    mutable size_t _offset;
};
)CODE";
  }

  static std::string GenerateFBEFinalModelPMRBytes_Source() {
    return R"CODE(
size_t FinalModel<pmr_buffer_t>::verify() const noexcept
{
    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    size_t buffer_size = _buffer.size();
    if ((fbe_full_offset + 4) > buffer_size)
        return std::numeric_limits<std::size_t>::max();

    uint32_t fbe_bytes_size = unaligned_load<uint32_t>(_buffer.data() + fbe_full_offset);
    if ((fbe_full_offset + 4 + fbe_bytes_size) > buffer_size)
        return std::numeric_limits<std::size_t>::max();

    return 4 + fbe_bytes_size;
}

size_t FinalModel<pmr_buffer_t>::get(void* data, size_t size) const noexcept
{
    assert(((size == 0) || (data != nullptr)) && "Invalid buffer!");
    if ((size > 0) && (data == nullptr))
        return 0;

    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    const uint8_t* buffer_data = _buffer.data();
    size_t buffer_size = _buffer.size();

    assert(((fbe_full_offset + 4) <= buffer_size) && "Model is broken!");
    if ((fbe_full_offset + 4) > buffer_size)
        return 0;

    uint32_t fbe_bytes_size = unaligned_load<uint32_t>(buffer_data + fbe_full_offset);
    assert(((fbe_full_offset + 4 + fbe_bytes_size) <= buffer_size) && "Model is broken!");
    if ((fbe_full_offset + 4 + fbe_bytes_size) > buffer_size)
        return 4;

    size_t result = std::min(size, (size_t)fbe_bytes_size);
    memcpy(data, buffer_data + fbe_full_offset + 4, result);
    return 4 + fbe_bytes_size;
}

size_t FinalModel<pmr_buffer_t>::get(std::pmr::vector<uint8_t>& value, std::pmr::memory_resource* resource) const noexcept
{
    value.clear();

    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    const uint8_t* buffer_data = _buffer.data();
    size_t buffer_size = _buffer.size();

    assert(((fbe_full_offset + 4) <= buffer_size) && "Model is broken!");
    if ((fbe_full_offset + 4) > buffer_size)
        return 0;

    uint32_t fbe_bytes_size = unaligned_load<uint32_t>(buffer_data + fbe_full_offset);
    assert(((fbe_full_offset + 4 + fbe_bytes_size) <= buffer_size) && "Model is broken!");
    if ((fbe_full_offset + 4 + fbe_bytes_size) > buffer_size)
        return 4;

    // Allocate and assign using the provided memory resource if available
    if (resource != nullptr && value.get_allocator().resource() != resource) {
        std::pmr::vector<uint8_t> temp(resource);
        const uint8_t* fbe_bytes = buffer_data + fbe_full_offset + 4;
        temp.assign(fbe_bytes, fbe_bytes + fbe_bytes_size);
        value = std::move(temp);
    } else {
        const uint8_t* fbe_bytes = buffer_data + fbe_full_offset + 4;
        value.assign(fbe_bytes, fbe_bytes + fbe_bytes_size);
    }
    return 4 + fbe_bytes_size;
}

size_t FinalModel<pmr_buffer_t>::set(const void* data, size_t size)
{
    assert(((size == 0) || (data != nullptr)) && "Invalid buffer!");
    if ((size > 0) && (data == nullptr))
        return 0;

    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    uint8_t* buffer_data = _buffer.data();
    size_t buffer_size = _buffer.size();

    assert(((fbe_full_offset + 4) <= buffer_size) && "Model is broken!");
    if ((fbe_full_offset + 4) > buffer_size)
        return 0;

    uint32_t fbe_bytes_size = (uint32_t)size;
    assert(((fbe_full_offset + 4 + fbe_bytes_size) <= buffer_size) && "Model is broken!");
    if ((fbe_full_offset + 4 + fbe_bytes_size) > buffer_size)
        return 4;

    unaligned_store<uint32_t>(buffer_data + fbe_full_offset, fbe_bytes_size);

    if (fbe_bytes_size > 0)
        memcpy((char*)(buffer_data + fbe_full_offset + 4), data, fbe_bytes_size);
    return 4 + fbe_bytes_size;
}
)CODE";
  }

  static std::string GenerateFBEFinalModelString_Header() {
    return R"CODE(
// Fast Binary Encoding final model string specialization
template <>
class FinalModel<FBEString>
{
public:
    FinalModel(FBEBuffer& buffer, size_t offset) noexcept : _buffer(buffer), _offset(offset) {}

    // Get the allocation size
    size_t fbe_allocation_size(const char* data, size_t size) const noexcept { return 4 + size; }
    template <size_t N>
    size_t fbe_allocation_size(const char (&data)[N]) const noexcept { return 4 + N; }
    template <size_t N>
    size_t fbe_allocation_size(const std::array<char, N>& data) const noexcept { return 4 + N; }
    size_t fbe_allocation_size(const FBEString& value) const noexcept { return 4 + value.size(); }

    // Get the field offset
    size_t fbe_offset() const noexcept { return _offset; }
    // Set the field offset
    size_t fbe_offset(size_t offset) const noexcept { return _offset = offset; }

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept { _offset -= size; }

    // Check if the string value is valid
    size_t verify() const noexcept;

    // Get the string value
    size_t get(char* data, size_t size) const noexcept;
    // Get the string value
    template <size_t N>
    size_t get(char (&data)[N]) const noexcept { return get(data, N); }
    // Get the string value
    template <size_t N>
    size_t get(std::array<char, N>& data) const noexcept { return get(data.data(), data.size()); }
    // Get the string value
    size_t get(FBEString& value) const noexcept;

    // Set the string value
    size_t set(const char* data, size_t size);
    // Set the string value
    template <size_t N>
    size_t set(const char (&data)[N]) { return set(data, N); }
    // Set the string value
    template <size_t N>
    size_t set(const std::array<char, N>& data) { return set(data.data(), data.size()); }
    // Set the string value
    size_t set(const FBEString& value);

private:
    FBEBuffer& _buffer;
    mutable size_t _offset;
};
)CODE";
  }

  static std::string GenerateFBEFinalModelString_Source() {
    return R"CODE(
size_t FinalModel<FBEString>::verify() const noexcept
{
    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    size_t buffer_size = _buffer.size();
    if ((fbe_full_offset + 4) > buffer_size)
        return std::numeric_limits<std::size_t>::max();

    uint32_t fbe_string_size = unaligned_load<uint32_t>(_buffer.data() + fbe_full_offset);
    if ((fbe_full_offset + 4 + fbe_string_size) > buffer_size)
        return std::numeric_limits<std::size_t>::max();

    return 4 + fbe_string_size;
}

size_t FinalModel<FBEString>::get(char* data, size_t size) const noexcept
{
    assert(((size == 0) || (data != nullptr)) && "Invalid buffer!");
    if ((size > 0) && (data == nullptr))
        return 0;

    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    const uint8_t* buffer_data = _buffer.data();
    size_t buffer_size = _buffer.size();

    assert(((fbe_full_offset + 4) <= buffer_size) && "Model is broken!");
    if ((fbe_full_offset + 4) > buffer_size)
        return 0;

    uint32_t fbe_string_size = unaligned_load<uint32_t>(buffer_data + fbe_full_offset);
    assert(((fbe_full_offset + 4 + fbe_string_size) <= buffer_size) && "Model is broken!");
    if ((fbe_full_offset + 4 + fbe_string_size) > buffer_size)
        return 4;

    size_t result = std::min(size, (size_t)fbe_string_size);
    memcpy(data, buffer_data + fbe_full_offset + 4, result);
    return 4 + fbe_string_size;
}

size_t FinalModel<FBEString>::get(FBEString& value) const noexcept
{
    #if !defined(USING_SEASTAR_STRING)
    value.clear();
    #endif

    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    const uint8_t* buffer_data = _buffer.data();
    size_t buffer_size = _buffer.size();

    assert(((fbe_full_offset + 4) <= buffer_size) && "Model is broken!");
    if ((fbe_full_offset + 4) > buffer_size)
        return 0;

    uint32_t fbe_string_size = unaligned_load<uint32_t>(buffer_data + fbe_full_offset);
    assert(((fbe_full_offset + 4 + fbe_string_size) <= buffer_size) && "Model is broken!");
    if ((fbe_full_offset + 4 + fbe_string_size) > buffer_size)
        return 4;
    #if !defined(USING_SEASTAR_STRING)
    value.assign((const char*)(buffer_data + fbe_full_offset + 4), fbe_string_size);
    #else
    value.resize(fbe_string_size);
    memcpy(value.data(), buffer_data + fbe_full_offset + 4, fbe_string_size);
    #endif
    return 4 + fbe_string_size;
}

size_t FinalModel<FBEString>::set(const char* data, size_t size)
{
    assert(((size == 0) || (data != nullptr)) && "Invalid buffer!");
    if ((size > 0) && (data == nullptr))
        return 0;

    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    uint8_t* buffer_data = _buffer.data();
    size_t buffer_size = _buffer.size();

    assert(((fbe_full_offset + 4) <= buffer_size) && "Model is broken!");
    if ((fbe_full_offset + 4) > buffer_size)
        return 0;

    uint32_t fbe_string_size = (uint32_t)size;
    assert(((fbe_full_offset + 4 + fbe_string_size) <= buffer_size) && "Model is broken!");
    if ((fbe_full_offset + 4 + fbe_string_size) > buffer_size)
        return 4;

    unaligned_store<uint32_t>(buffer_data + fbe_full_offset, fbe_string_size);

    if (fbe_string_size > 0)
        memcpy((char*)(buffer_data + fbe_full_offset + 4), data, fbe_string_size);
    return 4 + fbe_string_size;
}

size_t FinalModel<FBEString>::set(const FBEString& value)
{
    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    uint8_t* buffer_data = _buffer.data();
    size_t buffer_size = _buffer.size();

    assert(((fbe_full_offset + 4) <= buffer_size) && "Model is broken!");
    if ((fbe_full_offset + 4) > buffer_size)
        return 0;

    uint32_t fbe_string_size = (uint32_t)value.size();
    assert(((fbe_full_offset + 4 + fbe_string_size) <= buffer_size) && "Model is broken!");
    if ((fbe_full_offset + 4 + fbe_string_size) > buffer_size)
        return 4;

    unaligned_store<uint32_t>(buffer_data + fbe_full_offset, fbe_string_size);

    if (fbe_string_size > 0)
        memcpy((char*)(buffer_data + fbe_full_offset + 4), value.data(), fbe_string_size);
    return 4 + fbe_string_size;
}
)CODE";
  }

  static std::string GenerateFBEFinalModelArenaString_Header() {
    return R"CODE(
// Fast Binary Encoding final model ArenaString specialization
template <>
class FinalModel<ArenaString>
{
public:
    FinalModel(FBEBuffer& buffer, size_t offset) noexcept : _buffer(buffer), _offset(offset) {}

    // Get the allocation size
    size_t fbe_allocation_size(const char* data, size_t size) const noexcept { return 4 + size; }
    template <size_t N>
    size_t fbe_allocation_size(const char (&data)[N]) const noexcept { return 4 + N; }
    template <size_t N>
    size_t fbe_allocation_size(const std::array<char, N>& data) const noexcept { return 4 + N; }
    size_t fbe_allocation_size(const ArenaString& value) const noexcept { return 4 + value.size(); }

    // Get the field offset
    size_t fbe_offset() const noexcept { return _offset; }
    // Set the field offset
    size_t fbe_offset(size_t offset) const noexcept { return _offset = offset; }

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept { _offset -= size; }

    // Check if the string value is valid
    size_t verify() const noexcept;

    // Get the string value
    size_t get(char* data, size_t size) const noexcept;
    // Get the string value
    template <size_t N>
    size_t get(char (&data)[N]) const noexcept { return get(data, N); }
    // Get the string value
    template <size_t N>
    size_t get(std::array<char, N>& data) const noexcept { return get(data.data(), data.size()); }
    // Get the ArenaString value (with optional resource)
    size_t get(ArenaString& value) const noexcept { return get(value, nullptr); }
    size_t get(ArenaString& value, std::pmr::memory_resource* resource) const noexcept;

    // Set the string value
    size_t set(const char* data, size_t size);
    // Set the string value
    template <size_t N>
    size_t set(const char (&data)[N]) { return set(data, N); }
    // Set the string value
    template <size_t N>
    size_t set(const std::array<char, N>& data) { return set(data.data(), data.size()); }
    // Set the ArenaString value
    size_t set(const ArenaString& value);

private:
    FBEBuffer& _buffer;
    mutable size_t _offset;
};
)CODE";
  }

  static std::string GenerateFBEFinalModelArenaString_Source() {
    return R"CODE(
size_t FinalModel<ArenaString>::verify() const noexcept
{
    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    size_t buffer_size = _buffer.size();
    if ((fbe_full_offset + 4) > buffer_size)
        return std::numeric_limits<std::size_t>::max();

    uint32_t fbe_string_size = unaligned_load<uint32_t>(_buffer.data() + fbe_full_offset);
    if ((fbe_full_offset + 4 + fbe_string_size) > buffer_size)
        return std::numeric_limits<std::size_t>::max();

    return 4 + fbe_string_size;
}

size_t FinalModel<ArenaString>::get(char* data, size_t size) const noexcept
{
    assert(((size == 0) || (data != nullptr)) && "Invalid buffer!");
    if ((size > 0) && (data == nullptr))
        return 0;

    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    const uint8_t* buffer_data = _buffer.data();
    size_t buffer_size = _buffer.size();

    assert(((fbe_full_offset + 4) <= buffer_size) && "Model is broken!");
    if ((fbe_full_offset + 4) > buffer_size)
        return 0;

    uint32_t fbe_string_size = unaligned_load<uint32_t>(buffer_data + fbe_full_offset);
    assert(((fbe_full_offset + 4 + fbe_string_size) <= buffer_size) && "Model is broken!");
    if ((fbe_full_offset + 4 + fbe_string_size) > buffer_size)
        return 4;

    size_t result = std::min(size, (size_t)fbe_string_size);
    memcpy(data, buffer_data + fbe_full_offset + 4, result);
    return 4 + fbe_string_size;
}

size_t FinalModel<ArenaString>::get(ArenaString& value, std::pmr::memory_resource* resource) const noexcept
{
    value.clear();

    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    const uint8_t* buffer_data = _buffer.data();
    size_t buffer_size = _buffer.size();

    assert(((fbe_full_offset + 4) <= buffer_size) && "Model is broken!");
    if ((fbe_full_offset + 4) > buffer_size)
        return 0;

    uint32_t fbe_string_size = unaligned_load<uint32_t>(buffer_data + fbe_full_offset);
    assert(((fbe_full_offset + 4 + fbe_string_size) <= buffer_size) && "Model is broken!");
    if ((fbe_full_offset + 4 + fbe_string_size) > buffer_size)
        return 4;

    value.assign((const char*)(buffer_data + fbe_full_offset + 4), fbe_string_size);
    return 4 + fbe_string_size;
}

size_t FinalModel<ArenaString>::set(const char* data, size_t size)
{
    assert(((size == 0) || (data != nullptr)) && "Invalid buffer!");
    if ((size > 0) && (data == nullptr))
        return 0;

    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    uint8_t* buffer_data = _buffer.data();
    size_t buffer_size = _buffer.size();

    assert(((fbe_full_offset + 4) <= buffer_size) && "Model is broken!");
    if ((fbe_full_offset + 4) > buffer_size)
        return 0;

    uint32_t fbe_string_size = (uint32_t)size;
    assert(((fbe_full_offset + 4 + fbe_string_size) <= buffer_size) && "Model is broken!");
    if ((fbe_full_offset + 4 + fbe_string_size) > buffer_size)
        return 4;

    unaligned_store<uint32_t>(buffer_data + fbe_full_offset, fbe_string_size);

    if (fbe_string_size > 0)
        memcpy((char*)(buffer_data + fbe_full_offset + 4), data, fbe_string_size);
    return 4 + fbe_string_size;
}

size_t FinalModel<ArenaString>::set(const ArenaString& value)
{
    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    uint8_t* buffer_data = _buffer.data();
    size_t buffer_size = _buffer.size();

    assert(((fbe_full_offset + 4) <= buffer_size) && "Model is broken!");
    if ((fbe_full_offset + 4) > buffer_size)
        return 0;

    uint32_t fbe_string_size = (uint32_t)value.size();
    assert(((fbe_full_offset + 4 + fbe_string_size) <= buffer_size) && "Model is broken!");
    if ((fbe_full_offset + 4 + fbe_string_size) > buffer_size)
        return 4;

    unaligned_store<uint32_t>(buffer_data + fbe_full_offset, fbe_string_size);

    if (fbe_string_size > 0)
        memcpy((char*)(buffer_data + fbe_full_offset + 4), value.data(), fbe_string_size);
    return 4 + fbe_string_size;
}
)CODE";
  }

  static std::string GenerateFBEFinalModelOptional_Header() {
    return R"CODE(
// Fast Binary Encoding final model optional specialization
template <typename T>
class FinalModel<std::optional<T>>
{
public:
    FinalModel(FBEBuffer& buffer, size_t offset) noexcept : _buffer(buffer), _offset(offset), value(buffer, 0) {}

    // Get the allocation size
    size_t fbe_allocation_size(const std::optional<T>& opt) const noexcept { return 1 + (opt ? value.fbe_allocation_size(opt.value()) : 0); }

    // Get the field offset
    size_t fbe_offset() const noexcept { return _offset; }
    // Set the field offset
    size_t fbe_offset(size_t offset) const noexcept { return _offset = offset; }

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept { _offset -= size; }

    //! Is the value present?
    explicit operator bool() const noexcept { return has_value(); }

    // Checks if the object contains a value
    bool has_value() const noexcept;

    // Check if the optional value is valid
    size_t verify() const noexcept;

    // Get the optional value
    size_t get(std::optional<T>& opt) const noexcept;
    // Set the optional value
    size_t set(const std::optional<T>& opt);

private:
    FBEBuffer& _buffer;
    mutable size_t _offset;

public:
    // Base final model value
    FinalModel<T> value;
};
)CODE";
  }

  static std::string GenerateFBEFinalModelOptional_Inline() {
    return R"CODE(
template <typename T>
inline bool FinalModel<std::optional<T>>::has_value() const noexcept
{
    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    if ((fbe_full_offset + 1) > _buffer.size())
        return false;

    uint8_t fbe_has_value = *((const uint8_t*)(_buffer.data() + fbe_full_offset));
    return (fbe_has_value != 0);
}

template <typename T>
inline size_t FinalModel<std::optional<T>>::verify() const noexcept
{
    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    if ((fbe_full_offset + 1) > _buffer.size())
        return std::numeric_limits<std::size_t>::max();

    uint8_t fbe_has_value = *((const uint8_t*)(_buffer.data() + fbe_full_offset));
    if (fbe_has_value == 0)
        return 1;

    _buffer.shift(fbe_offset() + 1);
    size_t fbe_result = value.verify();
    _buffer.unshift(fbe_offset() + 1);
    return 1 + fbe_result;
}

template <typename T>
inline size_t FinalModel<std::optional<T>>::get(std::optional<T>& opt) const noexcept
{
    opt = std::nullopt;

    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    assert(((fbe_full_offset + 1) <= _buffer.size()) && "Model is broken!");
    if ((fbe_full_offset + 1) > _buffer.size())
        return 0;

    uint8_t fbe_has_value = *((const uint8_t*)(_buffer.data() + fbe_full_offset));
    if (fbe_has_value == 0)
        return 1;

    _buffer.shift(fbe_offset() + 1);
    T temp = T();
    size_t size = value.get(temp);
    opt.emplace(temp);
    _buffer.unshift(fbe_offset() + 1);
    return 1 + size;
}

template <typename T>
inline size_t FinalModel<std::optional<T>>::set(const std::optional<T>& opt)
{
    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    assert(((fbe_full_offset + 1) <= _buffer.size()) && "Model is broken!");
    if ((fbe_full_offset + 1) > _buffer.size())
        return 0;

    uint8_t fbe_has_value = opt ? 1 : 0;
    *((uint8_t*)(_buffer.data() + fbe_full_offset)) = fbe_has_value;
    if (fbe_has_value == 0)
        return 1;

    _buffer.shift(fbe_offset() + 1);
    size_t size = 0;
    if (opt)
        size = value.set(opt.value());
    _buffer.unshift(fbe_offset() + 1);
    return 1 + size;
}
)CODE";
  }

  static std::string GenerateFBEFinalModelArray_Header() {
    return R"CODE(
// Fast Binary Encoding final model array
template <typename T, size_t N>
class FinalModelArray
{
public:
    FinalModelArray(FBEBuffer& buffer, size_t offset) noexcept : _buffer(buffer), _offset(offset) {}

    // Get the allocation size
    template <size_t S>
    size_t fbe_allocation_size(const T (&values)[S]) const noexcept;
    template <size_t S>
    size_t fbe_allocation_size(const std::array<T, S>& values) const noexcept;
    size_t fbe_allocation_size(const FastVec<T>& values) const noexcept;

    // Get the field offset
    size_t fbe_offset() const noexcept { return _offset; }
    // Set the field offset
    size_t fbe_offset(size_t offset) const noexcept { return _offset = offset; }

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept { _offset -= size; }

    // Check if the array is valid
    size_t verify() const noexcept;

    // Get the array as C-array
    template <size_t S>
    size_t get(T (&values)[S]) const noexcept;
    // Get the array as std::array
    template <size_t S>
    size_t get(std::array<T, S>& values) const noexcept;
    // Get the array as FastVec
    size_t get(FastVec<T>& values) const noexcept;

    // Set the array as C-array
    template <size_t S>
    size_t set(const T (&values)[S]) noexcept;
    // Set the array as std::array
    template <size_t S>
    size_t set(const std::array<T, S>& values) noexcept;
    // Set the array as FastVec
    size_t set(const FastVec<T>& values) noexcept;

private:
    FBEBuffer& _buffer;
    mutable size_t _offset;
};
)CODE";
  }

  static std::string GenerateFBEFinalModelArray_Inline() {
    return R"CODE(
template <typename T, size_t N>
template <size_t S>
inline size_t FinalModelArray<T, N>::fbe_allocation_size(const T (&values)[S]) const noexcept
{
    constexpr size_t count = (S < N) ? S : N;
    if constexpr (is_fbe_final_primitive_v<T>) {
        return count * sizeof(T);
    } else {
        size_t size = 0;
        FinalModel<T> fbe_model(_buffer, fbe_offset());
        for (size_t i = 0; i < count; ++i)
            size += fbe_model.fbe_allocation_size(values[i]);
        return size;
    }
}

template <typename T, size_t N>
template <size_t S>
inline size_t FinalModelArray<T, N>::fbe_allocation_size(const std::array<T, S>& values) const noexcept
{
    constexpr size_t count = (S < N) ? S : N;
    if constexpr (is_fbe_final_primitive_v<T>) {
        return count * sizeof(T);
    } else {
        size_t size = 0;
        FinalModel<T> fbe_model(_buffer, fbe_offset());
        for (size_t i = 0; i < count; ++i)
            size += fbe_model.fbe_allocation_size(values[i]);
        return size;
    }
}

template <typename T, size_t N>
inline size_t FinalModelArray<T, N>::fbe_allocation_size(const FastVec<T>& values) const noexcept
{
    const size_t count = std::min(values.size(), N);
    if constexpr (is_fbe_final_primitive_v<T>) {
        return count * sizeof(T);
    } else {
        size_t size = 0;
        FinalModel<T> fbe_model(_buffer, fbe_offset());
        for (size_t i = 0; i < count; ++i)
            size += fbe_model.fbe_allocation_size(values[i]);
        return size;
    }
}

template <typename T, size_t N>
inline size_t FinalModelArray<T, N>::verify() const noexcept
{
    if ((_buffer.offset() + fbe_offset()) > _buffer.size())
        return std::numeric_limits<std::size_t>::max();

    if constexpr (is_fbe_final_primitive_v<T>) {
        // Bulk verification for primitive types - just check total size
        constexpr size_t total_size = N * sizeof(T);
        if ((_buffer.offset() + fbe_offset() + total_size) > _buffer.size())
            return std::numeric_limits<std::size_t>::max();
        return total_size;
    } else {
        size_t size = 0;
        FinalModel<T> fbe_model(_buffer, fbe_offset());
        for (size_t i = 0; i < N; ++i)
        {
            size_t offset = fbe_model.verify();
            if (offset == std::numeric_limits<std::size_t>::max())
                return std::numeric_limits<std::size_t>::max();
            fbe_model.fbe_shift(offset);
            size += offset;
        }
        return size;
    }
}

template <typename T, size_t N>
template <size_t S>
inline size_t FinalModelArray<T, N>::get(T (&values)[S]) const noexcept
{
    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    assert((fbe_full_offset <= _buffer.size()) && "Model is broken!");
    if (fbe_full_offset > _buffer.size())
        return 0;

    constexpr size_t count = (S < N) ? S : N;

    if constexpr (is_fbe_final_primitive_v<T>) {
        // Bulk copy for primitive types
        constexpr size_t total_size = count * sizeof(T);
        memcpy(values, _buffer.data() + fbe_full_offset, total_size);
        return total_size;
    } else {
        size_t size = 0;
        FinalModel<T> fbe_model(_buffer, fbe_offset());
        for (size_t i = 0; i < count; ++i)
        {
            size_t offset = fbe_model.get(values[i]);
            fbe_model.fbe_shift(offset);
            size += offset;
        }
        return size;
    }
}

template <typename T, size_t N>
template <size_t S>
inline size_t FinalModelArray<T, N>::get(std::array<T, S>& values) const noexcept
{
    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    assert((fbe_full_offset <= _buffer.size()) && "Model is broken!");
    if (fbe_full_offset > _buffer.size())
        return 0;

    constexpr size_t count = (S < N) ? S : N;

    if constexpr (is_fbe_final_primitive_v<T>) {
        // Bulk copy for primitive types
        constexpr size_t total_size = count * sizeof(T);
        memcpy(values.data(), _buffer.data() + fbe_full_offset, total_size);
        return total_size;
    } else {
        size_t size = 0;
        FinalModel<T> fbe_model(_buffer, fbe_offset());
        for (size_t i = 0; i < count; ++i)
        {
            size_t offset = fbe_model.get(values[i]);
            fbe_model.fbe_shift(offset);
            size += offset;
        }
        return size;
    }
}

template <typename T, size_t N>
inline size_t FinalModelArray<T, N>::get(FastVec<T>& values) const noexcept
{
    values.clear();

    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    assert((fbe_full_offset <= _buffer.size()) && "Model is broken!");
    if (fbe_full_offset > _buffer.size())
        return 0;

    values.reserve(N);

    if constexpr (is_fbe_final_primitive_v<T>) {
        // Bulk copy for primitive types
        constexpr size_t total_size = N * sizeof(T);
        values.resize(N);
        memcpy(values.data(), _buffer.data() + fbe_full_offset, total_size);
        return total_size;
    } else {
        size_t size = 0;
        FinalModel<T> fbe_model(_buffer, fbe_offset());
        for (size_t i = 0; i < N; ++i)
        {
            T value{};
            size_t offset = fbe_model.get(value);
            #if defined(USING_STD_VECTOR)
            values.emplace_back(std::move(value));
            #else
            values.template emplace_back(std::move(value));
            #endif
            fbe_model.fbe_shift(offset);
            size += offset;
        }
        return size;
    }
}

template <typename T, size_t N>
template <size_t S>
inline size_t FinalModelArray<T, N>::set(const T (&values)[S]) noexcept
{
    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    assert((fbe_full_offset <= _buffer.size()) && "Model is broken!");
    if (fbe_full_offset > _buffer.size())
        return 0;

    constexpr size_t count = (S < N) ? S : N;

    if constexpr (is_fbe_final_primitive_v<T>) {
        // Bulk copy for primitive types
        constexpr size_t total_size = count * sizeof(T);
        memcpy(_buffer.data() + fbe_full_offset, values, total_size);
        return total_size;
    } else {
        size_t size = 0;
        FinalModel<T> fbe_model(_buffer, fbe_offset());
        for (size_t i = 0; i < count; ++i)
        {
            size_t offset = fbe_model.set(values[i]);
            fbe_model.fbe_shift(offset);
            size += offset;
        }
        return size;
    }
}

template <typename T, size_t N>
template <size_t S>
inline size_t FinalModelArray<T, N>::set(const std::array<T, S>& values) noexcept
{
    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    assert((fbe_full_offset <= _buffer.size()) && "Model is broken!");
    if (fbe_full_offset > _buffer.size())
        return 0;

    constexpr size_t count = (S < N) ? S : N;

    if constexpr (is_fbe_final_primitive_v<T>) {
        // Bulk copy for primitive types
        constexpr size_t total_size = count * sizeof(T);
        memcpy(_buffer.data() + fbe_full_offset, values.data(), total_size);
        return total_size;
    } else {
        size_t size = 0;
        FinalModel<T> fbe_model(_buffer, fbe_offset());
        for (size_t i = 0; i < count; ++i)
        {
            size_t offset = fbe_model.set(values[i]);
            fbe_model.fbe_shift(offset);
            size += offset;
        }
        return size;
    }
}

template <typename T, size_t N>
inline size_t FinalModelArray<T, N>::set(const FastVec<T>& values) noexcept
{
    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    assert((fbe_full_offset <= _buffer.size()) && "Model is broken!");
    if (fbe_full_offset > _buffer.size())
        return 0;

    const size_t count = std::min(values.size(), N);

    if constexpr (is_fbe_final_primitive_v<T>) {
        // Bulk copy for primitive types
        const size_t total_size = count * sizeof(T);
        memcpy(_buffer.data() + fbe_full_offset, values.data(), total_size);
        return total_size;
    } else {
        size_t size = 0;
        FinalModel<T> fbe_model(_buffer, fbe_offset());
        for (size_t i = 0; i < count; ++i)
        {
            size_t offset = fbe_model.set(values[i]);
            fbe_model.fbe_shift(offset);
            size += offset;
        }
        return size;
    }
}
)CODE";
  }

  static std::string GenerateFBEFinalModelVector_Header() {
    return R"CODE(
// Fast Binary Encoding final model vector
template <typename T>
class FinalModelVector
{
public:
    FinalModelVector(FBEBuffer& buffer, size_t offset) noexcept : _buffer(buffer), _offset(offset) {}

    // Get the allocation size
    size_t fbe_allocation_size(const FastVec<T>& values) const noexcept;
    size_t fbe_allocation_size(const std::list<T>& values) const noexcept;
    size_t fbe_allocation_size(const std::set<T>& values) const noexcept;

    // Get the field offset
    size_t fbe_offset() const noexcept { return _offset; }
    // Set the field offset
    size_t fbe_offset(size_t offset) const noexcept { return _offset = offset; }

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept { _offset -= size; }

    // Check if the vector is valid
    size_t verify() const noexcept;

    // Get the vector as FastVec
    size_t get(FastVec<T>& values) const noexcept;
    // Get the vector as std::list
    size_t get(std::list<T>& values) const noexcept;
    // Get the vector as std::set
    size_t get(std::set<T>& values) const noexcept;

    // Get the vector as FastVec with memory resource
    size_t get(FastVec<T>& values, std::pmr::memory_resource* resource) const noexcept;
    // Get the vector as std::pmr::vector (with optional resource)
    size_t get(std::pmr::vector<T>& values) const noexcept { return get(values, nullptr); }
    size_t get(std::pmr::vector<T>& values, std::pmr::memory_resource* resource) const noexcept;
    // Get the vector as std::pmr::list (with optional resource)
    size_t get(std::pmr::list<T>& values) const noexcept { return get(values, nullptr); }
    size_t get(std::pmr::list<T>& values, std::pmr::memory_resource* resource) const noexcept;
    // Get the vector as std::pmr::set (with optional resource)
    size_t get(std::pmr::set<T>& values) const noexcept { return get(values, nullptr); }
    size_t get(std::pmr::set<T>& values, std::pmr::memory_resource* resource) const noexcept;

    // Set the vector as FastVec
    size_t set(const FastVec<T>& values) noexcept;
    // Set the vector as std::list
    size_t set(const std::list<T>& values) noexcept;
    // Set the vector as std::set
    size_t set(const std::set<T>& values) noexcept;

    // Set the vector as std::pmr::vector
    size_t set(const std::pmr::vector<T>& values) noexcept;
    // Set the vector as std::pmr::list
    size_t set(const std::pmr::list<T>& values) noexcept;
    // Set the vector as std::pmr::set
    size_t set(const std::pmr::set<T>& values) noexcept;

    // Get the allocation size for std::pmr::vector
    size_t fbe_allocation_size(const std::pmr::vector<T>& values) const noexcept;
    // Get the allocation size for std::pmr::list
    size_t fbe_allocation_size(const std::pmr::list<T>& values) const noexcept;
    // Get the allocation size for std::pmr::set
    size_t fbe_allocation_size(const std::pmr::set<T>& values) const noexcept;

#if defined(USING_BTREE_MAP)
    // Get the allocation size for FBE::set (btree_set)
    size_t fbe_allocation_size(const FBE::set<T>& values) const noexcept;
    // Get the allocation size for FBE::pmr::set (btree_set with pmr allocator)
    size_t fbe_allocation_size(const FBE::pmr::set<T>& values) const noexcept;
    // Get the vector as FBE::set (btree_set)
    size_t get(FBE::set<T>& values) const noexcept;
    // Get the vector as FBE::pmr::set (btree_set with pmr allocator, optional resource)
    size_t get(FBE::pmr::set<T>& values) const noexcept { return get(values, nullptr); }
    size_t get(FBE::pmr::set<T>& values, std::pmr::memory_resource* resource) const noexcept;
    // Set the vector as FBE::set (btree_set)
    size_t set(const FBE::set<T>& values) noexcept;
    // Set the vector as FBE::pmr::set (btree_set with pmr allocator)
    size_t set(const FBE::pmr::set<T>& values) noexcept;
#endif

private:
    FBEBuffer& _buffer;
    mutable size_t _offset;
};
)CODE";
  }

  static std::string GenerateFBEFinalModelVector_Inline() {
    return R"CODE(
template <typename T>
inline size_t FinalModelVector<T>::fbe_allocation_size(const FastVec<T>& values) const noexcept
{
    if constexpr (is_fbe_final_primitive_v<T>) {
        // Fixed size for primitive types
        return 4 + values.size() * sizeof(T);
    } else {
        size_t size = 4;
        FinalModel<T> fbe_model(_buffer, fbe_offset() + 4);
        for (const auto& value : values)
            size += fbe_model.fbe_allocation_size(value);
        return size;
    }
}

template <typename T>
inline size_t FinalModelVector<T>::fbe_allocation_size(const std::list<T>& values) const noexcept
{
    size_t size = 4;
    FinalModel<T> fbe_model(_buffer, fbe_offset() + 4);
    for (const auto& value : values)
        size += fbe_model.fbe_allocation_size(value);
    return size;
}

template <typename T>
inline size_t FinalModelVector<T>::fbe_allocation_size(const std::set<T>& values) const noexcept
{
    size_t size = 4;
    FinalModel<T> fbe_model(_buffer, fbe_offset() + 4);
    for (const auto& value : values)
        size += fbe_model.fbe_allocation_size(value);
    return size;
}

template <typename T>
inline size_t FinalModelVector<T>::verify() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + 4) > _buffer.size())
        return std::numeric_limits<std::size_t>::max();

    uint32_t fbe_vector_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());

    if constexpr (is_fbe_final_primitive_v<T>) {
        // Bulk verification for primitive types - just check total size
        size_t total_size = 4 + fbe_vector_size * sizeof(T);
        if ((_buffer.offset() + fbe_offset() + total_size) > _buffer.size())
            return std::numeric_limits<std::size_t>::max();
        return total_size;
    } else {
        size_t size = 4;
        FinalModel<T> fbe_model(_buffer, fbe_offset() + 4);
        for (size_t i = 0; i < fbe_vector_size; ++i)
        {
            size_t offset = fbe_model.verify();
            if (offset == std::numeric_limits<std::size_t>::max())
                return std::numeric_limits<std::size_t>::max();
            fbe_model.fbe_shift(offset);
            size += offset;
        }
        return size;
    }
}

template <typename T>
inline size_t FinalModelVector<T>::get(FastVec<T>& values) const noexcept
{
    values.clear();

    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    assert(((fbe_full_offset + 4) <= _buffer.size()) && "Model is broken!");
    if ((fbe_full_offset + 4) > _buffer.size())
        return 0;

    size_t fbe_vector_size = unaligned_load<uint32_t>(_buffer.data() + fbe_full_offset);
    if (fbe_vector_size == 0)
        return 4;

    if constexpr (is_fbe_final_primitive_v<T>) {
        // Bulk copy for primitive types
        const size_t data_size = fbe_vector_size * sizeof(T);
        values.resize(fbe_vector_size);
        memcpy(values.data(), _buffer.data() + fbe_full_offset + 4, data_size);
        return 4 + data_size;
    } else if constexpr (std::is_same_v<T, bool>) {
        // Special case for vector<bool> - uses proxy objects, can't pass by reference
        values.resize(fbe_vector_size);
        size_t size = 4;
        FinalModel<T> fbe_model(_buffer, fbe_offset() + 4);
        for (size_t i = 0; i < fbe_vector_size; ++i)
        {
            T temp{};
            size_t offset = fbe_model.get(temp);
            values[i] = temp;
            fbe_model.fbe_shift(offset);
            size += offset;
        }
        return size;
    } else {
        // Pre-allocate and deserialize directly into elements
        values.resize(fbe_vector_size);
        size_t size = 4;
        FinalModel<T> fbe_model(_buffer, fbe_offset() + 4);
        for (size_t i = 0; i < fbe_vector_size; ++i)
        {
            size_t offset = fbe_model.get(values[i]);
            fbe_model.fbe_shift(offset);
            size += offset;
        }
        return size;
    }
}

template <typename T>
inline size_t FinalModelVector<T>::get(std::list<T>& values) const noexcept
{
    values.clear();

    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    assert(((fbe_full_offset + 4) <= _buffer.size()) && "Model is broken!");
    if ((fbe_full_offset + 4) > _buffer.size())
        return 0;

    size_t fbe_vector_size = unaligned_load<uint32_t>(_buffer.data() + fbe_full_offset);
    if (fbe_vector_size == 0)
        return 4;

    size_t size = 4;
    FinalModel<T> fbe_model(_buffer, fbe_offset() + 4);
    for (size_t i = 0; i < fbe_vector_size; ++i)
    {
        T value{};
        size_t offset = fbe_model.get(value);
        values.emplace_back(std::move(value));
        fbe_model.fbe_shift(offset);
        size += offset;
    }
    return size;
}

template <typename T>
inline size_t FinalModelVector<T>::get(std::set<T>& values) const noexcept
{
    values.clear();

    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    assert(((fbe_full_offset + 4) <= _buffer.size()) && "Model is broken!");
    if ((fbe_full_offset + 4) > _buffer.size())
        return 0;

    size_t fbe_vector_size = unaligned_load<uint32_t>(_buffer.data() + fbe_full_offset);
    if (fbe_vector_size == 0)
        return 4;

    size_t size = 4;
    FinalModel<T> fbe_model(_buffer, fbe_offset() + 4);
    // Use hint-based insertion for O(1) amortized insertion (data is already sorted)
    auto hint = values.end();
    for (size_t i = 0; i < fbe_vector_size; ++i)
    {
        T value{};
        size_t offset = fbe_model.get(value);
        hint = values.emplace_hint(hint, std::move(value));
        fbe_model.fbe_shift(offset);
        size += offset;
    }
    return size;
}

template <typename T>
inline size_t FinalModelVector<T>::set(const FastVec<T>& values) noexcept
{
    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    assert(((fbe_full_offset + 4) <= _buffer.size()) && "Model is broken!");
    if ((fbe_full_offset + 4) > _buffer.size())
        return 0;

    unaligned_store<uint32_t>(_buffer.data() + fbe_full_offset, (uint32_t)values.size());

    if constexpr (is_fbe_final_primitive_v<T>) {
        // Bulk copy for primitive types
        const size_t data_size = values.size() * sizeof(T);
        memcpy(_buffer.data() + fbe_full_offset + 4, values.data(), data_size);
        return 4 + data_size;
    } else {
        size_t size = 4;
        FinalModel<T> fbe_model(_buffer, fbe_offset() + 4);
        for (const auto& value : values)
        {
            size_t offset = fbe_model.set(value);
            fbe_model.fbe_shift(offset);
            size += offset;
        }
        return size;
    }
}

template <typename T>
inline size_t FinalModelVector<T>::set(const std::list<T>& values) noexcept
{
    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    assert(((fbe_full_offset + 4) <= _buffer.size()) && "Model is broken!");
    if ((fbe_full_offset + 4) > _buffer.size())
        return 0;

    unaligned_store<uint32_t>(_buffer.data() + fbe_full_offset, (uint32_t)values.size());

    size_t size = 4;
    FinalModel<T> fbe_model(_buffer, fbe_offset() + 4);
    for (const auto& value : values)
    {
        size_t offset = fbe_model.set(value);
        fbe_model.fbe_shift(offset);
        size += offset;
    }
    return size;
}

template <typename T>
inline size_t FinalModelVector<T>::set(const std::set<T>& values) noexcept
{
    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    assert(((fbe_full_offset + 4) <= _buffer.size()) && "Model is broken!");
    if ((fbe_full_offset + 4) > _buffer.size())
        return 0;

    unaligned_store<uint32_t>(_buffer.data() + fbe_full_offset, (uint32_t)values.size());

    size_t size = 4;
    FinalModel<T> fbe_model(_buffer, fbe_offset() + 4);
    for (const auto& value : values)
    {
        size_t offset = fbe_model.set(value);
        fbe_model.fbe_shift(offset);
        size += offset;
    }
    return size;
}

// PMR allocation size methods
template <typename T>
inline size_t FinalModelVector<T>::fbe_allocation_size(const std::pmr::vector<T>& values) const noexcept
{
    if constexpr (is_fbe_final_primitive_v<T>) {
        return 4 + values.size() * sizeof(T);
    } else {
        size_t size = 4;
        FinalModel<T> fbe_model(_buffer, fbe_offset() + 4);
        for (const auto& value : values)
            size += fbe_model.fbe_allocation_size(value);
        return size;
    }
}

template <typename T>
inline size_t FinalModelVector<T>::fbe_allocation_size(const std::pmr::list<T>& values) const noexcept
{
    size_t size = 4;
    FinalModel<T> fbe_model(_buffer, fbe_offset() + 4);
    for (const auto& value : values)
        size += fbe_model.fbe_allocation_size(value);
    return size;
}

template <typename T>
inline size_t FinalModelVector<T>::fbe_allocation_size(const std::pmr::set<T>& values) const noexcept
{
    size_t size = 4;
    FinalModel<T> fbe_model(_buffer, fbe_offset() + 4);
    for (const auto& value : values)
        size += fbe_model.fbe_allocation_size(value);
    return size;
}

// PMR get methods with memory_resource
template <typename T>
inline size_t FinalModelVector<T>::get(FastVec<T>& values, [[maybe_unused]] std::pmr::memory_resource* resource) const noexcept
{
    // For non-PMR container, just delegate to the regular get
    return get(values);
}

template <typename T>
inline size_t FinalModelVector<T>::get(std::pmr::vector<T>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();

    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    assert(((fbe_full_offset + 4) <= _buffer.size()) && "Model is broken!");
    if ((fbe_full_offset + 4) > _buffer.size())
        return 0;

    size_t fbe_vector_size = unaligned_load<uint32_t>(_buffer.data() + fbe_full_offset);
    if (fbe_vector_size == 0)
        return 4;

    if constexpr (is_fbe_final_primitive_v<T>) {
        const size_t data_size = fbe_vector_size * sizeof(T);
        values.resize(fbe_vector_size);
        memcpy(values.data(), _buffer.data() + fbe_full_offset + 4, data_size);
        return 4 + data_size;
    } else {
        // Pre-allocate and deserialize directly into elements
        values.resize(fbe_vector_size);
        size_t size = 4;
        FinalModel<T> fbe_model(_buffer, fbe_offset() + 4);
        for (size_t i = 0; i < fbe_vector_size; ++i)
        {
            size_t offset = fbe_model.get(values[i], resource);
            fbe_model.fbe_shift(offset);
            size += offset;
        }
        return size;
    }
}

template <typename T>
inline size_t FinalModelVector<T>::get(std::pmr::list<T>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();

    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    assert(((fbe_full_offset + 4) <= _buffer.size()) && "Model is broken!");
    if ((fbe_full_offset + 4) > _buffer.size())
        return 0;

    size_t fbe_vector_size = unaligned_load<uint32_t>(_buffer.data() + fbe_full_offset);
    if (fbe_vector_size == 0)
        return 4;

    // Pre-allocate and deserialize directly into elements
    values.resize(fbe_vector_size);
    size_t size = 4;
    FinalModel<T> fbe_model(_buffer, fbe_offset() + 4);
    auto it = values.begin();
    for (size_t i = 0; i < fbe_vector_size; ++i, ++it)
    {
        size_t offset = fbe_model.get(*it, resource);
        fbe_model.fbe_shift(offset);
        size += offset;
    }
    return size;
}

template <typename T>
inline size_t FinalModelVector<T>::get(std::pmr::set<T>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();

    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    assert(((fbe_full_offset + 4) <= _buffer.size()) && "Model is broken!");
    if ((fbe_full_offset + 4) > _buffer.size())
        return 0;

    size_t fbe_vector_size = unaligned_load<uint32_t>(_buffer.data() + fbe_full_offset);
    if (fbe_vector_size == 0)
        return 4;

    size_t size = 4;
    FinalModel<T> fbe_model(_buffer, fbe_offset() + 4);
    auto hint = values.end();
    for (size_t i = 0; i < fbe_vector_size; ++i)
    {
        T value{};
        if constexpr (std::is_constructible_v<T, std::pmr::memory_resource*>) {
            value = T(resource);
        }
        size_t offset = fbe_model.get(value, resource);
        hint = values.emplace_hint(hint, std::move(value));
        fbe_model.fbe_shift(offset);
        size += offset;
    }
    return size;
}

// PMR set methods
template <typename T>
inline size_t FinalModelVector<T>::set(const std::pmr::vector<T>& values) noexcept
{
    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    assert(((fbe_full_offset + 4) <= _buffer.size()) && "Model is broken!");
    if ((fbe_full_offset + 4) > _buffer.size())
        return 0;

    unaligned_store<uint32_t>(_buffer.data() + fbe_full_offset, (uint32_t)values.size());

    if constexpr (is_fbe_final_primitive_v<T>) {
        const size_t data_size = values.size() * sizeof(T);
        memcpy(_buffer.data() + fbe_full_offset + 4, values.data(), data_size);
        return 4 + data_size;
    } else {
        size_t size = 4;
        FinalModel<T> fbe_model(_buffer, fbe_offset() + 4);
        for (const auto& value : values)
        {
            size_t offset = fbe_model.set(value);
            fbe_model.fbe_shift(offset);
            size += offset;
        }
        return size;
    }
}

template <typename T>
inline size_t FinalModelVector<T>::set(const std::pmr::list<T>& values) noexcept
{
    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    assert(((fbe_full_offset + 4) <= _buffer.size()) && "Model is broken!");
    if ((fbe_full_offset + 4) > _buffer.size())
        return 0;

    unaligned_store<uint32_t>(_buffer.data() + fbe_full_offset, (uint32_t)values.size());

    size_t size = 4;
    FinalModel<T> fbe_model(_buffer, fbe_offset() + 4);
    for (const auto& value : values)
    {
        size_t offset = fbe_model.set(value);
        fbe_model.fbe_shift(offset);
        size += offset;
    }
    return size;
}

template <typename T>
inline size_t FinalModelVector<T>::set(const std::pmr::set<T>& values) noexcept
{
    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    assert(((fbe_full_offset + 4) <= _buffer.size()) && "Model is broken!");
    if ((fbe_full_offset + 4) > _buffer.size())
        return 0;

    unaligned_store<uint32_t>(_buffer.data() + fbe_full_offset, (uint32_t)values.size());

    size_t size = 4;
    FinalModel<T> fbe_model(_buffer, fbe_offset() + 4);
    for (const auto& value : values)
    {
        size_t offset = fbe_model.set(value);
        fbe_model.fbe_shift(offset);
        size += offset;
    }
    return size;
}

#if defined(USING_BTREE_MAP)
template <typename T>
inline size_t FinalModelVector<T>::fbe_allocation_size(const FBE::set<T>& values) const noexcept
{
    size_t size = 4;
    FinalModel<T> fbe_model(_buffer, fbe_offset() + 4);
    for (const auto& value : values)
        size += fbe_model.fbe_allocation_size(value);
    return size;
}

template <typename T>
inline size_t FinalModelVector<T>::get(FBE::set<T>& values) const noexcept
{
    values.clear();

    assert(((_buffer.offset() + fbe_offset() + 4) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + 4) > _buffer.size())
        return 0;

    uint32_t fbe_vector_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());

    size_t size = 4;
    FinalModel<T> fbe_model(_buffer, fbe_offset() + 4);
    // Use hint-based insertion for O(1) amortized insertion (data is already sorted)
    auto hint = values.end();
    for (size_t i = 0; i < fbe_vector_size; ++i)
    {
        T value = T();
        size_t offset = fbe_model.get(value);
        hint = values.emplace_hint(hint, std::move(value));
        fbe_model.fbe_shift(offset);
        size += offset;
    }
    return size;
}

template <typename T>
inline size_t FinalModelVector<T>::set(const FBE::set<T>& values) noexcept
{
    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    assert(((fbe_full_offset + 4) <= _buffer.size()) && "Model is broken!");
    if ((fbe_full_offset + 4) > _buffer.size())
        return 0;

    unaligned_store<uint32_t>(_buffer.data() + fbe_full_offset, (uint32_t)values.size());

    size_t size = 4;
    FinalModel<T> fbe_model(_buffer, fbe_offset() + 4);
    for (const auto& value : values)
    {
        size_t offset = fbe_model.set(value);
        fbe_model.fbe_shift(offset);
        size += offset;
    }
    return size;
}

// FBE::pmr::set PMR methods
template <typename T>
inline size_t FinalModelVector<T>::fbe_allocation_size(const FBE::pmr::set<T>& values) const noexcept
{
    size_t size = 4;
    FinalModel<T> fbe_model(_buffer, fbe_offset() + 4);
    for (const auto& value : values)
        size += fbe_model.fbe_allocation_size(value);
    return size;
}

template <typename T>
inline size_t FinalModelVector<T>::get(FBE::pmr::set<T>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();

    assert(((_buffer.offset() + fbe_offset() + 4) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + 4) > _buffer.size())
        return 0;

    uint32_t fbe_vector_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());

    size_t size = 4;
    FinalModel<T> fbe_model(_buffer, fbe_offset() + 4);
    auto hint = values.end();
    for (size_t i = 0; i < fbe_vector_size; ++i)
    {
        T value{};
        if constexpr (std::is_constructible_v<T, std::pmr::memory_resource*>) {
            value = T(resource);
        }
        size_t offset = fbe_model.get(value, resource);
        hint = values.emplace_hint(hint, std::move(value));
        fbe_model.fbe_shift(offset);
        size += offset;
    }
    return size;
}

template <typename T>
inline size_t FinalModelVector<T>::set(const FBE::pmr::set<T>& values) noexcept
{
    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    assert(((fbe_full_offset + 4) <= _buffer.size()) && "Model is broken!");
    if ((fbe_full_offset + 4) > _buffer.size())
        return 0;

    unaligned_store<uint32_t>(_buffer.data() + fbe_full_offset, (uint32_t)values.size());

    size_t size = 4;
    FinalModel<T> fbe_model(_buffer, fbe_offset() + 4);
    for (const auto& value : values)
    {
        size_t offset = fbe_model.set(value);
        fbe_model.fbe_shift(offset);
        size += offset;
    }
    return size;
}
#endif
)CODE";
  }

  static std::string GenerateFBEFinalModelMap_Header() {
    return R"CODE(
// Fast Binary Encoding final model map
template <typename TKey, typename TValue>
class FinalModelMap
{
public:
    FinalModelMap(FBEBuffer& buffer, size_t offset) noexcept : _buffer(buffer), _offset(offset) {}

    // Get the allocation size
    size_t fbe_allocation_size(const std::map<TKey, TValue>& values) const noexcept;
    size_t fbe_allocation_size(const std::unordered_map<TKey, TValue>& values) const noexcept;

    // Get the field offset
    size_t fbe_offset() const noexcept { return _offset; }
    // Set the field offset
    size_t fbe_offset(size_t offset) const noexcept { return _offset = offset; }

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept { _offset -= size; }

    // Check if the map is valid
    size_t verify() const noexcept;

    // Get the map as std::map
    size_t get(std::map<TKey, TValue>& values) const noexcept;
    // Get the map as std::unordered_map
    size_t get(std::unordered_map<TKey, TValue>& values) const noexcept;

    // Get the map as std::map with memory resource
    size_t get(std::map<TKey, TValue>& values, std::pmr::memory_resource* resource) const noexcept;
    // Get the map as std::pmr::map (with optional resource)
    size_t get(std::pmr::map<TKey, TValue>& values) const noexcept { return get(values, nullptr); }
    size_t get(std::pmr::map<TKey, TValue>& values, std::pmr::memory_resource* resource) const noexcept;
    // Get the map as std::pmr::unordered_map (with optional resource)
    size_t get(std::pmr::unordered_map<TKey, TValue>& values) const noexcept { return get(values, nullptr); }
    size_t get(std::pmr::unordered_map<TKey, TValue>& values, std::pmr::memory_resource* resource) const noexcept;

    // Set the map as std::map
    size_t set(const std::map<TKey, TValue>& values) noexcept;
    // Set the map as std::unordered_map
    size_t set(const std::unordered_map<TKey, TValue>& values) noexcept;

    // Set the map as std::pmr::map
    size_t set(const std::pmr::map<TKey, TValue>& values) noexcept;
    // Set the map as std::pmr::unordered_map
    size_t set(const std::pmr::unordered_map<TKey, TValue>& values) noexcept;

    // Get the allocation size for std::pmr::map
    size_t fbe_allocation_size(const std::pmr::map<TKey, TValue>& values) const noexcept;
    // Get the allocation size for std::pmr::unordered_map
    size_t fbe_allocation_size(const std::pmr::unordered_map<TKey, TValue>& values) const noexcept;

#if defined(USING_BTREE_MAP)
    // Get the allocation size for FBE::map (btree_map)
    size_t fbe_allocation_size(const FBE::map<TKey, TValue>& values) const noexcept;
    // Get the allocation size for FBE::pmr::map (btree_map with pmr allocator)
    size_t fbe_allocation_size(const FBE::pmr::map<TKey, TValue>& values) const noexcept;
    // Get the map as FBE::map (btree_map)
    size_t get(FBE::map<TKey, TValue>& values) const noexcept;
    // Get the map as FBE::pmr::map (btree_map with pmr allocator, optional resource)
    size_t get(FBE::pmr::map<TKey, TValue>& values) const noexcept { return get(values, nullptr); }
    size_t get(FBE::pmr::map<TKey, TValue>& values, std::pmr::memory_resource* resource) const noexcept;
    // Set the map as FBE::map (btree_map)
    size_t set(const FBE::map<TKey, TValue>& values) noexcept;
    // Set the map as FBE::pmr::map (btree_map with pmr allocator)
    size_t set(const FBE::pmr::map<TKey, TValue>& values) noexcept;
#endif

private:
    FBEBuffer& _buffer;
    mutable size_t _offset;
};
)CODE";
  }

  static std::string GenerateFBEFinalModelMap_Inline() {
    return R"CODE(
template <typename TKey, typename TValue>
inline size_t FinalModelMap<TKey, TValue>::fbe_allocation_size(const std::map<TKey, TValue>& values) const noexcept
{
    size_t size = 4;
    FinalModel<TKey> fbe_model_key(_buffer, fbe_offset() + 4);
    FinalModel<TValue> fbe_model_value(_buffer, fbe_offset() + 4);
    for (const auto& value : values)
    {
        size += fbe_model_key.fbe_allocation_size(value.first);
        size += fbe_model_value.fbe_allocation_size(value.second);
    }
    return size;
}

template <typename TKey, typename TValue>
inline size_t FinalModelMap<TKey, TValue>::fbe_allocation_size(const std::unordered_map<TKey, TValue>& values) const noexcept
{
    size_t size = 4;
    FinalModel<TKey> fbe_model_key(_buffer, fbe_offset() + 4);
    FinalModel<TValue> fbe_model_value(_buffer, fbe_offset() + 4);
    for (const auto& value : values)
    {
        size += fbe_model_key.fbe_allocation_size(value.first);
        size += fbe_model_value.fbe_allocation_size(value.second);
    }
    return size;
}

template <typename TKey, typename TValue>
inline size_t FinalModelMap<TKey, TValue>::verify() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + 4) > _buffer.size())
        return std::numeric_limits<std::size_t>::max();

    uint32_t fbe_map_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());

    size_t size = 4;
    FinalModel<TKey> fbe_model_key(_buffer, fbe_offset() + 4);
    FinalModel<TValue> fbe_model_value(_buffer, fbe_offset() + 4);
    for (size_t i = fbe_map_size; i-- > 0;)
    {
        size_t offset_key = fbe_model_key.verify();
        if (offset_key == std::numeric_limits<std::size_t>::max())
            return std::numeric_limits<std::size_t>::max();
        fbe_model_key.fbe_shift(offset_key);
        fbe_model_value.fbe_shift(offset_key);
        size += offset_key;
        size_t offset_value = fbe_model_value.verify();
        if (offset_value == std::numeric_limits<std::size_t>::max())
            return std::numeric_limits<std::size_t>::max();
        fbe_model_key.fbe_shift(offset_value);
        fbe_model_value.fbe_shift(offset_value);
        size += offset_value;
    }
    return size;
}

template <typename TKey, typename TValue>
inline size_t FinalModelMap<TKey, TValue>::get(std::map<TKey, TValue>& values) const noexcept
{
    values.clear();

    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    assert(((fbe_full_offset + 4) <= _buffer.size()) && "Model is broken!");
    if ((fbe_full_offset + 4) > _buffer.size())
        return 0;

    size_t fbe_map_size = unaligned_load<uint32_t>(_buffer.data() + fbe_full_offset);
    if (fbe_map_size == 0)
        return 4;

    size_t size = 4;
    FinalModel<TKey> fbe_model_key(_buffer, fbe_offset() + 4);
    FinalModel<TValue> fbe_model_value(_buffer, fbe_offset() + 4);
    for (size_t i = 0; i < fbe_map_size; ++i)
    {
        TKey key{};
        TValue value{};
        size_t offset_key = fbe_model_key.get(key);
        fbe_model_key.fbe_shift(offset_key);
        fbe_model_value.fbe_shift(offset_key);
        size_t offset_value = fbe_model_value.get(value);
        fbe_model_key.fbe_shift(offset_value);
        fbe_model_value.fbe_shift(offset_value);
        values.emplace(std::move(key), std::move(value));
        size += offset_key + offset_value;
    }
    return size;
}

template <typename TKey, typename TValue>
inline size_t FinalModelMap<TKey, TValue>::get(std::unordered_map<TKey, TValue>& values) const noexcept
{
    values.clear();

    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    assert(((fbe_full_offset + 4) <= _buffer.size()) && "Model is broken!");
    if ((fbe_full_offset + 4) > _buffer.size())
        return 0;

    size_t fbe_map_size = unaligned_load<uint32_t>(_buffer.data() + fbe_full_offset);
    if (fbe_map_size == 0)
        return 4;

    values.reserve(fbe_map_size);
    size_t size = 4;
    FinalModel<TKey> fbe_model_key(_buffer, fbe_offset() + 4);
    FinalModel<TValue> fbe_model_value(_buffer, fbe_offset() + 4);
    for (size_t i = 0; i < fbe_map_size; ++i)
    {
        TKey key{};
        TValue value{};
        size_t offset_key = fbe_model_key.get(key);
        fbe_model_key.fbe_shift(offset_key);
        fbe_model_value.fbe_shift(offset_key);
        size_t offset_value = fbe_model_value.get(value);
        fbe_model_key.fbe_shift(offset_value);
        fbe_model_value.fbe_shift(offset_value);
        values.emplace(std::move(key), std::move(value));
        size += offset_key + offset_value;
    }
    return size;
}

template <typename TKey, typename TValue>
inline size_t FinalModelMap<TKey, TValue>::set(const std::map<TKey, TValue>& values) noexcept
{
    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    assert(((fbe_full_offset + 4) <= _buffer.size()) && "Model is broken!");
    if ((fbe_full_offset + 4) > _buffer.size())
        return 0;

    unaligned_store<uint32_t>(_buffer.data() + fbe_full_offset, (uint32_t)values.size());

    size_t size = 4;
    FinalModel<TKey> fbe_model_key(_buffer, fbe_offset() + 4);
    FinalModel<TValue> fbe_model_value(_buffer, fbe_offset() + 4);
    for (const auto& value : values)
    {
        size_t offset_key = fbe_model_key.set(value.first);
        fbe_model_key.fbe_shift(offset_key);
        fbe_model_value.fbe_shift(offset_key);
        size_t offset_value = fbe_model_value.set(value.second);
        fbe_model_key.fbe_shift(offset_value);
        fbe_model_value.fbe_shift(offset_value);
        size += offset_key + offset_value;
    }
    return size;
}

template <typename TKey, typename TValue>
inline size_t FinalModelMap<TKey, TValue>::set(const std::unordered_map<TKey, TValue>& values) noexcept
{
    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    assert(((fbe_full_offset + 4) <= _buffer.size()) && "Model is broken!");
    if ((fbe_full_offset + 4) > _buffer.size())
        return 0;

    unaligned_store<uint32_t>(_buffer.data() + fbe_full_offset, (uint32_t)values.size());

    size_t size = 4;
    FinalModel<TKey> fbe_model_key(_buffer, fbe_offset() + 4);
    FinalModel<TValue> fbe_model_value(_buffer, fbe_offset() + 4);
    for (const auto& value : values)
    {
        size_t offset_key = fbe_model_key.set(value.first);
        fbe_model_key.fbe_shift(offset_key);
        fbe_model_value.fbe_shift(offset_key);
        size_t offset_value = fbe_model_value.set(value.second);
        fbe_model_key.fbe_shift(offset_value);
        fbe_model_value.fbe_shift(offset_value);
        size += offset_key + offset_value;
    }
    return size;
}

// PMR allocation size methods
template <typename TKey, typename TValue>
inline size_t FinalModelMap<TKey, TValue>::fbe_allocation_size(const std::pmr::map<TKey, TValue>& values) const noexcept
{
    size_t size = 4;
    FinalModel<TKey> fbe_model_key(_buffer, fbe_offset() + 4);
    FinalModel<TValue> fbe_model_value(_buffer, fbe_offset() + 4);
    for (const auto& value : values)
    {
        size += fbe_model_key.fbe_allocation_size(value.first);
        size += fbe_model_value.fbe_allocation_size(value.second);
    }
    return size;
}

template <typename TKey, typename TValue>
inline size_t FinalModelMap<TKey, TValue>::fbe_allocation_size(const std::pmr::unordered_map<TKey, TValue>& values) const noexcept
{
    size_t size = 4;
    FinalModel<TKey> fbe_model_key(_buffer, fbe_offset() + 4);
    FinalModel<TValue> fbe_model_value(_buffer, fbe_offset() + 4);
    for (const auto& value : values)
    {
        size += fbe_model_key.fbe_allocation_size(value.first);
        size += fbe_model_value.fbe_allocation_size(value.second);
    }
    return size;
}

// PMR get methods with memory_resource
template <typename TKey, typename TValue>
inline size_t FinalModelMap<TKey, TValue>::get(std::map<TKey, TValue>& values, [[maybe_unused]] std::pmr::memory_resource* resource) const noexcept
{
    // For non-PMR container, just delegate to the regular get
    return get(values);
}

template <typename TKey, typename TValue>
inline size_t FinalModelMap<TKey, TValue>::get(std::pmr::map<TKey, TValue>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();

    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    assert(((fbe_full_offset + 4) <= _buffer.size()) && "Model is broken!");
    if ((fbe_full_offset + 4) > _buffer.size())
        return 0;

    size_t fbe_map_size = unaligned_load<uint32_t>(_buffer.data() + fbe_full_offset);
    if (fbe_map_size == 0)
        return 4;

    size_t size = 4;
    FinalModel<TKey> fbe_model_key(_buffer, fbe_offset() + 4);
    FinalModel<TValue> fbe_model_value(_buffer, fbe_offset() + 4);
    for (size_t i = 0; i < fbe_map_size; ++i)
    {
        TKey key{};
        TValue value{};
        if constexpr (std::is_constructible_v<TKey, std::pmr::memory_resource*>) {
            key = TKey(resource);
        }
        if constexpr (std::is_constructible_v<TValue, std::pmr::memory_resource*>) {
            value = TValue(resource);
        }
        size_t offset_key = fbe_model_key.get(key, resource);
        fbe_model_key.fbe_shift(offset_key);
        fbe_model_value.fbe_shift(offset_key);
        size_t offset_value = fbe_model_value.get(value, resource);
        fbe_model_key.fbe_shift(offset_value);
        fbe_model_value.fbe_shift(offset_value);
        values.emplace(std::move(key), std::move(value));
        size += offset_key + offset_value;
    }
    return size;
}

template <typename TKey, typename TValue>
inline size_t FinalModelMap<TKey, TValue>::get(std::pmr::unordered_map<TKey, TValue>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();

    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    assert(((fbe_full_offset + 4) <= _buffer.size()) && "Model is broken!");
    if ((fbe_full_offset + 4) > _buffer.size())
        return 0;

    size_t fbe_map_size = unaligned_load<uint32_t>(_buffer.data() + fbe_full_offset);
    if (fbe_map_size == 0)
        return 4;

    values.reserve(fbe_map_size);
    size_t size = 4;
    FinalModel<TKey> fbe_model_key(_buffer, fbe_offset() + 4);
    FinalModel<TValue> fbe_model_value(_buffer, fbe_offset() + 4);
    for (size_t i = 0; i < fbe_map_size; ++i)
    {
        TKey key{};
        TValue value{};
        if constexpr (std::is_constructible_v<TKey, std::pmr::memory_resource*>) {
            key = TKey(resource);
        }
        if constexpr (std::is_constructible_v<TValue, std::pmr::memory_resource*>) {
            value = TValue(resource);
        }
        size_t offset_key = fbe_model_key.get(key, resource);
        fbe_model_key.fbe_shift(offset_key);
        fbe_model_value.fbe_shift(offset_key);
        size_t offset_value = fbe_model_value.get(value, resource);
        fbe_model_key.fbe_shift(offset_value);
        fbe_model_value.fbe_shift(offset_value);
        values.emplace(std::move(key), std::move(value));
        size += offset_key + offset_value;
    }
    return size;
}

// PMR set methods
template <typename TKey, typename TValue>
inline size_t FinalModelMap<TKey, TValue>::set(const std::pmr::map<TKey, TValue>& values) noexcept
{
    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    assert(((fbe_full_offset + 4) <= _buffer.size()) && "Model is broken!");
    if ((fbe_full_offset + 4) > _buffer.size())
        return 0;

    unaligned_store<uint32_t>(_buffer.data() + fbe_full_offset, (uint32_t)values.size());

    size_t size = 4;
    FinalModel<TKey> fbe_model_key(_buffer, fbe_offset() + 4);
    FinalModel<TValue> fbe_model_value(_buffer, fbe_offset() + 4);
    for (const auto& value : values)
    {
        size_t offset_key = fbe_model_key.set(value.first);
        fbe_model_key.fbe_shift(offset_key);
        fbe_model_value.fbe_shift(offset_key);
        size_t offset_value = fbe_model_value.set(value.second);
        fbe_model_key.fbe_shift(offset_value);
        fbe_model_value.fbe_shift(offset_value);
        size += offset_key + offset_value;
    }
    return size;
}

template <typename TKey, typename TValue>
inline size_t FinalModelMap<TKey, TValue>::set(const std::pmr::unordered_map<TKey, TValue>& values) noexcept
{
    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    assert(((fbe_full_offset + 4) <= _buffer.size()) && "Model is broken!");
    if ((fbe_full_offset + 4) > _buffer.size())
        return 0;

    unaligned_store<uint32_t>(_buffer.data() + fbe_full_offset, (uint32_t)values.size());

    size_t size = 4;
    FinalModel<TKey> fbe_model_key(_buffer, fbe_offset() + 4);
    FinalModel<TValue> fbe_model_value(_buffer, fbe_offset() + 4);
    for (const auto& value : values)
    {
        size_t offset_key = fbe_model_key.set(value.first);
        fbe_model_key.fbe_shift(offset_key);
        fbe_model_value.fbe_shift(offset_key);
        size_t offset_value = fbe_model_value.set(value.second);
        fbe_model_key.fbe_shift(offset_value);
        fbe_model_value.fbe_shift(offset_value);
        size += offset_key + offset_value;
    }
    return size;
}

#if defined(USING_BTREE_MAP)
template <typename TKey, typename TValue>
inline size_t FinalModelMap<TKey, TValue>::fbe_allocation_size(const FBE::map<TKey, TValue>& values) const noexcept
{
    size_t size = 4;
    FinalModel<TKey> fbe_model_key(_buffer, fbe_offset() + 4);
    FinalModel<TValue> fbe_model_value(_buffer, fbe_offset() + 4);
    for (const auto& value : values)
        size += fbe_model_key.fbe_allocation_size(value.first) + fbe_model_value.fbe_allocation_size(value.second);
    return size;
}

template <typename TKey, typename TValue>
inline size_t FinalModelMap<TKey, TValue>::get(FBE::map<TKey, TValue>& values) const noexcept
{
    values.clear();

    assert(((_buffer.offset() + fbe_offset() + 4) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + 4) > _buffer.size())
        return 0;

    uint32_t fbe_map_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());

    size_t size = 4;
    FinalModel<TKey> fbe_model_key(_buffer, fbe_offset() + 4);
    FinalModel<TValue> fbe_model_value(_buffer, fbe_offset() + 4);
    for (size_t i = 0; i < fbe_map_size; ++i)
    {
        TKey key = TKey();
        TValue value = TValue();
        size_t offset_key = fbe_model_key.get(key);
        fbe_model_key.fbe_shift(offset_key);
        fbe_model_value.fbe_shift(offset_key);
        size_t offset_value = fbe_model_value.get(value);
        fbe_model_key.fbe_shift(offset_value);
        fbe_model_value.fbe_shift(offset_value);
        values.emplace(std::move(key), std::move(value));
        size += offset_key + offset_value;
    }
    return size;
}

template <typename TKey, typename TValue>
inline size_t FinalModelMap<TKey, TValue>::set(const FBE::map<TKey, TValue>& values) noexcept
{
    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    assert(((fbe_full_offset + 4) <= _buffer.size()) && "Model is broken!");
    if ((fbe_full_offset + 4) > _buffer.size())
        return 0;

    unaligned_store<uint32_t>(_buffer.data() + fbe_full_offset, (uint32_t)values.size());

    size_t size = 4;
    FinalModel<TKey> fbe_model_key(_buffer, fbe_offset() + 4);
    FinalModel<TValue> fbe_model_value(_buffer, fbe_offset() + 4);
    for (const auto& value : values)
    {
        size_t offset_key = fbe_model_key.set(value.first);
        fbe_model_key.fbe_shift(offset_key);
        fbe_model_value.fbe_shift(offset_key);
        size_t offset_value = fbe_model_value.set(value.second);
        fbe_model_key.fbe_shift(offset_value);
        fbe_model_value.fbe_shift(offset_value);
        size += offset_key + offset_value;
    }
    return size;
}

// FBE::pmr::map PMR methods
template <typename TKey, typename TValue>
inline size_t FinalModelMap<TKey, TValue>::fbe_allocation_size(const FBE::pmr::map<TKey, TValue>& values) const noexcept
{
    size_t size = 4;
    FinalModel<TKey> fbe_model_key(_buffer, fbe_offset() + 4);
    FinalModel<TValue> fbe_model_value(_buffer, fbe_offset() + 4);
    for (const auto& value : values)
        size += fbe_model_key.fbe_allocation_size(value.first) + fbe_model_value.fbe_allocation_size(value.second);
    return size;
}

template <typename TKey, typename TValue>
inline size_t FinalModelMap<TKey, TValue>::get(FBE::pmr::map<TKey, TValue>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();

    assert(((_buffer.offset() + fbe_offset() + 4) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + 4) > _buffer.size())
        return 0;

    uint32_t fbe_map_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());

    size_t size = 4;
    FinalModel<TKey> fbe_model_key(_buffer, fbe_offset() + 4);
    FinalModel<TValue> fbe_model_value(_buffer, fbe_offset() + 4);
    for (size_t i = 0; i < fbe_map_size; ++i)
    {
        TKey key{};
        TValue value{};
        if constexpr (std::is_constructible_v<TKey, std::pmr::memory_resource*>) {
            key = TKey(resource);
        }
        if constexpr (std::is_constructible_v<TValue, std::pmr::memory_resource*>) {
            value = TValue(resource);
        }
        size_t offset_key = fbe_model_key.get(key, resource);
        fbe_model_key.fbe_shift(offset_key);
        fbe_model_value.fbe_shift(offset_key);
        size_t offset_value = fbe_model_value.get(value, resource);
        fbe_model_key.fbe_shift(offset_value);
        fbe_model_value.fbe_shift(offset_value);
        values.emplace(std::move(key), std::move(value));
        size += offset_key + offset_value;
    }
    return size;
}

template <typename TKey, typename TValue>
inline size_t FinalModelMap<TKey, TValue>::set(const FBE::pmr::map<TKey, TValue>& values) noexcept
{
    size_t fbe_full_offset = _buffer.offset() + fbe_offset();
    assert(((fbe_full_offset + 4) <= _buffer.size()) && "Model is broken!");
    if ((fbe_full_offset + 4) > _buffer.size())
        return 0;

    unaligned_store<uint32_t>(_buffer.data() + fbe_full_offset, (uint32_t)values.size());

    size_t size = 4;
    FinalModel<TKey> fbe_model_key(_buffer, fbe_offset() + 4);
    FinalModel<TValue> fbe_model_value(_buffer, fbe_offset() + 4);
    for (const auto& value : values)
    {
        size_t offset_key = fbe_model_key.set(value.first);
        fbe_model_key.fbe_shift(offset_key);
        fbe_model_value.fbe_shift(offset_key);
        size_t offset_value = fbe_model_value.set(value.second);
        fbe_model_key.fbe_shift(offset_value);
        fbe_model_value.fbe_shift(offset_value);
        size += offset_key + offset_value;
    }
    return size;
}
#endif
)CODE";
  }

  static std::string GenerateFBESender_Header() {
    return R"CODE(
// Fast Binary Encoding base sender
class Sender
{
public:
    Sender(const Sender&) = delete;
    Sender(Sender&&) noexcept = delete;
    virtual ~Sender() = default;

    Sender& operator=(const Sender&) = delete;
    Sender& operator=(Sender&&) noexcept = delete;

    // Get the sender buffer
    FBEBuffer& buffer() noexcept { return *_buffer; }
    const FBEBuffer& buffer() const noexcept { return *_buffer; }

    // Get the final protocol flag
    bool final() const noexcept { return _final; }

    // Get the logging flag
    bool logging() const noexcept { return _logging; }
    // Enable/Disable logging
    void logging(bool enable) noexcept { _logging = enable; }

    // Reset the sender buffer
    void reset() noexcept { _buffer->reset(); }

    // Send serialized buffer.
    // Direct call of the method requires knowledge about internals of FBE models serialization.
    // Use it with care!
    size_t send_serialized(size_t serialized);

protected:
    // Send message handler
    virtual size_t onSend(const void* data, size_t size) = 0;
    // Send log message handler
    virtual void onSendLog(const std::string& message) const {}

protected:
    std::shared_ptr<FBEBuffer> _buffer;
    bool _logging;
    bool _final;

    Sender() : Sender(nullptr) {}
    Sender(const std::shared_ptr<FBEBuffer>& buffer) : _logging(false), _final(false) { _buffer = buffer ? buffer : std::make_shared<FBEBuffer>(); }

    // Enable/Disable final protocol
    void final(bool enable) noexcept { _final = enable; }
};
)CODE";
  }

  static std::string GenerateFBESender_Source() {
    return R"CODE(
size_t Sender::send_serialized(size_t serialized)
{
    assert((serialized > 0) && "Invalid size of the serialized buffer!");
    if (serialized == 0)
        return 0;

    // Shift the send buffer
    this->_buffer->shift(serialized);

    // Send the value
    size_t sent = onSend(this->_buffer->data(), this->_buffer->size());
    this->_buffer->remove(0, sent);
    return sent;
}
)CODE";
  }

  static std::string GenerateFBEReceiver_Header() {
    return R"CODE(
// Fast Binary Encoding base receiver
class Receiver
{
public:
    Receiver(const Receiver&) = delete;
    Receiver(Receiver&&) = delete;
    virtual ~Receiver() = default;

    Receiver& operator=(const Receiver&) = delete;
    Receiver& operator=(Receiver&&) = delete;

    // Get the receiver buffer
    FBEBuffer& buffer() noexcept { return *_buffer; }
    const FBEBuffer& buffer() const noexcept { return *_buffer; }

    // Get the final protocol flag
    bool final() const noexcept { return _final; }

    // Get the logging flag
    bool logging() const noexcept { return _logging; }
    // Enable/Disable logging
    void logging(bool enable) noexcept { _logging = enable; }

    // Reset the receiver buffer
    void reset() noexcept { _buffer->reset(); }

    // Receive data
    void receive(const void* data, size_t size);

protected:
    // Receive message handler
    virtual bool onReceive(size_t type, const void* data, size_t size) = 0;
    // Receive log message handler
    virtual void onReceiveLog(const std::string& message) const {}

protected:
    std::shared_ptr<FBEBuffer> _buffer;
    bool _logging;
    bool _final;

    Receiver() : Receiver(nullptr) {}
    Receiver(const std::shared_ptr<FBEBuffer>& buffer) : _logging(false), _final(false) { _buffer = buffer ? buffer : std::make_shared<FBEBuffer>(); }

    // Enable/Disable final protocol
    void final(bool enable) noexcept { _final = enable; }
};
)CODE";
  }

  static std::string GenerateFBEReceiver_Source() {
    return R"CODE(
void Receiver::receive(const void* data, size_t size)
{
    if (size == 0)
        return;

    assert((data != nullptr) && "Invalid buffer!");
    if (data == nullptr)
        return;

    // Storage buffer
    uint8_t* buffer1 = _buffer->data();
    size_t offset0 = _buffer->offset();
    size_t offset1 = _buffer->size();
    size_t size1 = _buffer->size();

    // Receive buffer
    const uint8_t* buffer2 = (const uint8_t*)data;
    size_t offset2 = 0;
    size_t size2 = size;

    // While receive buffer is available to handle...
    while (offset2 < size2)
    {
        const uint8_t* message_buffer = nullptr;
        size_t message_size = 0;

        // Try to receive message size
        bool message_size_copied = false;
        bool message_size_found = false;
        while (!message_size_found)
        {
            // Look into the storage buffer
            if (offset0 < size1)
            {
                size_t count = std::min(size1 - offset0, (size_t)4);
                if (count == 4)
                {
                    message_size_copied = true;
                    message_size_found = true;
                    message_size = (size_t)(unaligned_load<uint32_t>(buffer1 + offset0));
                    offset0 += 4;
                    break;
                }
                else
                {
                    // Fill remaining data from the receive buffer
                    if (offset2 < size2)
                    {
                        count = std::min(size2 - offset2, 4 - count);

                        // Allocate and refresh the storage buffer
                        _buffer->allocate(count);
                        buffer1 = _buffer->data();
                        size1 += count;

                        memcpy(buffer1 + offset1, buffer2 + offset2, count);
                        offset1 += count;
                        offset2 += count;
                        continue;
                    }
                    else
                        break;
                }
            }

            // Look into the receive buffer
            if (offset2 < size2)
            {
                size_t count = std::min(size2 - offset2, (size_t)4);
                if (count == 4)
                {
                    message_size_found = true;
                    message_size = (size_t)(unaligned_load<uint32_t>(buffer2 + offset2));
                    offset2 += 4;
                    break;
                }
                else
                {
                    // Allocate and refresh the storage buffer
                    _buffer->allocate(count);
                    buffer1 = _buffer->data();
                    size1 += count;

                    memcpy(buffer1 + offset1, buffer2 + offset2, count);
                    offset1 += count;
                    offset2 += count;
                    continue;
                }
            }
            else
                break;
        }

        if (!message_size_found)
            return;

        // Check the message full size
        size_t min_size = _final ? (4 + 4) : (4 + 4 + 4 + 4);
        assert((message_size >= min_size) && "Invalid receive data!");
        if (message_size < min_size)
            return;

        // Try to receive message body
        bool message_found = false;
        while (!message_found)
        {
            // Look into the storage buffer
            if (offset0 < size1)
            {
                size_t count = std::min(size1 - offset0, message_size - 4);
                if (count == (message_size - 4))
                {
                    message_found = true;
                    message_buffer = buffer1 + offset0 - 4;
                    offset0 += message_size - 4;
                    break;
                }
                else
                {
                    // Fill remaining data from the receive buffer
                    if (offset2 < size2)
                    {
                        // Copy message size into the storage buffer
                        if (!message_size_copied)
                        {
                            // Allocate and refresh the storage buffer
                            _buffer->allocate(4);
                            buffer1 = _buffer->data();
                            size1 += 4;

                            *((uint32_t*)(buffer1 + offset0)) = (uint32_t)message_size;
                            offset0 += 4;
                            offset1 += 4;

                            message_size_copied = true;
                        }

                        count = std::min(size2 - offset2, message_size - 4 - count);

                        // Allocate and refresh the storage buffer
                        _buffer->allocate(count);
                        buffer1 = _buffer->data();
                        size1 += count;

                        memcpy(buffer1 + offset1, buffer2 + offset2, count);
                        offset1 += count;
                        offset2 += count;
                        continue;
                    }
                    else
                        break;
                }
            }

            // Look into the receive buffer
            if (offset2 < size2)
            {
                size_t count = std::min(size2 - offset2, message_size - 4);
                if (!message_size_copied && (count == (message_size - 4)))
                {
                    message_found = true;
                    message_buffer = buffer2 + offset2 - 4;
                    offset2 += message_size - 4;
                    break;
                }
                else
                {
                    // Copy message size into the storage buffer
                    if (!message_size_copied)
                    {
                        // Allocate and refresh the storage buffer
                        _buffer->allocate(4);
                        buffer1 = _buffer->data();
                        size1 += 4;

                        *((uint32_t*)(buffer1 + offset0)) = (uint32_t)message_size;
                        offset0 += 4;
                        offset1 += 4;

                        message_size_copied = true;
                    }

                    // Allocate and refresh the storage buffer
                    _buffer->allocate(count);
                    buffer1 = _buffer->data();
                    size1 += count;

                    memcpy(buffer1 + offset1, buffer2 + offset2, count);
                    offset1 += count;
                    offset2 += count;
                    continue;
                }
            }
            else
                break;
        }

        if (!message_found)
        {
            // Copy message size into the storage buffer
            if (!message_size_copied)
            {
                // Allocate and refresh the storage buffer
                _buffer->allocate(4);
                buffer1 = _buffer->data();
                size1 += 4;

                *((uint32_t*)(buffer1 + offset0)) = (uint32_t)message_size;
                offset0 += 4;
                offset1 += 4;

                message_size_copied = true;
            }
            return;
        }

        [[maybe_unused]] uint32_t fbe_struct_size;
        uint32_t fbe_struct_type;

        // Read the message parameters
        if (_final)
        {
            fbe_struct_size = unaligned_load<uint32_t>(message_buffer);
            fbe_struct_type = unaligned_load<uint32_t>(message_buffer + 4);
        }
        else
        {
            uint32_t fbe_struct_offset = unaligned_load<uint32_t>(message_buffer + 4);
            fbe_struct_size = unaligned_load<uint32_t>(message_buffer + fbe_struct_offset);
            fbe_struct_type = unaligned_load<uint32_t>(message_buffer + fbe_struct_offset + 4);
        }

        // Handle the message
        onReceive(fbe_struct_type, message_buffer, message_size);

        // Reset the storage buffer
        _buffer->reset();

        // Refresh the storage buffer
        buffer1 = _buffer->data();
        offset0 = _buffer->offset();
        offset1 = _buffer->size();
        size1 = _buffer->size();
    }
}
)CODE";
  }

  static std::string GenerateFBEJson() {
    return R"CODE(
namespace JSON {

template <class TWriter, typename T>
struct KeyWriter
{
    static bool to_json_key(TWriter& writer, const T& key)
    {
        return writer.Key(std::to_string(key).data());
    }
};

template <class TWriter, typename T>
bool to_json_key(TWriter& writer, const T& key)
{
    return KeyWriter<TWriter, T>::to_json_key(writer, key);
}

template <class TWriter>
struct KeyWriter<TWriter, FBE::decimal_t>
{
    static bool to_json_key(TWriter& writer, const FBE::decimal_t& key)
    {
        return writer.Key(key.string());
    }
};

template <class TWriter>
struct KeyWriter<TWriter, FBE::uuid_t>
{
    static bool to_json_key(TWriter& writer, const FBE::uuid_t& key)
    {
        return writer.Key(key.string());
    }
};

template <class TWriter>
struct KeyWriter<TWriter, char>
{
    static bool to_json_key(TWriter& writer, const char& key)
    {
        return writer.Key(&key, 1);
    }
};

#if defined(USING_SEASTAR_STRING) || defined(USING_SMALL_STRING)
template <class TWriter>
struct KeyWriter<TWriter, std::string_view>
{
    static bool to_json_key(TWriter& writer, const std::string_view& key)
    {
        return writer.Key(key);
    }
};
#endif

template <class TWriter>
struct KeyWriter<TWriter, FBEString>
{
    static bool to_json_key(TWriter& writer, const FBEString& key)
    {
        return writer.Key(key.data(), key.size());
    }
};

template <class TWriter>
struct KeyWriter<TWriter, ArenaString>
{
    static bool to_json_key(TWriter& writer, const ArenaString& key)
    {
        return writer.Key(key.data(), key.size());
    }
};

template <class TWriter, size_t N>
struct KeyWriter<TWriter, char[N]>
{
    static bool to_json_key(TWriter& writer, const char (&key)[N])
    {
        return writer.Key(key, N - 1);
    }
};

template <class TWriter, typename T>
struct ValueWriter
{
    static bool to_json(TWriter& writer, const T& value, bool scope = true)
    {
        throw std::logic_error("Not implemented!");
    }
};

template <class TWriter, typename T>
bool to_json(TWriter& writer, const T& value, bool scope = true)
{
    return ValueWriter<TWriter, T>::to_json(writer, value, scope);
}

template <class TWriter>
struct ValueWriter<TWriter, bool>
{
    static bool to_json(TWriter& writer, const bool& value, bool scope = true)
    {
        return writer.Bool(value);
    }
};

template <class TWriter>
struct ValueWriter<TWriter, char>
{
    static bool to_json(TWriter& writer, const char& value, bool scope = true)
    {
        return writer.Uint(value);
    }
};

template <class TWriter>
struct ValueWriter<TWriter, wchar_t>
{
    static bool to_json(TWriter& writer, const wchar_t& value, bool scope = true)
    {
        return writer.Uint(value);
    }
};

template <class TWriter>
struct ValueWriter<TWriter, int8_t>
{
    static bool to_json(TWriter& writer, const int8_t& value, bool scope = true)
    {
        return writer.Int(value);
    }
};

template <class TWriter>
struct ValueWriter<TWriter, uint8_t>
{
    static bool to_json(TWriter& writer, const uint8_t& value, bool scope = true)
    {
        return writer.Uint(value);
    }
};

template <class TWriter>
struct ValueWriter<TWriter, int16_t>
{
    static bool to_json(TWriter& writer, const int16_t& value, bool scope = true)
    {
        return writer.Int(value);
    }
};

template <class TWriter>
struct ValueWriter<TWriter, uint16_t>
{
    static bool to_json(TWriter& writer, const uint16_t& value, bool scope = true)
    {
        return writer.Uint(value);
    }
};

template <class TWriter>
struct ValueWriter<TWriter, int32_t>
{
    static bool to_json(TWriter& writer, const int32_t& value, bool scope = true)
    {
        return writer.Int(value);
    }
};

template <class TWriter>
struct ValueWriter<TWriter, uint32_t>
{
    static bool to_json(TWriter& writer, const uint32_t& value, bool scope = true)
    {
        return writer.Uint(value);
    }
};

template <class TWriter>
struct ValueWriter<TWriter, int64_t>
{
    static bool to_json(TWriter& writer, const int64_t& value, bool scope = true)
    {
        return writer.Int64(value);
    }
};

template <class TWriter>
struct ValueWriter<TWriter, uint64_t>
{
    static bool to_json(TWriter& writer, const uint64_t& value, bool scope = true)
    {
        return writer.Uint64(value);
    }
};

template <class TWriter>
struct ValueWriter<TWriter, float>
{
    static bool to_json(TWriter& writer, const float& value, bool scope = true)
    {
        return writer.Double(value);
    }
};

template <class TWriter>
struct ValueWriter<TWriter, double>
{
    static bool to_json(TWriter& writer, const double& value, bool scope = true)
    {
        return writer.Double(value);
    }
};

template <class TWriter>
struct ValueWriter<TWriter, FBE::decimal_t>
{
    static bool to_json(TWriter& writer, const FBE::decimal_t& value, bool scope = true)
    {
        return writer.String(value.string());
    }
};

template <class TWriter>
struct ValueWriter<TWriter, FBE::uuid_t>
{
    static bool to_json(TWriter& writer, const FBE::uuid_t& value, bool scope = true)
    {
        return writer.String(value.string());
    }
};

#if defined(USING_SEASTAR_STRING) || defined(USING_SMALL_STRING)
template <class TWriter>
struct ValueWriter<TWriter, std::string_view>
{
    static bool to_json(TWriter& writer, const std::string_view& value, bool scope = true)
    {
        return writer.String(value);
    }
};
#endif

template <class TWriter>
struct ValueWriter<TWriter, FBEString>
{
    static bool to_json(TWriter& writer, const FBEString& value, bool scope = true)
    {
        return writer.String(value.data(), value.size());
    }
};

template <class TWriter>
struct ValueWriter<TWriter, ArenaString>
{
    static bool to_json(TWriter& writer, const ArenaString& value, bool scope = true)
    {
        return writer.String(value.data(), value.size());
    }
};

template <class TWriter, std::size_t N>
struct ValueWriter<TWriter, char[N]>
{
    static bool to_json(TWriter& writer, const char (&value)[N], bool scope = true)
    {
        return writer.String(value, N);
    }
};

template <class TWriter, typename T>
struct ValueWriter<TWriter, std::optional<T>>
{
    static bool to_json(TWriter& writer, const std::optional<T>& value, bool scope = true)
    {
        if (value)
            return ValueWriter<TWriter, T>::to_json(writer, value.value(), true);
        else
            return writer.Null();
    }
};

template <class TWriter>
struct ValueWriter<TWriter, buffer_t>
{
    static bool to_json(TWriter& writer, const buffer_t& values, bool scope = true)
    {
        return writer.String(values.base64encode());
    }
};

template <class TWriter, typename T, size_t N>
struct ValueWriter<TWriter, std::array<T, N>>
{
    static bool to_json(TWriter& writer, const std::array<T, N>& values, bool scope = true)
    {
        writer.StartArray();
        for (const auto& value : values)
            if (!ValueWriter<TWriter, T>::to_json(writer, value, true))
                return false;
        writer.EndArray();
        return true;
    }
};

template <class TWriter, typename T>
struct ValueWriter<TWriter, FastVec<T>>
{
    static bool to_json(TWriter& writer, const FastVec<T>& values, bool scope = true)
    {
        writer.StartArray();
        for (const auto& value : values)
            if (!FBE::JSON::to_json(writer, value, true))
                return false;
        writer.EndArray();
        return true;
    }
};

template <class TWriter, typename T>
struct ValueWriter<TWriter, std::list<T>>
{
    static bool to_json(TWriter& writer, const std::list<T>& values, bool scope = true)
    {
        writer.StartArray();
        for (const auto& value : values)
            if (!FBE::JSON::to_json(writer, value, true))
                return false;
        writer.EndArray();
        return true;
    }
};

template <class TWriter, typename T>
struct ValueWriter<TWriter, std::set<T>>
{
    static bool to_json(TWriter& writer, const std::set<T>& values, bool scope = true)
    {
        writer.StartArray();
        for (const auto& value : values)
            if (!FBE::JSON::to_json(writer, value, true))
                return false;
        writer.EndArray();
        return true;
    }
};

template <class TWriter, typename TKey, typename TValue>
struct ValueWriter<TWriter, std::map<TKey, TValue>>
{
    static bool to_json(TWriter& writer, const std::map<TKey, TValue>& values, bool scope = true)
    {
        writer.StartObject();
        for (const auto& value : values)
        {
            if (!FBE::JSON::to_json_key(writer, value.first))
                return false;
            if (!FBE::JSON::to_json(writer, value.second, true))
                return false;
        }
        writer.EndObject();
        return true;
    }
};

template <class TWriter, typename TKey, typename TValue>
struct ValueWriter<TWriter, std::unordered_map<TKey, TValue>>
{
    static bool to_json(TWriter& writer, const std::unordered_map<TKey, TValue>& values, bool scope = true)
    {
        writer.StartObject();
        for (const auto& value : values)
        {
            if (!FBE::JSON::to_json_key(writer, value.first))
                return false;
            if (!FBE::JSON::to_json(writer, value.second, true))
                return false;
        }
        writer.EndObject();
        return true;
    }
};

#if defined(USING_BTREE_MAP)
template <class TWriter, typename T>
struct ValueWriter<TWriter, FBE::set<T>>
{
    static bool to_json(TWriter& writer, const FBE::set<T>& values, bool scope = true)
    {
        writer.StartArray();
        for (const auto& value : values)
            if (!FBE::JSON::to_json(writer, value, true))
                return false;
        writer.EndArray();
        return true;
    }
};

template <class TWriter, typename TKey, typename TValue>
struct ValueWriter<TWriter, FBE::map<TKey, TValue>>
{
    static bool to_json(TWriter& writer, const FBE::map<TKey, TValue>& values, bool scope = true)
    {
        writer.StartObject();
        for (const auto& value : values)
        {
            if (!FBE::JSON::to_json_key(writer, value.first))
                return false;
            if (!FBE::JSON::to_json(writer, value.second, true))
                return false;
        }
        writer.EndObject();
        return true;
    }
};
#endif

template <class TJson, typename T>
struct ValueReader
{
    static bool from_json(const TJson& json, T& value)
    {
        throw std::logic_error("Not implemented!");
    }
};

template <class TJson, typename T>
bool from_json(const TJson& json, T& value)
{
    return ValueReader<TJson, T>::from_json(json, value);
}

template <class TJson, typename T>
struct KeyReader
{
    static bool from_json_key(const TJson& json, T& key)
    {
        std::string str;
        if (!FBE::JSON::from_json(json, str))
            return false;

        std::istringstream(str) >> key;
        return true;
    }
};

template <class TJson, typename T>
bool from_json_key(const TJson& json, T& key)
{
    return KeyReader<TJson, T>::from_json_key(json, key);
}

#if defined(USING_SEASTAR_STRING) || defined(USING_SMALL_STRING)
template <class TJson>
struct KeyReader<TJson, std::string>
{
    static bool from_json_key(const TJson& json, std::string& key)
    {
        return FBE::JSON::from_json(json, key);
    }
};
#endif

template <class TJson>
struct KeyReader<TJson, FBEString>
{
    static bool from_json_key(const TJson& json, FBEString& key)
    {
        return FBE::JSON::from_json(json, key);
    }
};

template <class TJson>
struct KeyReader<TJson, ArenaString>
{
    static bool from_json_key(const TJson& json, ArenaString& key)
    {
        return FBE::JSON::from_json(json, key);
    }
};

template <class TJson>
struct KeyReader<TJson, FBE::decimal_t>
{
    static bool from_json_key(const TJson& json, FBE::decimal_t& key)
    {
        return FBE::JSON::from_json(json, key);
    }
};

template <class TJson>
struct KeyReader<TJson, FBE::uuid_t>
{
    static bool from_json_key(const TJson& json, FBE::uuid_t& key)
    {
        return FBE::JSON::from_json(json, key);
    }
};

template <class TJson, size_t N>
struct KeyReader<TJson, char[N]>
{
    static bool from_json_key(const TJson& json, char (&key)[N])
    {
        return FBE::JSON::from_json(json, key);
    }
};

template <class TJson>
struct ValueReader<TJson, bool>
{
    static bool from_json(const TJson& json, bool& value)
    {
        value = false;

        // Schema validation
        if (json.IsNull() || !json.IsBool())
            return false;

        // Save the value
        value = json.GetBool();
        return true;
    }
};

template <class TJson>
struct ValueReader<TJson, char>
{
    static bool from_json(const TJson& json, char& value)
    {
        value = '\0';

        // Schema validation
        if (json.IsNull() || !json.IsUint())
            return false;

        // Save the value
        value = (char)json.GetUint();
        return true;
    }
};

template <class TJson>
struct ValueReader<TJson, wchar_t>
{
    static bool from_json(const TJson& json, wchar_t& value)
    {
        value = L'\0';

        // Schema validation
        if (json.IsNull() || !json.IsUint())
            return false;

        // Save the value
        value = (wchar_t)json.GetUint();
        return true;
    }
};

template <class TJson>
struct ValueReader<TJson, int8_t>
{
    static bool from_json(const TJson& json, int8_t& value)
    {
        value = 0;

        // Schema validation
        if (json.IsNull() || !json.IsInt())
            return false;

        // Save the value
        value = (int8_t)json.GetInt();
        return true;
    }
};

template <class TJson>
struct ValueReader<TJson, uint8_t>
{
    static bool from_json(const TJson& json, uint8_t& value)
    {
        value = 0;

        // Schema validation
        if (json.IsNull() || !json.IsUint())
            return false;

        // Save the value
        value = (uint8_t)json.GetUint();
        return true;
    }
};

template <class TJson>
struct ValueReader<TJson, int16_t>
{
    static bool from_json(const TJson& json, int16_t& value)
    {
        value = 0;

        // Schema validation
        if (json.IsNull() || !json.IsInt())
            return false;

        // Save the value
        value = (int16_t)json.GetInt();
        return true;
    }
};

template <class TJson>
struct ValueReader<TJson, uint16_t>
{
    static bool from_json(const TJson& json, uint16_t& value)
    {
        value = 0;

        // Schema validation
        if (json.IsNull() || !json.IsUint())
            return false;

        // Save the value
        value = (uint16_t)json.GetUint();
        return true;
    }
};

template <class TJson>
struct ValueReader<TJson, int32_t>
{
    static bool from_json(const TJson& json, int32_t& value)
    {
        value = 0;

        // Schema validation
        if (json.IsNull() || !json.IsInt())
            return false;

        // Save the value
        value = (int32_t)json.GetInt();
        return true;
    }
};

template <class TJson>
struct ValueReader<TJson, uint32_t>
{
    static bool from_json(const TJson& json, uint32_t& value)
    {
        value = 0;

        // Schema validation
        if (json.IsNull() || !json.IsUint())
            return false;

        // Save the value
        value = (uint32_t)json.GetUint();
        return true;
    }
};

template <class TJson>
struct ValueReader<TJson, int64_t>
{
    static bool from_json(const TJson& json, int64_t& value)
    {
        value = 0;

        // Schema validation
        if (json.IsNull() || !json.IsInt64())
            return false;

        // Save the value
        value = (int64_t)json.GetInt64();
        return true;
    }
};

template <class TJson>
struct ValueReader<TJson, uint64_t>
{
    static bool from_json(const TJson& json, uint64_t& value)
    {
        value = 0;

        // Schema validation
        if (json.IsNull() || !json.IsUint64())
            return false;

        // Save the value
        value = (uint64_t)json.GetUint64();
        return true;
    }
};

template <class TJson>
struct ValueReader<TJson, float>
{
    static bool from_json(const TJson& json, float& value)
    {
        value = 0.0f;

        // Schema validation
        if (json.IsNull() || !(json.IsInt() || json.IsFloat()))
            return false;

        // Save the value
        value = json.GetFloat();
        return true;
    }
};

template <class TJson>
struct ValueReader<TJson, double>
{
    static bool from_json(const TJson& json, double& value)
    {
        value = 0.0;

        // Schema validation
        if (json.IsNull() || !(json.IsInt() || json.IsDouble()))
            return false;

        // Save the value
        value = json.GetDouble();
        return true;
    }
};

template <class TJson>
struct ValueReader<TJson, FBE::decimal_t>
{
    static bool from_json(const TJson& json, FBE::decimal_t& value)
    {
        value = 0.0;

        // Schema validation
        if (json.IsNull() || !json.IsString())
            return false;

        // Save the value
        try
        {
            std::string str(json.GetString(), (size_t)json.GetStringLength());
            value = std::stod(str);
            return true;
        }
        catch (...)
        {
            return false;
        }
    }
};

template <class TJson>
struct ValueReader<TJson, FBE::uuid_t>
{
    static bool from_json(const TJson& json, FBE::uuid_t& value)
    {
        value = uuid_t::nil();

        // Schema validation
        if (json.IsNull() || !json.IsString())
            return false;

        // Save the value
        try
        {
            std::string str(json.GetString(), (size_t)json.GetStringLength());
            value = str;
            return true;
        }
        catch (...)
        {
            return false;
        }
    }
};

#if defined(USING_SEASTAR_STRING) || defined(USING_SMALL_STRING)
template <class TJson>
struct ValueReader<TJson, std::string>
{
    static bool from_json(const TJson& json, std::string& value)
    {
        value = "";

        // Schema validation
        if (json.IsNull() || !json.IsString())
            return false;

        // Save the value
        value.assign(json.GetString(), (size_t)json.GetStringLength());
        return true;
    }
};
#endif

template <class TJson>
struct ValueReader<TJson, FBEString>
{
    static bool from_json(const TJson& json, FBEString& value)
    {
        value = "";

        // Schema validation
        if (json.IsNull() || !json.IsString())
            return false;

        // Save the value
        #if defined(USING_SEASTAR_STRING)
        value = json.GetString();
        #else
        value.assign(json.GetString(), (size_t)json.GetStringLength());
        #endif
        return true;
    }
};

template <class TJson>
struct ValueReader<TJson, ArenaString>
{
    static bool from_json(const TJson& json, ArenaString& value)
    {
        value = "";

        // Schema validation
        if (json.IsNull() || !json.IsString())
            return false;

        // Save the value
        value.assign(json.GetString(), (size_t)json.GetStringLength());
        return true;
    }
};

template <class TJson, size_t N>
struct ValueReader<TJson, char[N]>
{
    static bool from_json(const TJson& json, char (&value)[N])
    {
        // Schema validation
        if (json.IsNull() || !json.IsString())
            return false;

        // Save the value
        size_t length = std::min((size_t)json.GetStringLength(), N);
        std::memcpy(value, json.GetString(), length);
        // Write the end of string character if possible
        if (length < N)
            value[length] = '\0';
        return true;
    }
};

template <class TJson, typename T>
struct ValueReader<TJson, std::optional<T>>
{
    static bool from_json(const TJson& json, std::optional<T>& value)
    {
        value = std::nullopt;

        // Empty optional value
        if (json.IsNull())
            return true;

        // Try to get the value
        T temp = T();
        if (!FBE::JSON::from_json(json, temp))
            return false;

        // Save the value
        value.emplace(temp);
        return true;
    }
};

template <class TJson>
struct ValueReader<TJson, buffer_t>
{
    static bool from_json(const TJson& json, buffer_t& value)
    {
        // Schema validation
        if (json.IsNull() || !json.IsString())
            return false;

        std::string str(json.GetString(), (size_t)json.GetStringLength());
        value = buffer_t::base64decode(str);
        return true;
    }
};

template <class TJson, typename T, size_t N>
struct ValueReader<TJson, std::array<T, N>>
{
    static bool from_json(const TJson& json, std::array<T, N>& values)
    {
        // Schema validation
        if (json.IsNull() || !json.IsArray())
            return false;

        // Collect array items
        size_t length = json.GetArray().Size();
        for (size_t i = 0; (i < length) && (i < N); ++i)
            if (!FBE::JSON::from_json(json.GetArray()[(rapidjson::SizeType)i], values[i]))
                return false;
        return true;
    }
};

template <class TJson, typename T>
struct ValueReader<TJson, FastVec<T>>
{
    static bool from_json(const TJson& json, FastVec<T>& values)
    {
        values.clear();

        // Schema validation
        if (json.IsNull() || !json.IsArray())
            return false;

        // Collect vector items
        values.reserve(json.GetArray().Size());
        for (const auto& item : json.GetArray())
        {
            T temp = T();
            if (!FBE::JSON::from_json(item, temp))
                return false;
            #if defined(USING_STD_VECTOR)
            values.emplace_back(temp);
            #else
            values.template emplace_back(temp);
            #endif
        }
        return true;
    }
};

template <class TJson, typename T>
struct ValueReader<TJson, std::list<T>>
{
    static bool from_json(const TJson& json, std::list<T>& values)
    {
        values.clear();

        // Schema validation
        if (json.IsNull() || !json.IsArray())
            return false;

        // Collect list items
        for (const auto& item : json.GetArray())
        {
            T temp = T();
            if (!FBE::JSON::from_json(item, temp))
                return false;
            values.emplace_back(temp);
        }
        return true;
    }
};

template <class TJson, typename T>
struct ValueReader<TJson, std::set<T>>
{
    static bool from_json(const TJson& json, std::set<T>& values)
    {
        values.clear();

        // Schema validation
        if (json.IsNull() || !json.IsArray())
            return false;

        // Collect set items
        for (const auto& item : json.GetArray())
        {
            T temp = T();
            if (!FBE::JSON::from_json(item, temp))
                return false;
            values.emplace(temp);
        }
        return true;
    }
};

template <class TJson, typename TKey, typename TValue>
struct ValueReader<TJson, std::map<TKey, TValue>>
{
    static bool from_json(const TJson& json, std::map<TKey, TValue>& values)
    {
        values.clear();

        // Schema validation
        if (json.IsNull() || !json.IsObject())
            return false;

        // Collect map items
        for (auto it = json.MemberBegin(); it != json.MemberEnd(); ++it)
        {
            TKey key;
            TValue value;
            if (!FBE::JSON::from_json_key(it->name, key))
                return false;
            if (!FBE::JSON::from_json(it->value, value))
                return false;
            values.emplace(key, value);
        }
        return true;
    }
};

template <class TJson, typename TKey, typename TValue>
struct ValueReader<TJson, std::unordered_map<TKey, TValue>>
{
    static bool from_json(const TJson& json, std::unordered_map<TKey, TValue>& values)
    {
        values.clear();

        // Schema validation
        if (json.IsNull() || !json.IsObject())
            return false;

        // Collect hash items
        for (auto it = json.MemberBegin(); it != json.MemberEnd(); ++it)
        {
            TKey key;
            TValue value;
            if (!FBE::JSON::from_json_key(it->name, key))
                return false;
            if (!FBE::JSON::from_json(it->value, value))
                return false;
            values.emplace(key, value);
        }
        return true;
    }
};

#if defined(USING_BTREE_MAP)
template <class TJson, typename T>
struct ValueReader<TJson, FBE::set<T>>
{
    static bool from_json(const TJson& json, FBE::set<T>& values)
    {
        values.clear();

        // Schema validation
        if (json.IsNull() || !json.IsArray())
            return false;

        // Collect set items
        for (const auto& item : json.GetArray())
        {
            T temp = T();
            if (!FBE::JSON::from_json(item, temp))
                return false;
            values.emplace(temp);
        }
        return true;
    }
};

template <class TJson, typename TKey, typename TValue>
struct ValueReader<TJson, FBE::map<TKey, TValue>>
{
    static bool from_json(const TJson& json, FBE::map<TKey, TValue>& values)
    {
        values.clear();

        // Schema validation
        if (json.IsNull() || !json.IsObject())
            return false;

        // Collect map items
        for (auto it = json.MemberBegin(); it != json.MemberEnd(); ++it)
        {
            TKey key;
            TValue value;
            if (!FBE::JSON::from_json_key(it->name, key))
                return false;
            if (!FBE::JSON::from_json(it->value, value))
                return false;
            values.emplace(key, value);
        }
        return true;
    }
};
#endif

template <class TJson, typename T>
struct NodeReader
{
    static bool from_json(const TJson& json, T& value, const char* key)
    {
        if (key == nullptr)
            return false;

        // Try to find a member with the given key
        rapidjson::Value::ConstMemberIterator member = json.FindMember(key);
        if (member == json.MemberEnd())
            // omitted field in json str is allowed.
            return true;

        return FBE::JSON::from_json(member->value, value);
    }
};

template <class TJson, typename T>
bool from_json(const TJson& json, T& value, const char* key)
{
    return NodeReader<TJson, T>::from_json(json, value, key);
}

template <class TJson, typename T>
struct ChildNodeReader
{
    static bool from_json_child(const TJson& json, T& value, const char* key)
    {
        if (key == nullptr)
            return false;

        // Try to find a member with the given key
        rapidjson::Value::ConstMemberIterator member = json.FindMember(key);
        if (member == json.MemberEnd())
            return false;

        // Schema validation
        if (member->value.IsNull() || !member->value.IsObject())
            return false;

        // Deserialize the child object
        return FBE::JSON::from_json(member->value.GetObject(), value);
    }
};

template <class TJson, typename T>
bool from_json_child(const TJson& json, T& value, const char* key)
{
    return ChildNodeReader<TJson, T>::from_json_child(json, value, key);
}

} // namespace JSON
)CODE";
  }

  static std::string GenerateFBEBaseFieldModel_Header() {
    return R"CODE(
// base class for struct field
class BaseFieldModel
{
public:
    virtual ~BaseFieldModel() = default;
    // Get the field offset
    virtual size_t fbe_offset() const noexcept = 0;
    // Get the field size
    virtual size_t fbe_size() const noexcept = 0;
    // Get the field body size
    // virtual size_t fbe_body() const noexcept = 0;
    // Get the field extra size
    virtual size_t fbe_extra() const noexcept = 0;

    // Shift the current field offset
    virtual void fbe_shift(size_t size) noexcept = 0;
    // Unshift the current field offset
    virtual void fbe_unshift(size_t size) noexcept = 0;

    // Check if the struct value is valid
    virtual bool verify(bool fbe_verify_type = true) const noexcept = 0;
    virtual bool verify_fields(size_t fbe_struct_size) const noexcept = 0;

    // Get the struct value (begin phase)
    virtual size_t get_begin() const noexcept = 0;
    // Get the struct value (end phase)
    virtual void get_end(size_t fbe_begin) const noexcept = 0;

    // Set the struct value (begin phase)
    virtual size_t set_begin() = 0;
    // Set the struct value (end phase)
    virtual void set_end(size_t fbe_begin) = 0;

    // Set the struct value
    virtual void set(const ::FBE::Base& fbe_value, std::pmr::memory_resource* resource) noexcept = 0;
    // Set the struct fields values
    virtual void set_fields(const ::FBE::Base& fbe_value, std::pmr::memory_resource* resource) noexcept = 0;

    // Get the struct value
    virtual void get(::FBE::Base& fbe_value, std::pmr::memory_resource* resource) noexcept = 0;
    // Get the struct fields values
    virtual void get_fields(::FBE::Base& fbe_value, size_t fbe_struct_size, std::pmr::memory_resource* resource) noexcept = 0;
};
)CODE";
  }

  static std::string GenerateFBEFieldModelStructOptional_Header() {
    return R"CODE(
template <typename T, typename TStruct>
class FieldModelStructOptional
{
public:
    FieldModelStructOptional(FBEBuffer& buffer, size_t offset) noexcept: _buffer(buffer), _offset(offset), value(buffer, 0) {}

    // Get the field offset
    size_t fbe_offset() const noexcept { return _offset; }
    // Get the field size
    size_t fbe_size() const noexcept { return 1 + 4; }
    // Get the field extra size
    size_t fbe_extra() const noexcept;

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept { _offset -= size; }

    //! Is the value present?
    explicit operator bool() const noexcept { return has_value(); }

    // Checks if the object contains a value
    bool has_value() const noexcept;

    // Check if the optional value is valid
    bool verify() const noexcept;

    // Get the optional value (being phase)
    size_t get_begin() const noexcept;
    // Get the optional value (end phase)
    void get_end(size_t fbe_begin) const noexcept;

    // Get the optional value
    void get(std::optional<TStruct>& opt, std::pmr::memory_resource* resource) noexcept;

    // Set the optional value (begin phase)
    size_t set_begin(bool has_value);
    // Set the optional value (end phase)
    void set_end(size_t fbe_begin);

    // Set the optional value
    void set(const std::optional<TStruct>& opt, std::pmr::memory_resource* resource);

private:
    FBEBuffer& _buffer;
    size_t _offset;

public:
    // Base field model value
    T value;
};
)CODE";
  }

  static std::string GenerateFBEFieldModelStructOptional_Inline() {
    return R"CODE(
template <typename T, typename TStruct>
inline size_t FieldModelStructOptional<T, TStruct>::fbe_extra() const noexcept
{
    if (!has_value())
        return 0;

    uint32_t fbe_optional_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset() + 1);
    if ((fbe_optional_offset == 0) || ((_buffer.offset() + fbe_optional_offset + 4) > _buffer.size()))
        return 0;

    _buffer.shift(fbe_optional_offset);
    size_t fbe_result = value.fbe_size() + value.fbe_extra();
    _buffer.unshift(fbe_optional_offset);
    return fbe_result;
}

template <typename T, typename TStruct>
inline bool FieldModelStructOptional<T, TStruct>::has_value() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return false;

    uint8_t fbe_has_value = *((const uint8_t*)(_buffer.data() + _buffer.offset() + fbe_offset()));
    return (fbe_has_value != 0);
}

template <typename T, typename TStruct>
inline bool FieldModelStructOptional<T, TStruct>::verify() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return true;

    uint8_t fbe_has_value = *((const uint8_t*)(_buffer.data() + _buffer.offset() + fbe_offset()));
    if (fbe_has_value == 0)
        return true;

    uint32_t fbe_optional_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset() + 1);
    if (fbe_optional_offset == 0)
        return false;

    _buffer.shift(fbe_optional_offset);
    bool fbe_result = value.verify();
    _buffer.unshift(fbe_optional_offset);
    return fbe_result;
}

template <typename T, typename TStruct>
inline size_t FieldModelStructOptional<T, TStruct>::get_begin() const noexcept
{
    if (!has_value())
        return 0;

    uint32_t fbe_optional_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset() + 1);
    assert((fbe_optional_offset > 0) && "Model is broken!");
    if (fbe_optional_offset == 0)
        return 0;

    _buffer.shift(fbe_optional_offset);
    return fbe_optional_offset;
}

template <typename T, typename TStruct>
inline void FieldModelStructOptional<T, TStruct>::get_end(size_t fbe_begin) const noexcept
{
    _buffer.unshift(fbe_begin);
}

template <typename T, typename TStruct>
inline void FieldModelStructOptional<T, TStruct>::get(std::optional<TStruct>& opt, std::pmr::memory_resource* resource) noexcept
{

    size_t fbe_begin = get_begin();
    if (fbe_begin == 0)
        return;
    
    if constexpr(std::is_constructible_v<TStruct, std::pmr::memory_resource*> and not is_variant_v<TStruct>) {
        TStruct temp = TStruct(resource);
        value.get(temp, resource);
        opt.emplace(std::move(temp));

        get_end(fbe_begin);
    } else {
        TStruct temp = TStruct();
        value.get(temp, resource);
        opt.emplace(std::move(temp));

        get_end(fbe_begin);
    }
}

template <typename T, typename TStruct>
inline size_t FieldModelStructOptional<T, TStruct>::set_begin(bool has_value)
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return 0;

    uint8_t fbe_has_value = has_value ? 1 : 0;
    *((uint8_t*)(_buffer.data() + _buffer.offset() + fbe_offset())) = fbe_has_value;
    if (fbe_has_value == 0)
        return 0;

    uint32_t fbe_optional_size = (uint32_t)value.fbe_size();
    uint32_t fbe_optional_offset = (uint32_t)(_buffer.allocate(fbe_optional_size) - _buffer.offset());
    assert(((fbe_optional_offset > 0) && ((_buffer.offset() + fbe_optional_offset + fbe_optional_size) <= _buffer.size())) && "Model is broken!");
    if ((fbe_optional_offset == 0) || ((_buffer.offset() + fbe_optional_offset + fbe_optional_size) > _buffer.size()))
        return 0;

    unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset() + 1, fbe_optional_offset);

    _buffer.shift(fbe_optional_offset);
    return fbe_optional_offset;
}

template <typename T, typename TStruct>
inline void FieldModelStructOptional<T, TStruct>::set_end(size_t fbe_begin)
{
    _buffer.unshift(fbe_begin);
}

template <typename T, typename TStruct>
inline void FieldModelStructOptional<T, TStruct>::set(const std::optional<TStruct>& opt, std::pmr::memory_resource* resource)
{
    size_t fbe_begin = set_begin(opt.has_value());
    if (fbe_begin == 0)
        return;

    if (opt)
        value.set(opt.value(), resource);

    set_end(fbe_begin);
}
)CODE";
  }

  static std::string GenerateFBEFieldModelCustomArray_Header() {
    std::string code = R"CODE(
// Fast Binary Encoding field model array
template <typename T, typename TStruct, size_t N>
class FieldModelCustomArray
{
public:
    FieldModelCustomArray(FBEBuffer& buffer, size_t offset) noexcept : _buffer(buffer), _offset(offset), _model(buffer, offset) {}

    // Get the field offset
    size_t fbe_offset() const noexcept { return _offset; }
    // Get the field size
    size_t fbe_size() const noexcept { return N * _model.fbe_size(); }
    // Get the field extra size
    size_t fbe_extra() const noexcept { return 0; }

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept { _offset -= size; }

    // Get the array
    const uint8_t* data() const noexcept;
    // Get the array
    uint8_t* data() noexcept;
    // Get the array offset
    size_t offset() const noexcept { return 0; }
    // Get the array size
    size_t size() const noexcept { return N; }

    // Array index operator
    T operator[](size_t index) const noexcept;

    // Check if the array is valid
    bool verify() const noexcept;

    // Get the array as C-array
    template <size_t S>
    void get(TStruct (&values)[S], std::pmr::memory_resource* resource) const noexcept;
    template <size_t S>
    void get(TStruct* (&values)[S], std::pmr::memory_resource* resource) const noexcept;
    // Get the array as std::array
    template <size_t S>
    void get(std::array<TStruct, S>& values, std::pmr::memory_resource* resource) const noexcept;
    template <size_t S>
    void get(std::array<TStruct*, S>& values, std::pmr::memory_resource* resource) const noexcept;

    // Set the array as C-array
    template <size_t S>
    void set(const TStruct (&values)[S], std::pmr::memory_resource* resource) noexcept;
    template <size_t S>
    void set(const TStruct* (&values)[S], std::pmr::memory_resource* resource) noexcept;
    
    // Set the array as std::array
    template <size_t S>
    void set(const std::array<TStruct, S>& values, std::pmr::memory_resource* resource) noexcept;
    template <size_t S>
    void set(const std::array<TStruct*, S>& values, std::pmr::memory_resource* resource) noexcept;
)CODE";

    std::string code_extra = R"CODE(
    // Get the array as FastVec
    void get(FastVec<TStruct>& values, std::pmr::memory_resource* resource) const noexcept;
    void get(FastVec<TStruct*>& values, std::pmr::memory_resource* resource) const noexcept;

    // Set the array as FastVec
    void set(const FastVec<TStruct>& values, std::pmr::memory_resource* resource) noexcept;
    void set(const FastVec<TStruct*>& values, std::pmr::memory_resource* resource) noexcept;
)CODE";

    code += code_extra;

    code_extra = std::regex_replace(code_extra, std::regex("FastVec"),
                                    "std::pmr::vector");
    code_extra = std::regex_replace(code_extra, std::regex("std::list"),
                                    "std::pmr::list");
    code += code_extra;

    code += R"CODE(
private:
    FBEBuffer& _buffer;
    size_t _offset;
    T _model;
};
)CODE";

    return code;
  }

  static std::string GenerateFBEFieldModelCustomArray_Inline() {
    std::string code = R"CODE(
template <typename T, typename TStruct, size_t N>

inline const uint8_t* FieldModelCustomArray<T, TStruct, N>::data() const noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    return _buffer.data() + _buffer.offset() + fbe_offset();
}

template <typename T, typename TStruct, size_t N>
inline uint8_t* FieldModelCustomArray<T, TStruct, N>::data() noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    return _buffer.data() + _buffer.offset() + fbe_offset();
}

template <typename T, typename TStruct, size_t N>
inline T FieldModelCustomArray<T, TStruct, N>::operator[](size_t index) const noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    assert((index < N) && "Index is out of bounds!");

    T fbe_model(_buffer, fbe_offset());
    fbe_model.fbe_shift(index * fbe_model.fbe_size());
    return fbe_model;
}

template <typename T, typename TStruct, size_t N>
inline bool FieldModelCustomArray<T, TStruct, N>::verify() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return false;

    T fbe_model(_buffer, fbe_offset());
    for (size_t i = N; i-- > 0;)
    {
        if (!fbe_model.verify())
            return false;
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }

    return true;
}

template <typename T, typename TStruct, size_t N>
template <size_t S>
inline void FieldModelCustomArray<T, TStruct, N>::get(TStruct (&values)[S], std::pmr::memory_resource* resource) const noexcept
{
    auto fbe_model = (*this)[0];
    for (size_t i = 0; (i < S) && (i < N); ++i)
    {
        fbe_model.get(values[i], nullptr);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct, size_t N>
template <size_t S>
inline void FieldModelCustomArray<T, TStruct, N>::get(TStruct* (&values)[S], std::pmr::memory_resource* resource) const noexcept
{
    auto fbe_model = (*this)[0];
    for (size_t i = 0; (i < S) && (i < N); ++i)
    {
        TStruct* value = nullptr;
        fbe_model.get(&value, nullptr);
        values[i] = value;
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct, size_t N>
template <size_t S>
inline void FieldModelCustomArray<T, TStruct, N>::get(std::array<TStruct, S>& values, std::pmr::memory_resource* resource) const noexcept
{
    auto fbe_model = (*this)[0];
    for (size_t i = 0; (i < S) && (i < N); ++i)
    {
        fbe_model.get(values[i], nullptr);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct, size_t N>
template <size_t S>
inline void FieldModelCustomArray<T, TStruct, N>::get(std::array<TStruct*, S>& values, std::pmr::memory_resource* resource) const noexcept
{
    auto fbe_model = (*this)[0];
    for (size_t i = 0; (i < S) && (i < N); ++i)
    {
        TStruct* value = nullptr;
        fbe_model.get(&value, nullptr);
        values[i] = value;
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct, size_t N>
template <size_t S>
inline void FieldModelCustomArray<T, TStruct, N>::set(const TStruct (&values)[S], std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = (*this)[0];
    for (size_t i = 0; (i < S) && (i < N); ++i)
    {
        fbe_model.set(values[i], resource);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct, size_t N>
template <size_t S>
inline void FieldModelCustomArray<T, TStruct, N>::set(const TStruct* (&values)[S], std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = (*this)[0];
    for (size_t i = 0; (i < S) && (i < N); ++i)
    {
        fbe_model.set(values[i], resource);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct, size_t N>
template <size_t S>
inline void FieldModelCustomArray<T, TStruct, N>::set(const std::array<TStruct, S>& values, std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = (*this)[0];
    for (size_t i = 0; (i < S) && (i < N); ++i)
    {
        fbe_model.set(values[i], resource);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct, size_t N>
template <size_t S>
inline void FieldModelCustomArray<T, TStruct, N>::set(const std::array<TStruct*, S>& values, std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = (*this)[0];
    for (size_t i = 0; (i < S) && (i < N); ++i)
    {
        fbe_model.set(values[i], resource);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}
)CODE";

    std::string code_extra = R"CODE(
template <typename T, typename TStruct, size_t N>
inline void FieldModelCustomArray<T, TStruct, N>::get(FastVec<TStruct>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();
    values.reserve(N);

    auto fbe_model = (*this)[0];
    for (size_t i = N; i-- > 0;)
    {
        TStruct value;
        fbe_model.get(value, resource);
        #if defined(USING_STD_VECTOR)
        values.emplace_back(std::move(value));
        #else
        values.template emplace_back(std::move(value));
        #endif
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct, size_t N>
inline void FieldModelCustomArray<T, TStruct, N>::get(FastVec<TStruct*>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();
    values.reserve(N);

    auto fbe_model = (*this)[0];
    for (size_t i = N; i-- > 0;)
    {
        TStruct* value = nullptr;
        fbe_model.get(&value, resource);
        #if defined(USING_STD_VECTOR)
        values.emplace_back(std::move(value));
        #else
        values.template emplace_back(std::move(value));
        #endif
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct, size_t N>
inline void FieldModelCustomArray<T, TStruct, N>::set(const FastVec<TStruct>& values, std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = (*this)[0];
    for (size_t i = 0; (i < values.size()) && (i < N); ++i)
    {
        fbe_model.set(values[i], resource);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct, size_t N>
inline void FieldModelCustomArray<T, TStruct, N>::set(const FastVec<TStruct*>& values, std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = (*this)[0];
    for (size_t i = 0; (i < values.size()) && (i < N); ++i)
    {
        fbe_model.set(values[i], resource);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}
)CODE";
    code += code_extra;

    code_extra = std::regex_replace(code_extra, std::regex("FastVec"),
                                    "std::pmr::vector");
    code_extra = std::regex_replace(code_extra,
                                    std::regex("values.template emplace_back"),
                                    "values.emplace_back");
    code_extra = std::regex_replace(code_extra, std::regex("std::list"),
                                    "std::pmr::list");
    code += code_extra;
    return code;
  }

  static std::string GenerateFBEFieldModelCustomVector_Header() {
    std::string code = R"CODE(
template <typename T, typename TStruct>
class FieldModelCustomVector
{
public:
    FieldModelCustomVector(FBEBuffer& buffer, size_t offset) noexcept: _buffer(buffer), _offset(offset) {}

    // Get the field offset
    size_t fbe_offset() const noexcept { return _offset; }
    // Get the field size
    size_t fbe_size() const noexcept { return 4; }
    // Get the field extra size
    size_t fbe_extra() const noexcept;

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept { _offset -= size; }

    // Get the vector offset
    size_t offset() const noexcept;
    // Get the vector size
    size_t size() const noexcept;

    // Vector index operator
    T operator[](size_t index) const noexcept;
    // Resize the vector and get its first model
    T resize(size_t size);

    // Check if the vector is valid
    bool verify() const noexcept;
)CODE";

    std::string code_extra = R"CODE(
    // Get the vector as FastVec
    void get(FastVec<TStruct>& values, std::pmr::memory_resource* resource) const noexcept;
    void get(FastVec<TStruct*>& values, std::pmr::memory_resource* resource) const noexcept;
    // Get the vector as std::list
    void get(std::list<TStruct>& values, std::pmr::memory_resource* resource) const noexcept;
    void get(std::list<TStruct*>& values, std::pmr::memory_resource* resource) const noexcept;
    // Get the vector as std::set
    void get(std::set<TStruct>& values, std::pmr::memory_resource* resource) const noexcept;
    void get(std::set<TStruct*>& values, std::pmr::memory_resource* resource) const noexcept;
    // Set the vector as FastVec
    void set(const FastVec<TStruct>& values, std::pmr::memory_resource* resource) noexcept;
    void set(const FastVec<TStruct*>& values, std::pmr::memory_resource* resource) noexcept;
    // Set the vector as std::list
    void set(const std::list<TStruct>& values, std::pmr::memory_resource* resource) noexcept;
    void set(const std::list<TStruct*>& values, std::pmr::memory_resource* resource) noexcept;
    // Set the vector as std::set
    void set(const std::set<TStruct>& values, std::pmr::memory_resource* resource) noexcept;
    void set(const std::set<TStruct*>& values, std::pmr::memory_resource* resource) noexcept;
)CODE";

    code += code_extra;

    code_extra = std::regex_replace(code_extra, std::regex("FastVec"),
                                    "std::pmr::vector");
    code_extra = std::regex_replace(code_extra, std::regex("std::list"),
                                    "std::pmr::list");
    code_extra =
        std::regex_replace(code_extra, std::regex("std::set"), "std::pmr::set");
    code += code_extra;

    code += R"CODE(
private:
    FBEBuffer& _buffer;
    size_t _offset;
};
)CODE";
    return code;
  }

  static std::string GenerateFBEFieldModelCustomVector_Inline() {
    std::string code = R"CODE(
template <typename T, typename TStruct>
size_t FieldModelCustomVector<T, TStruct>::fbe_extra() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return 0;

    uint32_t fbe_vector_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    if ((fbe_vector_offset == 0) || ((_buffer.offset() + fbe_vector_offset + 4) > _buffer.size()))
        return 0;

    uint32_t fbe_vector_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_vector_offset);

    size_t fbe_result = 4;
    T fbe_model(_buffer, fbe_vector_offset + 4);
    for (size_t i = fbe_vector_size; i-- > 0;)
    {
        fbe_result += fbe_model.fbe_size() + fbe_model.fbe_extra();
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
    return fbe_result;
}

template <typename T, typename TStruct>
size_t FieldModelCustomVector<T, TStruct>::offset() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return 0;

    uint32_t fbe_vector_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    return fbe_vector_offset;
}

template <typename T, typename TStruct>
size_t FieldModelCustomVector<T, TStruct>::size() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return 0;

    uint32_t fbe_vector_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    if ((fbe_vector_offset == 0) || ((_buffer.offset() + fbe_vector_offset + 4) > _buffer.size()))
        return 0;

    uint32_t fbe_vector_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_vector_offset);
    return fbe_vector_size;
}

template <typename T, typename TStruct>
inline T FieldModelCustomVector<T, TStruct>::operator[](size_t index) const noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");

    uint32_t fbe_vector_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    assert(((fbe_vector_offset > 0) && ((_buffer.offset() + fbe_vector_offset + 4) <= _buffer.size())) && "Model is broken!");

    [[maybe_unused]] uint32_t fbe_vector_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_vector_offset);
    assert((index < fbe_vector_size) && "Index is out of bounds!");

    T fbe_model(_buffer, fbe_vector_offset + 4);
    fbe_model.fbe_shift(index * fbe_model.fbe_size());
    return fbe_model;
}

template <typename T, typename TStruct>
inline T FieldModelCustomVector<T, TStruct>::resize(size_t size)
{
    T fbe_model(_buffer, fbe_offset());

    uint32_t fbe_vector_size = (uint32_t)(size * fbe_model.fbe_size());
    uint32_t fbe_vector_offset = (uint32_t)(_buffer.allocate(4 + fbe_vector_size) - _buffer.offset());
    assert(((fbe_vector_offset > 0) && ((_buffer.offset() + fbe_vector_offset + 4) <= _buffer.size())) && "Model is broken!");

    unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset(), fbe_vector_offset);
    unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_vector_offset, size);

    memset((char*)(_buffer.data() + _buffer.offset() + fbe_vector_offset + 4), 0, fbe_vector_size);

    return T(_buffer, fbe_vector_offset + 4);
}

template <typename T, typename TStruct>
inline bool FieldModelCustomVector<T, TStruct>::verify() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return true;

    uint32_t fbe_vector_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    if (fbe_vector_offset == 0)
        return true;

    if ((_buffer.offset() + fbe_vector_offset + 4) > _buffer.size())
        return false;

    uint32_t fbe_vector_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_vector_offset);

    T fbe_model(_buffer, fbe_vector_offset + 4);
    for (size_t i = fbe_vector_size; i-- > 0;)
    {
        if (!fbe_model.verify())
            return false;
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }

    return true;
}
)CODE";

    std::string code_extra = R"CODE(
template <typename T, typename TStruct>
inline void FieldModelCustomVector<T, TStruct>::get(FastVec<TStruct>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();

    size_t fbe_vector_size = size();
    if (fbe_vector_size == 0)
        return;

    values.reserve(fbe_vector_size);

    auto fbe_model = (*this)[0];
    for (size_t i = fbe_vector_size; i-- > 0;)
    {
        if constexpr(std::is_constructible_v<TStruct, std::pmr::memory_resource*> and not is_variant_v<TStruct>) {
            TStruct value = TStruct(resource);
            fbe_model.get(value, resource);
            #if defined(USING_STD_VECTOR)
            values.emplace_back(std::move(value));
            #else
            values.template emplace_back(std::move(value));
            #endif
        } else {
            TStruct value = TStruct();
            fbe_model.get(value, resource);
            #if defined(USING_STD_VECTOR)
            values.emplace_back(std::move(value));
            #else
            values.template emplace_back(std::move(value));
            #endif
        }
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct>
inline void FieldModelCustomVector<T, TStruct>::get(FastVec<TStruct*>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();

    size_t fbe_vector_size = size();
    if (fbe_vector_size == 0)
        return;

    values.reserve(fbe_vector_size);

    auto fbe_model = (*this)[0];
    for (size_t i = fbe_vector_size; i-- > 0;)
    {
        TStruct* value = nullptr;
        fbe_model.get(&value, resource);
        #if defined(USING_STD_VECTOR)
        values.emplace_back(value);
        #else
        values.template emplace_back(value);
        #endif
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct>
inline void FieldModelCustomVector<T, TStruct>::get(std::list<TStruct>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();

    size_t fbe_vector_size = size();
    if (fbe_vector_size == 0)
        return;

    auto fbe_model = (*this)[0];
    for (size_t i = fbe_vector_size; i-- > 0;)
    {
        TStruct value = TStruct();
        fbe_model.get(value, resource);
        values.emplace_back(std::move(value));
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct>
inline void FieldModelCustomVector<T, TStruct>::get(std::list<TStruct*>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();

    size_t fbe_vector_size = size();
    if (fbe_vector_size == 0)
        return;

    auto fbe_model = (*this)[0];
    for (size_t i = fbe_vector_size; i-- > 0;)
    {
        TStruct* value = nullptr;
        fbe_model.get(&value, resource);
        values.emplace_back(value);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct>
inline void FieldModelCustomVector<T, TStruct>::get(std::set<TStruct>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();

    size_t fbe_vector_size = size();
    if (fbe_vector_size == 0)
        return;

    auto fbe_model = (*this)[0];
    // Use hint-based insertion for O(1) amortized insertion (data is already sorted)
    auto hint = values.end();
    for (size_t i = 0; i < fbe_vector_size; ++i)
    {
        TStruct value = TStruct();
        fbe_model.get(value, resource);
        hint = values.emplace_hint(hint, std::move(value));
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct>
inline void FieldModelCustomVector<T, TStruct>::get(std::set<TStruct*>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();

    size_t fbe_vector_size = size();
    if (fbe_vector_size == 0)
        return;

    auto fbe_model = (*this)[0];
    // Use hint-based insertion for O(1) amortized insertion (data is already sorted)
    auto hint = values.end();
    for (size_t i = 0; i < fbe_vector_size; ++i)
    {
        TStruct* value = nullptr;
        fbe_model.get(&value, resource);
        hint = values.emplace_hint(hint, value);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}
template <typename T, typename TStruct>
inline void FieldModelCustomVector<T, TStruct>::set(const FastVec<TStruct>& values, std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.set(value, resource);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct>
inline void FieldModelCustomVector<T, TStruct>::set(const FastVec<TStruct*>& values, std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.set(value, resource);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct>
inline void FieldModelCustomVector<T, TStruct>::set(const std::list<TStruct>& values, std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.set(value, resource);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct>
inline void FieldModelCustomVector<T, TStruct>::set(const std::list<TStruct*>& values, std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.set(value, resource);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct>
inline void FieldModelCustomVector<T, TStruct>::set(const std::set<TStruct>& values, std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.set(value, resource);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct>
inline void FieldModelCustomVector<T, TStruct>::set(const std::set<TStruct*>& values, std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.set(value, resource);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}
)CODE";

    code += code_extra;

    code_extra = std::regex_replace(code_extra, std::regex("FastVec"),
                                    "std::pmr::vector");
    code_extra = std::regex_replace(code_extra,
                                    std::regex("values.template emplace_back"),
                                    "values.emplace_back");
    code_extra = std::regex_replace(code_extra, std::regex("std::list"),
                                    "std::pmr::list");
    code_extra =
        std::regex_replace(code_extra, std::regex("std::set"), "std::pmr::set");
    code += code_extra;
    return code;
  }

  static std::string GenerateFBEFieldModelCustomMap_Header() {
    std::string code = R"CODE(
template <typename TKey, typename TValue, typename TKStruct, typename TValueStruct>
class FieldModelCustomMap
{
public:
    FieldModelCustomMap(FBEBuffer& buffer, size_t offset) noexcept : _buffer(buffer), _offset(offset) {}

    // Get the field offset
    size_t fbe_offset() const noexcept { return _offset; }
    // Get the field size
    size_t fbe_size() const noexcept { return 4; }
    // Get the field extra size
    size_t fbe_extra() const noexcept;

    // Shift the current field offset
    void fbe_shift(size_t size) noexcept { _offset += size; }
    // Unshift the current field offset
    void fbe_unshift(size_t size) noexcept { _offset -= size; }

    // Get the map offset
    size_t offset() const noexcept;
    // Get the map size
    size_t size() const noexcept;

    // Map index operator
    std::pair<TKey, TValue> operator[](size_t index) const noexcept;

    // Resize the map and get its first model
    std::pair<TKey, TValue> resize(size_t size);

    // Check if the map is valid
    bool verify() const noexcept;
)CODE";

    std::string code_extra = R"CODE(
    // Get the map as std::map
    void get(std::map<TKStruct, TValueStruct>& values, std::pmr::memory_resource* resource = nullptr) const noexcept;
    void get(std::map<TKStruct, TValueStruct*>& values, std::pmr::memory_resource* resource = nullptr) const noexcept;
    // Get the map as std::unordered_map
    void get(std::unordered_map<TKStruct, TValueStruct>& values, std::pmr::memory_resource* resource = nullptr) const noexcept;
    void get(std::unordered_map<TKStruct, TValueStruct*>& values, std::pmr::memory_resource* resource = nullptr) const noexcept;

    // Set the map as std::map
    void set(const std::map<TKStruct, TValueStruct>& values, std::pmr::memory_resource* resource) noexcept;
    void set(const std::map<TKStruct, TValueStruct*>& values, std::pmr::memory_resource* resource) noexcept;
    // Set the map as std::unordered_map
    void set(const std::unordered_map<TKStruct, TValueStruct>& values, std::pmr::memory_resource* resource) noexcept;
    void set(const std::unordered_map<TKStruct, TValueStruct*>& values, std::pmr::memory_resource* resource) noexcept;
)CODE";

    code += code_extra;

    code_extra =
        std::regex_replace(code_extra, std::regex("std::map"), "std::pmr::map");
    code_extra =
        std::regex_replace(code_extra, std::regex("std::unordered_map"),
                           "std::pmr::unordered_map");
    code += code_extra;

    code += R"CODE(
#if defined(USING_BTREE_MAP)
    // Get the map as FBE::map (btree_map with std allocator)
    void get(FBE::map<TKStruct, TValueStruct>& values, std::pmr::memory_resource* resource = nullptr) const noexcept;
    void get(FBE::map<TKStruct, TValueStruct*>& values, std::pmr::memory_resource* resource = nullptr) const noexcept;
    // Get the map as FBE::pmr::map (btree_map with pmr allocator)
    void get(FBE::pmr::map<TKStruct, TValueStruct>& values, std::pmr::memory_resource* resource = nullptr) const noexcept;
    void get(FBE::pmr::map<TKStruct, TValueStruct*>& values, std::pmr::memory_resource* resource = nullptr) const noexcept;
    // Set the map as FBE::map (btree_map with std allocator)
    void set(const FBE::map<TKStruct, TValueStruct>& values, std::pmr::memory_resource* resource) noexcept;
    void set(const FBE::map<TKStruct, TValueStruct*>& values, std::pmr::memory_resource* resource) noexcept;
    // Set the map as FBE::pmr::map (btree_map with pmr allocator)
    void set(const FBE::pmr::map<TKStruct, TValueStruct>& values, std::pmr::memory_resource* resource) noexcept;
    void set(const FBE::pmr::map<TKStruct, TValueStruct*>& values, std::pmr::memory_resource* resource) noexcept;
#endif

private:
    FBEBuffer& _buffer;
    size_t _offset;
};
)CODE";
    return code;
  }

  static std::string GenerateFBEFieldModelCustomMap_Inline() {
    std::string code = R"CODE(
template <typename TKey, typename TValue, typename TKStruct, typename TValueStruct>
inline size_t FieldModelCustomMap<TKey, TValue, TKStruct, TValueStruct>::fbe_extra() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return 0;

    uint32_t fbe_map_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    if ((fbe_map_offset == 0) || ((_buffer.offset() + fbe_map_offset + 4) > _buffer.size()))
        return 0;

    uint32_t fbe_map_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_map_offset);

    size_t fbe_result = 4;
    TKey fbe_model_key(_buffer, fbe_map_offset + 4);
    TValue fbe_model_value(_buffer, fbe_map_offset + 4 + fbe_model_key.fbe_size());
    for (size_t i = fbe_map_size; i-- > 0;)
    {
        fbe_result += fbe_model_key.fbe_size() + fbe_model_key.fbe_extra();
        fbe_model_key.fbe_shift(fbe_model_key.fbe_size() + fbe_model_value.fbe_size());
        fbe_result += fbe_model_value.fbe_size() + fbe_model_value.fbe_extra();
        fbe_model_value.fbe_shift(fbe_model_key.fbe_size() + fbe_model_value.fbe_size());
    }
    return fbe_result;
}

template <typename TKey, typename TValue, typename TKStruct, typename TValueStruct>
inline size_t FieldModelCustomMap<TKey, TValue, TKStruct, TValueStruct>::offset() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return 0;

    uint32_t fbe_map_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    return fbe_map_offset;
}

template <typename TKey, typename TValue, typename TKStruct, typename TValueStruct>
inline size_t FieldModelCustomMap<TKey, TValue, TKStruct, TValueStruct>::size() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return 0;

    uint32_t fbe_map_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    if ((fbe_map_offset == 0) || ((_buffer.offset() + fbe_map_offset + 4) > _buffer.size()))
        return 0;

    uint32_t fbe_map_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_map_offset);
    return fbe_map_size;
}

template <typename TKey, typename TValue, typename TKStruct, typename TValueStruct>
inline std::pair<TKey, TValue> FieldModelCustomMap<TKey, TValue, TKStruct, TValueStruct>::operator[](size_t index) const noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");

    uint32_t fbe_map_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    assert(((fbe_map_offset > 0) && ((_buffer.offset() + fbe_map_offset + 4) <= _buffer.size())) && "Model is broken!");

    [[maybe_unused]] uint32_t fbe_map_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_map_offset);
    assert((index < fbe_map_size) && "Index is out of bounds!");

    TKey fbe_model_key(_buffer, fbe_map_offset + 4);
    TValue fbe_model_value(_buffer, fbe_map_offset + 4 + fbe_model_key.fbe_size());
    fbe_model_key.fbe_shift(index * (fbe_model_key.fbe_size() + fbe_model_value.fbe_size()));
    fbe_model_value.fbe_shift(index * (fbe_model_key.fbe_size() + fbe_model_value.fbe_size()));
    return std::make_pair(fbe_model_key, fbe_model_value);
}

template <typename TKey, typename TValue, typename TKStruct, typename TValueStruct>
inline std::pair<TKey, TValue> FieldModelCustomMap<TKey, TValue, TKStruct, TValueStruct>::resize(size_t size)
{
    TKey fbe_model_key(_buffer, fbe_offset());
    TValue fbe_model_value(_buffer, fbe_offset() + fbe_model_key.fbe_size());

    uint32_t fbe_map_size = (uint32_t)(size * (fbe_model_key.fbe_size() + fbe_model_value.fbe_size()));
    uint32_t fbe_map_offset = (uint32_t)(_buffer.allocate(4 + fbe_map_size) - _buffer.offset());
    assert(((fbe_map_offset > 0) && ((_buffer.offset() + fbe_map_offset + 4 + fbe_map_size) <= _buffer.size())) && "Model is broken!");

    unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset(), fbe_map_offset);
    unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_map_offset, size);

    memset((char*)(_buffer.data() + _buffer.offset() + fbe_map_offset + 4), 0, fbe_map_size);

    return std::make_pair(TKey(_buffer, fbe_map_offset + 4), TValue(_buffer, fbe_map_offset + 4 + fbe_model_key.fbe_size()));
}

template <typename TKey, typename TValue, typename TKStruct, typename TValueStruct>
inline bool FieldModelCustomMap<TKey, TValue, TKStruct, TValueStruct>::verify() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return true;

    uint32_t fbe_map_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    if (fbe_map_offset == 0)
        return true;

    if ((_buffer.offset() + fbe_map_offset + 4) > _buffer.size())
        return false;

    uint32_t fbe_map_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_map_offset);

    TKey fbe_model_key(_buffer, fbe_map_offset + 4);
    TValue fbe_model_value(_buffer, fbe_map_offset + 4 + fbe_model_key.fbe_size());
    for (size_t i = fbe_map_size; i-- > 0;)
    {
        if (!fbe_model_key.verify())
            return false;
        fbe_model_key.fbe_shift(fbe_model_key.fbe_size() + fbe_model_value.fbe_size());
        if (!fbe_model_value.verify())
            return false;
        fbe_model_value.fbe_shift(fbe_model_key.fbe_size() + fbe_model_value.fbe_size());
    }

    return true;
}
)CODE";

    std::string code_extra = R"CODE(
template <typename TKey, typename TValue, typename TKStruct, typename TValueStruct>
inline void FieldModelCustomMap<TKey, TValue, TKStruct, TValueStruct>::get(std::map<TKStruct, TValueStruct>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();

    size_t fbe_map_size = size();
    if (fbe_map_size == 0)
        return;

    auto fbe_model = (*this)[0];
    for (size_t i = fbe_map_size; i-- > 0;)
    {
        TKStruct key;
        TValueStruct value;
        fbe_model.first.get(key, nullptr);
        fbe_model.second.get(value, nullptr);
        values.emplace(std::move(key), std::move(value));
        fbe_model.first.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
        fbe_model.second.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
    }
}

template <typename TKey, typename TValue, typename TKStruct, typename TValueStruct>
inline void FieldModelCustomMap<TKey, TValue, TKStruct, TValueStruct>::get(std::map<TKStruct, TValueStruct*>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();

    size_t fbe_map_size = size();
    if (fbe_map_size == 0)
        return;

    auto fbe_model = (*this)[0];
    for (size_t i = fbe_map_size; i-- > 0;)
    {
        TKStruct key;
        TValueStruct* value = nullptr;
        fbe_model.first.get(key, nullptr);
        fbe_model.second.get(&value, nullptr);
        values.emplace(std::move(key), value);
        fbe_model.first.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
        fbe_model.second.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
    }
}

template <typename TKey, typename TValue, typename TKStruct, typename TValueStruct>
inline void FieldModelCustomMap<TKey, TValue, TKStruct, TValueStruct>::get(std::unordered_map<TKStruct, TValueStruct>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();

    size_t fbe_map_size = size();
    if (fbe_map_size == 0)
        return;

    auto fbe_model = (*this)[0];
    for (size_t i = fbe_map_size; i-- > 0;)
    {
        TKStruct key;
        TValueStruct value;
        fbe_model.first.get(key, nullptr);
        fbe_model.second.get(value, nullptr);
        values.emplace(std::move(key), std::move(value));
        fbe_model.first.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
        fbe_model.second.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
    }
}

template <typename TKey, typename TValue, typename TKStruct, typename TValueStruct>
inline void FieldModelCustomMap<TKey, TValue, TKStruct, TValueStruct>::get(std::unordered_map<TKStruct, TValueStruct*>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();

    size_t fbe_map_size = size();
    if (fbe_map_size == 0)
        return;

    auto fbe_model = (*this)[0];
    for (size_t i = fbe_map_size; i-- > 0;)
    {
        TKStruct key;
        TValueStruct* value = nullptr;
        fbe_model.first.get(key, nullptr);
        fbe_model.second.get(&value, nullptr);
        values.emplace(std::move(key), value);
        fbe_model.first.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
        fbe_model.second.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
    }
}

template <typename TKey, typename TValue, typename TKStruct, typename TValueStruct>
inline void FieldModelCustomMap<TKey, TValue, TKStruct, TValueStruct>::set(const std::map<TKStruct, TValueStruct>& values, std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.first.set(value.first, resource);
        fbe_model.first.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
        fbe_model.second.set(value.second, resource);
        fbe_model.second.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
    }
}

template <typename TKey, typename TValue, typename TKStruct, typename TValueStruct>
inline void FieldModelCustomMap<TKey, TValue, TKStruct, TValueStruct>::set(const std::map<TKStruct, TValueStruct*>& values, std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.first.set(value.first, resource);
        fbe_model.first.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
        fbe_model.second.set(value.second, resource);
        fbe_model.second.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
    }
}

template <typename TKey, typename TValue, typename TKStruct, typename TValueStruct>
inline void FieldModelCustomMap<TKey, TValue, TKStruct, TValueStruct>::set(const std::unordered_map<TKStruct, TValueStruct>& values, std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.first.set(value.first, resource);
        fbe_model.first.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
        fbe_model.second.set(value.second, resource);
        fbe_model.second.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
    }
}

template <typename TKey, typename TValue, typename TKStruct, typename TValueStruct>
inline void FieldModelCustomMap<TKey, TValue, TKStruct, TValueStruct>::set(const std::unordered_map<TKStruct, TValueStruct*>& values, std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.first.set(value.first, resource);
        fbe_model.first.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
        fbe_model.second.set(value.second, resource);
        fbe_model.second.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
    }
}
)CODE";

    code += code_extra;

    code_extra =
        std::regex_replace(code_extra, std::regex("std::map"), "std::pmr::map");
    code_extra =
        std::regex_replace(code_extra, std::regex("std::unordered_map"),
                           "std::pmr::unordered_map");
    code += code_extra;

    code += R"CODE(
#if defined(USING_BTREE_MAP)
template <typename TKey, typename TValue, typename TKStruct, typename TValueStruct>
inline void FieldModelCustomMap<TKey, TValue, TKStruct, TValueStruct>::get(FBE::map<TKStruct, TValueStruct>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();

    size_t fbe_map_size = size();
    if (fbe_map_size == 0)
        return;

    auto fbe_model = (*this)[0];
    for (size_t i = fbe_map_size; i-- > 0;)
    {
        TKStruct key;
        TValueStruct value;
        fbe_model.first.get(key, nullptr);
        fbe_model.second.get(value, nullptr);
        values.emplace(std::move(key), std::move(value));
        fbe_model.first.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
        fbe_model.second.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
    }
}

template <typename TKey, typename TValue, typename TKStruct, typename TValueStruct>
inline void FieldModelCustomMap<TKey, TValue, TKStruct, TValueStruct>::get(FBE::map<TKStruct, TValueStruct*>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();

    size_t fbe_map_size = size();
    if (fbe_map_size == 0)
        return;

    auto fbe_model = (*this)[0];
    for (size_t i = fbe_map_size; i-- > 0;)
    {
        TKStruct key;
        TValueStruct* value = nullptr;
        fbe_model.first.get(key, nullptr);
        fbe_model.second.get(&value, nullptr);
        values.emplace(std::move(key), value);
        fbe_model.first.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
        fbe_model.second.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
    }
}

template <typename TKey, typename TValue, typename TKStruct, typename TValueStruct>
inline void FieldModelCustomMap<TKey, TValue, TKStruct, TValueStruct>::set(const FBE::map<TKStruct, TValueStruct>& values, std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.first.set(value.first, resource);
        fbe_model.first.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
        fbe_model.second.set(value.second, resource);
        fbe_model.second.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
    }
}

template <typename TKey, typename TValue, typename TKStruct, typename TValueStruct>
inline void FieldModelCustomMap<TKey, TValue, TKStruct, TValueStruct>::set(const FBE::map<TKStruct, TValueStruct*>& values, std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.first.set(value.first, resource);
        fbe_model.first.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
        fbe_model.second.set(value.second, resource);
        fbe_model.second.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
    }
}

template <typename TKey, typename TValue, typename TKStruct, typename TValueStruct>
inline void FieldModelCustomMap<TKey, TValue, TKStruct, TValueStruct>::get(FBE::pmr::map<TKStruct, TValueStruct>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();

    size_t fbe_map_size = size();
    if (fbe_map_size == 0)
        return;

    auto fbe_model = (*this)[0];
    for (size_t i = fbe_map_size; i-- > 0;)
    {
        TKStruct key = TKStruct();
        TValueStruct value = TValueStruct();
        fbe_model.first.get(key, resource);
        fbe_model.second.get(value, resource);
        values.emplace(std::move(key), std::move(value));
        fbe_model.first.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
        fbe_model.second.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
    }
}

template <typename TKey, typename TValue, typename TKStruct, typename TValueStruct>
inline void FieldModelCustomMap<TKey, TValue, TKStruct, TValueStruct>::get(FBE::pmr::map<TKStruct, TValueStruct*>& values, std::pmr::memory_resource* resource) const noexcept
{
    values.clear();

    size_t fbe_map_size = size();
    if (fbe_map_size == 0)
        return;

    auto fbe_model = (*this)[0];
    for (size_t i = fbe_map_size; i-- > 0;)
    {
        TKStruct key = TKStruct();
        TValueStruct* value = nullptr;
        fbe_model.first.get(key, resource);
        fbe_model.second.get(&value, resource);
        values.emplace(std::move(key), value);
        fbe_model.first.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
        fbe_model.second.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
    }
}

template <typename TKey, typename TValue, typename TKStruct, typename TValueStruct>
inline void FieldModelCustomMap<TKey, TValue, TKStruct, TValueStruct>::set(const FBE::pmr::map<TKStruct, TValueStruct>& values, std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.first.set(value.first, resource);
        fbe_model.first.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
        fbe_model.second.set(value.second, resource);
        fbe_model.second.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
    }
}

template <typename TKey, typename TValue, typename TKStruct, typename TValueStruct>
inline void FieldModelCustomMap<TKey, TValue, TKStruct, TValueStruct>::set(const FBE::pmr::map<TKStruct, TValueStruct*>& values, std::pmr::memory_resource* resource) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.first.set(value.first, resource);
        fbe_model.first.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
        fbe_model.second.set(value.second, resource);
        fbe_model.second.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
    }
}
#endif
)CODE";
    return code;
  }
};
} // namespace FBE

#endif // GENERATOR_CPP_FIXTURE_H
