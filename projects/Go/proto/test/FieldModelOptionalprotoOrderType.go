//------------------------------------------------------------------------------
// Automatically generated by the Fast Binary Encoding compiler, do not modify!
// https://github.com/chronoxor/FastBinaryEncoding
// Source: FBE
// FBE version: 1.11.0.0
//------------------------------------------------------------------------------

package test

import "errors"
import "fbeproj/proto/fbe"
import "fbeproj/proto/proto"

// Workaround for Go unused imports issue
var _ = errors.New
var _ = fbe.Version
var _ = proto.Version

// Fast Binary Encoding optional protoOrderType field model
type FieldModelOptionalprotoOrderType struct {
    // Field model buffer
    buffer *fbe.Buffer
    // Field model buffer offset
    offset int

    // Base field model value
    value *proto.FieldModelOrderType
}

// Create a new optional protoOrderType field model
func NewFieldModelOptionalprotoOrderType(buffer *fbe.Buffer, offset int) *FieldModelOptionalprotoOrderType {
    fbeResult := FieldModelOptionalprotoOrderType{buffer: buffer, offset: offset}
    fbeResult.value = proto.NewFieldModelOrderType(buffer, 0)
    return &fbeResult
}

// Get the optional field model value
func (fm *FieldModelOptionalprotoOrderType) Value() *proto.FieldModelOrderType { return fm.value }

// Get the field size
func (fm *FieldModelOptionalprotoOrderType) FBESize() int { return 1 + 4 }

// Get the field extra size
func (fm *FieldModelOptionalprotoOrderType) FBEExtra() int {
    if !fm.HasValue() {
        return 0
    }

    fbeOptionalOffset := int(fbe.ReadUInt32(fm.buffer.Data(), fm.buffer.Offset() + fm.FBEOffset() + 1))
    if (fbeOptionalOffset == 0) || ((fm.buffer.Offset() + fbeOptionalOffset + 4) > fm.buffer.Size()) {
        return 0
    }

    fm.buffer.Shift(fbeOptionalOffset)
    fbeResult := fm.value.FBESize() + fm.value.FBEExtra()
    fm.buffer.Unshift(fbeOptionalOffset)
    return fbeResult
}

// Get the field offset
func (fm *FieldModelOptionalprotoOrderType) FBEOffset() int { return fm.offset }
// Set the field offset
func (fm *FieldModelOptionalprotoOrderType) SetFBEOffset(value int) { fm.offset = value }

// Shift the current field offset
func (fm *FieldModelOptionalprotoOrderType) FBEShift(size int) { fm.offset += size }
// Unshift the current field offset
func (fm *FieldModelOptionalprotoOrderType) FBEUnshift(size int) { fm.offset -= size }

// Check if the object contains a value
func (fm *FieldModelOptionalprotoOrderType) HasValue() bool {
    if (fm.buffer.Offset() + fm.FBEOffset() + fm.FBESize()) > fm.buffer.Size() {
        return false
    }

    fbeHasValue := fbe.ReadUInt8(fm.buffer.Data(), fm.buffer.Offset() + fm.FBEOffset())
    return fbeHasValue != 0
}

// Check if the optional value is valid
func (fm *FieldModelOptionalprotoOrderType) Verify() bool {
    if (fm.buffer.Offset() + fm.FBEOffset() + fm.FBESize()) > fm.buffer.Size() {
        return true
    }

    fbeHasValue := fbe.ReadUInt8(fm.buffer.Data(), fm.buffer.Offset() + fm.FBEOffset())
    if fbeHasValue == 0 {
        return true
    }

    fbeOptionalOffset := int(fbe.ReadUInt32(fm.buffer.Data(), fm.buffer.Offset() + fm.FBEOffset() + 1))
    if fbeOptionalOffset == 0 {
        return false
    }

    fm.buffer.Shift(fbeOptionalOffset)
    fbeResult := fm.value.Verify()
    fm.buffer.Unshift(fbeOptionalOffset)
    return fbeResult
}

// Get the optional value (being phase)
func (fm *FieldModelOptionalprotoOrderType) GetBegin() (int, error) {
    if !fm.HasValue() {
        return 0, nil
    }

    fbeOptionalOffset := int(fbe.ReadUInt32(fm.buffer.Data(), fm.buffer.Offset() + fm.FBEOffset() + 1))
    if fbeOptionalOffset <= 0 {
        return 0, errors.New("model is broken")
    }

    fm.buffer.Shift(fbeOptionalOffset)
    return fbeOptionalOffset, nil
}

// Get the optional value (end phase)
func (fm *FieldModelOptionalprotoOrderType) GetEnd(fbeBegin int) {
    fm.buffer.Unshift(fbeBegin)
}

// Get the optional value
func (fm *FieldModelOptionalprotoOrderType) Get() (*proto.OrderType, error) {
    var fbeValue *proto.OrderType = nil

    fbeBegin, err := fm.GetBegin()
    if fbeBegin == 0 {
        return fbeValue, err
    }

    fbeValue = proto.NewOrderType()

    err = fm.value.GetValue(fbeValue)
    fm.GetEnd(fbeBegin)
    return fbeValue, err
}

// Set the optional value (begin phase)
func (fm *FieldModelOptionalprotoOrderType) SetBegin(hasValue bool) (int, error) {
    if (fm.buffer.Offset() + fm.FBEOffset() + fm.FBESize()) > fm.buffer.Size() {
        return 0, nil
    }

    fbeHasValue := uint8(0)
    if hasValue {
        fbeHasValue = uint8(1)
    }
    fbe.WriteUInt8(fm.buffer.Data(), fm.buffer.Offset() + fm.FBEOffset(), fbeHasValue)
    if fbeHasValue == 0 {
        return 0, nil
    }

    fbeOptionalSize := fm.value.FBESize()
    fbeOptionalOffset := fm.buffer.Allocate(fbeOptionalSize) - fm.buffer.Offset()
    if (fbeOptionalOffset <= 0) || ((fm.buffer.Offset() + fbeOptionalOffset + fbeOptionalSize) > fm.buffer.Size()) {
        return 0, errors.New("model is broken")
    }

    fbe.WriteUInt32(fm.buffer.Data(), fm.buffer.Offset() + fm.FBEOffset() + 1, uint32(fbeOptionalOffset))

    fm.buffer.Shift(fbeOptionalOffset)
    return fbeOptionalOffset, nil
}

// Set the optional value (end phase)
func (fm *FieldModelOptionalprotoOrderType) SetEnd(fbeBegin int) {
    fm.buffer.Unshift(fbeBegin)
}

// Set the optional value
func (fm *FieldModelOptionalprotoOrderType) Set(fbeValue *proto.OrderType) error {
    fbeBegin, err := fm.SetBegin(fbeValue != nil)
    if fbeBegin == 0 {
        return err
    }

    err = fm.value.Set(fbeValue)
    fm.SetEnd(fbeBegin)
    return err
}
