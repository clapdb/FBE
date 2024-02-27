//------------------------------------------------------------------------------
// Automatically generated by the Fast Binary Encoding compiler, do not modify!
// https://github.com/chronoxor/FastBinaryEncoding
// Source: FBE
// FBE version: 1.11.0.0
//------------------------------------------------------------------------------

#pragma once

#ifdef isset
#undef isset
#endif

#if defined(__clang__)
#pragma clang system_header
#elif defined(__GNUC__)
#pragma GCC system_header
#elif defined(_MSC_VER)
#pragma system_header
#endif

#include "fbe.h"

namespace FBE {

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
    void get(T& value, pmr::memory_resource* resource, T defaults = (T)0) const noexcept;
    // Set the field value
    void set(T value, pmr::memory_resource* resource) noexcept;

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
auto variant_emplace_value(V& fbe_value, pmr::memory_resource* resource) {
    if constexpr (std::is_integral_v<T> or std::is_floating_point_v<T> or std::is_enum_v<T> or std::is_same_v<T, std::string> or not std::is_constructible_v<T, pmr::memory_resource*>) {
        fbe_value.template emplace<T>();
    } else {
        fbe_value.template emplace<T>(resource);
    }
}

template<typename M, typename T>
auto variant_get_value(M& model, T& value, pmr::memory_resource* resource) {
    if constexpr (std::is_integral_v<T> or std::is_floating_point_v<T> or std::is_enum_v<T> or std::is_same_v<T, std::string> or not (std::is_constructible_v<T, pmr::memory_resource*> or is_variant_v<T>)) {
        model.get(value, nullptr);
    } else {
        model.get(value, resource);
    }
}

template<typename M, typename T>
auto variant_get_value(M& model, T** value, pmr::memory_resource* resource) {
    if constexpr (std::is_integral_v<T> or std::is_floating_point_v<T> or std::is_enum_v<T> or std::is_same_v<T, std::string> or not (std::is_constructible_v<T, pmr::memory_resource*> or is_variant_v<T>)) {
        model.get(value, nullptr);
    } else {
        model.get(value, resource);
    }
}

template<typename M, typename T>
auto variant_set_value(M* model, T& value, pmr::memory_resource* resource) {
    if constexpr (std::is_integral_v<T> or std::is_floating_point_v<T> or std::is_enum_v<T> or std::is_same_v<T, std::string> or not (std::is_constructible_v<T, pmr::memory_resource*> or is_variant_v<T>)) {
        model->set(value, nullptr);
    } else {
        model->set(value, resource);
    }
}



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
    void get(decimal_t& value, pmr::memory_resource* resource, decimal_t defaults = decimal_t()) const noexcept;
    // Set the decimal value
    void set(decimal_t value, pmr::memory_resource* resource) noexcept;

private:
    FBEBuffer& _buffer;
    size_t _offset;

    static uint64_t extract(double a) noexcept;
    static uint64_t uint32x32(uint32_t a, uint32_t b) noexcept;
    static void uint64x64(uint64_t a, uint64_t b, uint64_t& low64, uint32_t& high32) noexcept;
};

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
    void get(uuid_t& value, pmr::memory_resource* resource, uuid_t defaults = uuid_t::nil()) const noexcept;
    // Set the UUID value
    void set(uuid_t value, pmr::memory_resource* resource) noexcept;

private:
    FBEBuffer& _buffer;
    size_t _offset;
};

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
    size_t get(void* data, size_t size, pmr::memory_resource* resource) const noexcept;
    // Get the bytes value
    template <size_t N>
    size_t get(uint8_t (&data)[N], pmr::memory_resource* resource) const noexcept { return get(data, N, resource); }
    // Get the bytes value
    template <size_t N>
    size_t get(std::array<uint8_t, N>& data, pmr::memory_resource* resource) const noexcept { return get(data.data(), data.size(), resource); }
    // Get the bytes value
    void get(FastVec<uint8_t>& value, pmr::memory_resource* resource) const noexcept;
    // Get the bytes value
    void get(buffer_t& value, pmr::memory_resource* resource) const noexcept { get(value.buffer(), resource); }

    // Set the bytes value
    void set(const void* data, size_t size, pmr::memory_resource* resource);
    // Set the bytes value
    template <size_t N>
    void set(const uint8_t (&data)[N], pmr::memory_resource* resource) { set(data, N, resource); }
    // Set the bytes value
    template <size_t N>
    void set(const std::array<uint8_t, N>& data, pmr::memory_resource* resource) { set(data.data(), data.size(), resource); }
    // Set the bytes value
    void set(const FastVec<uint8_t>& value, pmr::memory_resource* resource) { set(value.data(), value.size(), resource); }
    // Set the bytes value
    void set(const buffer_t& value, pmr::memory_resource* resource) { set(value.buffer(), resource); }

private:
    FBEBuffer& _buffer;
    size_t _offset;
};

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
    size_t get(uint8_t (&data)[N], pmr::memory_resource* resource) const noexcept { return get(data, N); }
    // Get the bytes value
    void get(pmr::vector<uint8_t>& value, pmr::memory_resource* resource) const noexcept;
    // Get the bytes value
    void get(pmr_buffer_t& value, pmr::memory_resource* resource) const noexcept { get(value.buffer(), resource); }

    // Set the bytes value
    void set(const void* data, size_t size, pmr::memory_resource* resource);
    // Set the bytes value
    template <size_t N>
    void set(const uint8_t (&data)[N], pmr::memory_resource* resource) { set(data, N, resource); }
    // Set the bytes value
    void set(const pmr::vector<uint8_t>& value, pmr::memory_resource* resource) { set(value.data(), value.size(), resource); }
    // Set the bytes value
    void set(const pmr_buffer_t& value, pmr::memory_resource* resource) { set(value.buffer(), resource); }

private:
    FBEBuffer& _buffer;
    size_t _offset;
};

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
    size_t get(char (&data)[N], pmr::memory_resource* resource) const noexcept { return get(data, N); }
    // Get the string value
    template <size_t N>
    size_t get(std::array<char, N>& data, pmr::memory_resource* resource) const noexcept { return get(data.data(), data.size()); }
    // Get the pmr string value
    void get(FBEString& value, pmr::memory_resource* resource) const noexcept;
    // Get the pmr string value
    void get(FBEString& value, pmr::memory_resource* resource, const FBEString& defaults) const noexcept;

    // Set the string value
    void set(const char* data, size_t size, pmr::memory_resource* resource);
    // Set the string value
    template <size_t N>
    void set(const char (&data)[N], pmr::memory_resource* resource) { set(data, N); }
    // Set the string value
    template <size_t N>
    void set(const std::array<char, N>& data, pmr::memory_resource* resource) { set(data.data(), data.size()); }
    // Set the string value
    void set(const FBEString& value, pmr::memory_resource* resource);

private:
    FBEBuffer& _buffer;
    size_t _offset;
};

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
    size_t get(char (&data)[N], pmr::memory_resource* resource) const noexcept { return get(data, N); }
    // Get the string value
    template <size_t N>
    size_t get(std::array<char, N>& data, pmr::memory_resource* resource) const noexcept { return get(data.data(), data.size()); }
    // Get the pmr string value
    void get(ArenaString& value, pmr::memory_resource* resource) const noexcept;
    // Get the pmr string value
    void get(ArenaString& value, pmr::memory_resource* resource, const ArenaString& defaults) const noexcept;

    // Set the string value
    void set(const char* data, size_t size, pmr::memory_resource* resource);
    // Set the string value
    template <size_t N>
    void set(const char (&data)[N], pmr::memory_resource* resource) { set(data, N); }
    // Set the string value
    template <size_t N>
    void set(const std::array<char, N>& data, pmr::memory_resource* resource) { set(data.data(), data.size()); }
    // Set the string value
    void set(const ArenaString& value, pmr::memory_resource* resource);

private:
    FBEBuffer& _buffer;
    size_t _offset;
};

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
    void get(std::optional<T>& opt, pmr::memory_resource* resource, const std::optional<T>& defaults) const noexcept;
    // Get the optional value
    void get(std::optional<T>& opt, pmr::memory_resource* resource) const noexcept;

    // Set the optional value (begin phase)
    size_t set_begin(bool has_value);
    // Set the optional value (end phase)
    void set_end(size_t fbe_begin);

    // Set the optional value
    void set(const std::optional<T>& opt, pmr::memory_resource* resource);

private:
    FBEBuffer& _buffer;
    size_t _offset;

public:
    // Base field model value
    FieldModel<T> value;
};

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
    void get(T (&values)[S], pmr::memory_resource* resource = nullptr) const noexcept;
    // Get the array as std::array
    template <size_t S>
    void get(std::array<T, S>& values, pmr::memory_resource* resource = nullptr) const noexcept;
    // Get the array as FastVec
    void get(FastVec<T>& values, pmr::memory_resource* resource = nullptr) const noexcept;

    // Set the array as C-array
    template <size_t S>
    void set(const T (&values)[S], pmr::memory_resource* resource) noexcept;
    // Set the array as std::array
    template <size_t S>
    void set(const std::array<T, S>& values, pmr::memory_resource* resource) noexcept;
    // Set the array as FastVec
    void set(const FastVec<T>& values, pmr::memory_resource* resource) noexcept;

private:
    FBEBuffer& _buffer;
    size_t _offset;
    FieldModel<T> _model;
};

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
    void get(FastVec<T>& values, pmr::memory_resource* resource) const noexcept;
    // Get the vector as std::list
    void get(std::list<T>& values, pmr::memory_resource* resource) const noexcept;
    // Get the vector as std::set
    void get(std::set<T>& values, pmr::memory_resource* resource) const noexcept;

    // Get the vector as pmr::vector
    void get(pmr::vector<T>& values, pmr::memory_resource* resource) const noexcept;
    // Get the vector as pmr::list
    void get(pmr::list<T>& values, pmr::memory_resource* resource) const noexcept;
    // Get the vector as pmr::set
    void get(pmr::set<T>& values, pmr::memory_resource* resource) const noexcept;

    // Set the vector as FastVec
    void set(const FastVec<T>& values, pmr::memory_resource* resource) noexcept;
    // Set the vector as std::list
    void set(const std::list<T>& values, pmr::memory_resource* resource) noexcept;
    // Set the vector as std::set
    void set(const std::set<T>& values, pmr::memory_resource* resource) noexcept;

    // Set the vector as pmr::vector
    void set(const pmr::vector<T>& values, pmr::memory_resource* resource) noexcept;
    // Set the vector as pmr::list
    void set(const pmr::list<T>& values, pmr::memory_resource* resource) noexcept;
    // Set the vector as pmr::set
    void set(const pmr::set<T>& values, pmr::memory_resource* resource) noexcept;

private:
    FBEBuffer& _buffer;
    size_t _offset;
};

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
    void get(std::map<TKey, TValue>& values, pmr::memory_resource* resource = nullptr) const noexcept;
    // Get the map as std::unordered_map
    void get(std::unordered_map<TKey, TValue>& values, pmr::memory_resource* resource = nullptr) const noexcept;

    // Get the map as pmr::map
    void get(pmr::map<TKey, TValue>& values, pmr::memory_resource* resource) const noexcept;
    // Get the map as pmr::unordered_map
    void get(pmr::unordered_map<TKey, TValue>& values, pmr::memory_resource* resource) const noexcept;

    // Set the map as std::map
    void set(const std::map<TKey, TValue>& values, pmr::memory_resource* resource) noexcept;
    // Set the map as std::unordered_map
    void set(const std::unordered_map<TKey, TValue>& values, pmr::memory_resource* resource) noexcept;

    // Set the map as pmr::map
    void set(const pmr::map<TKey, TValue>& values, pmr::memory_resource* resource) noexcept;
    // Set the map as pmr::unordered_map
    void set(const pmr::unordered_map<TKey, TValue>& values, pmr::memory_resource* resource) noexcept;

private:
    FBEBuffer& _buffer;
    size_t _offset;
};

} // namespace FBE

#include "fbe_models.inl"
