//------------------------------------------------------------------------------
// Automatically generated by the Fast Binary Encoding compiler, do not modify!
// https://github.com/chronoxor/FastBinaryEncoding
// Source: arena_common.fbe
// FBE version: 1.10.0.0
//------------------------------------------------------------------------------

#include "arena_common_pmr_models.h"

namespace FBE {

FieldModel<::arena_common_pmr::Expr>::FieldModel(FBEBuffer& buffer, size_t offset) noexcept : _buffer(buffer), _offset(offset)
{}

size_t FieldModel<::arena_common_pmr::Expr>::fbe_body() const noexcept
{
    // variant type's fbe_size not included
    size_t fbe_result = 4;
    return fbe_result;
}

size_t FieldModel<::arena_common_pmr::Expr>::fbe_extra() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return 0;

    uint32_t fbe_struct_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    if ((fbe_struct_offset == 0) || ((_buffer.offset() + fbe_struct_offset + 4) > _buffer.size()))
        return 0;

    _buffer.shift(fbe_struct_offset);

    size_t fbe_result = fbe_body();

    _buffer.unshift(fbe_struct_offset);

    return fbe_result;
}

bool FieldModel<::arena_common_pmr::Expr>::verify() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return true;

    uint32_t fbe_variant_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    if ((fbe_variant_offset == 0) || ((_buffer.offset() + fbe_variant_offset + 4) > _buffer.size()))
        return false;

    uint32_t fbe_variant_type = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_variant_offset);
    if (fbe_variant_type < 0 || fbe_variant_type >= 3)
        return false;

    _buffer.shift(fbe_variant_offset);
    switch(fbe_variant_type) {
        case 0: {
            FieldModel<bool> fbe_model(_buffer, 4);
            if (!fbe_model.verify())
                return false;
            break;
        }
        case 1: {
            FieldModel<int32_t> fbe_model(_buffer, 4);
            if (!fbe_model.verify())
                return false;
            break;
        }
        case 2: {
            FieldModel<stdb::memory::arena_string> fbe_model(_buffer, 4);
            if (!fbe_model.verify())
                return false;
            break;
        }
    }

    _buffer.unshift(fbe_variant_offset);
    return true;
}

void FieldModel<::arena_common_pmr::Expr>::get(::arena_common_pmr::Expr& fbe_value) const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    uint32_t fbe_variant_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    assert(((fbe_variant_offset > 0) && ((_buffer.offset() + fbe_variant_offset + 4) <= _buffer.size())) && "Model is broken!");
    if ((fbe_variant_offset == 0) || ((_buffer.offset() + fbe_variant_offset + 4) > _buffer.size()))
        return;
    uint32_t vairant_type_index = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_variant_offset);
    assert(vairant_type_index >= 0 && vairant_type_index < 3 && "Model is broken!");

    _buffer.shift(fbe_variant_offset);

    switch(vairant_type_index) {
        case 0: {
            FieldModel<bool> fbe_model(_buffer, 4);
            fbe_value.emplace<bool>();
            auto& value = std::get<0>(fbe_value);
            fbe_model.get(value);
            break;
        }
        case 1: {
            FieldModel<int32_t> fbe_model(_buffer, 4);
            fbe_value.emplace<int32_t>();
            auto& value = std::get<1>(fbe_value);
            fbe_model.get(value);
            break;
        }
        case 2: {
            FieldModel<stdb::memory::arena_string> fbe_model(_buffer, 4);
            fbe_value.emplace<stdb::memory::arena_string>();
            auto& value = std::get<2>(fbe_value);
            fbe_model.get(value);
            break;
        }
    }

    _buffer.unshift(fbe_variant_offset);
}

size_t FieldModel<::arena_common_pmr::Expr>::set_begin(size_t variant_type_fbe_size, size_t variant_type_index)
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return 0;

    uint32_t fbe_variant_size = (uint32_t)(fbe_body() + variant_type_fbe_size);
    uint32_t fbe_variant_offset = (uint32_t)(_buffer.allocate(fbe_variant_size) - _buffer.offset());
    assert(((fbe_variant_offset > 0) && ((_buffer.offset() + fbe_variant_offset + fbe_variant_size) <= _buffer.size())) && "Model is broken!");
    if ((fbe_variant_offset == 0) || ((_buffer.offset() + fbe_variant_offset + fbe_variant_size) > _buffer.size()))
        return 0;

    unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset(), fbe_variant_offset);
    unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_variant_offset, variant_type_index);

    _buffer.shift(fbe_variant_offset);
    return fbe_variant_offset;
}

void FieldModel<::arena_common_pmr::Expr>::set_end(size_t fbe_begin)
{
    _buffer.unshift(fbe_begin);
}

// Set the variant value
void FieldModel<::arena_common_pmr::Expr>::set(const ::arena_common_pmr::Expr& fbe_value) noexcept
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return;

    std::visit(
        overloaded
        {
            [this, fbe_variant_index = fbe_value.index()](bool v) {
                FieldModel<bool> fbe_model(_buffer, 4);
                size_t fbe_begin = set_begin(fbe_model.fbe_size(), fbe_variant_index);
                if (fbe_begin == 0)
                    return;
                fbe_model.set(v);
                set_end(fbe_begin);
            }
            , [this, fbe_variant_index = fbe_value.index()](int32_t v) {
                FieldModel<int32_t> fbe_model(_buffer, 4);
                size_t fbe_begin = set_begin(fbe_model.fbe_size(), fbe_variant_index);
                if (fbe_begin == 0)
                    return;
                fbe_model.set(v);
                set_end(fbe_begin);
            }
            , [this, fbe_variant_index = fbe_value.index()](const stdb::memory::arena_string& v) {
                FieldModel<stdb::memory::arena_string> fbe_model(_buffer, 4);
                size_t fbe_begin = set_begin(fbe_model.fbe_size(), fbe_variant_index);
                if (fbe_begin == 0)
                    return;
                fbe_model.set(v);
                set_end(fbe_begin);
            }
        },
        fbe_value
    );
}


FieldModel<::arena_common_pmr::Alias>::FieldModel(FBEBuffer& buffer, size_t offset) noexcept : _buffer(buffer), _offset(offset)
    , name(buffer, 4 + 4)
    , optr(buffer, name.fbe_offset() + name.fbe_size())
    , expr(buffer, optr.fbe_offset() + optr.fbe_size())
{}

size_t FieldModel<::arena_common_pmr::Alias>::fbe_body() const noexcept
{
    size_t fbe_result = 4 + 4
        + name.fbe_size()
        + optr.fbe_size()
        + expr.fbe_size()
        ;
    return fbe_result;
}

size_t FieldModel<::arena_common_pmr::Alias>::fbe_extra() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return 0;

    uint32_t fbe_struct_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    if ((fbe_struct_offset == 0) || ((_buffer.offset() + fbe_struct_offset + 4) > _buffer.size()))
        return 0;

    _buffer.shift(fbe_struct_offset);

    size_t fbe_result = fbe_body()
        + name.fbe_extra()
        + optr.fbe_extra()
        + expr.fbe_extra()
        ;

    _buffer.unshift(fbe_struct_offset);

    return fbe_result;
}

bool FieldModel<::arena_common_pmr::Alias>::verify(bool fbe_verify_type) const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return true;

    uint32_t fbe_struct_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    if ((fbe_struct_offset == 0) || ((_buffer.offset() + fbe_struct_offset + 4 + 4) > _buffer.size()))
        return false;

    uint32_t fbe_struct_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_struct_offset);
    if (fbe_struct_size < (4 + 4))
        return false;

    uint32_t fbe_struct_type = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_struct_offset + 4);
    if (fbe_verify_type && (fbe_struct_type != fbe_type()))
        return false;

    _buffer.shift(fbe_struct_offset);
    bool fbe_result = verify_fields(fbe_struct_size);
    _buffer.unshift(fbe_struct_offset);
    return fbe_result;
}

bool FieldModel<::arena_common_pmr::Alias>::verify_fields([[maybe_unused]] size_t fbe_struct_size) const noexcept
{
    size_t fbe_current_size = 4 + 4;

    if ((fbe_current_size + name.fbe_size()) > fbe_struct_size)
        return true;
    if (!name.verify())
        return false;
    fbe_current_size += name.fbe_size();

    if ((fbe_current_size + optr.fbe_size()) > fbe_struct_size)
        return true;
    if (!optr.verify())
        return false;
    fbe_current_size += optr.fbe_size();

    if ((fbe_current_size + expr.fbe_size()) > fbe_struct_size)
        return true;
    if (!expr.verify())
        return false;
    fbe_current_size += expr.fbe_size();

    return true;
}

size_t FieldModel<::arena_common_pmr::Alias>::get_begin() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return 0;

    uint32_t fbe_struct_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    assert(((fbe_struct_offset > 0) && ((_buffer.offset() + fbe_struct_offset + 4 + 4) <= _buffer.size())) && "Model is broken!");
    if ((fbe_struct_offset == 0) || ((_buffer.offset() + fbe_struct_offset + 4 + 4) > _buffer.size()))
        return 0;

    uint32_t fbe_struct_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_struct_offset);
    assert((fbe_struct_size >= (4 + 4)) && "Model is broken!");
    if (fbe_struct_size < (4 + 4))
        return 0;

    _buffer.shift(fbe_struct_offset);
    return fbe_struct_offset;
}

void FieldModel<::arena_common_pmr::Alias>::get_end(size_t fbe_begin) const noexcept
{
    _buffer.unshift(fbe_begin);
}

void FieldModel<::arena_common_pmr::Alias>::get(::arena_common_pmr::Alias& fbe_value) const noexcept
{
    size_t fbe_begin = get_begin();
    if (fbe_begin == 0)
        return;

    uint32_t fbe_struct_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset());
    get_fields(fbe_value, fbe_struct_size);
    get_end(fbe_begin);
}

void FieldModel<::arena_common_pmr::Alias>::get_fields([[maybe_unused]] ::arena_common_pmr::Alias& fbe_value, [[maybe_unused]] size_t fbe_struct_size) const noexcept
{
    size_t fbe_current_size = 4 + 4;

    if ((fbe_current_size + name.fbe_size()) <= fbe_struct_size)
        name.get(fbe_value.name);
    else
        fbe_value.name = "";
    fbe_current_size += name.fbe_size();

    if ((fbe_current_size + optr.fbe_size()) <= fbe_struct_size)
        optr.get(fbe_value.optr);
    else
        fbe_value.optr = ::arena_common_pmr::Optr();
    fbe_current_size += optr.fbe_size();

    if ((fbe_current_size + expr.fbe_size()) <= fbe_struct_size)
        expr.get(fbe_value.expr);
    else
        fbe_value.expr = ::arena_common_pmr::Expr();
    fbe_current_size += expr.fbe_size();
}

size_t FieldModel<::arena_common_pmr::Alias>::set_begin()
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return 0;

    uint32_t fbe_struct_size = (uint32_t)fbe_body();
    uint32_t fbe_struct_offset = (uint32_t)(_buffer.allocate(fbe_struct_size) - _buffer.offset());
    assert(((fbe_struct_offset > 0) && ((_buffer.offset() + fbe_struct_offset + fbe_struct_size) <= _buffer.size())) && "Model is broken!");
    if ((fbe_struct_offset == 0) || ((_buffer.offset() + fbe_struct_offset + fbe_struct_size) > _buffer.size()))
        return 0;

    unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset(), fbe_struct_offset);
    unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_struct_offset, fbe_struct_size);
    unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_struct_offset + 4, (uint32_t)fbe_type());

    _buffer.shift(fbe_struct_offset);
    return fbe_struct_offset;
}

void FieldModel<::arena_common_pmr::Alias>::set_end(size_t fbe_begin)
{
    _buffer.unshift(fbe_begin);
}

void FieldModel<::arena_common_pmr::Alias>::set(const ::arena_common_pmr::Alias& fbe_value) noexcept
{
    size_t fbe_begin = set_begin();
    if (fbe_begin == 0)
        return;

    set_fields(fbe_value);
    set_end(fbe_begin);
}

void FieldModel<::arena_common_pmr::Alias>::set_fields([[maybe_unused]] const ::arena_common_pmr::Alias& fbe_value) noexcept
{
    name.set(fbe_value.name);
    optr.set(fbe_value.optr);
    expr.set(fbe_value.expr);
}

namespace arena_common_pmr {

bool AliasModel::verify()
{
    if ((this->buffer().offset() + model.fbe_offset() - 4) > this->buffer().size())
        return false;

    uint32_t fbe_full_size = unaligned_load<uint32_t>(this->buffer().data() + this->buffer().offset() + model.fbe_offset() - 4);
    if (fbe_full_size < model.fbe_size())
        return false;

    return model.verify();
}

size_t AliasModel::create_begin()
{
    size_t fbe_begin = this->buffer().allocate(4 + model.fbe_size());
    return fbe_begin;
}

size_t AliasModel::create_end(size_t fbe_begin)
{
    size_t fbe_end = this->buffer().size();
    uint32_t fbe_full_size = (uint32_t)(fbe_end - fbe_begin);
    unaligned_store<uint32_t>(this->buffer().data() + this->buffer().offset() + model.fbe_offset() - 4, fbe_full_size);
    return fbe_full_size;
}

size_t AliasModel::serialize(const ::arena_common_pmr::Alias& value)
{
    size_t fbe_begin = create_begin();
    model.set(value);
    size_t fbe_full_size = create_end(fbe_begin);
    return fbe_full_size;
}

size_t AliasModel::deserialize(::arena_common_pmr::Alias& value) const noexcept
{
    if ((this->buffer().offset() + model.fbe_offset() - 4) > this->buffer().size())
        return 0;

    uint32_t fbe_full_size = unaligned_load<uint32_t>(this->buffer().data() + this->buffer().offset() + model.fbe_offset() - 4);
    assert((fbe_full_size >= model.fbe_size()) && "Model is broken!");
    if (fbe_full_size < model.fbe_size())
        return 0;

    model.get(value);
    return fbe_full_size;
}

} // namespace arena_common_pmr

FieldModel<::arena_common_pmr::Expression>::FieldModel(FBEBuffer& buffer, size_t offset) noexcept : _buffer(buffer), _offset(offset)
    , keys(buffer, 4 + 4)
    , aliases(buffer, keys.fbe_offset() + keys.fbe_size())
    , alias_int(buffer, aliases.fbe_offset() + aliases.fbe_size())
{}

size_t FieldModel<::arena_common_pmr::Expression>::fbe_body() const noexcept
{
    size_t fbe_result = 4 + 4
        + keys.fbe_size()
        + aliases.fbe_size()
        + alias_int.fbe_size()
        ;
    return fbe_result;
}

size_t FieldModel<::arena_common_pmr::Expression>::fbe_extra() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return 0;

    uint32_t fbe_struct_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    if ((fbe_struct_offset == 0) || ((_buffer.offset() + fbe_struct_offset + 4) > _buffer.size()))
        return 0;

    _buffer.shift(fbe_struct_offset);

    size_t fbe_result = fbe_body()
        + keys.fbe_extra()
        + aliases.fbe_extra()
        + alias_int.fbe_extra()
        ;

    _buffer.unshift(fbe_struct_offset);

    return fbe_result;
}

bool FieldModel<::arena_common_pmr::Expression>::verify(bool fbe_verify_type) const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return true;

    uint32_t fbe_struct_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    if ((fbe_struct_offset == 0) || ((_buffer.offset() + fbe_struct_offset + 4 + 4) > _buffer.size()))
        return false;

    uint32_t fbe_struct_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_struct_offset);
    if (fbe_struct_size < (4 + 4))
        return false;

    uint32_t fbe_struct_type = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_struct_offset + 4);
    if (fbe_verify_type && (fbe_struct_type != fbe_type()))
        return false;

    _buffer.shift(fbe_struct_offset);
    bool fbe_result = verify_fields(fbe_struct_size);
    _buffer.unshift(fbe_struct_offset);
    return fbe_result;
}

bool FieldModel<::arena_common_pmr::Expression>::verify_fields([[maybe_unused]] size_t fbe_struct_size) const noexcept
{
    size_t fbe_current_size = 4 + 4;

    if ((fbe_current_size + keys.fbe_size()) > fbe_struct_size)
        return true;
    if (!keys.verify())
        return false;
    fbe_current_size += keys.fbe_size();

    if ((fbe_current_size + aliases.fbe_size()) > fbe_struct_size)
        return true;
    if (!aliases.verify())
        return false;
    fbe_current_size += aliases.fbe_size();

    if ((fbe_current_size + alias_int.fbe_size()) > fbe_struct_size)
        return true;
    if (!alias_int.verify())
        return false;
    fbe_current_size += alias_int.fbe_size();

    return true;
}

size_t FieldModel<::arena_common_pmr::Expression>::get_begin() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return 0;

    uint32_t fbe_struct_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    assert(((fbe_struct_offset > 0) && ((_buffer.offset() + fbe_struct_offset + 4 + 4) <= _buffer.size())) && "Model is broken!");
    if ((fbe_struct_offset == 0) || ((_buffer.offset() + fbe_struct_offset + 4 + 4) > _buffer.size()))
        return 0;

    uint32_t fbe_struct_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_struct_offset);
    assert((fbe_struct_size >= (4 + 4)) && "Model is broken!");
    if (fbe_struct_size < (4 + 4))
        return 0;

    _buffer.shift(fbe_struct_offset);
    return fbe_struct_offset;
}

void FieldModel<::arena_common_pmr::Expression>::get_end(size_t fbe_begin) const noexcept
{
    _buffer.unshift(fbe_begin);
}

void FieldModel<::arena_common_pmr::Expression>::get(::arena_common_pmr::Expression& fbe_value) const noexcept
{
    size_t fbe_begin = get_begin();
    if (fbe_begin == 0)
        return;

    uint32_t fbe_struct_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset());
    get_fields(fbe_value, fbe_struct_size);
    get_end(fbe_begin);
}

void FieldModel<::arena_common_pmr::Expression>::get_fields([[maybe_unused]] ::arena_common_pmr::Expression& fbe_value, [[maybe_unused]] size_t fbe_struct_size) const noexcept
{
    size_t fbe_current_size = 4 + 4;

    if ((fbe_current_size + keys.fbe_size()) <= fbe_struct_size)
        keys.get(fbe_value.keys);
    else
        fbe_value.keys.clear();
    fbe_current_size += keys.fbe_size();

    if ((fbe_current_size + aliases.fbe_size()) <= fbe_struct_size)
        aliases.get(fbe_value.aliases);
    else
        fbe_value.aliases.clear();
    fbe_current_size += aliases.fbe_size();

    if ((fbe_current_size + alias_int.fbe_size()) <= fbe_struct_size)
        alias_int.get(fbe_value.alias_int);
    else
        fbe_value.alias_int.clear();
    fbe_current_size += alias_int.fbe_size();
}

size_t FieldModel<::arena_common_pmr::Expression>::set_begin()
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return 0;

    uint32_t fbe_struct_size = (uint32_t)fbe_body();
    uint32_t fbe_struct_offset = (uint32_t)(_buffer.allocate(fbe_struct_size) - _buffer.offset());
    assert(((fbe_struct_offset > 0) && ((_buffer.offset() + fbe_struct_offset + fbe_struct_size) <= _buffer.size())) && "Model is broken!");
    if ((fbe_struct_offset == 0) || ((_buffer.offset() + fbe_struct_offset + fbe_struct_size) > _buffer.size()))
        return 0;

    unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset(), fbe_struct_offset);
    unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_struct_offset, fbe_struct_size);
    unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_struct_offset + 4, (uint32_t)fbe_type());

    _buffer.shift(fbe_struct_offset);
    return fbe_struct_offset;
}

void FieldModel<::arena_common_pmr::Expression>::set_end(size_t fbe_begin)
{
    _buffer.unshift(fbe_begin);
}

void FieldModel<::arena_common_pmr::Expression>::set(const ::arena_common_pmr::Expression& fbe_value) noexcept
{
    size_t fbe_begin = set_begin();
    if (fbe_begin == 0)
        return;

    set_fields(fbe_value);
    set_end(fbe_begin);
}

void FieldModel<::arena_common_pmr::Expression>::set_fields([[maybe_unused]] const ::arena_common_pmr::Expression& fbe_value) noexcept
{
    keys.set(fbe_value.keys);
    aliases.set(fbe_value.aliases);
    alias_int.set(fbe_value.alias_int);
}

namespace arena_common_pmr {

bool ExpressionModel::verify()
{
    if ((this->buffer().offset() + model.fbe_offset() - 4) > this->buffer().size())
        return false;

    uint32_t fbe_full_size = unaligned_load<uint32_t>(this->buffer().data() + this->buffer().offset() + model.fbe_offset() - 4);
    if (fbe_full_size < model.fbe_size())
        return false;

    return model.verify();
}

size_t ExpressionModel::create_begin()
{
    size_t fbe_begin = this->buffer().allocate(4 + model.fbe_size());
    return fbe_begin;
}

size_t ExpressionModel::create_end(size_t fbe_begin)
{
    size_t fbe_end = this->buffer().size();
    uint32_t fbe_full_size = (uint32_t)(fbe_end - fbe_begin);
    unaligned_store<uint32_t>(this->buffer().data() + this->buffer().offset() + model.fbe_offset() - 4, fbe_full_size);
    return fbe_full_size;
}

size_t ExpressionModel::serialize(const ::arena_common_pmr::Expression& value)
{
    size_t fbe_begin = create_begin();
    model.set(value);
    size_t fbe_full_size = create_end(fbe_begin);
    return fbe_full_size;
}

size_t ExpressionModel::deserialize(::arena_common_pmr::Expression& value) const noexcept
{
    if ((this->buffer().offset() + model.fbe_offset() - 4) > this->buffer().size())
        return 0;

    uint32_t fbe_full_size = unaligned_load<uint32_t>(this->buffer().data() + this->buffer().offset() + model.fbe_offset() - 4);
    assert((fbe_full_size >= model.fbe_size()) && "Model is broken!");
    if (fbe_full_size < model.fbe_size())
        return 0;

    model.get(value);
    return fbe_full_size;
}

} // namespace arena_common_pmr

} // namespace FBE
