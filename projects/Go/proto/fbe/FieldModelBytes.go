//------------------------------------------------------------------------------
// Automatically generated by the Fast Binary Encoding compiler, do not modify!
// https://github.com/chronoxor/FastBinaryEncoding
// Source: FBE
// FBE version: 1.10.0.0
//------------------------------------------------------------------------------

package fbe

import "errors"

// Fast Binary Encoding bytes field model
type FieldModelBytes struct {
    // Field model buffer
    buffer *Buffer
    // Field model buffer offset
    offset int
}

// Create a new bytes field model
func NewFieldModelBytes(buffer *Buffer, offset int) *FieldModelBytes {
    return &FieldModelBytes{buffer: buffer, offset: offset}
}

// Get the field size
func (fm *FieldModelBytes) FBESize() int { return 4 }
// Get the field extra size
func (fm *FieldModelBytes) FBEExtra() int {
    if (fm.buffer.Offset() + fm.FBEOffset() + fm.FBESize()) > fm.buffer.Size() {
        return 0
    }

    fbeBytesOffset := int(ReadUInt32(fm.buffer.Data(), fm.buffer.Offset() + fm.FBEOffset()))
    if (fbeBytesOffset == 0) || ((fm.buffer.Offset() + fbeBytesOffset + 4) > fm.buffer.Size()) {
        return 0
    }

    fbeBytesSize := int(ReadUInt32(fm.buffer.Data(), fm.buffer.Offset() + fbeBytesOffset))
    return 4 + fbeBytesSize
}

// Get the field offset
func (fm *FieldModelBytes) FBEOffset() int { return fm.offset }
// Set the field offset
func (fm *FieldModelBytes) SetFBEOffset(value int) { fm.offset = value }

// Shift the current field offset
func (fm *FieldModelBytes) FBEShift(size int) { fm.offset += size }
// Unshift the current field offset
func (fm *FieldModelBytes) FBEUnshift(size int) { fm.offset -= size }

// Check if the bytes value is valid
func (fm *FieldModelBytes) Verify() bool {
    if (fm.buffer.Offset() + fm.FBEOffset() + fm.FBESize()) > fm.buffer.Size() {
        return true
    }

    fbeBytesOffset := int(ReadUInt32(fm.buffer.Data(), fm.buffer.Offset() + fm.FBEOffset()))
    if fbeBytesOffset == 0 {
        return true
    }

    if (fm.buffer.Offset() + fbeBytesOffset + 4) > fm.buffer.Size() {
        return false
    }

    fbeBytesSize := int(ReadUInt32(fm.buffer.Data(), fm.buffer.Offset() + fbeBytesOffset))
    if (fm.buffer.Offset() + fbeBytesOffset + 4 + fbeBytesSize) > fm.buffer.Size() {
        return false
    }

    return true
}

// Get the bytes value
func (fm *FieldModelBytes) Get() ([]byte, error) {
    return fm.GetDefault(make([]byte, 0))
}

// Get the bytes value with provided default value
func (fm *FieldModelBytes) GetDefault(defaults []byte) ([]byte, error) {
    if (fm.buffer.Offset() + fm.FBEOffset() + fm.FBESize()) > fm.buffer.Size() {
        return defaults, nil
    }

    fbeBytesOffset := int(ReadUInt32(fm.buffer.Data(), fm.buffer.Offset() + fm.FBEOffset()))
    if fbeBytesOffset == 0 {
        return defaults, nil
    }

    if (fm.buffer.Offset() + fbeBytesOffset + 4) > fm.buffer.Size() {
        return defaults, errors.New("model is broken")
    }

    fbeBytesSize := int(ReadUInt32(fm.buffer.Data(), fm.buffer.Offset() + fbeBytesOffset))
    if (fm.buffer.Offset() + fbeBytesOffset + 4 + fbeBytesSize) > fm.buffer.Size() {
        return defaults, errors.New("model is broken")
    }

    return ReadBytes(fm.buffer.Data(), fm.buffer.Offset() + fbeBytesOffset + 4, fbeBytesSize), nil
}

// Set the bytes value
func (fm *FieldModelBytes) Set(value []byte) error {
    if (fm.buffer.Offset() + fm.FBEOffset() + fm.FBESize()) > fm.buffer.Size() {
        return errors.New("model is broken")
    }

    fbeBytesSize := len(value)
    fbeBytesOffset := fm.buffer.Allocate(4 + fbeBytesSize) - fm.buffer.Offset()
    if (fbeBytesOffset <= 0) || ((fm.buffer.Offset() + fbeBytesOffset + 4 + fbeBytesSize) > fm.buffer.Size()) {
        return errors.New("model is broken")
    }

    WriteUInt32(fm.buffer.Data(), fm.buffer.Offset() + fm.FBEOffset(), uint32(fbeBytesOffset))
    WriteUInt32(fm.buffer.Data(), fm.buffer.Offset() + fbeBytesOffset, uint32(fbeBytesSize))
    WriteBytes(fm.buffer.Data(), fm.buffer.Offset() + fbeBytesOffset + 4, value)
    return nil
}