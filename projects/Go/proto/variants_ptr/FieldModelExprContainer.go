//------------------------------------------------------------------------------
// Automatically generated by the Fast Binary Encoding compiler, do not modify!
// https://github.com/chronoxor/FastBinaryEncoding
// Source: variants_ptr.fbe
// FBE version: 1.11.0.0
//------------------------------------------------------------------------------

package variants_ptr

import "errors"
import "fbeproj/proto/fbe"

// Workaround for Go unused imports issue
var _ = errors.New
var _ = fbe.Version

// Fast Binary Encoding ExprContainer field model
type FieldModelExprContainer struct {
    // Field model buffer
    buffer *fbe.Buffer
    // Field model buffer offset
    offset int

    E *FieldModelExpr
    Eo *FieldModelOptionalExpr
}

// Create a new ExprContainer field model
func NewFieldModelExprContainer(buffer *fbe.Buffer, offset int) *FieldModelExprContainer {
    fbeResult := FieldModelExprContainer{buffer: buffer, offset: offset}
    fbeResult.E = NewFieldModelExpr(buffer, 4 + 4)
    fbeResult.Eo = NewFieldModelOptionalExpr(buffer, fbeResult.E.FBEOffset() + fbeResult.E.FBESize())
    return &fbeResult
}

// Get the field size
func (fm *FieldModelExprContainer) FBESize() int { return 4 }

// Get the field body size
func (fm *FieldModelExprContainer) FBEBody() int {
    fbeResult := 4 + 4 +
        fm.E.FBESize() +
        fm.Eo.FBESize() +
        0
    return fbeResult
}

// Get the field extra size
func (fm *FieldModelExprContainer) FBEExtra() int {
    if (fm.buffer.Offset() + fm.FBEOffset() + fm.FBESize()) > fm.buffer.Size() {
        return 0
    }

    fbeStructOffset := int(fbe.ReadUInt32(fm.buffer.Data(), fm.buffer.Offset() + fm.FBEOffset()))
    if (fbeStructOffset == 0) || ((fm.buffer.Offset() + fbeStructOffset + 4) > fm.buffer.Size()) {
        return 0
    }

    fm.buffer.Shift(fbeStructOffset)

    fbeResult := fm.FBEBody() +
        fm.E.FBEExtra() +
        fm.Eo.FBEExtra() +
        0

    fm.buffer.Unshift(fbeStructOffset)

    return fbeResult
}

// Get the field type
func (fm *FieldModelExprContainer) FBEType() int { return 2 }

// Get the field offset
func (fm *FieldModelExprContainer) FBEOffset() int { return fm.offset }
// Set the field offset
func (fm *FieldModelExprContainer) SetFBEOffset(value int) { fm.offset = value }

// Shift the current field offset
func (fm *FieldModelExprContainer) FBEShift(size int) { fm.offset += size }
// Unshift the current field offset
func (fm *FieldModelExprContainer) FBEUnshift(size int) { fm.offset -= size }

// Check if the struct value is valid
func (fm *FieldModelExprContainer) Verify() bool { return fm.VerifyType(true) }

// Check if the struct value and its type are valid
func (fm *FieldModelExprContainer) VerifyType(fbeVerifyType bool) bool {
    if (fm.buffer.Offset() + fm.FBEOffset() + fm.FBESize()) > fm.buffer.Size() {
        return true
    }

    fbeStructOffset := int(fbe.ReadUInt32(fm.buffer.Data(), fm.buffer.Offset() + fm.FBEOffset()))
    if (fbeStructOffset == 0) || ((fm.buffer.Offset() + fbeStructOffset + 4 + 4) > fm.buffer.Size()) {
        return false
    }

    fbeStructSize := int(fbe.ReadUInt32(fm.buffer.Data(), fm.buffer.Offset() + fbeStructOffset))
    if fbeStructSize < (4 + 4) {
        return false
    }

    fbeStructType := int(fbe.ReadUInt32(fm.buffer.Data(), fm.buffer.Offset() + fbeStructOffset + 4))
    if fbeVerifyType && (fbeStructType != fm.FBEType()) {
        return false
    }

    fm.buffer.Shift(fbeStructOffset)
    fbeResult := fm.VerifyFields(fbeStructSize)
    fm.buffer.Unshift(fbeStructOffset)
    return fbeResult
}

// // Check if the struct value fields are valid
func (fm *FieldModelExprContainer) VerifyFields(fbeStructSize int) bool {
    fbeCurrentSize := 4 + 4

    if (fbeCurrentSize + fm.E.FBESize()) > fbeStructSize {
        return true
    }
    if !fm.E.Verify() {
        return false
    }
    fbeCurrentSize += fm.E.FBESize()

    if (fbeCurrentSize + fm.Eo.FBESize()) > fbeStructSize {
        return true
    }
    if !fm.Eo.Verify() {
        return false
    }
    fbeCurrentSize += fm.Eo.FBESize()

    return true
}

// Get the struct value (begin phase)
func (fm *FieldModelExprContainer) GetBegin() (int, error) {
    if (fm.buffer.Offset() + fm.FBEOffset() + fm.FBESize()) > fm.buffer.Size() {
        return 0, nil
    }

    fbeStructOffset := int(fbe.ReadUInt32(fm.buffer.Data(), fm.buffer.Offset() + fm.FBEOffset()))
    if (fbeStructOffset == 0) || ((fm.buffer.Offset() + fbeStructOffset + 4 + 4) > fm.buffer.Size()) {
        return 0, errors.New("model is broken")
    }

    fbeStructSize := int(fbe.ReadUInt32(fm.buffer.Data(), fm.buffer.Offset() + fbeStructOffset))
    if fbeStructSize < (4 + 4) {
        return 0, errors.New("model is broken")
    }

    fm.buffer.Shift(fbeStructOffset)
    return fbeStructOffset, nil
}

// Get the struct value (end phase)
func (fm *FieldModelExprContainer) GetEnd(fbeBegin int) {
    fm.buffer.Unshift(fbeBegin)
}

// Get the struct value
func (fm *FieldModelExprContainer) Get() (*ExprContainer, error) {
    fbeResult := NewExprContainer()
    return fbeResult, fm.GetValue(fbeResult)
}

// Get the struct value by the given pointer
func (fm *FieldModelExprContainer) GetValue(fbeValue *ExprContainer) error {
    fbeBegin, err := fm.GetBegin()
    if fbeBegin == 0 {
        return err
    }

    fbeStructSize := int(fbe.ReadUInt32(fm.buffer.Data(), fm.buffer.Offset()))
    fm.GetFields(fbeValue, fbeStructSize)
    fm.GetEnd(fbeBegin)
    return nil
}

// Get the struct fields values
func (fm *FieldModelExprContainer) GetFields(fbeValue *ExprContainer, fbeStructSize int) {
    fbeCurrentSize := 4 + 4

    if (fbeCurrentSize + fm.E.FBESize()) <= fbeStructSize {
        _ = fm.E.GetValue(&fbeValue.E)
    } else {
        fbeValue.E = NewExpr()
    }
    fbeCurrentSize += fm.E.FBESize()

    if (fbeCurrentSize + fm.Eo.FBESize()) <= fbeStructSize {
        fbeValue.Eo, _ = fm.Eo.Get()
    } else {
        fbeValue.Eo = nil
    }
    fbeCurrentSize += fm.Eo.FBESize()
}

// Set the struct value (begin phase)
func (fm *FieldModelExprContainer) SetBegin() (int, error) {
    if (fm.buffer.Offset() + fm.FBEOffset() + fm.FBESize()) > fm.buffer.Size() {
        return 0, errors.New("model is broken")
    }

    fbeStructSize := fm.FBEBody()
    fbeStructOffset := fm.buffer.Allocate(fbeStructSize) - fm.buffer.Offset()
    if (fbeStructOffset <= 0) || ((fm.buffer.Offset() + fbeStructOffset + fbeStructSize) > fm.buffer.Size()) {
        return 0, errors.New("model is broken")
    }

    fbe.WriteUInt32(fm.buffer.Data(), fm.buffer.Offset() + fm.FBEOffset(), uint32(fbeStructOffset))
    fbe.WriteUInt32(fm.buffer.Data(), fm.buffer.Offset() + fbeStructOffset, uint32(fbeStructSize))
    fbe.WriteUInt32(fm.buffer.Data(), fm.buffer.Offset() + fbeStructOffset + 4, uint32(fm.FBEType()))

    fm.buffer.Shift(fbeStructOffset)
    return fbeStructOffset, nil
}

// Set the struct value (end phase)
func (fm *FieldModelExprContainer) SetEnd(fbeBegin int) {
    fm.buffer.Unshift(fbeBegin)
}

// Set the struct value
func (fm *FieldModelExprContainer) Set(fbeValue *ExprContainer) error {
    fbeBegin, err := fm.SetBegin()
    if fbeBegin == 0 {
        return err
    }

    err = fm.SetFields(fbeValue)
    fm.SetEnd(fbeBegin)
    return err
}

// Set the struct fields values
func (fm *FieldModelExprContainer) SetFields(fbeValue *ExprContainer) error {
    var err error = nil

    if err = fm.E.Set(&fbeValue.E); err != nil {
        return err
    }
    if err = fm.Eo.Set(fbeValue.Eo); err != nil {
        return err
    }
    return err
}