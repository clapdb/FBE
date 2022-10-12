/*!
    \file generator_cpp.cpp
    \brief Fast binary encoding C++ generator implementation
    \author Ivan Shynkarenka
    \date 20.04.2018
    \copyright MIT License
*/

namespace FBE {

void GeneratorCpp::GenerateFBEPtr_Header(const CppCommon::Path& path)
{
    // Create package path
    CppCommon::Directory::CreateTree(path);

    // Generate the common file
    CppCommon::Path common = path / "fbe_ptr.h";
    WriteBegin();

    // Generate common header
    GenerateHeader("FBE");

    // Generate namespace begin
    WriteLine();
    WriteLineIndent("namespace FBE {");

    std::string code = R"CODE(
struct Base
{
    virtual ~Base() = default;
};
)CODE";

    // Prepare code template
    code = std::regex_replace(code, std::regex("\n"), EndLine());

    Write(code);

    // Generate namespace end
    WriteLine();
    WriteLineIndent("} // namespace FBE");

    // Generate common footer
    GenerateFooter();

    // Store the common file
    WriteEnd();
    Store(common);
}

void GeneratorCpp::GenerateFBEBaseFieldModel_Header()
{
    std::string code = R"CODE(
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
    virtual void set(const ::FBE::Base& fbe_value) noexcept = 0;
    // Set the struct fields values
    virtual void set_fields(const ::FBE::Base& fbe_value) noexcept = 0;

    // Get the struct value
    virtual void get(::FBE::Base& fbe_value) noexcept = 0;
    // Get the struct fields values
    virtual void get_fields(::FBE::Base& fbe_value, size_t fbe_struct_size) noexcept = 0;
};
)CODE";

    // Prepare code template
    code = std::regex_replace(code, std::regex("\n"), EndLine());

    Write(code);
}

void GeneratorCpp::GenerateFBEFieldModelStructOptional_Header()
{
    std::string code = R"CODE(
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
    void get(std::optional<TStruct>& opt) noexcept;

    // Set the optional value (begin phase)
    size_t set_begin(bool has_value);
    // Set the optional value (end phase)
    void set_end(size_t fbe_begin);

    // Set the optional value
    void set(const std::optional<TStruct>& opt);

private:
    FBEBuffer& _buffer;
    size_t _offset;

public:
    // Base field model value
    T value;
};
)CODE";

    // Prepare code template
    code = std::regex_replace(code, std::regex("\n"), EndLine());

    Write(code);
}

void GeneratorCpp::GenerateFBEFieldModelStructOptional_Inline()
{
    std::string code = R"CODE(
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
inline void FieldModelStructOptional<T, TStruct>::get(std::optional<TStruct>& opt) noexcept
{

    size_t fbe_begin = get_begin();
    if (fbe_begin == 0)
        return;

    TStruct temp = TStruct();
    value.get(temp);
    opt.emplace(std::move(temp));

    get_end(fbe_begin);
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
inline void FieldModelStructOptional<T, TStruct>::set(const std::optional<TStruct>& opt)
{
    size_t fbe_begin = set_begin(opt.has_value());
    if (fbe_begin == 0)
        return;

    if (opt)
        value.set(opt.value());

    set_end(fbe_begin);
}
)CODE";

    // Prepare code template
    code = std::regex_replace(code, std::regex("\n"), EndLine());

    Write(code);
}

void GeneratorCpp::GenerateFBEFieldModelCustomArray_Header()
{
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
    void get(TStruct (&values)[S]) const noexcept;
    template <size_t S>
    void get(TStruct* (&values)[S]) const noexcept;
    // Get the array as std::array
    template <size_t S>
    void get(std::array<TStruct, S>& values) const noexcept;
    template <size_t S>
    void get(std::array<TStruct*, S>& values) const noexcept;

    // Set the array as C-array
    template <size_t S>
    void set(const TStruct (&values)[S]) noexcept;
    template <size_t S>
    void set(const TStruct* (&values)[S]) noexcept;
    
    // Set the array as std::array
    template <size_t S>
    void set(const std::array<TStruct, S>& values) noexcept;
    template <size_t S>
    void set(const std::array<TStruct*, S>& values) noexcept;
)CODE";

    std::string code_extra = R"CODE(
    // Get the array as std::vector
    void get(std::vector<TStruct>& values) const noexcept;
    void get(std::vector<TStruct*>& values) const noexcept;

    // Set the array as std::vector
    void set(const std::vector<TStruct>& values) noexcept;
    void set(const std::vector<TStruct*>& values) noexcept;
)CODE";

    code += code_extra;

    code_extra = std::regex_replace(code_extra, std::regex("std::vector"), "pmr::vector");
    code_extra = std::regex_replace(code_extra, std::regex("std::list"), "pmr::list");
    code += code_extra;

    code += R"CODE(
private:
    FBEBuffer& _buffer;
    size_t _offset;
    T _model;
};
)CODE";

    // Prepare code template
    code = std::regex_replace(code, std::regex("\n"), EndLine());

    Write(code);
}

void GeneratorCpp::GenerateFBEFieldModelCustomArray_Inline()
{
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
inline void FieldModelCustomArray<T, TStruct, N>::get(TStruct (&values)[S]) const noexcept
{
    auto fbe_model = (*this)[0];
    for (size_t i = 0; (i < S) && (i < N); ++i)
    {
        fbe_model.get(values[i]);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct, size_t N>
template <size_t S>
inline void FieldModelCustomArray<T, TStruct, N>::get(TStruct* (&values)[S]) const noexcept
{
    auto fbe_model = (*this)[0];
    for (size_t i = 0; (i < S) && (i < N); ++i)
    {
        TStruct* value = new TStruct();
        fbe_model.get(&value);
        values[i] = value;
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct, size_t N>
template <size_t S>
inline void FieldModelCustomArray<T, TStruct, N>::get(std::array<TStruct, S>& values) const noexcept
{
    auto fbe_model = (*this)[0];
    for (size_t i = 0; (i < S) && (i < N); ++i)
    {
        fbe_model.get(values[i]);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct, size_t N>
template <size_t S>
inline void FieldModelCustomArray<T, TStruct, N>::get(std::array<TStruct*, S>& values) const noexcept
{
    auto fbe_model = (*this)[0];
    for (size_t i = 0; (i < S) && (i < N); ++i)
    {
        TStruct* value = new TStruct();
        fbe_model.get(&value);
        values[i] = value;
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct, size_t N>
template <size_t S>
inline void FieldModelCustomArray<T, TStruct, N>::set(const TStruct (&values)[S]) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = (*this)[0];
    for (size_t i = 0; (i < S) && (i < N); ++i)
    {
        fbe_model.set(values[i]);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct, size_t N>
template <size_t S>
inline void FieldModelCustomArray<T, TStruct, N>::set(const TStruct* (&values)[S]) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = (*this)[0];
    for (size_t i = 0; (i < S) && (i < N); ++i)
    {
        fbe_model.set(values[i]);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct, size_t N>
template <size_t S>
inline void FieldModelCustomArray<T, TStruct, N>::set(const std::array<TStruct, S>& values) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = (*this)[0];
    for (size_t i = 0; (i < S) && (i < N); ++i)
    {
        fbe_model.set(values[i]);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct, size_t N>
template <size_t S>
inline void FieldModelCustomArray<T, TStruct, N>::set(const std::array<TStruct*, S>& values) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = (*this)[0];
    for (size_t i = 0; (i < S) && (i < N); ++i)
    {
        fbe_model.set(values[i]);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}
)CODE";

std::string code_extra = R"CODE(
template <typename T, typename TStruct, size_t N>
inline void FieldModelCustomArray<T, TStruct, N>::get(std::vector<TStruct>& values) const noexcept
{
    values.clear();
    values.reserve(N);

    auto fbe_model = (*this)[0];
    for (size_t i = N; i-- > 0;)
    {
        TStruct value;
        fbe_model.get(value);
        values.emplace_back(std::move(value));
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct, size_t N>
inline void FieldModelCustomArray<T, TStruct, N>::get(std::vector<TStruct*>& values) const noexcept
{
    values.clear();
    values.reserve(N);

    auto fbe_model = (*this)[0];
    for (size_t i = N; i-- > 0;)
    {
        TStruct* value = new TStruct();
        fbe_model.get(&value);
        values.emplace_back(value);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct, size_t N>
inline void FieldModelCustomArray<T, TStruct, N>::set(const std::vector<TStruct>& values) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = (*this)[0];
    for (size_t i = 0; (i < values.size()) && (i < N); ++i)
    {
        fbe_model.set(values[i]);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct, size_t N>
inline void FieldModelCustomArray<T, TStruct, N>::set(const std::vector<TStruct*>& values) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = (*this)[0];
    for (size_t i = 0; (i < values.size()) && (i < N); ++i)
    {
        fbe_model.set(values[i]);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}
)CODE";
    code += code_extra;

    code_extra = std::regex_replace(code_extra, std::regex("std::vector"), "pmr::vector");
    code_extra = std::regex_replace(code_extra, std::regex("std::list"), "pmr::list");
    code += code_extra;

    // Prepare code template
    code = std::regex_replace(code, std::regex("\n"), EndLine());


    Write(code);
}

void GeneratorCpp::GenerateFBEFieldModelCustomVector_Header()
{
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
    // Get the vector as std::vector
    void get(std::vector<TStruct>& values) const noexcept;
    void get(std::vector<TStruct*>& values) const noexcept;
    // Get the vector as std::list
    void get(std::list<TStruct>& values) const noexcept;
    void get(std::list<TStruct*>& values) const noexcept;
    // Get the vector as std::set
    void get(std::set<TStruct>& values) const noexcept;
    void get(std::set<TStruct*>& values) const noexcept;

    // Set the vector as std::vector
    void set(const std::vector<TStruct>& values) noexcept;
    void set(const std::vector<TStruct*>& values) noexcept;
    // Set the vector as std::list
    void set(const std::list<TStruct>& values) noexcept;
    void set(const std::list<TStruct*>& values) noexcept;
    // Set the vector as std::set
    void set(const std::set<TStruct>& values) noexcept;
    void set(const std::set<TStruct*>& values) noexcept;
)CODE";

    code += code_extra;
    code_extra = std::regex_replace(code_extra, std::regex("std::vector"), "pmr::vector");
    code_extra = std::regex_replace(code_extra, std::regex("std::list"), "pmr::list");
    code_extra = std::regex_replace(code_extra, std::regex("std::set"), "pmr::set");
    code += code_extra;

code += R"CODE(
private:
    FBEBuffer& _buffer;
    size_t _offset;
};
)CODE";

    // Prepare code template
    code = std::regex_replace(code, std::regex("\n"), EndLine());

    Write(code);
}

void GeneratorCpp::GenerateFBEFieldModelCustomVector_Inline()
{
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
inline void FieldModelCustomVector<T, TStruct>::get(std::vector<TStruct>& values) const noexcept
{
    values.clear();

    size_t fbe_vector_size = size();
    if (fbe_vector_size == 0)
        return;

    values.reserve(fbe_vector_size);

    auto fbe_model = (*this)[0];
    for (size_t i = fbe_vector_size; i-- > 0;)
    {
        TStruct value = TStruct();
        fbe_model.get(value);
        values.emplace_back(std::move(value));
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct>
inline void FieldModelCustomVector<T, TStruct>::get(std::vector<TStruct*>& values) const noexcept
{
    values.clear();

    size_t fbe_vector_size = size();
    if (fbe_vector_size == 0)
        return;

    values.reserve(fbe_vector_size);

    auto fbe_model = (*this)[0];
    for (size_t i = fbe_vector_size; i-- > 0;)
    {
        TStruct* value = new TStruct();
        fbe_model.get(&value);
        values.emplace_back(value);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct>
inline void FieldModelCustomVector<T, TStruct>::get(std::list<TStruct>& values) const noexcept
{
    values.clear();

    size_t fbe_vector_size = size();
    if (fbe_vector_size == 0)
        return;

    auto fbe_model = (*this)[0];
    for (size_t i = fbe_vector_size; i-- > 0;)
    {
        TStruct value = TStruct();
        fbe_model.get(value);
        values.emplace_back(std::move(value));
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct>
inline void FieldModelCustomVector<T, TStruct>::get(std::list<TStruct*>& values) const noexcept
{
    values.clear();

    size_t fbe_vector_size = size();
    if (fbe_vector_size == 0)
        return;

    auto fbe_model = (*this)[0];
    for (size_t i = fbe_vector_size; i-- > 0;)
    {
        TStruct* value = new TStruct();
        fbe_model.get(&value);
        values.emplace_back(value);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct>
inline void FieldModelCustomVector<T, TStruct>::get(std::set<TStruct>& values) const noexcept
{
    values.clear();

    size_t fbe_vector_size = size();
    if (fbe_vector_size == 0)
        return;

    auto fbe_model = (*this)[0];
    for (size_t i = fbe_vector_size; i-- > 0;)
    {
        TStruct value = TStruct();
        fbe_model.get(value);
        values.emplace(std::move(value));
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct>
inline void FieldModelCustomVector<T, TStruct>::get(std::set<TStruct*>& values) const noexcept
{
    values.clear();

    size_t fbe_vector_size = size();
    if (fbe_vector_size == 0)
        return;

    auto fbe_model = (*this)[0];
    for (size_t i = fbe_vector_size; i-- > 0;)
    {
        TStruct* value = new TStruct();
        fbe_model.get(&value);
        values.emplace(value);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct>
inline void FieldModelCustomVector<T, TStruct>::set(const std::vector<TStruct>& values) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.set(value);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct>
inline void FieldModelCustomVector<T, TStruct>::set(const std::vector<TStruct*>& values) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.set(value);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct>
inline void FieldModelCustomVector<T, TStruct>::set(const std::list<TStruct>& values) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.set(value);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct>
inline void FieldModelCustomVector<T, TStruct>::set(const std::list<TStruct*>& values) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.set(value);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct>
inline void FieldModelCustomVector<T, TStruct>::set(const std::set<TStruct>& values) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.set(value);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}

template <typename T, typename TStruct>
inline void FieldModelCustomVector<T, TStruct>::set(const std::set<TStruct*>& values) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.set(value);
        fbe_model.fbe_shift(fbe_model.fbe_size());
    }
}
)CODE";

    code += code_extra;

    code_extra = std::regex_replace(code_extra, std::regex("std::vector"), "pmr::vector");
    code_extra = std::regex_replace(code_extra, std::regex("std::list"), "pmr::list");
    code_extra = std::regex_replace(code_extra, std::regex("std::set"), "pmr::set");
    code += code_extra;

    // Prepare code template
    code = std::regex_replace(code, std::regex("\n"), EndLine());

    Write(code);
}

void GeneratorCpp::GenerateFBEFieldModelCustomMap_Header()
{
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
    void get(std::map<TKStruct, TValueStruct>& values) const noexcept;
    void get(std::map<TKStruct, TValueStruct*>& values) const noexcept;
    // Get the map as std::unordered_map
    void get(std::unordered_map<TKStruct, TValueStruct>& values) const noexcept;
    void get(std::unordered_map<TKStruct, TValueStruct*>& values) const noexcept;

    // Set the map as std::map
    void set(const std::map<TKStruct, TValueStruct>& values) noexcept;
    void set(const std::map<TKStruct, TValueStruct*>& values) noexcept;
    // Set the map as std::unordered_map
    void set(const std::unordered_map<TKStruct, TValueStruct>& values) noexcept;
    void set(const std::unordered_map<TKStruct, TValueStruct*>& values) noexcept;
)CODE";

    code += code_extra;

    code_extra = std::regex_replace(code_extra, std::regex("std::map"), "pmr::map");
    code_extra = std::regex_replace(code_extra, std::regex("std::unordered_map"), "pmr::unordered_map");
    code += code_extra;

    code += R"CODE(
private:
    FBEBuffer& _buffer;
    size_t _offset;
};
)CODE";

    // Prepare code template
    code = std::regex_replace(code, std::regex("\n"), EndLine());


    Write(code);
}

void GeneratorCpp::GenerateFBEFieldModelCustomMap_Inline()
{
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
inline void FieldModelCustomMap<TKey, TValue, TKStruct, TValueStruct>::get(std::map<TKStruct, TValueStruct>& values) const noexcept
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
        fbe_model.first.get(key);
        fbe_model.second.get(value);
        values.emplace(std::move(key), std::move(value));
        fbe_model.first.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
        fbe_model.second.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
    }
}

template <typename TKey, typename TValue, typename TKStruct, typename TValueStruct>
inline void FieldModelCustomMap<TKey, TValue, TKStruct, TValueStruct>::get(std::map<TKStruct, TValueStruct*>& values) const noexcept
{
    values.clear();

    size_t fbe_map_size = size();
    if (fbe_map_size == 0)
        return;

    auto fbe_model = (*this)[0];
    for (size_t i = fbe_map_size; i-- > 0;)
    {
        TKStruct key;
        TValueStruct* value = new TValueStruct();
        fbe_model.first.get(key);
        fbe_model.second.get(&value);
        values.emplace(std::move(key), value);
        fbe_model.first.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
        fbe_model.second.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
    }
}

template <typename TKey, typename TValue, typename TKStruct, typename TValueStruct>
inline void FieldModelCustomMap<TKey, TValue, TKStruct, TValueStruct>::get(std::unordered_map<TKStruct, TValueStruct>& values) const noexcept
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
        fbe_model.first.get(key);
        fbe_model.second.get(value);
        values.emplace(std::move(key), std::move(value));
        fbe_model.first.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
        fbe_model.second.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
    }
}

template <typename TKey, typename TValue, typename TKStruct, typename TValueStruct>
inline void FieldModelCustomMap<TKey, TValue, TKStruct, TValueStruct>::get(std::unordered_map<TKStruct, TValueStruct*>& values) const noexcept
{
    values.clear();

    size_t fbe_map_size = size();
    if (fbe_map_size == 0)
        return;

    auto fbe_model = (*this)[0];
    for (size_t i = fbe_map_size; i-- > 0;)
    {
        TKStruct key;
        TValueStruct* value = new TValueStruct();
        fbe_model.first.get(key);
        fbe_model.second.get(&value);
        values.emplace(std::move(key), value);
        fbe_model.first.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
        fbe_model.second.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
    }
}

template <typename TKey, typename TValue, typename TKStruct, typename TValueStruct>
inline void FieldModelCustomMap<TKey, TValue, TKStruct, TValueStruct>::set(const std::map<TKStruct, TValueStruct>& values) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.first.set(value.first);
        fbe_model.first.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
        fbe_model.second.set(value.second);
        fbe_model.second.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
    }
}

template <typename TKey, typename TValue, typename TKStruct, typename TValueStruct>
inline void FieldModelCustomMap<TKey, TValue, TKStruct, TValueStruct>::set(const std::map<TKStruct, TValueStruct*>& values) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.first.set(value.first);
        fbe_model.first.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
        fbe_model.second.set(value.second);
        fbe_model.second.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
    }
}

template <typename TKey, typename TValue, typename TKStruct, typename TValueStruct>
inline void FieldModelCustomMap<TKey, TValue, TKStruct, TValueStruct>::set(const std::unordered_map<TKStruct, TValueStruct>& values) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.first.set(value.first);
        fbe_model.first.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
        fbe_model.second.set(value.second);
        fbe_model.second.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
    }
}

template <typename TKey, typename TValue, typename TKStruct, typename TValueStruct>
inline void FieldModelCustomMap<TKey, TValue, TKStruct, TValueStruct>::set(const std::unordered_map<TKStruct, TValueStruct*>& values) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    auto fbe_model = resize(values.size());
    for (const auto& value : values)
    {
        fbe_model.first.set(value.first);
        fbe_model.first.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
        fbe_model.second.set(value.second);
        fbe_model.second.fbe_shift(fbe_model.first.fbe_size() + fbe_model.second.fbe_size());
    }
}
)CODE";

    code += code_extra;

    code_extra = std::regex_replace(code_extra, std::regex("std::map"), "pmr::map");
    code_extra = std::regex_replace(code_extra, std::regex("std::unordered_map"), "pmr::unordered_map");
    code += code_extra;

    // Prepare code template
    code = std::regex_replace(code, std::regex("\n"), EndLine());

    Write(code);
}

void GeneratorCpp::GenerateFBECustomModels_Header(const CppCommon::Path& path)
{
    // Create package path
    CppCommon::Directory::CreateTree(path);

    // Generate the field models file
    CppCommon::Path common = path / "fbe_custom_models.h";
    WriteBegin();

    // Generate field models header
    GenerateHeader("FBE");

    // Generate imports
    GenerateImports("fbe_models.h");
    GenerateImports("fbe_ptr.h");

    // Generate namespace begin
    WriteLine();
    WriteLineIndent("namespace FBE {");

    // Generate field models
    GenerateFBEBaseFieldModel_Header();
    GenerateFBEFieldModelCustomArray_Header();
    GenerateFBEFieldModelCustomVector_Header();
    GenerateFBEFieldModelCustomMap_Header();
    GenerateFBEFieldModelStructOptional_Header();

    // Generate namespace end
    WriteLine();
    WriteLineIndent("} // namespace FBE");

    // Generate inline import
    GenerateImports("fbe_custom_models.inl");

    // Generate field models footer
    GenerateFooter();

    // Store the field models file
    WriteEnd();
    Store(common);
}

void GeneratorCpp::GenerateFBECustomModels_Inline(const CppCommon::Path& path)
{
    // Create package path
    CppCommon::Directory::CreateTree(path);

    // Generate the field models file
    CppCommon::Path common = path / "fbe_custom_models.inl";
    WriteBegin();

    // Generate field models inline
    GenerateInline("FBE");

    // Generate namespace begin
    WriteLine();
    WriteLineIndent("namespace FBE {");

    // Generate field models
    GenerateFBEFieldModelCustomArray_Inline();
    GenerateFBEFieldModelCustomVector_Inline();
    GenerateFBEFieldModelCustomMap_Inline();
    GenerateFBEFieldModelStructOptional_Inline();

    // Generate namespace end
    WriteLine();
    WriteLineIndent("} // namespace FBE");

    // Generate field models footer
    GenerateFooter();

    // Store the field models file
    WriteEnd();
    Store(common);
}

void GeneratorCpp::GeneratePtrPackage_Header(const std::shared_ptr<Package>& p)
{
    CppCommon::Path output = _output;

    // Create package path
    CppCommon::Directory::CreateTree(output);

    // Generate the output file
    output /= *p->name + "_ptr.h";
    WriteBegin();

    // Generate package header
    GenerateHeader(CppCommon::Path(_input).filename().string());

    // Generate imports
    GenerateImports(p);
    GenerateImports("fbe_ptr.h");

    // Generate namespace begin
    WriteLine();
    WriteLineIndent("namespace " + *p->name + " {");

    // Generate namespace body
    if (p->body)
    {
        // Generate child enums
        for (const auto& e : p->body->enums)
            GenerateEnum(p, e);

        // Generate child flags
        for (const auto& f : p->body->flags)
            GenerateFlags(p, f);

        if (!p->body->variants.empty()) {
            // forward declarations
            GeneratorStructForwardDeclaration(p->body->structs);
            for (const auto& v : p->body->variants)
            {
                GenerateVariantAlias(p, v);
            }
        }

        // Generate child structs
        for (const auto& s : p->body->structs)
            GeneratePtrStruct_Header(p, s);
    }

    // Generate namespace end
    WriteLine();
    WriteLineIndent("} // namespace " + *p->name);

    // Generate package footer
    GenerateFooter();

    // Store the output file
    WriteEnd();
    Store(output);
}

void GeneratorCpp::GeneratePtrPackage_Source(const std::shared_ptr<Package>& p)
{
    CppCommon::Path output = _output;

    // Create package path
    CppCommon::Directory::CreateTree(output);

    // Generate the output file
    output /= *p->name + "_ptr.cpp";
    WriteBegin();

    // Generate package source
    GenerateSource(CppCommon::Path(_input).filename().string());

    // Generate imports
    GenerateImports(*p->name + "_ptr.h");

    // Generate namespace begin
    WriteLine();
    WriteLineIndent("namespace " + *p->name + " {");

    // Generate namespace body
    if (p->body)
    {
        // Generate child enums
        for (const auto& e : p->body->enums)
        {
            // Generate enum output stream
            GenerateEnumOutputStream(e);

            // Generate enum logging stream
            if (Logging())
                GenerateEnumLoggingStream(e);
        }

        // Generate child flags
        for (const auto& f : p->body->flags)
        {
            // Generate flags output stream
            GenerateFlagsOutputStream(f);

            // Generate flags logging stream
            if (Logging())
                GenerateFlagsLoggingStream(f);
        }

        if (!p->body->variants.empty()) {
            for (const auto& v : p->body->variants)
            {
                GenerateVariantOutputStream(p, v);
            }
        }

        // Generate child structs
        for (const auto& s : p->body->structs)
        {
            GeneratePtrStruct_Source(p, s);

            // Generate struct output stream
            GenerateStructOutputStream(p, s);

            // Generate struct logging stream
            if (Logging())
                GenerateStructLoggingStream(p, s);
        }
    }

    // Generate namespace end
    WriteLine();
    WriteLineIndent("} // namespace " + *p->name);

    // Generate package footer
    GenerateFooter();

    // Store the output file
    WriteEnd();
    Store(output);
}


void GeneratorCpp::GeneratePtrPackageModels_Header(const std::shared_ptr<Package>& p)
{
    CppCommon::Path output = _output;

    // Create package path
    CppCommon::Directory::CreateTree(output);

    // Generate the output file
    output /= *p->name + "_ptr_models.h";
    WriteBegin();

    // Generate package models header
    GenerateHeader(CppCommon::Path(_input).filename().string());

    // Generate imports
    GenerateImports("fbe_custom_models.h");
    GenerateImportsModels(p, false, true);

    // Generate namespace begin
    WriteLine();
    WriteLineIndent("namespace FBE {");

    // Generate namespace body
    if (p->body)
    {
        // Generate child enums
        for (const auto& e : p->body->enums)
        {
            // Generate enum field model
            GenerateEnumFieldModel(p, e);
        }

        // Generate child flags
        for (const auto& f : p->body->flags)
        {
            // Generate flags field model
            GenerateFlagsFieldModel(p, f);
        }

        // Generate variant
        for(const auto& v : p->body->variants) {
            GenerateVariantFieldModel_Header(p, v);
        }

        // Generate child ptr structs
        for (const auto& s : p->body->structs)
        {
            // Generate struct ptr field models
            GenerateStructFieldPtrModel_Header(p, s);
        }

        // Generate child structs
        for (const auto& s : p->body->structs)
        {
            GeneratePtrStructFieldModel_Header(p, s);
            GeneratePtrStructModel_Header(p, s);
        }
    }

    // Generate namespace end
    WriteLine();
    WriteLineIndent("} // namespace FBE");

    // Generate package footer
    GenerateFooter();

    // Store the output file
    WriteEnd();
    Store(output);
}

void GeneratorCpp::GeneratePtrPackageModels_Source(const std::shared_ptr<Package>& p)
{
    CppCommon::Path output = _output;

    // Create package path
    CppCommon::Directory::CreateTree(output);

    // Generate the output file
    output /= *p->name + "_ptr_models.cpp";
    WriteBegin();

    // Generate package models source
    GenerateSource(CppCommon::Path(_input).filename().string());

    // Generate imports
    GenerateImports(*p->name + "_ptr_models.h");

    // Generate namespace begin
    WriteLine();
    WriteLineIndent("namespace FBE {");

    // Generate namespace body
    if (p->body)
    {
        // Generate variant
        for(const auto& v : p->body->variants) {
            GenerateVariantFieldModel_Source(p, v);
        }

        // Generate child structs
        for (const auto& s : p->body->structs)
        {
            // Generate struct field models
            GenerateStructFieldPtrModel_Source(p, s);
            GeneratePtrStructFieldModel_Source(p, s);
            GeneratePtrStructModel_Source(p, s);
        }
    }

    // Generate namespace end
    WriteLine();
    WriteLineIndent("} // namespace FBE");

    // Generate package footer
    GenerateFooter();

    // Store the output file
    WriteEnd();
    Store(output);
}

void GeneratorCpp::GeneratePtrStruct_Header(const std::shared_ptr<Package>& p, const std::shared_ptr<StructType>& s)
{
    // Generate struct response forward declaration
    if (s->response)
    {
        std::string response = *s->response->response;
        bool imported = CppCommon::StringUtils::ReplaceAll(response, ".", "");
        if (!imported)
        {
            WriteLine();
            WriteLineIndent("struct " + response + ";");
        }
    }

    // Generate struct forward declaration if has ptr field;
    if (s->body)
    {
        std::set<std::string> unique_type_set;
        for (const auto& field : s->body->fields)
        {
            if (field->ptr)
            {
                if (!IsCurrentPackageType(*field->type) || IsKnownType(*field->type) || *field->type == *s->name) {
                    continue;
                }
                if (unique_type_set.find(*field->type) != unique_type_set.end()) {
                    continue;
                } else {
                    if (IsVariantType(p, *field->type))
                        continue;
                    WriteLine();
                    WriteLineIndent("struct " + *field->type + ";");
                    unique_type_set.insert(*field->type);
                }
            }
        }
    }


    // Generate struct begin
    WriteLine();
    WriteIndent("struct " + std::string(s->attributes->deprecated ? "[[deprecated]] " : "") + *s->name);
    if (s->base && !s->base->empty()) {
        Write(" : public " + ConvertPtrTypeName(*p->name, *s->base));
    } else {
        Write(" : FBE::Base");
    }
    WriteLine();
    WriteLineIndent("{");
    Indent(1);

    // Generate struct response type definition
    if (s->response)
    {
        std::string response = *s->response->response;
        CppCommon::StringUtils::ReplaceAll(response, ".", "::");
        WriteLineIndent("typedef " + response + " Response;");
        if (s->body && !s->body->fields.empty())
            WriteLine();
    }

    // Generate struct body
    if (Arena())
    {
        for(auto &tag: ArenaTags()){
            WriteLineIndent(tag + ";");
        }
        WriteLine();
    }


    // Generate data member
    if (s->body)
    {
        for (const auto& field : s->body->fields)
        {
            WriteIndent();
            if (field->attributes && field->attributes->deprecated)
                Write("[[deprecated]] ");
            WriteLine(ConvertPtrTypeName(*p->name, *field, false) + " " + *field->name + ";");
        }
        if (!s->body->fields.empty())
            WriteLine();
    }

    // Generate struct FBE type property
    if (s->base && !s->base->empty() && (s->type == 0))
        WriteLineIndent("size_t fbe_type() const noexcept { return " + ConvertPtrTypeName(*p->name, *s->base) + "::fbe_type(); }");
    else
        WriteLineIndent("size_t fbe_type() const noexcept { return " + std::to_string(s->type) + "; }");

    // Generate struct default constructor
    bool first = true;
    WriteLine();
    WriteLineIndent(*s->name + "();");

    // Generate cstr with Arena
    if (Arena()) {
        WriteLineIndent("explicit " + *s->name + "(allocator_type alloc);");
    }

    // Generate struct initialization constructor
    if ((s->base && !s->base->empty()) || (s->body && !s->body->fields.empty()))
    {
        first = true;
        size_t args = 0;
        if (s->base && !s->base->empty())
            ++args;
        if (s->body && !s->body->fields.empty())
            args += s->body->fields.size();
        WriteIndent(((args <= 1) ? "explicit " : "") + *s->name + "(");
        if (s->base && !s->base->empty())
        {
            Write("const " + ConvertPtrTypeName(*p->name, *s->base) + "& base");
            first = false;
        }
        if (s->body)
        {
            for (const auto& field : s->body->fields)
            {
                Write(std::string(first ? "" : ", ") + ConvertPtrTypeNameAsArgument(*p->name, *field) + " arg_" + *field->name);
                first = false;
            }
        }
        WriteLine(");");
    }

    // copy constructor
    WriteLineIndent(*s->name + "(const " + *s->name + "& other) = default;");
    // move constructor
    WriteLineIndent(*s->name + "(" + *s->name + "&& other) noexcept;");
    // destructor
    WriteLineIndent("~" + *s->name + "() override;");
    WriteLine();
    // copy assignment operator
    WriteLineIndent(*s->name + "& operator=(const " + *s->name + "& other) = default;");
    // move assignment operator
    WriteLineIndent(*s->name + "& operator=(" + *s->name + "&& other) noexcept;");

    // Generate struct compare operators
    WriteLine();
    WriteLineIndent("bool operator==(const " + *s->name + "& other) const noexcept;");
    WriteLineIndent("bool operator!=(const " + *s->name + "& other) const noexcept { return !operator==(other); }");
    WriteLineIndent("bool operator<(const " + *s->name + "& other) const noexcept;");
    WriteLineIndent("bool operator<=(const " + *s->name + "& other) const noexcept { return operator<(other) || operator==(other); }");
    WriteLineIndent("bool operator>(const " + *s->name + "& other) const noexcept { return !operator<=(other); }");
    WriteLineIndent("bool operator>=(const " + *s->name + "& other) const noexcept { return !operator<(other); }");

    // Generate struct string convert
    WriteLine();
    WriteLineIndent("std::string string() const;");

    // Generate struct output stream operator
    WriteLine();
    WriteLineIndent("friend std::ostream& operator<<(std::ostream& stream, const " + *s->name + "& value);");

    // Generate struct output stream operator
    if (Logging())
    {
        WriteLine();
        WriteLineIndent("#if defined(LOGGING_PROTOCOL)");
        WriteLineIndent("friend CppLogging::Record& operator<<(CppLogging::Record& record, const " + *s->name + "& value);");
        WriteLineIndent("#endif");
    }

    // Generate struct swap methods
    WriteLine();
    WriteLineIndent("void swap(" + *s->name + "& other) noexcept;");
    WriteLineIndent("friend void swap(" + *s->name + "& value1, " + *s->name + "& value2) noexcept { value1.swap(value2); }");

    // Generate struct end
    Indent(-1);
    WriteLineIndent("};");

    // Generate namespace end
    WriteLine();
    WriteLineIndent("} // namespace " + *p->name);

    // Generate struct hash
    GenerateStructHash(p, s);

    // Generate namespace begin
    WriteLine();
    WriteLineIndent("namespace " + *p->name + " {");
}

bool GeneratorCpp::IsCurrentPackageType(const std::string& field_type, const std::string& delimiter) {
    auto found_delimiter = field_type.find(delimiter);
    return found_delimiter == std::string::npos;
}

void GeneratorCpp::GeneratePtrStruct_Source(const std::shared_ptr<Package>& p, const std::shared_ptr<StructType>& s)
{
    WriteLine();

    // Generate struct default constructor
    bool first = true;
    WriteLineIndent(*s->name + "::" + *s->name + "()");
    Indent(1);
    if (s->base && !s->base->empty())
    {
        WriteLineIndent(": " + ConvertPtrTypeName(*p->name, *s->base) + "()");
        first = false;
    }
    if (s->body)
    {
        for (const auto& field : s->body->fields)
        {
            WriteIndent();
            Write(std::string(first ? ": " : ", ") + *field->name + "(");
            if (field->ptr && !IsContainerType(*field)) {
                Write("nullptr");
            } else if (field->value || IsPrimitiveType(*field->type, field->optional)) {
                Write(ConvertDefault(*p->name, *field));
            }
            Write(")");
            WriteLine();
            first = false;
        }
    }
    Indent(-1);
    WriteLineIndent("{}");

    // Generate struct constructor with arena
    if (Arena()) {
        first = true;
        WriteLine();
        WriteLineIndent(*s->name + "::" + *s->name + "([[maybe_unused]] allocator_type alloc)");
        Indent(1);
        if (s->base && !s->base->empty())
        {
            WriteLineIndent(": " + ConvertPtrTypeName(*p->name, *s->base) + "()");
            first = false;
        }
        if (s->body)
        {
            auto enums = p->body->enums;
            for (const auto& field : s->body->fields)
            {
                WriteIndent();
                Write(std::string(first ? ": " : ", ") + *field->name + "(");
                // priority: container > optional > imported type > ptr > bytes/string/primitive type/custom type > variant
                if (IsContainerType(*field)) {
                    Write("alloc");
                } else if (field->optional) {
                    Write("std::nullopt");
                } else if (!IsCurrentPackageType(*field->type)) {
                    Write(std::string("assign_member<") + ConvertTypeName(*p->name, *field) + ">(alloc)");
                } else if (field->ptr) {
                    Write("nullptr");
                } else if (*field->type == "bytes" || *field->type == "string") {
                    Write("alloc");
                } else if (field->value || IsPrimitiveType(*field->type, field->optional)) {
                    Write(ConvertDefault(*p->name, *field));
                // only struct(no optional or enum) should be initialized with arena
                } else if (!IsVariantType(p, *field->type) && std::find_if(enums.begin(), enums.end(),
                 [t = *field->type](const std::shared_ptr<EnumType>& e) -> bool { 
                     return *e->name == t; }) == enums.end()) {
                    Write("alloc");
                }
                Write(")");
                WriteLine();
                first = false;
            }
        }
        Indent(-1);
        WriteLineIndent("{}");
    }

    std::vector<std::string> unique_ptr_members;
    std::vector<std::shared_ptr<StructField>> collection_of_container_ptrs;
    std::vector<std::shared_ptr<StructField>> collection_of_optional_fields;
    // Generate struct initialization constructor
    if ((s->base && !s->base->empty()) || (s->body && !s->body->fields.empty()))
    {
        first = true;
        WriteLine();
        WriteIndent(*s->name + "::" + *s->name + "(");
        if (s->base && !s->base->empty())
        {
            Write("const " + ConvertPtrTypeName(*p->name, *s->base) + "& base");
            first = false;
        }
        if (s->body)
        {
            for (const auto& field : s->body->fields)
            {
                Write(std::string(first ? "" : ", ") + ConvertPtrTypeNameAsArgument(*p->name, *field) + " arg_" + *field->name);
                first = false;
            }
        }
        WriteLine(")");
        Indent(1);
        first = true;
        if (s->base && !s->base->empty())
        {
            WriteLineIndent(": " + ConvertPtrTypeName(*p->name, *s->base) + "(base)");
            first = false;
        }
        if (s->body)
        {
            for (const auto& field : s->body->fields)
            {
                // 对于指针类型，非容器可以直接处理，容器则需要在body中逐个释放
                if (field->ptr) {
                    if (IsContainerType(*field)) {
                        collection_of_container_ptrs.push_back(field);
                        WriteLineIndent(std::string(first ? ": " : ", ") + *field->name + "()");
                    }
                    else {
                        unique_ptr_members.push_back(*field->name);
                        WriteLineIndent(std::string(first ? ": " : ", ") + *field->name + "(arg_" + *field->name + ".release())");
                    }
                } else if (IsKnownType(*field->type)) {
                    WriteLineIndent(std::string(first ? ": " : ", ") + *field->name + "(arg_" + *field->name + ")");
                } else if (field->optional) {
                    collection_of_optional_fields.push_back(field);
                    WriteLineIndent(std::string(first ? ": " : ", ") + *field->name + "()");
                } else {
                    WriteLineIndent(std::string(first ? ": " : ", ") + *field->name + "(std::move(arg_" + *field->name + "))");
                }
                first = false;
            }
        }
        Indent(-1);
        if (collection_of_container_ptrs.empty() && collection_of_optional_fields.empty()) {
            WriteLineIndent("{}");
        } else {
                WriteLineIndent("{");
                Indent(1);
                for (const auto& field : collection_of_container_ptrs) {
                    if (field->map || field->hash) {
                        WriteLineIndent("for (auto& it: arg_" + *field->name + ")");
                        Indent(1);
                        WriteLineIndent(*field->name + ".emplace(it.first, it.second.release());");
                        Indent(-1);
                    } else if (field->vector || field->list) {
                        WriteLineIndent("for (auto& it : arg_" + *field->name + ")");
                        Indent(1);
                        WriteLineIndent(*field->name + ".emplace_back(it.release());");
                        Indent(-1);
                    } else if (field->array) {
                        WriteLineIndent("for (uint32_t i = 0 ; i < " + std::to_string(field->N) + "; ++i)");
                        Indent(1);
                        WriteLineIndent(*field->name + "[i] = arg_" + *field->name + "[i].release();");
                        Indent(-1);
                    }
                }
                for (const auto&field : collection_of_optional_fields) {
                    WriteLineIndent("if (arg_" + *field->name + ".has_value()) {");
                    Indent(1);
                    WriteLineIndent(*field->name + ".emplace(std::move(arg_" + *field->name + ".value()));");
                    WriteLineIndent("arg_" + *field->name + ".reset();");
                    Indent(-1);
                    WriteLineIndent("}");
                }
                Indent(-1);
                WriteLineIndent("}");
        }
    }

    // Generate struct move constructor
    collection_of_optional_fields.clear();

    WriteLine();
    WriteLineIndent(*s->name + "::" + *s->name + "([[maybe_unused]] " + *s->name + "&& other) noexcept");
    Indent(1);
    // generate the base move
    first = true;
    if (s->base && !s->base->empty())
    {
        WriteLineIndent(": "+ ConvertPtrTypeName(*p->name, *s->base) + "(std::move(other))");
        first = false;
    }
    // generate the field move
    if (s->body)
    {
        for (const auto& field : s->body->fields)
        {
            if (IsContainerType(*field)) {
                WriteLineIndent(std::string(first ? ": " : ", ") + *field->name + "(std::move(other." + *field->name + "))");
            } else if (field->ptr) {
                WriteLineIndent(std::string(first ? ": " : ", ") + *field->name + "(std::exchange(other." + *field->name + ", nullptr))");
            } else if (field->optional) {
                collection_of_optional_fields.push_back(field);
                WriteLineIndent(std::string(first ? ": " : ", ") + *field->name + "()");
            } else if (IsPrimitiveType(*field->type, field->optional)) {
                WriteLineIndent(std::string(first ? ": " : ", ") + *field->name + "(std::exchange(other." + *field->name + ", " + ConvertDefault(*p->name, *field) + "))");
            } else {
                WriteLineIndent(std::string(first ? ": " : ", ") + *field->name + "(std::move(other." + *field->name + "))");
            }
            first = false;
        }
    }
    Indent(-1);
    if (collection_of_optional_fields.empty()) {
        WriteLineIndent("{}");
    } else {
        WriteLineIndent("{");
        Indent(1);
        for (const auto&field : collection_of_optional_fields) {
            WriteLineIndent("if (other." + *field->name + ".has_value()) {");
            Indent(1);
            if (IsPrimitiveType(*field->type, false)) {
                WriteLineIndent(*field->name + ".emplace(other." + *field->name + ".value());");
            } else {
                WriteLineIndent(*field->name + ".emplace(std::move(other." + *field->name + ".value()));");
            }
            WriteLineIndent("other." + *field->name + ".reset();");
            Indent(-1);
            WriteLineIndent("}");
        }
        Indent(-1);
        WriteLineIndent("}");
    }


    WriteLine();
    WriteLineIndent(*s->name + "::~" + *s->name + "()");
    WriteLine("{");
    // destructor
    if (!unique_ptr_members.empty() || !collection_of_container_ptrs.empty()) {

        Indent(1);
        for (const auto& field : unique_ptr_members) {
            WriteLineIndent("if (" + field + ") delete " + field + ";");
        }
        for (const auto& field : collection_of_container_ptrs) {
            if (field->map || field->hash) {
                WriteLineIndent("for (auto& it: " + *field->name + ")");
                Indent(1);
                WriteLineIndent("delete it.second;");
                Indent(-1);
            } else if (field->vector || field->list) {
                WriteLineIndent("for (auto* it : " + *field->name + ")");
                Indent(1);
                WriteLineIndent("delete it;");
                Indent(-1);
            } else if (field->array) {
                WriteLineIndent("for (uint32_t i = 0 ; i < " + std::to_string(field->N) + "; ++i)");
                Indent(1);
                WriteLineIndent(std::string("delete ") + *field->name + "[i];");
                Indent(-1);
            }
        }
        Indent(-1);
    }
    WriteLine("}");

    // Generate struct compare operators
    WriteLine();
    WriteLineIndent("bool " + *s->name + "::operator==([[maybe_unused]] const " + *s->name + "& other) const noexcept");
    WriteLineIndent("{");
    Indent(1);
    WriteLineIndent("return (");
    Indent(1);
    first = true;
    if (s->base && !s->base->empty())
    {
        WriteLineIndent(ConvertPtrTypeName(*p->name, *s->base) + "::operator==(other)");
        first = false;
    }
    if (s->body)
    {
        for (const auto& field : s->body->fields)
        {
            if (field->keys)
            {
                WriteLineIndent(std::string(first ? "(" : "&& (") + *field->name + " == other." + *field->name + ")");
                first = false;
            }
        }
    }
    if (!s->keys)
    {
        WriteLineIndent(std::string(first ? "true" : "&& true"));
        first = false;
    }
    WriteLineIndent(");");
    Indent(-1);
    Indent(-1);
    WriteLineIndent("}");

    WriteLine();
    WriteLineIndent("bool " + *s->name + "::operator<([[maybe_unused]] const " + *s->name + "& other) const noexcept");
    WriteLineIndent("{");
    Indent(1);
    if (s->base && !s->base->empty())
    {
        WriteLineIndent("if (" + ConvertPtrTypeName(*p->name, *s->base) + "::operator<(other))");
        Indent(1);
        WriteLineIndent("return true;");
        Indent(-1);
        WriteLineIndent("if (other." + ConvertPtrTypeName(*p->name, *s->base) + "::operator<(*this))");
        Indent(1);
        WriteLineIndent("return false;");
        Indent(-1);
    }
    if (s->body)
    {
        for (const auto& field : s->body->fields)
        {
            if (field->keys)
            {
                WriteLineIndent("if (" + *field->name + " < other." + *field->name + ")");
                Indent(1);
                WriteLineIndent("return true;");
                Indent(-1);
                WriteLineIndent("if (other." + *field->name + " < " + *field->name + ")");
                Indent(1);
                WriteLineIndent("return false;");
                Indent(-1);
            }
        }
    }
    WriteLineIndent("return false;");
    Indent(-1);
    WriteLineIndent("}");


    // Generate struct move assignment operator
    WriteLine();
    WriteLineIndent(*s->name + "& " + *s->name + "::operator=(" + *s->name + "&& other) noexcept");
    WriteLineIndent("{");
    Indent(1);
    WriteLineIndent("if (this != &other)");
    WriteLineIndent("{");
    Indent(1);
    // generate the base move
    if (s->base && !s->base->empty())
    {
        WriteLineIndent("Base::operator = (std::move(other))");
    }
    // generate the field move
    if (s->body)
    {
        for (const auto& field : s->body->fields)
        {
            if (IsContainerType(*field)) {
                WriteLineIndent(*field->name + " = std::move(other." + *field->name + ");");
            } else if (field->ptr) {
                WriteLineIndent(*field->name + " = std::exchange(other." + *field->name + ", nullptr);");
            } else if (field->optional) {
                WriteLineIndent("if (other." + *field->name + ".has_value()) {");
                Indent(1);
                if (IsPrimitiveType(*field->type, false)) {
                    WriteLineIndent(*field->name + ".emplace(other." + *field->name + ".value());");
                } else {
                    WriteLineIndent(*field->name + ".emplace(std::move(other." + *field->name + ".value()));");
                }
                WriteLineIndent("other." + *field->name + ".reset();");
                Indent(-1);
                WriteLineIndent("}");
            } else if (IsPrimitiveType(*field->type, field->optional)) {
                WriteLineIndent(*field->name + " = std::exchange(other." + *field->name + ", " + ConvertDefault(*p->name, *field) + ");");

            } else {
                WriteLineIndent(*field->name + " = std::move(other." + *field->name + ");");
            }
            first = false;
        }
    }
    Indent(-1);
    WriteLineIndent("}");
    WriteLineIndent("return *this;");
    Indent(-1);
    WriteLineIndent("}");

    // Generate struct string method
    WriteLine();
    WriteLineIndent("std::string " + *s->name + "::string() const");
    WriteLineIndent("{");
    Indent(1);
    WriteLineIndent("std::stringstream ss; ss << *this; return ss.str();");
    Indent(-1);
    WriteLineIndent("}");

    // Generate struct swap method
    WriteLine();
    WriteLineIndent("void " + *s->name + "::swap([[maybe_unused]] " + *s->name + "& other) noexcept");
    WriteLineIndent("{");
    Indent(1);
    WriteLineIndent("using std::swap;");
    if (s->base && !s->base->empty())
        WriteLineIndent(ConvertPtrTypeName(*p->name, *s->base) + "::swap(other);");
    if (s->body)
        for (const auto& field : s->body->fields)
            WriteLineIndent("swap(" + *field->name + ", other." + *field->name + ");");
    Indent(-1);
    WriteLineIndent("}");
}

void GeneratorCpp::GenerateVariantFieldModel_Header(const std::shared_ptr<Package>& p, const std::shared_ptr<VariantType>& v)
{
    std::string variant_name = "::" + *p->name + "::" + *v->name;
    std::string class_name = "FieldModel<" + variant_name + ">";

    // Generate variant field model begin
    WriteLine();
    WriteLineIndent("template <>");
    WriteLineIndent("class " + class_name);
    WriteLineIndent("{");
    WriteLineIndent("public:");
    Indent(1);

    // Generate variant field model constructor
    WriteLineIndent("FieldModel(FBEBuffer& buffer, size_t offset) noexcept;");

    // Generate variant field model FBE methods
    WriteLine();
    WriteLineIndent("// Get the field offset");
    WriteLineIndent("size_t fbe_offset() const noexcept { return _offset; }");
    WriteLineIndent("// Get the field size");
    WriteLineIndent("size_t fbe_size() const noexcept { return 4; }");
    WriteLineIndent("// Get the field body size");
    WriteLineIndent("size_t fbe_body() const noexcept;");
    WriteLineIndent("// Get the field extra size");
    WriteLineIndent("size_t fbe_extra() const noexcept;");
    WriteLine();
    WriteLineIndent("// Shift the current field offset");
    WriteLineIndent("void fbe_shift(size_t size) noexcept { _offset += size; }");
    WriteLineIndent("// Unshift the current field offset");
    WriteLineIndent("void fbe_unshift(size_t size) noexcept { _offset -= size; }");

    // Generate variant field model verify(), verify_fields() methods
    WriteLine();
    WriteLineIndent("// Check if the variant value is valid");
    WriteLineIndent("bool verify() const noexcept;");

    // Generate variant field model get(), get_fields() methods
    WriteLine();
    WriteLineIndent("// Get the variant value");
    WriteLineIndent("void get(" + variant_name + "& fbe_value) const noexcept;");

    WriteLine();
    WriteLineIndent("// Set the variant value (begin phase)");
    WriteLineIndent("size_t set_begin(size_t variant_type_fbe_size, size_t variant_type_index);");
    WriteLineIndent("// Set the variant value (end phase)");
    WriteLineIndent("void set_end(size_t fbe_begin);");

    // Generate variant field model set(), set_fields() methods
    WriteLine();
    WriteLineIndent("// Set the variant value");
    WriteLineIndent("void set(const " + variant_name + "& fbe_value) noexcept;");

    // Generate variant field model buffer & offset
    Indent(-1);
    WriteLine();
    WriteLineIndent("private:");
    Indent(1);
    WriteLineIndent("FBEBuffer& _buffer;");
    WriteLineIndent("size_t _offset;");
    Indent(-1);
    WriteLineIndent("};");
}

void GeneratorCpp::GenerateVariantFieldModel_Source(const std::shared_ptr<Package>& p, const std::shared_ptr<VariantType>& v)
{
    std::string variant_name = "::" + *p->name + "::" + *v->name;
    std::string class_name = "FieldModel<" + variant_name + ">";

    // Generate variant field model begin
    WriteLine();
    WriteLineIndent(class_name + "::FieldModel(FBEBuffer& buffer, size_t offset) noexcept : _buffer(buffer), _offset(offset)");
    WriteLineIndent("{}");
    WriteLine();

    WriteLineIndent("size_t " + class_name + "::fbe_body() const noexcept");
    WriteLineIndent("{");
    Indent(1);
    WriteLineIndent("// variant type's fbe_size not included");
    WriteLineIndent("size_t fbe_result = 4;");
    WriteLineIndent("return fbe_result;");
    Indent(-1);
    WriteLineIndent("}");
    WriteLine();

    WriteLineIndent("size_t " + class_name + "::fbe_extra() const noexcept");
    WriteLineIndent("{");
    Indent(1);
    WriteLineIndent("if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())");
    Indent(1);
    WriteLineIndent("return 0;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("uint32_t fbe_struct_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());");
    WriteLineIndent("if ((fbe_struct_offset == 0) || ((_buffer.offset() + fbe_struct_offset + 4) > _buffer.size()))");
    Indent(1);
    WriteLineIndent("return 0;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("_buffer.shift(fbe_struct_offset);");
    WriteLine();
    WriteLineIndent("size_t fbe_result = fbe_body();");
    Indent(1);
    // TODO(liuqi): calculate contained type's fbe_extra()
    Indent(-1);
    WriteLine();
    WriteLineIndent("_buffer.unshift(fbe_struct_offset);");
    WriteLine();
    WriteLineIndent("return fbe_result;");
    Indent(-1);
    WriteLineIndent("}");
    WriteLine();

    // Generate variant field model verify(), verify_fields() methods
    WriteLineIndent("bool " + class_name + "::verify() const noexcept");
    WriteLineIndent("{");
    Indent(1);
    WriteLineIndent("if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())");
    Indent(1);
    WriteLineIndent("return true;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("uint32_t fbe_variant_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());");
    WriteLineIndent("if ((fbe_variant_offset == 0) || ((_buffer.offset() + fbe_variant_offset + 4) > _buffer.size()))");
    Indent(1);
    WriteLineIndent("return false;");
    Indent(-1);
    WriteLine();
    // type
    WriteLineIndent("uint32_t fbe_variant_type = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_variant_offset);");
    WriteLineIndent("if (fbe_variant_type < 0 || fbe_variant_type >= " + std::to_string(v->body->values.size()) + ")"); 
    Indent(1);
    WriteLineIndent("return false;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("_buffer.shift(fbe_variant_offset);");
    WriteLineIndent("switch(fbe_variant_type) {");
    Indent(1);
    for(auto index = 0; index < v->body->values.size(); index ++) {
        WriteLineIndent("case " + std::to_string(index) + ": {");
        Indent(1);
        auto& value = v->body->values[index];
        WriteLineIndent(ConvertPtrVariantFieldModelType(p, value) + " fbe_model(_buffer, 4);");
        WriteLineIndent("if (!fbe_model.verify())");
        Indent(1);
        WriteLineIndent("return false;");
        Indent(-1);
        WriteLineIndent("break;");
        Indent(-1);
        WriteLineIndent("}");
    }
    Indent(-1);
    WriteLineIndent("}");
    WriteLine();
    WriteLineIndent("_buffer.unshift(fbe_variant_offset);");
    WriteLineIndent("return true;");
    Indent(-1);
    WriteLineIndent("}");
    WriteLine();

    // Generate struct field model get() method
    WriteLineIndent("void " + class_name + "::get(" + variant_name + "& fbe_value) const noexcept");
    WriteLineIndent("{");
    Indent(1);
    WriteLineIndent("if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())");
    Indent(1);
    WriteLineIndent("return;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("uint32_t fbe_variant_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());");
    WriteLineIndent("assert(((fbe_variant_offset > 0) && ((_buffer.offset() + fbe_variant_offset + 4) <= _buffer.size())) && \"Model is broken!\");");
    WriteLineIndent("if ((fbe_variant_offset == 0) || ((_buffer.offset() + fbe_variant_offset + 4) > _buffer.size()))");
    Indent(1);
    WriteLineIndent("return;");
    Indent(-1);
    WriteLineIndent("uint32_t vairant_type_index = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_variant_offset);");
    WriteLineIndent("assert(vairant_type_index >= 0 && vairant_type_index < " + std::to_string(v->body->values.size()) + " && \"Model is broken!\");"); 
    WriteLine();
    WriteLineIndent("_buffer.shift(fbe_variant_offset);");
    WriteLine();
    WriteLineIndent("switch(vairant_type_index) {");
    Indent(1);
    for(auto index = 0; index < v->body->values.size(); index ++) {
        WriteLineIndent("case " + std::to_string(index) + ": {");
        Indent(1);
        auto& value = v->body->values[index];
        WriteLineIndent(ConvertPtrVariantFieldModelType(p, value) + " fbe_model(_buffer, 4);");
        // initialize variant
        auto variant_type = ConvertVariantTypeName(*p->name, *value);
        WriteLineIndent("fbe_value.emplace<" + variant_type + ">();");
        WriteLineIndent("auto& value = std::get<" +  std::to_string(index) + ">(fbe_value);");
        WriteLineIndent(std::string("fbe_model.get(") + ((!IsContainerType(*value) && value->ptr) ? "&" : "") + "value);");
        WriteLineIndent("break;");
        Indent(-1);
        WriteLineIndent("}");
    }
    Indent(-1);
    WriteLineIndent("}");
    WriteLine();
    WriteLineIndent("_buffer.unshift(fbe_variant_offset);");
    Indent(-1);
    WriteLineIndent("}");
    WriteLine();

    // Generate variant field model set_begin() method
    // We don't know the type of the variant in advance. So we need to pass variant_type_fbe_size to set_begin to alloc.
    WriteLineIndent("size_t " + class_name + "::set_begin(size_t variant_type_fbe_size, size_t variant_type_index)");
    WriteLineIndent("{");
    Indent(1);
    WriteLineIndent("assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && \"Model is broken!\");");
    WriteLineIndent("if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())");
    Indent(1);
    WriteLineIndent("return 0;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("uint32_t fbe_variant_size = (uint32_t)(fbe_body() + variant_type_fbe_size);");
    WriteLineIndent("uint32_t fbe_variant_offset = (uint32_t)(_buffer.allocate(fbe_variant_size) - _buffer.offset());");
    WriteLineIndent("assert(((fbe_variant_offset > 0) && ((_buffer.offset() + fbe_variant_offset + fbe_variant_size) <= _buffer.size())) && \"Model is broken!\");");
    WriteLineIndent("if ((fbe_variant_offset == 0) || ((_buffer.offset() + fbe_variant_offset + fbe_variant_size) > _buffer.size()))");
    Indent(1);
    WriteLineIndent("return 0;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset(), fbe_variant_offset);");
    WriteLineIndent("unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_variant_offset, variant_type_index);");
    WriteLine();
    WriteLineIndent("_buffer.shift(fbe_variant_offset);");
    WriteLineIndent("return fbe_variant_offset;");
    Indent(-1);
    WriteLineIndent("}");
    WriteLine();

    // Generate variant field model set_end() method
    WriteLineIndent("void " + class_name + "::set_end(size_t fbe_begin)");
    WriteLineIndent("{");
    Indent(1);
    WriteLineIndent("_buffer.unshift(fbe_begin);");
    Indent(-1);
    WriteLineIndent("}");
    WriteLine();

    // Generate variant field model set() method
    WriteLineIndent("// Set the variant value");
    WriteLineIndent("void " + class_name + "::set(const " + variant_name + "& fbe_value) noexcept");
    WriteLineIndent("{");
    Indent(1);
    WriteLineIndent("assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && \"Model is broken!\");");
    WriteLineIndent("if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())");
    Indent(1);
    WriteLineIndent("return;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("std::visit(");
    Indent(1);
    WriteLineIndent("overloaded");
    WriteLineIndent("{");
    Indent(1);
    bool first = true;
    for(auto index = 0; index < v->body->values.size(); index ++) {
        WriteIndent(first ? "" : ", ");
        auto& value = v->body->values[index];
        Write("[this, fbe_variant_index = fbe_value.index()](");
        Write(ConvertVariantTypeNameAsArgument(*p->name, *value));
        WriteLine(" v) {");
        Indent(1);
        WriteLineIndent(ConvertPtrVariantFieldModelType(p, value) + " fbe_model(_buffer, 4);");
        WriteLineIndent("size_t fbe_begin = set_begin(fbe_model.fbe_size(), fbe_variant_index);");
        WriteLineIndent("if (fbe_begin == 0)");
        Indent(1);
        WriteLineIndent("return;");
        Indent(-1);
        WriteLineIndent("fbe_model.set(v);");
        WriteLineIndent("set_end(fbe_begin);");
        Indent(-1);
        WriteLineIndent("}");
        first = false;
    }
    Indent(-1);
    WriteLineIndent("},");
    WriteLineIndent("fbe_value");
    Indent(-1);
    WriteLineIndent(");");
    Indent(-1);
    WriteLineIndent("}");
    WriteLine();
}


void GeneratorCpp::GenerateStructFieldPtrModel_Header(const std::shared_ptr<Package>& p, const std::shared_ptr<StructType>& s)
{
    std::string struct_name = "::" + *p->name + "::" + *s->name;
    std::string class_name = "FieldModelPtr_" + *p->name + "_" + *s->name;

    // Generate struct field ptr model begin
    WriteLine();
    WriteLineIndent("class " + class_name);
    WriteLineIndent("{");
    WriteLineIndent("public:");
    Indent(1);

    // Generate struct field ptr model constructor
    WriteLineIndent(class_name + "(FBEBuffer& buffer, size_t offset) noexcept;");
    WriteLineIndent("~" + class_name + "();");

    // Generate struct field ptr model FBE methods
    WriteLine();
    WriteLineIndent("// Get the field offset");
    WriteLineIndent("size_t fbe_offset() const noexcept { return _offset; }");
    WriteLineIndent("// Get the field size");
    WriteLineIndent("size_t fbe_size() const noexcept { return 5; }");
    WriteLineIndent("// Get the field extra size");
    WriteLineIndent("size_t fbe_extra() const noexcept;");
    WriteLineIndent("// Get the field type");
    WriteLineIndent("static constexpr size_t fbe_type() noexcept { return " + std::to_string(s->type) + "; }");
    WriteLine();
    WriteLineIndent("// Shift the current field offset");
    WriteLineIndent("void fbe_shift(size_t size) noexcept { _offset += size; }");
    WriteLineIndent("// Unshift the current field offset");
    WriteLineIndent("void fbe_unshift(size_t size) noexcept { _offset -= size; }");

    // Generate struct field model verify(), verify_fields() methods
    WriteLine();
    WriteLineIndent("// Check if the struct value is valid");
    WriteLineIndent("bool verify() const noexcept;");
    WriteLineIndent("// Check if the struct ptr is nullptr");
    WriteLineIndent("bool has_value() const noexcept;");

    // Generate struct field model get_begin(), get_end() methods
    WriteLine();
    WriteLineIndent("// Get the struct value (begin phase)");
    WriteLineIndent("size_t get_begin() const noexcept;");
    WriteLineIndent("// Get the struct value (end phase)");
    WriteLineIndent("void get_end(size_t fbe_begin) const noexcept;");

    // Generate struct field model get(), get_fields() methods
    WriteLine();
    WriteLineIndent("// Get the struct value");
    WriteLineIndent("void get(" + struct_name + "** fbe_value) noexcept;");
    WriteLineIndent("// Get the struct fields values");
    WriteLineIndent("void get_fields(" + struct_name + "& fbe_value, size_t fbe_struct_size) noexcept;");

    // Generate struct field model set_begin(), set_end() methods
    WriteLine();
    WriteLineIndent("// Set the struct value (begin phase)");
    WriteLineIndent("size_t set_begin(bool has_value);");
    WriteLineIndent("// Set the struct value (end phase)");
    WriteLineIndent("void set_end(size_t fbe_begin);");

    // Generate struct field model set(), set_fields() methods
    WriteLine();
    WriteLineIndent("// Set the struct value");
    WriteLineIndent("void set(const " + struct_name + "* fbe_value) noexcept;");
    WriteLineIndent("// Set the struct fields values");
    WriteLineIndent("void set_fields(const " + struct_name + "* fbe_value) noexcept;");

    // Generate struct field model buffer & offset
    Indent(-1);
    WriteLine();
    WriteLineIndent("private:");
    Indent(1);
    WriteLineIndent("FBEBuffer& _buffer;");
    WriteLineIndent("size_t _offset;");

    // Generate struct field model accessors
    Indent(-1);
    WriteLine();
    WriteLineIndent("public:");
    Indent(1);
    WriteLineIndent("BaseFieldModel* ptr{nullptr};");

    // Generate struct field model end
    Indent(-1);
    WriteLineIndent("};");
}

void GeneratorCpp::GeneratePtrStructFieldModel_Header(const std::shared_ptr<Package>& p, const std::shared_ptr<StructType>& s)
{
    std::string struct_name = "::" + *p->name + "::" + *s->name;
    std::string class_name = "FieldModel_" + *p->name + "_" + *s->name;

    // Generate struct field model begin
    WriteLine();
    WriteLineIndent("// Fast Binary Encoding " + struct_name + " field model");
    WriteLineIndent("class " + class_name + " : public BaseFieldModel");
    WriteLineIndent("{");
    WriteLineIndent("public:");
    Indent(1);

    // Generate struct field model constructor
    WriteLineIndent(class_name + "(FBEBuffer& buffer, size_t offset) noexcept;");

    // Generate struct field model FBE methods
    WriteLine();
    WriteLineIndent("// Get the field offset");
    WriteLineIndent("size_t fbe_offset() const noexcept override { return _offset; }");
    WriteLineIndent("// Get the field size");
    WriteLineIndent("size_t fbe_size() const noexcept override { return 4; }");
    WriteLineIndent("// Get the field body size");
    WriteLineIndent("size_t fbe_body() const noexcept;");
    WriteLineIndent("// Get the field extra size");
    WriteLineIndent("size_t fbe_extra() const noexcept override;");
    WriteLineIndent("// Get the field type");
    if (s->base && !s->base->empty() && (s->type == 0))
        WriteLineIndent("static constexpr size_t fbe_type() noexcept { return FieldModel<" + ConvertPtrTypeName(*p->name, *s->base) + ">::fbe_type(); }");
    else
        WriteLineIndent("static constexpr size_t fbe_type() noexcept { return " + std::to_string(s->type) + "; }");
    WriteLine();
    WriteLineIndent("// Shift the current field offset");
    WriteLineIndent("void fbe_shift(size_t size) noexcept override { _offset += size; }");
    WriteLineIndent("// Unshift the current field offset");
    WriteLineIndent("void fbe_unshift(size_t size) noexcept override { _offset -= size; }");

    // Generate struct field model verify(), verify_fields() methods
    WriteLine();
    WriteLineIndent("// Check if the struct value is valid");
    WriteLineIndent("bool verify(bool fbe_verify_type = true) const noexcept override;");
    WriteLineIndent("// Check if the struct fields are valid");
    WriteLineIndent("bool verify_fields(size_t fbe_struct_size) const noexcept override;");

    // Generate struct field model get_begin(), get_end() methods
    WriteLine();
    WriteLineIndent("// Get the struct value (begin phase)");
    WriteLineIndent("size_t get_begin() const noexcept override;");
    WriteLineIndent("// Get the struct value (end phase)");
    WriteLineIndent("void get_end(size_t fbe_begin) const noexcept override;");

    // Generate struct field model get(), get_fields() methods
    WriteLine();
    WriteLineIndent("// Get the struct value");
    WriteLineIndent("void get(::FBE::Base& fbe_value) noexcept override;");
    WriteLineIndent("// Get the struct fields values");
    WriteLineIndent("void get_fields(::FBE::Base& fbe_value, size_t fbe_struct_size) noexcept override;");

    // Generate struct field model set_begin(), set_end() methods
    WriteLine();
    WriteLineIndent("// Set the struct value (begin phase)");
    WriteLineIndent("size_t set_begin() override;");
    WriteLineIndent("// Set the struct value (end phase)");
    WriteLineIndent("void set_end(size_t fbe_begin) override;");

    // Generate struct field model set(), set_fields() methods
    WriteLine();
    WriteLineIndent("// Set the struct value");
    WriteLineIndent("void set(const ::FBE::Base& fbe_value) noexcept override;");
    WriteLineIndent("// Set the struct fields values");
    WriteLineIndent("void set_fields(const ::FBE::Base& fbe_value) noexcept override;");

    // Generate struct field model buffer & offset
    Indent(-1);
    WriteLine();
    WriteLineIndent("private:");
    Indent(1);
    WriteLineIndent("FBEBuffer& _buffer;");
    WriteLineIndent("size_t _offset;");

    // Generate struct field model accessors
    Indent(-1);
    WriteLine();
    WriteLineIndent("public:");
    Indent(1);
    if (s->base && !s->base->empty())
        // TODO: check if this is correct
        WriteLineIndent("FieldModel<" + ConvertPtrTypeName(*p->name, *s->base) + "> parent;");
    if (s->body)
    {
        for (const auto& field : s->body->fields)
        {
            WriteLineIndent(ConvertPtrFieldModelType(p, field) + " " + *field->name + ";");
        }
    }

    // Generate struct field model end
    Indent(-1);
    WriteLineIndent("};");
}

void GeneratorCpp::GeneratePtrStructFieldModel_Source(const std::shared_ptr<Package>& p, const std::shared_ptr<StructType>& s)
{
    std::string struct_name = "::" + *p->name + "::" + *s->name;
    std::string class_name = "FieldModel_" + *p->name + "_" + *s->name;

    // Generate struct field model constructor
    WriteLine();
    WriteLineIndent(class_name + "::" + class_name + "(FBEBuffer& buffer, size_t offset) noexcept : _buffer(buffer), _offset(offset)");
    Indent(1);
    std::string prev_offset("4");
    std::string prev_size("4");
    if (s->base && !s->base->empty())
    {
        WriteLineIndent(", parent(buffer, " + prev_offset + " + " + prev_size + ")");
        prev_offset = "parent.fbe_offset()";
        prev_size = "parent.fbe_body() - 4 - 4";
    }
    if (s->body)
    {
        for (const auto& field : s->body->fields)
        {
            WriteLineIndent(", " + *field->name + "(buffer, " + prev_offset + " + " + prev_size + ")");
            prev_offset = *field->name + ".fbe_offset()";
            prev_size = *field->name + ".fbe_size()";
        }
    }
    Indent(-1);
    WriteLineIndent("{}");
    WriteLine();

    // Generate struct field model FBE methods
    WriteLineIndent("size_t " + class_name + "::fbe_body() const noexcept");
    WriteLineIndent("{");
    Indent(1);
    WriteLineIndent("size_t fbe_result = 4 + 4");
    Indent(1);
    if (s->base && !s->base->empty())
        WriteLineIndent("+ parent.fbe_body() - 4 - 4");
    if (s->body)
        for (const auto& field : s->body->fields)
            WriteLineIndent("+ " + *field->name + ".fbe_size()");
    WriteLineIndent(";");
    Indent(-1);
    WriteLineIndent("return fbe_result;");
    Indent(-1);
    WriteLineIndent("}");
    WriteLine();
    WriteLineIndent("size_t " + class_name + "::fbe_extra() const noexcept");
    WriteLineIndent("{");
    Indent(1);
    WriteLineIndent("if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())");
    Indent(1);
    WriteLineIndent("return 0;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("uint32_t fbe_struct_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());");
    WriteLineIndent("if ((fbe_struct_offset == 0) || ((_buffer.offset() + fbe_struct_offset + 4) > _buffer.size()))");
    Indent(1);
    WriteLineIndent("return 0;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("_buffer.shift(fbe_struct_offset);");
    WriteLine();
    WriteLineIndent("size_t fbe_result = fbe_body()");
    Indent(1);
    if (s->base && !s->base->empty())
        WriteLineIndent("+ parent.fbe_extra()");
    if (s->body)
        for (const auto& field : s->body->fields)
            WriteLineIndent("+ " + *field->name + ".fbe_extra()");
    WriteLineIndent(";");
    Indent(-1);
    WriteLine();
    WriteLineIndent("_buffer.unshift(fbe_struct_offset);");
    WriteLine();
    WriteLineIndent("return fbe_result;");
    Indent(-1);
    WriteLineIndent("}");
    WriteLine();

    // Generate struct field model verify() method
    WriteLineIndent("bool " + class_name + "::verify(bool fbe_verify_type) const noexcept");
    WriteLineIndent("{");
    Indent(1);
    WriteLineIndent("if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())");
    Indent(1);
    WriteLineIndent("return true;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("uint32_t fbe_struct_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());");
    WriteLineIndent("if ((fbe_struct_offset == 0) || ((_buffer.offset() + fbe_struct_offset + 4 + 4) > _buffer.size()))");
    Indent(1);
    WriteLineIndent("return false;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("uint32_t fbe_struct_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_struct_offset);");
    WriteLineIndent("if (fbe_struct_size < (4 + 4))");
    Indent(1);
    WriteLineIndent("return false;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("uint32_t fbe_struct_type = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_struct_offset + 4);");
    WriteLineIndent("if (fbe_verify_type && (fbe_struct_type != fbe_type()))");
    Indent(1);
    WriteLineIndent("return false;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("_buffer.shift(fbe_struct_offset);");
    WriteLineIndent("bool fbe_result = verify_fields(fbe_struct_size);");
    WriteLineIndent("_buffer.unshift(fbe_struct_offset);");
    WriteLineIndent("return fbe_result;");
    Indent(-1);
    WriteLineIndent("}");
    WriteLine();

    // Generate struct field model verify_fields() method
    WriteLineIndent("bool " + class_name + "::verify_fields([[maybe_unused]] size_t fbe_struct_size) const noexcept");
    WriteLineIndent("{");
    Indent(1);
    if ((s->base && !s->base->empty()) || (s->body && !s->body->fields.empty()))
    {
        WriteLineIndent("size_t fbe_current_size = 4 + 4;");
        if (s->base && !s->base->empty())
        {
            WriteLine();
            WriteLineIndent("if ((fbe_current_size + parent.fbe_body() - 4 - 4) > fbe_struct_size)");
            Indent(1);
            WriteLineIndent("return true;");
            Indent(-1);
            WriteLineIndent("if (!parent.verify_fields(fbe_struct_size))");
            Indent(1);
            WriteLineIndent("return false;");
            Indent(-1);
            WriteLineIndent("fbe_current_size += parent.fbe_body() - 4 - 4;");
        }
        if (s->body)
        {
            for (const auto& field : s->body->fields)
            {
                WriteLine();
                WriteLineIndent("if ((fbe_current_size + " + *field->name + ".fbe_size()) > fbe_struct_size)");
                Indent(1);
                WriteLineIndent("return true;");
                Indent(-1);
                WriteLineIndent("if (!" + *field->name + ".verify())");
                Indent(1);
                WriteLineIndent("return false;");
                Indent(-1);
                WriteLineIndent("fbe_current_size += " + *field->name + ".fbe_size();");
            }
        }
        WriteLine();
    }
    WriteLineIndent("return true;");
    Indent(-1);
    WriteLineIndent("}");
    WriteLine();

    // Generate struct field model get_begin() method
    WriteLineIndent("size_t " + class_name + "::get_begin() const noexcept");
    WriteLineIndent("{");
    Indent(1);
    WriteLineIndent("if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())");
    Indent(1);
    WriteLineIndent("return 0;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("uint32_t fbe_struct_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());");
    WriteLineIndent("assert(((fbe_struct_offset > 0) && ((_buffer.offset() + fbe_struct_offset + 4 + 4) <= _buffer.size())) && \"Model is broken!\");");
    WriteLineIndent("if ((fbe_struct_offset == 0) || ((_buffer.offset() + fbe_struct_offset + 4 + 4) > _buffer.size()))");
    Indent(1);
    WriteLineIndent("return 0;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("uint32_t fbe_struct_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_struct_offset);");
    WriteLineIndent("assert((fbe_struct_size >= (4 + 4)) && \"Model is broken!\");");
    WriteLineIndent("if (fbe_struct_size < (4 + 4))");
    Indent(1);
    WriteLineIndent("return 0;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("_buffer.shift(fbe_struct_offset);");
    WriteLineIndent("return fbe_struct_offset;");
    Indent(-1);
    WriteLineIndent("}");
    WriteLine();

    // Generate struct field model get_end() method
    WriteLineIndent("void " + class_name + "::get_end(size_t fbe_begin) const noexcept");
    WriteLineIndent("{");
    Indent(1);
    WriteLineIndent("_buffer.unshift(fbe_begin);");
    Indent(-1);
    WriteLineIndent("}");
    WriteLine();

    // Generate struct field model get() method
    WriteLineIndent("void " + class_name + "::get(::FBE::Base& fbe_value) noexcept");
    WriteLineIndent("{");
    Indent(1);
    WriteLineIndent("size_t fbe_begin = get_begin();");
    WriteLineIndent("if (fbe_begin == 0)");
    Indent(1);
    WriteLineIndent("return;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("uint32_t fbe_struct_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset());");
    WriteLineIndent("get_fields(fbe_value, fbe_struct_size);");
    WriteLineIndent("get_end(fbe_begin);");
    Indent(-1);
    WriteLineIndent("}");
    WriteLine();

    // Generate struct field model get_fields() method
    WriteLineIndent("void " + class_name + "::get_fields([[maybe_unused]] ::FBE::Base& base_fbe_value, [[maybe_unused]] size_t fbe_struct_size) noexcept");
    WriteLineIndent("{");
    Indent(1);
    if ((s->base && !s->base->empty()) || (s->body && !s->body->fields.empty()))
    {
        WriteLineIndent(struct_name + "& fbe_value = static_cast<" + struct_name + "&>(base_fbe_value);");
        WriteLineIndent("size_t fbe_current_size = 4 + 4;");
        if (s->base && !s->base->empty())
        {
            WriteLine();
            WriteLineIndent("if ((fbe_current_size + parent.fbe_body() - 4 - 4) <= fbe_struct_size)");
            Indent(1);
            WriteLineIndent("parent.get_fields(fbe_value, fbe_struct_size);");
            Indent(-1);
            WriteLineIndent("fbe_current_size += parent.fbe_body() - 4 - 4;");
        }
        if (s->body)
        {
            for (const auto& field : s->body->fields)
            {
                WriteLine();
                WriteLineIndent("if ((fbe_current_size + " + *field->name + ".fbe_size()) <= fbe_struct_size)");
                Indent(1);
                if (!field->ptr || IsContainerType(*field)) {
                    WriteLineIndent("{");
                    Indent(1);
                    WriteLineIndent(*field->name + ".get(fbe_value." + *field->name + ");");
                    Indent(-1);
                    WriteLineIndent("}");
                } else {
                    WriteLineIndent("{");
                    Indent(1);
                    WriteLineIndent(*field->name + ".get(&fbe_value." + *field->name + ");");
                    Indent(-1);
                    WriteLineIndent("}");


                }
                Indent(-1);
                WriteLineIndent("else");
                Indent(1);
                if (field->array)
                {
                    // Do nothing here...
                }
                else if (field->vector || field->list || field->set || field->map || field->hash || ((*field->type == "bytes") && !field->optional))
                    WriteLineIndent("fbe_value." + *field->name + ".clear();");
                else
                    WriteLineIndent("fbe_value." + *field->name + " = " + ConvertDefault(*p->name, *field) + ";");
                Indent(-1);
                WriteLineIndent("fbe_current_size += " + *field->name + ".fbe_size();");
            }
        }
    }
    Indent(-1);
    WriteLineIndent("}");
    WriteLine();

    // Generate struct field model set_begin() method
    WriteLineIndent("size_t " + class_name + "::set_begin()");
    WriteLineIndent("{");
    Indent(1);
    WriteLineIndent("assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && \"Model is broken!\");");
    WriteLineIndent("if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())");
    Indent(1);
    WriteLineIndent("return 0;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("uint32_t fbe_struct_size = (uint32_t)fbe_body();");
    WriteLineIndent("uint32_t fbe_struct_offset = (uint32_t)(_buffer.allocate(fbe_struct_size) - _buffer.offset());");
    WriteLineIndent("assert(((fbe_struct_offset > 0) && ((_buffer.offset() + fbe_struct_offset + fbe_struct_size) <= _buffer.size())) && \"Model is broken!\");");
    WriteLineIndent("if ((fbe_struct_offset == 0) || ((_buffer.offset() + fbe_struct_offset + fbe_struct_size) > _buffer.size()))");
    Indent(1);
    WriteLineIndent("return 0;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset(), fbe_struct_offset);");
    WriteLineIndent("unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_struct_offset, fbe_struct_size);");
    WriteLineIndent("unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_struct_offset + 4, fbe_type());");
    WriteLine();
    WriteLineIndent("_buffer.shift(fbe_struct_offset);");
    WriteLineIndent("return fbe_struct_offset;");
    Indent(-1);
    WriteLineIndent("}");
    WriteLine();

    // Generate struct field model set_end() method
    WriteLineIndent("void " + class_name + "::set_end(size_t fbe_begin)");
    WriteLineIndent("{");
    Indent(1);
    WriteLineIndent("_buffer.unshift(fbe_begin);");
    Indent(-1);
    WriteLineIndent("}");
    WriteLine();

    // Generate struct field model set() method
    WriteLineIndent("void " + class_name + "::set(const ::FBE::Base& fbe_value) noexcept");
    WriteLineIndent("{");
    Indent(1);
    WriteLineIndent("size_t fbe_begin = set_begin();");
    WriteLineIndent("if (fbe_begin == 0)");
    Indent(1);
    WriteLineIndent("return;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("set_fields(fbe_value);");
    WriteLineIndent("set_end(fbe_begin);");
    Indent(-1);
    WriteLineIndent("}");
    WriteLine();

    // Generate struct field model set_fields() method
    WriteLineIndent("void " + class_name + "::set_fields([[maybe_unused]] const ::FBE::Base& base_fbe_value) noexcept");
    WriteLineIndent("{");
    Indent(1);
    WriteLineIndent("[[maybe_unused]] const " + struct_name + "& fbe_value = static_cast<const " + struct_name + "&>(base_fbe_value);");
    if ((s->base && !s->base->empty()) || (s->body && !s->body->fields.empty()))
    {
        if (s->base && !s->base->empty())
            WriteLineIndent("parent.set_fields(fbe_value);");
        if (s->body)
            for (const auto& field : s->body->fields)
            {
                if (IsStructType(p, *field->type) && !field->ptr && !IsContainerType(*field) && !field->optional)
                    WriteLineIndent(*field->name + ".set(static_cast<const ::" + *p->name + "::" + *field->type + "&>(" + "fbe_value." + *field->name + "));");
                else
                    WriteLineIndent(*field->name + ".set(fbe_value." + *field->name + ");");
            }
    }
    Indent(-1);
    WriteLineIndent("}");
}

void GeneratorCpp::GenerateStructFieldPtrModel_Source(const std::shared_ptr<Package>& p, const std::shared_ptr<StructType>& s)
{
    std::string struct_name = "::" + *p->name + "::" + *s->name;
    std::string class_name = "FieldModelPtr_" + *p->name + "_" + *s->name;

    // Generate struct field model constructor
    WriteLine();
    WriteLineIndent(class_name + "::" + class_name +"(FBEBuffer& buffer, size_t offset) noexcept : _buffer(buffer), _offset(offset)");
    WriteLineIndent("{}");
    WriteLine();

    WriteLineIndent(class_name + "::~" + class_name +"()");
    WriteLineIndent("{");
    Indent(1);
    WriteLineIndent("if (ptr) delete ptr;");
    Indent(-1);
    WriteLineIndent("}");
    WriteLine();

    // Generate struct field ptr model FBE methods
    WriteLineIndent("size_t " + class_name + "::fbe_extra() const noexcept");
    WriteLineIndent("{");
    Indent(1);
    WriteLineIndent("if (!ptr) return 0;");
    WriteLine();
    WriteLineIndent("if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())");
    Indent(1);
    WriteLineIndent("return 0;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("uint32_t fbe_ptr_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset() + 1);");
    WriteLineIndent("if ((fbe_ptr_offset == 0) || ((_buffer.offset() + fbe_ptr_offset + 4) > _buffer.size()))");
    Indent(1);
    WriteLineIndent("return 0;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("_buffer.shift(fbe_ptr_offset);");
    WriteLineIndent("size_t fbe_result = ptr->fbe_size() + ptr->fbe_extra();");
    WriteLineIndent("_buffer.unshift(fbe_ptr_offset);");
    WriteLine();
    WriteLineIndent("return fbe_result;");
    Indent(-1);
    WriteLineIndent("}");
    WriteLine();

    // Generate struct field model verify() method
    WriteLineIndent("bool " + class_name + "::verify() const noexcept");
    WriteLineIndent("{");
    Indent(1);
    WriteLineIndent("if (!ptr) return true;");
    WriteLine();
    WriteLineIndent("if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())");
    Indent(1);
    WriteLineIndent("return true;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("uint8_t fbe_has_value = *((const uint8_t *)(_buffer.data() + _buffer.offset() + fbe_offset()));");
    WriteLineIndent("if (fbe_has_value == 0)");
    Indent(1);
    WriteLineIndent("return true;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("uint32_t fbe_optional_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset() + 1);");

    WriteLineIndent("if (fbe_optional_offset == 0)");
    Indent(1);
    WriteLineIndent("return false;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("_buffer.shift(fbe_optional_offset);");
    WriteLineIndent("bool fbe_result = ptr->verify();");
    WriteLineIndent("_buffer.unshift(fbe_optional_offset);");
    WriteLineIndent("return fbe_result;");
    Indent(-1);
    WriteLineIndent("}");
    WriteLine();

    // Generate struct field model has_value() method
    WriteLineIndent("bool " + class_name + "::has_value() const noexcept");
    WriteLineIndent("{");
    Indent(1);
    WriteLineIndent("if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())");
    Indent(1);
    WriteLineIndent("return false;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("uint8_t fbe_has_value = *((const uint8_t *)(_buffer.data() + _buffer.offset() + fbe_offset()));");
    WriteLineIndent("return (fbe_has_value != 0);");
    Indent(-1);
    WriteLineIndent("}");
    WriteLine();

    // Generate struct field model get_begin() method
    WriteLineIndent("size_t " + class_name + "::get_begin() const noexcept");
    WriteLineIndent("{");
    Indent(1);
    WriteLineIndent("if (!has_value())");
    Indent(1);
    WriteLineIndent("return 0;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("uint32_t fbe_ptr_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset() + 1);");
    WriteLineIndent("assert((fbe_ptr_offset > 0) && \"Model is broken!\");");
    WriteLineIndent("if (fbe_ptr_offset == 0)");
    Indent(1);
    WriteLineIndent("return 0;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("_buffer.shift(fbe_ptr_offset);");
    WriteLineIndent("return fbe_ptr_offset;");
    Indent(-1);
    WriteLineIndent("}");
    WriteLine();

    // Generate struct field model get_end() method
    WriteLineIndent("void " + class_name + "::get_end(size_t fbe_begin) const noexcept");
    WriteLineIndent("{");
    Indent(1);
    WriteLineIndent("_buffer.unshift(fbe_begin);");
    Indent(-1);
    WriteLineIndent("}");
    WriteLine();

    // Generate struct field model get() method
    WriteLineIndent("void " + class_name + "::get(" + struct_name + "** fbe_value) noexcept");
    WriteLineIndent("{");
    Indent(1);
    WriteLineIndent("size_t fbe_begin = get_begin();");
    WriteLineIndent("if (fbe_begin == 0)");
    Indent(1);
    WriteLineIndent("return;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("ptr = new FieldModel_" + *p->name + "_" + *s->name + "(_buffer, 0);");
    WriteLine();
    WriteLineIndent(struct_name + " *tempModel = new " + struct_name + "();");
    WriteLineIndent("ptr->get(*tempModel);");
    WriteLineIndent("*fbe_value = tempModel;");
    WriteLine();
    WriteLineIndent("get_end(fbe_begin);");
    Indent(-1);
    WriteLineIndent("}");
    WriteLine();

    // Generate struct field model set_begin() method
    WriteLineIndent("size_t " + class_name + "::set_begin(bool has_value)");
    WriteLineIndent("{");
    Indent(1);
    WriteLineIndent("assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && \"Model is broken!\");");
    WriteLineIndent("if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())");
    Indent(1);
    WriteLineIndent("return 0;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("uint8_t fbe_has_value = has_value ? 1 : 0;");
    WriteLineIndent("*((uint8_t *)(_buffer.data() + _buffer.offset() + fbe_offset())) = fbe_has_value;");
    WriteLineIndent("if (fbe_has_value == 0)");
    Indent(1);
    WriteLineIndent("return 0;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("uint32_t fbe_ptr_size = 4;");
    WriteLineIndent("uint32_t fbe_ptr_offset = (uint32_t)(_buffer.allocate(fbe_ptr_size) - _buffer.offset());");
    WriteLineIndent("assert(((fbe_ptr_offset > 0) && ((_buffer.offset() + fbe_ptr_offset + fbe_ptr_size) <= _buffer.size())) && \"Model is broken!\");");
    WriteLineIndent("if ((fbe_ptr_offset == 0) || ((_buffer.offset() + fbe_ptr_offset + fbe_ptr_size) > _buffer.size()))");
    Indent(1);
    WriteLineIndent("return 0;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset() + 1, fbe_ptr_offset);");

    WriteLine();
    WriteLineIndent("_buffer.shift(fbe_ptr_offset);");
    WriteLineIndent("return fbe_ptr_offset;");
    Indent(-1);
    WriteLineIndent("}");
    WriteLine();

    // Generate struct field model set_end() method
    WriteLineIndent("void " + class_name + "::set_end(size_t fbe_begin)");
    WriteLineIndent("{");
    Indent(1);
    WriteLineIndent("_buffer.unshift(fbe_begin);");
    Indent(-1);
    WriteLineIndent("}");
    WriteLine();

    // Generate struct field model set() method
    WriteLineIndent("void " + class_name + "::set(const " + struct_name + "* fbe_value) noexcept");
    WriteLineIndent("{");
    Indent(1);
    WriteLineIndent("size_t fbe_begin = set_begin(fbe_value != nullptr);");
    WriteLineIndent("if (fbe_begin == 0)");
    Indent(1);
    WriteLineIndent("return;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("if (fbe_value != nullptr) {");
    Indent(1);
    WriteLineIndent("BaseFieldModel* temp = new FieldModel_" + *p->name + "_" + *s->name + "(_buffer, 0);");
    WriteLineIndent("ptr = temp;");
    WriteLineIndent("ptr->set(*fbe_value);");
    Indent(-1);
    WriteLineIndent("}");
    WriteLine();
    WriteLineIndent("set_end(fbe_begin);");
    Indent(-1);
    WriteLineIndent("}");
}

void GeneratorCpp::GeneratePtrStructModel_Header(const std::shared_ptr<Package>& p, const std::shared_ptr<StructType>& s)
{
    // Generate namespace begin
    WriteLine();
    WriteLineIndent("namespace " + *p->name + " {");

    std::string struct_name = "::" + *p->name + "::" + *s->name;
    std::string class_name = "FieldModel_" + *p->name + "_" + *s->name;

    // Generate struct model begin
    WriteLine();
    WriteLineIndent("// Fast Binary Encoding " + *s->name + " model");
    WriteLineIndent("class " + *s->name + "Model : public FBE::Model");
    WriteLineIndent("{");
    WriteLineIndent("public:");
    Indent(1);

    // Generate struct model constructor
    WriteLineIndent(*s->name + "Model() : model(this->buffer(), 4) {}");
    WriteLineIndent(*s->name + "Model(const std::shared_ptr<FBEBuffer>& buffer) : FBE::Model(buffer), model(this->buffer(), 4) {}");

    // Generate struct model FBE methods
    WriteLine();
    WriteLineIndent("// Get the model size");
    WriteLineIndent("size_t fbe_size() const noexcept { return model.fbe_size() + model.fbe_extra(); }");
    WriteLineIndent("// Get the model type");
    WriteLineIndent("static constexpr size_t fbe_type() noexcept { return " + class_name + "::fbe_type(); }");

    // Generate struct model verify() method
    WriteLine();
    WriteLineIndent("// Check if the struct value is valid");
    WriteLineIndent("bool verify();");

    // Generate struct model create_begin(), create_end() methods
    WriteLine();
    WriteLineIndent("// Create a new model (begin phase)");
    WriteLineIndent("size_t create_begin();");
    WriteLineIndent("// Create a new model (end phase)");
    WriteLineIndent("size_t create_end(size_t fbe_begin);");

    // Generate struct model serialize(), deserialize() methods
    WriteLine();
    WriteLineIndent("// Serialize the struct value");
    WriteLineIndent("size_t serialize(const " + struct_name + "& value);");
    WriteLineIndent("// Deserialize the struct value");
    WriteLineIndent("size_t deserialize(" + struct_name + "& value) noexcept;");

    // Generate struct model next() method
    WriteLine();
    WriteLineIndent("// Move to the next struct value");
    WriteLineIndent("void next(size_t prev) noexcept { model.fbe_shift(prev); }");

    // Generate struct model accessor
    Indent(-1);
    WriteLine();
    WriteLineIndent("public:");
    Indent(1);
    WriteLineIndent(class_name + " model;");

    // Generate struct model end
    Indent(-1);
    WriteLineIndent("};");

    // Generate namespace end
    WriteLine();
    WriteLineIndent("} // namespace " + *p->name);
}

void GeneratorCpp::GeneratePtrStructModel_Source(const std::shared_ptr<Package>& p, const std::shared_ptr<StructType>& s)
{
    // Generate namespace begin
    WriteLine();
    WriteLineIndent("namespace " + *p->name + " {");

    std::string struct_name = "::" + *p->name + "::" + *s->name;
    std::string model_name = *s->name + "Model";

    // Generate struct model verify() method
    WriteLine();
    WriteLineIndent("bool " + model_name + "::verify()");
    WriteLineIndent("{");
    Indent(1);
    WriteLineIndent("if ((this->buffer().offset() + model.fbe_offset() - 4) > this->buffer().size())");
    Indent(1);
    WriteLineIndent("return false;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("uint32_t fbe_full_size = unaligned_load<uint32_t>(this->buffer().data() + this->buffer().offset() + model.fbe_offset() - 4);");
    WriteLineIndent("if (fbe_full_size < model.fbe_size())");
    Indent(1);
    WriteLineIndent("return false;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("return model.verify();");
    Indent(-1);
    WriteLineIndent("}");
    WriteLine();

    // Generate struct model create_begin() method
    WriteLineIndent("size_t " + model_name + "::create_begin()");
    WriteLineIndent("{");
    Indent(1);
    WriteLineIndent("size_t fbe_begin = this->buffer().allocate(4 + model.fbe_size());");
    WriteLineIndent("return fbe_begin;");
    Indent(-1);
    WriteLineIndent("}");
    WriteLine();

    // Generate struct model create_end() method
    WriteLineIndent("size_t " + model_name + "::create_end(size_t fbe_begin)");
    WriteLineIndent("{");
    Indent(1);
    WriteLineIndent("size_t fbe_end = this->buffer().size();");
    WriteLineIndent("uint32_t fbe_full_size = (uint32_t)(fbe_end - fbe_begin);");
    WriteLineIndent("*((uint32_t*)(this->buffer().data() + this->buffer().offset() + model.fbe_offset() - 4)) = fbe_full_size;");
    WriteLineIndent("return fbe_full_size;");
    Indent(-1);
    WriteLineIndent("}");
    WriteLine();

    // Generate struct model serialize() method
    WriteLineIndent("size_t " + model_name + "::serialize(const " + struct_name + "& value)");
    WriteLineIndent("{");
    Indent(1);
    WriteLineIndent("size_t fbe_begin = create_begin();");
    WriteLineIndent("model.set(value);");
    WriteLineIndent("size_t fbe_full_size = create_end(fbe_begin);");
    WriteLineIndent("return fbe_full_size;");
    Indent(-1);
    WriteLineIndent("}");
    WriteLine();

    // Generate struct model deserialize() method
    WriteLineIndent("size_t " + model_name + "::deserialize(" + struct_name + "& value) noexcept");
    WriteLineIndent("{");
    Indent(1);
    WriteLineIndent("if ((this->buffer().offset() + model.fbe_offset() - 4) > this->buffer().size())");
    Indent(1);
    WriteLineIndent("return 0;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("uint32_t fbe_full_size = unaligned_load<uint32_t>(this->buffer().data() + this->buffer().offset() + model.fbe_offset() - 4);");
    WriteLineIndent("assert((fbe_full_size >= model.fbe_size()) && \"Model is broken!\");");
    WriteLineIndent("if (fbe_full_size < model.fbe_size())");
    Indent(1);
    WriteLineIndent("return 0;");
    Indent(-1);
    WriteLine();
    WriteLineIndent("model.get(value);");
    WriteLineIndent("return fbe_full_size;");
    Indent(-1);
    WriteLineIndent("}");

    // Generate namespace end
    WriteLine();
    WriteLineIndent("} // namespace " + *p->name);
}

bool GeneratorCpp::IsContainerType(const StructField &field) {
    return (field.array || field.vector || field.list || field.set || field.map || field.hash);
}

bool GeneratorCpp::IsContainerType(const VariantValue &variant) {
    return (variant.vector || variant.list || variant.map || variant.hash);
}

bool GeneratorCpp::IsStructType(const std::shared_ptr<Package>& p, const std::string& field_type) {
    for (const auto &s:  p->body->structs) {
        if (*s->name == field_type) {
            return true;
        }
    }
    return false;
}

std::string GeneratorCpp::ConvertPtrTypeName(const std::string& package, const std::string& type) {
    if (type == "bool")
        return "bool";
    else if (type == "byte")
        return "uint8_t";
    else if (type == "bytes")
        return Arena() ? "FBE::pmr_buffer_t" : "FBE::buffer_t";
    else if (type == "char")
        return "char";
    else if (type == "wchar")
        return "wchar_t";
    else if (type == "int8")
        return "int8_t";
    else if (type == "uint8")
        return "uint8_t";
    else if (type == "int16")
        return "int16_t";
    else if (type == "uint16")
        return "uint16_t";
    else if (type == "int32")
        return "int32_t";
    else if (type == "uint32")
        return "uint32_t";
    else if (type == "int64")
        return "int64_t";
    else if (type == "uint64")
        return "uint64_t";
    else if (type == "float")
        return "float";
    else if (type == "double")
        return "double";
    else if (type == "decimal")
        return "FBE::decimal_t";
    else if (type == "string") 
        return Arena()? "stdb::memory::arena_string": "stdb::memory::string";
    else if (type == "timestamp")
        return "uint64_t";
    else if (type == "uuid")
        return "FBE::uuid_t";
    
    std::string result = type;
    bool pkg = !CppCommon::StringUtils::ReplaceAll(result, ".", "::");
    std::string ret = (pkg ? ("::" + package) : "") + "::" + result;
    return ret;
}

// because of the y file. optional and typeptr will not be true in the same, and the ptr will never pointer to primitive type.
std::string GeneratorCpp::ConvertPtrTypeName(const std::string& package, const std::string& type, bool optional, bool typeptr, bool as_argument)
{
    // TODO: conflict with pointer, we need to check. Or omit?
    if (optional)
        return "std::optional<" + ConvertPtrTypeName(package, type) + ">";

    auto ret =  ConvertPtrTypeName(package, type);
    if (typeptr)
    {
        if (as_argument)
            return "std::unique_ptr<" + ret + ">";
        else
            return ret + "*";
    }
    return ret;
}

std::string
GeneratorCpp::ConvertPtrTypeName(const std::string &package, const StructField &field, bool as_argument)
{
    std::string prefix = "std";
    if (Arena()) {
        prefix = "pmr";
    }
    bool typeptr = field.ptr;
    if (field.array)
        return "std::array<" + ConvertPtrTypeName(package, *field.type, field.optional, typeptr, as_argument) + ", " + std::to_string(field.N) + ">";
    else if (field.optional)
        return "std::optional<" + ConvertPtrTypeName(package, *field.type) + ">";
    else if (field.vector)
        return prefix + "::vector<" + ConvertPtrTypeName(package, *field.type, field.optional, typeptr, as_argument) + ">";
    else if (field.list)
        return prefix + "::list<" + ConvertPtrTypeName(package, *field.type, field.optional, typeptr, as_argument) + ">";
    else if (field.set)
        return prefix + "::set<" + ConvertPtrTypeName(package, *field.key, false, typeptr, as_argument) + ">";
    else if (field.map)
        return prefix + "::map<" + ConvertPtrTypeName(package, *field.key, false, false, as_argument) + ", " + ConvertPtrTypeName(package, *field.type, field.optional, typeptr, as_argument) +">";
    else if (field.hash)
        return prefix + "::unordered_map<" + ConvertPtrTypeName(package, *field.key, false, false, as_argument) + ", " + ConvertPtrTypeName(package, *field.type, field.optional, typeptr, as_argument) +">";
    auto s = ConvertPtrTypeName(package, *field.type, field.optional, typeptr, as_argument);
    if (Ptr() && !IsKnownType(*field.type) && !field.ptr && as_argument)
        s += "&&";
    return s;
}

std::string GeneratorCpp::ConvertVariantTypeName(const std::string& package, const VariantValue& variant)
{
    std::string prefix = "std";
    if (Arena()) {
        prefix = "pmr";
    }
    if (variant.vector)
        return prefix + "::vector<" + ConvertPtrTypeName(package, *variant.type, false, variant.ptr, false) + ">";
    else if (variant.list)
        return prefix + "::list<" + ConvertPtrTypeName(package, *variant.type, false, variant.ptr, false) + ">";
    else if (variant.map)
        return prefix + "::map<" + ConvertPtrTypeName(package, *variant.key, false, false, false) + ", " + ConvertPtrTypeName(package, *variant.type, false, variant.ptr, false) +">";
    else if (variant.hash)
        return prefix + "::unordered_map<" + ConvertPtrTypeName(package, *variant.key, false, false, false) + ", " + ConvertPtrTypeName(package, *variant.type, false, variant.ptr, false) +">";
    return ConvertPtrTypeName(package, *variant.type, false, variant.ptr, false);
}

std::string GeneratorCpp::ConvertVariantTypeNameAsArgument(const std::string& package, const VariantValue& variant)
{
    if (variant.ptr) {
        return "const " + ConvertVariantTypeName(package, variant);
    }
    if (IsContainerType(variant))
        return "const " + ConvertVariantTypeName(package, variant) + "&";
    if (IsPrimitiveType(*variant.type, false)) {
        return ConvertVariantTypeName(package, variant);
    }
    return "const " + ConvertVariantTypeName(package, variant) + "&";
}

// two cases:
// 1. struct should be rvalue references, because we disable copy cstr
// 2. for container of ptrs, use unique_ptr instead.
std::string GeneratorCpp::ConvertPtrTypeNameAsArgument(const std::string& package, const StructField& field)
{
    if (field.ptr || IsPrimitiveType(*field.type, false))
        return ConvertPtrTypeName(package, field, true);
    if (IsKnownType(*field.type))
        return "const " + ConvertPtrTypeName(package, field, true) + "&";

    return ConvertPtrTypeName(package, field, true);
}

// Generates fieldmodel for data members in a struct field.
// only used for ptr-based FBE
std::string GeneratorCpp::ConvertPtrFieldModelType(const std::shared_ptr<Package>& p, const std::shared_ptr<StructField>& field) {
    std::string field_model_type;
    if (IsStructType(p, *field->type) || (ImportPtr() && !IsCurrentPackageType(*field->type))) {
        std::string model_name = std::string("FieldModel") + (field->ptr ? "Ptr" : "") + "_" +  (IsCurrentPackageType(*field->type) ? (*p->name + "_") : "") + *field->type;
        CppCommon::StringUtils::ReplaceAll(model_name, ".", "_");
        if (IsContainerType(*field)) {
            field_model_type = "FieldModel";
            if (field->array) {
                field_model_type += "CustomArray<" + model_name + ", " + ConvertPtrTypeName(*p->name, *field->type) + ", " + std::to_string(field->N) + ">";
            } else if (field->vector || field->list || field->set) {
                field_model_type += "CustomVector<" + model_name + ", " + ConvertPtrTypeName(*p->name, *field->type) + ">";
            } else if (field->map || field->hash) {
                // TODO: specification是可以指定为key的，但是因为StructField的ptr指针对value，所以我们暂且不支持对key支持pointer
                std::string kType;
                // 1. struct type
                // 2. import-ptr struct type
                if (IsStructType(p, *field->key) || (!IsCurrentPackageType(*field->key) && ImportPtr())) {
                    kType = std::string("FieldModel") + "_" + (IsCurrentPackageType(*field->type) ? (*p->name + "_") : "") + *field->type;
                    CppCommon::StringUtils::ReplaceAll(kType, ".", "_");

                } else {
                    kType = std::string("FieldModel<") + ConvertPtrTypeName(*p->name, *field->key) + ">";
                }
                auto kStruct = ConvertPtrTypeName(*p->name, *field->key);
                auto vStruct = ConvertPtrTypeName(*p->name, *field->type);
                field_model_type += "CustomMap<" + kType + ", " + model_name + ", " + kStruct  + ", " + vStruct + ">";
            }
        } else if (field->optional) {
            field_model_type = "FieldModelStructOptional<" + model_name + ", " + ConvertPtrTypeName(*p->name, *field->type) + ">";
        } else
            field_model_type = model_name;
    } else if (field->array)
        field_model_type = "FieldModelArray<" + ConvertPtrTypeName(*p->name, *field->type, field->optional, field->ptr, false) + ", " + std::to_string(field->N) + ">";
    else if (field->vector || field->list || field->set)
        field_model_type = "FieldModelVector<" + ConvertPtrTypeName(*p->name, *field->type, field->optional, field->ptr, false) + ">";
    else if (field->map || field->hash)
        field_model_type = "FieldModelMap<" + ConvertPtrTypeName(*p->name, *field->key) + ", " + ConvertPtrTypeName(*p->name, *field->type, field->optional, field->ptr, false) + ">";
    else
        field_model_type = "FieldModel<" + ConvertPtrTypeName(*p->name, *field->type, field->optional, field->ptr, false) + ">";
    return field_model_type;
}

std::string GeneratorCpp::ConvertPtrVariantFieldModelType(const std::shared_ptr<Package>& p, const std::shared_ptr<VariantValue>& variant) {
    std::string variant_field_model_type;
    if (Ptr()) {
        if (IsStructType(p, *variant->type) && !IsKnownType(*variant->type)) {
            std::string model_name = std::string("FieldModel") + (variant->ptr ? "Ptr" : "") + "_" + *p->name + "_" + *variant->type;
            if (IsContainerType(*variant)) {
                variant_field_model_type = "FieldModel";
                if (variant->vector || variant->list)
                    variant_field_model_type += "CustomVector<" + model_name + ", " + ConvertPtrTypeName(*p->name, *variant->type) + ">";
                else if (variant->map || variant->hash){
                    std::string kType = "FieldModel";
                    if (IsKnownType(*variant->key)) {
                        kType += "<" + ConvertPtrTypeName(*p->name, *variant->key) + ">";
                    } else {
                        kType +=  "_" + *p->name + "_" + *variant->type;
                    }
                    auto kStruct = ConvertPtrTypeName(*p->name, *variant->key);
                    auto vStruct = ConvertPtrTypeName(*p->name, *variant->type);
                    variant_field_model_type += "CustomMap<" + kType + ", " + model_name + ", " + kStruct  + ", " + vStruct + ">";
                }
            } else {
                variant_field_model_type += model_name;
            }
        }  else if (variant->vector || variant->list)
            variant_field_model_type = "FieldModelVector<" + ConvertPtrTypeName(*p->name, *variant->type, false, variant->ptr, false) + ">";
        else if (variant->map || variant->hash)
            variant_field_model_type = "FieldModelMap<" + ConvertPtrTypeName(*p->name, *variant->key) + ", " + ConvertPtrTypeName(*p->name, *variant->type, false, variant->ptr, false) + ">";
        else
            variant_field_model_type = "FieldModel<" + ConvertPtrTypeName(*p->name, *variant->type, false, variant->ptr, false) + ">";
    } else if (variant->vector || variant->list) // template based
        variant_field_model_type = "FieldModelVector<" + ConvertTypeName(*p->name, *variant->type, false) + ">";
    else if (variant->map || variant->hash)
        variant_field_model_type = "FieldModelMap<" + ConvertTypeName(*p->name, *variant->key, false) + ", " + ConvertTypeName(*p->name, *variant->type, false) + ">";
    else
        variant_field_model_type = "FieldModel<" + ConvertTypeName(*p->name, *variant->type, false) + ">";
    return variant_field_model_type;
}

} // namespace FBE
