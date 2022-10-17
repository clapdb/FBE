//------------------------------------------------------------------------------
// Automatically generated by the Fast Binary Encoding compiler, do not modify!
// https://github.com/chronoxor/FastBinaryEncoding
// Source: extra.fbe
// FBE version: 1.10.0.0
//------------------------------------------------------------------------------

#include "extra_ptr_models.h"

namespace FBE {

FieldModelPtr_extra_Info::FieldModelPtr_extra_Info(FBEBuffer& buffer, size_t offset) noexcept : _buffer(buffer), _offset(offset)
{}

FieldModelPtr_extra_Info::~FieldModelPtr_extra_Info()
{
    if (ptr) delete ptr;
}

size_t FieldModelPtr_extra_Info::fbe_extra() const noexcept
{
    if (!ptr) return 0;

    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return 0;

    uint32_t fbe_ptr_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset() + 1);
    if ((fbe_ptr_offset == 0) || ((_buffer.offset() + fbe_ptr_offset + 4) > _buffer.size()))
        return 0;

    _buffer.shift(fbe_ptr_offset);
    size_t fbe_result = ptr->fbe_size() + ptr->fbe_extra();
    _buffer.unshift(fbe_ptr_offset);

    return fbe_result;
}

bool FieldModelPtr_extra_Info::verify() const noexcept
{
    if (!ptr) return true;

    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return true;

    uint8_t fbe_has_value = *((const uint8_t *)(_buffer.data() + _buffer.offset() + fbe_offset()));
    if (fbe_has_value == 0)
        return true;

    uint32_t fbe_optional_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset() + 1);
    if (fbe_optional_offset == 0)
        return false;

    _buffer.shift(fbe_optional_offset);
    bool fbe_result = ptr->verify();
    _buffer.unshift(fbe_optional_offset);
    return fbe_result;
}

bool FieldModelPtr_extra_Info::has_value() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return false;

    uint8_t fbe_has_value = *((const uint8_t *)(_buffer.data() + _buffer.offset() + fbe_offset()));
    return (fbe_has_value != 0);
}

size_t FieldModelPtr_extra_Info::get_begin() const noexcept
{
    if (!has_value())
        return 0;

    uint32_t fbe_ptr_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset() + 1);
    assert((fbe_ptr_offset > 0) && "Model is broken!");
    if (fbe_ptr_offset == 0)
        return 0;

    _buffer.shift(fbe_ptr_offset);
    return fbe_ptr_offset;
}

void FieldModelPtr_extra_Info::get_end(size_t fbe_begin) const noexcept
{
    _buffer.unshift(fbe_begin);
}

void FieldModelPtr_extra_Info::get(::extra::Info** fbe_value) noexcept
{
    size_t fbe_begin = get_begin();
    if (fbe_begin == 0)
        return;

    ptr = new FieldModel_extra_Info(_buffer, 0);

    ::extra::Info *tempModel = new ::extra::Info();
    ptr->get(*tempModel);
    *fbe_value = tempModel;

    get_end(fbe_begin);
}

size_t FieldModelPtr_extra_Info::set_begin(bool has_value)
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return 0;

    uint8_t fbe_has_value = has_value ? 1 : 0;
    *((uint8_t *)(_buffer.data() + _buffer.offset() + fbe_offset())) = fbe_has_value;
    if (fbe_has_value == 0)
        return 0;

    uint32_t fbe_ptr_size = 4;
    uint32_t fbe_ptr_offset = (uint32_t)(_buffer.allocate(fbe_ptr_size) - _buffer.offset());
    assert(((fbe_ptr_offset > 0) && ((_buffer.offset() + fbe_ptr_offset + fbe_ptr_size) <= _buffer.size())) && "Model is broken!");
    if ((fbe_ptr_offset == 0) || ((_buffer.offset() + fbe_ptr_offset + fbe_ptr_size) > _buffer.size()))
        return 0;

    unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset() + 1, fbe_ptr_offset);

    _buffer.shift(fbe_ptr_offset);
    return fbe_ptr_offset;
}

void FieldModelPtr_extra_Info::set_end(size_t fbe_begin)
{
    _buffer.unshift(fbe_begin);
}

void FieldModelPtr_extra_Info::set(const ::extra::Info* fbe_value) noexcept
{
    size_t fbe_begin = set_begin(fbe_value != nullptr);
    if (fbe_begin == 0)
        return;

    if (fbe_value != nullptr) {
        BaseFieldModel* temp = new FieldModel_extra_Info(_buffer, 0);
        ptr = temp;
        ptr->set(*fbe_value);
    }

    set_end(fbe_begin);
}

FieldModel_extra_Info::FieldModel_extra_Info(FBEBuffer& buffer, size_t offset) noexcept : _buffer(buffer), _offset(offset)
    , info(buffer, 4 + 4)
    , extra(buffer, info.fbe_offset() + info.fbe_size())
    , extras(buffer, extra.fbe_offset() + extra.fbe_size())
    , extras1(buffer, extras.fbe_offset() + extras.fbe_size())
{}

size_t FieldModel_extra_Info::fbe_body() const noexcept
{
    size_t fbe_result = 4 + 4
        + info.fbe_size()
        + extra.fbe_size()
        + extras.fbe_size()
        + extras1.fbe_size()
        ;
    return fbe_result;
}

size_t FieldModel_extra_Info::fbe_extra() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return 0;

    uint32_t fbe_struct_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    if ((fbe_struct_offset == 0) || ((_buffer.offset() + fbe_struct_offset + 4) > _buffer.size()))
        return 0;

    _buffer.shift(fbe_struct_offset);

    size_t fbe_result = fbe_body()
        + info.fbe_extra()
        + extra.fbe_extra()
        + extras.fbe_extra()
        + extras1.fbe_extra()
        ;

    _buffer.unshift(fbe_struct_offset);

    return fbe_result;
}

bool FieldModel_extra_Info::verify(bool fbe_verify_type) const noexcept
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

bool FieldModel_extra_Info::verify_fields([[maybe_unused]] size_t fbe_struct_size) const noexcept
{
    size_t fbe_current_size = 4 + 4;

    if ((fbe_current_size + info.fbe_size()) > fbe_struct_size)
        return true;
    if (!info.verify())
        return false;
    fbe_current_size += info.fbe_size();

    if ((fbe_current_size + extra.fbe_size()) > fbe_struct_size)
        return true;
    if (!extra.verify())
        return false;
    fbe_current_size += extra.fbe_size();

    if ((fbe_current_size + extras.fbe_size()) > fbe_struct_size)
        return true;
    if (!extras.verify())
        return false;
    fbe_current_size += extras.fbe_size();

    if ((fbe_current_size + extras1.fbe_size()) > fbe_struct_size)
        return true;
    if (!extras1.verify())
        return false;
    fbe_current_size += extras1.fbe_size();

    return true;
}

size_t FieldModel_extra_Info::get_begin() const noexcept
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

void FieldModel_extra_Info::get_end(size_t fbe_begin) const noexcept
{
    _buffer.unshift(fbe_begin);
}

void FieldModel_extra_Info::get(::FBE::Base& fbe_value) noexcept
{
    size_t fbe_begin = get_begin();
    if (fbe_begin == 0)
        return;

    uint32_t fbe_struct_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset());
    get_fields(fbe_value, fbe_struct_size);
    get_end(fbe_begin);
}

void FieldModel_extra_Info::get_fields([[maybe_unused]] ::FBE::Base& base_fbe_value, [[maybe_unused]] size_t fbe_struct_size) noexcept
{
    ::extra::Info& fbe_value = static_cast<::extra::Info&>(base_fbe_value);
    size_t fbe_current_size = 4 + 4;

    if ((fbe_current_size + info.fbe_size()) <= fbe_struct_size)
        {
            info.get(fbe_value.info);
        }
    else
        fbe_value.info = "";
    fbe_current_size += info.fbe_size();

    if ((fbe_current_size + extra.fbe_size()) <= fbe_struct_size)
        {
            extra.get(&fbe_value.extra);
        }
    else
        fbe_value.extra = nullptr;
    fbe_current_size += extra.fbe_size();

    if ((fbe_current_size + extras.fbe_size()) <= fbe_struct_size)
        {
            extras.get(fbe_value.extras);
        }
    else
        fbe_value.extras.clear();
    fbe_current_size += extras.fbe_size();

    if ((fbe_current_size + extras1.fbe_size()) <= fbe_struct_size)
        {
            extras1.get(fbe_value.extras1);
        }
    else
        fbe_value.extras1.clear();
    fbe_current_size += extras1.fbe_size();
}

size_t FieldModel_extra_Info::set_begin()
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
    unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_struct_offset + 4, fbe_type());

    _buffer.shift(fbe_struct_offset);
    return fbe_struct_offset;
}

void FieldModel_extra_Info::set_end(size_t fbe_begin)
{
    _buffer.unshift(fbe_begin);
}

void FieldModel_extra_Info::set(const ::FBE::Base& fbe_value) noexcept
{
    size_t fbe_begin = set_begin();
    if (fbe_begin == 0)
        return;

    set_fields(fbe_value);
    set_end(fbe_begin);
}

void FieldModel_extra_Info::set_fields([[maybe_unused]] const ::FBE::Base& base_fbe_value) noexcept
{
    [[maybe_unused]] const ::extra::Info& fbe_value = static_cast<const ::extra::Info&>(base_fbe_value);
    info.set(fbe_value.info);
    extra.set(fbe_value.extra);
    extras.set(fbe_value.extras);
    extras1.set(fbe_value.extras1);
}

namespace extra {

bool InfoModel::verify()
{
    if ((this->buffer().offset() + model.fbe_offset() - 4) > this->buffer().size())
        return false;

    uint32_t fbe_full_size = unaligned_load<uint32_t>(this->buffer().data() + this->buffer().offset() + model.fbe_offset() - 4);
    if (fbe_full_size < model.fbe_size())
        return false;

    return model.verify();
}

size_t InfoModel::create_begin()
{
    size_t fbe_begin = this->buffer().allocate(4 + model.fbe_size());
    return fbe_begin;
}

size_t InfoModel::create_end(size_t fbe_begin)
{
    size_t fbe_end = this->buffer().size();
    uint32_t fbe_full_size = (uint32_t)(fbe_end - fbe_begin);
    *((uint32_t*)(this->buffer().data() + this->buffer().offset() + model.fbe_offset() - 4)) = fbe_full_size;
    return fbe_full_size;
}

size_t InfoModel::serialize(const ::extra::Info& value)
{
    size_t fbe_begin = create_begin();
    model.set(value);
    size_t fbe_full_size = create_end(fbe_begin);
    return fbe_full_size;
}

size_t InfoModel::deserialize(::extra::Info& value) noexcept
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

} // namespace extra

FieldModelPtr_extra_Extra::FieldModelPtr_extra_Extra(FBEBuffer& buffer, size_t offset) noexcept : _buffer(buffer), _offset(offset)
{}

FieldModelPtr_extra_Extra::~FieldModelPtr_extra_Extra()
{
    if (ptr) delete ptr;
}

size_t FieldModelPtr_extra_Extra::fbe_extra() const noexcept
{
    if (!ptr) return 0;

    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return 0;

    uint32_t fbe_ptr_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset() + 1);
    if ((fbe_ptr_offset == 0) || ((_buffer.offset() + fbe_ptr_offset + 4) > _buffer.size()))
        return 0;

    _buffer.shift(fbe_ptr_offset);
    size_t fbe_result = ptr->fbe_size() + ptr->fbe_extra();
    _buffer.unshift(fbe_ptr_offset);

    return fbe_result;
}

bool FieldModelPtr_extra_Extra::verify() const noexcept
{
    if (!ptr) return true;

    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return true;

    uint8_t fbe_has_value = *((const uint8_t *)(_buffer.data() + _buffer.offset() + fbe_offset()));
    if (fbe_has_value == 0)
        return true;

    uint32_t fbe_optional_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset() + 1);
    if (fbe_optional_offset == 0)
        return false;

    _buffer.shift(fbe_optional_offset);
    bool fbe_result = ptr->verify();
    _buffer.unshift(fbe_optional_offset);
    return fbe_result;
}

bool FieldModelPtr_extra_Extra::has_value() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return false;

    uint8_t fbe_has_value = *((const uint8_t *)(_buffer.data() + _buffer.offset() + fbe_offset()));
    return (fbe_has_value != 0);
}

size_t FieldModelPtr_extra_Extra::get_begin() const noexcept
{
    if (!has_value())
        return 0;

    uint32_t fbe_ptr_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset() + 1);
    assert((fbe_ptr_offset > 0) && "Model is broken!");
    if (fbe_ptr_offset == 0)
        return 0;

    _buffer.shift(fbe_ptr_offset);
    return fbe_ptr_offset;
}

void FieldModelPtr_extra_Extra::get_end(size_t fbe_begin) const noexcept
{
    _buffer.unshift(fbe_begin);
}

void FieldModelPtr_extra_Extra::get(::extra::Extra** fbe_value) noexcept
{
    size_t fbe_begin = get_begin();
    if (fbe_begin == 0)
        return;

    ptr = new FieldModel_extra_Extra(_buffer, 0);

    ::extra::Extra *tempModel = new ::extra::Extra();
    ptr->get(*tempModel);
    *fbe_value = tempModel;

    get_end(fbe_begin);
}

size_t FieldModelPtr_extra_Extra::set_begin(bool has_value)
{
    assert(((_buffer.offset() + fbe_offset() + fbe_size()) <= _buffer.size()) && "Model is broken!");
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return 0;

    uint8_t fbe_has_value = has_value ? 1 : 0;
    *((uint8_t *)(_buffer.data() + _buffer.offset() + fbe_offset())) = fbe_has_value;
    if (fbe_has_value == 0)
        return 0;

    uint32_t fbe_ptr_size = 4;
    uint32_t fbe_ptr_offset = (uint32_t)(_buffer.allocate(fbe_ptr_size) - _buffer.offset());
    assert(((fbe_ptr_offset > 0) && ((_buffer.offset() + fbe_ptr_offset + fbe_ptr_size) <= _buffer.size())) && "Model is broken!");
    if ((fbe_ptr_offset == 0) || ((_buffer.offset() + fbe_ptr_offset + fbe_ptr_size) > _buffer.size()))
        return 0;

    unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset() + 1, fbe_ptr_offset);

    _buffer.shift(fbe_ptr_offset);
    return fbe_ptr_offset;
}

void FieldModelPtr_extra_Extra::set_end(size_t fbe_begin)
{
    _buffer.unshift(fbe_begin);
}

void FieldModelPtr_extra_Extra::set(const ::extra::Extra* fbe_value) noexcept
{
    size_t fbe_begin = set_begin(fbe_value != nullptr);
    if (fbe_begin == 0)
        return;

    if (fbe_value != nullptr) {
        BaseFieldModel* temp = new FieldModel_extra_Extra(_buffer, 0);
        ptr = temp;
        ptr->set(*fbe_value);
    }

    set_end(fbe_begin);
}

FieldModel_extra_Extra::FieldModel_extra_Extra(FBEBuffer& buffer, size_t offset) noexcept : _buffer(buffer), _offset(offset)
    , num(buffer, 4 + 4)
    , data(buffer, num.fbe_offset() + num.fbe_size())
    , info(buffer, data.fbe_offset() + data.fbe_size())
    , info2(buffer, info.fbe_offset() + info.fbe_size())
    , info3(buffer, info2.fbe_offset() + info2.fbe_size())
    , infov(buffer, info3.fbe_offset() + info3.fbe_size())
    , infopv(buffer, infov.fbe_offset() + infov.fbe_size())
    , infol(buffer, infopv.fbe_offset() + infopv.fbe_size())
    , infopl(buffer, infol.fbe_offset() + infol.fbe_size())
{}

size_t FieldModel_extra_Extra::fbe_body() const noexcept
{
    size_t fbe_result = 4 + 4
        + num.fbe_size()
        + data.fbe_size()
        + info.fbe_size()
        + info2.fbe_size()
        + info3.fbe_size()
        + infov.fbe_size()
        + infopv.fbe_size()
        + infol.fbe_size()
        + infopl.fbe_size()
        ;
    return fbe_result;
}

size_t FieldModel_extra_Extra::fbe_extra() const noexcept
{
    if ((_buffer.offset() + fbe_offset() + fbe_size()) > _buffer.size())
        return 0;

    uint32_t fbe_struct_offset = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset() + fbe_offset());
    if ((fbe_struct_offset == 0) || ((_buffer.offset() + fbe_struct_offset + 4) > _buffer.size()))
        return 0;

    _buffer.shift(fbe_struct_offset);

    size_t fbe_result = fbe_body()
        + num.fbe_extra()
        + data.fbe_extra()
        + info.fbe_extra()
        + info2.fbe_extra()
        + info3.fbe_extra()
        + infov.fbe_extra()
        + infopv.fbe_extra()
        + infol.fbe_extra()
        + infopl.fbe_extra()
        ;

    _buffer.unshift(fbe_struct_offset);

    return fbe_result;
}

bool FieldModel_extra_Extra::verify(bool fbe_verify_type) const noexcept
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

bool FieldModel_extra_Extra::verify_fields([[maybe_unused]] size_t fbe_struct_size) const noexcept
{
    size_t fbe_current_size = 4 + 4;

    if ((fbe_current_size + num.fbe_size()) > fbe_struct_size)
        return true;
    if (!num.verify())
        return false;
    fbe_current_size += num.fbe_size();

    if ((fbe_current_size + data.fbe_size()) > fbe_struct_size)
        return true;
    if (!data.verify())
        return false;
    fbe_current_size += data.fbe_size();

    if ((fbe_current_size + info.fbe_size()) > fbe_struct_size)
        return true;
    if (!info.verify())
        return false;
    fbe_current_size += info.fbe_size();

    if ((fbe_current_size + info2.fbe_size()) > fbe_struct_size)
        return true;
    if (!info2.verify())
        return false;
    fbe_current_size += info2.fbe_size();

    if ((fbe_current_size + info3.fbe_size()) > fbe_struct_size)
        return true;
    if (!info3.verify())
        return false;
    fbe_current_size += info3.fbe_size();

    if ((fbe_current_size + infov.fbe_size()) > fbe_struct_size)
        return true;
    if (!infov.verify())
        return false;
    fbe_current_size += infov.fbe_size();

    if ((fbe_current_size + infopv.fbe_size()) > fbe_struct_size)
        return true;
    if (!infopv.verify())
        return false;
    fbe_current_size += infopv.fbe_size();

    if ((fbe_current_size + infol.fbe_size()) > fbe_struct_size)
        return true;
    if (!infol.verify())
        return false;
    fbe_current_size += infol.fbe_size();

    if ((fbe_current_size + infopl.fbe_size()) > fbe_struct_size)
        return true;
    if (!infopl.verify())
        return false;
    fbe_current_size += infopl.fbe_size();

    return true;
}

size_t FieldModel_extra_Extra::get_begin() const noexcept
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

void FieldModel_extra_Extra::get_end(size_t fbe_begin) const noexcept
{
    _buffer.unshift(fbe_begin);
}

void FieldModel_extra_Extra::get(::FBE::Base& fbe_value) noexcept
{
    size_t fbe_begin = get_begin();
    if (fbe_begin == 0)
        return;

    uint32_t fbe_struct_size = unaligned_load<uint32_t>(_buffer.data() + _buffer.offset());
    get_fields(fbe_value, fbe_struct_size);
    get_end(fbe_begin);
}

void FieldModel_extra_Extra::get_fields([[maybe_unused]] ::FBE::Base& base_fbe_value, [[maybe_unused]] size_t fbe_struct_size) noexcept
{
    ::extra::Extra& fbe_value = static_cast<::extra::Extra&>(base_fbe_value);
    size_t fbe_current_size = 4 + 4;

    if ((fbe_current_size + num.fbe_size()) <= fbe_struct_size)
        {
            num.get(fbe_value.num);
        }
    else
        fbe_value.num = (int64_t)0ll;
    fbe_current_size += num.fbe_size();

    if ((fbe_current_size + data.fbe_size()) <= fbe_struct_size)
        {
            data.get(fbe_value.data);
        }
    else
        fbe_value.data = "";
    fbe_current_size += data.fbe_size();

    if ((fbe_current_size + info.fbe_size()) <= fbe_struct_size)
        {
            info.get(&fbe_value.info);
        }
    else
        fbe_value.info = nullptr;
    fbe_current_size += info.fbe_size();

    if ((fbe_current_size + info2.fbe_size()) <= fbe_struct_size)
        {
            info2.get(&fbe_value.info2);
        }
    else
        fbe_value.info2 = nullptr;
    fbe_current_size += info2.fbe_size();

    if ((fbe_current_size + info3.fbe_size()) <= fbe_struct_size)
        {
            info3.get(fbe_value.info3);
        }
    else
        fbe_value.info3 = ::extra::Info();
    fbe_current_size += info3.fbe_size();

    if ((fbe_current_size + infov.fbe_size()) <= fbe_struct_size)
        {
            infov.get(fbe_value.infov);
        }
    else
        fbe_value.infov.clear();
    fbe_current_size += infov.fbe_size();

    if ((fbe_current_size + infopv.fbe_size()) <= fbe_struct_size)
        {
            infopv.get(fbe_value.infopv);
        }
    else
        fbe_value.infopv.clear();
    fbe_current_size += infopv.fbe_size();

    if ((fbe_current_size + infol.fbe_size()) <= fbe_struct_size)
        {
            infol.get(fbe_value.infol);
        }
    else
        fbe_value.infol.clear();
    fbe_current_size += infol.fbe_size();

    if ((fbe_current_size + infopl.fbe_size()) <= fbe_struct_size)
        {
            infopl.get(fbe_value.infopl);
        }
    else
        fbe_value.infopl.clear();
    fbe_current_size += infopl.fbe_size();
}

size_t FieldModel_extra_Extra::set_begin()
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
    unaligned_store<uint32_t>(_buffer.data() + _buffer.offset() + fbe_struct_offset + 4, fbe_type());

    _buffer.shift(fbe_struct_offset);
    return fbe_struct_offset;
}

void FieldModel_extra_Extra::set_end(size_t fbe_begin)
{
    _buffer.unshift(fbe_begin);
}

void FieldModel_extra_Extra::set(const ::FBE::Base& fbe_value) noexcept
{
    size_t fbe_begin = set_begin();
    if (fbe_begin == 0)
        return;

    set_fields(fbe_value);
    set_end(fbe_begin);
}

void FieldModel_extra_Extra::set_fields([[maybe_unused]] const ::FBE::Base& base_fbe_value) noexcept
{
    [[maybe_unused]] const ::extra::Extra& fbe_value = static_cast<const ::extra::Extra&>(base_fbe_value);
    num.set(fbe_value.num);
    data.set(fbe_value.data);
    info.set(fbe_value.info);
    info2.set(fbe_value.info2);
    info3.set(static_cast<const ::extra::Info&>(fbe_value.info3));
    infov.set(fbe_value.infov);
    infopv.set(fbe_value.infopv);
    infol.set(fbe_value.infol);
    infopl.set(fbe_value.infopl);
}

namespace extra {

bool ExtraModel::verify()
{
    if ((this->buffer().offset() + model.fbe_offset() - 4) > this->buffer().size())
        return false;

    uint32_t fbe_full_size = unaligned_load<uint32_t>(this->buffer().data() + this->buffer().offset() + model.fbe_offset() - 4);
    if (fbe_full_size < model.fbe_size())
        return false;

    return model.verify();
}

size_t ExtraModel::create_begin()
{
    size_t fbe_begin = this->buffer().allocate(4 + model.fbe_size());
    return fbe_begin;
}

size_t ExtraModel::create_end(size_t fbe_begin)
{
    size_t fbe_end = this->buffer().size();
    uint32_t fbe_full_size = (uint32_t)(fbe_end - fbe_begin);
    *((uint32_t*)(this->buffer().data() + this->buffer().offset() + model.fbe_offset() - 4)) = fbe_full_size;
    return fbe_full_size;
}

size_t ExtraModel::serialize(const ::extra::Extra& value)
{
    size_t fbe_begin = create_begin();
    model.set(value);
    size_t fbe_full_size = create_end(fbe_begin);
    return fbe_full_size;
}

size_t ExtraModel::deserialize(::extra::Extra& value) noexcept
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

} // namespace extra

} // namespace FBE