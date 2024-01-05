//------------------------------------------------------------------------------
// Automatically generated by the Fast Binary Encoding compiler, do not modify!
// https://github.com/chronoxor/FastBinaryEncoding
// Source: proto.fbe
// FBE version: 1.11.0.0
//------------------------------------------------------------------------------

package proto

import "errors"
import "fbeproj/proto/fbe"

// Workaround for Go unused imports issue
var _ = errors.New
var _ = fbe.Version

// Fast Binary Encoding PremiumAccount field model
type FieldModelPremiumAccount struct {
    // Field model buffer
    buffer *fbe.Buffer
    // Field model buffer offset
    offset int

    Id *fbe.FieldModelInt32
    Name *fbe.FieldModelString
    Info *fbe.FieldModelString
    Private_wallet *FieldModelBalance
    Private_orders *FieldModelVectorOrder
    Private_state *FieldModelState
}

// Create a new PremiumAccount field model
func NewFieldModelPremiumAccount(buffer *fbe.Buffer, offset int) *FieldModelPremiumAccount {
    fbeResult := FieldModelPremiumAccount{buffer: buffer, offset: offset}
    fbeResult.Id = fbe.NewFieldModelInt32(buffer, 4 + 4)
    fbeResult.Name = fbe.NewFieldModelString(buffer, fbeResult.Id.FBEOffset() + fbeResult.Id.FBESize())
    fbeResult.Info = fbe.NewFieldModelString(buffer, fbeResult.Name.FBEOffset() + fbeResult.Name.FBESize())
    fbeResult.Private_wallet = NewFieldModelBalance(buffer, fbeResult.Info.FBEOffset() + fbeResult.Info.FBESize())
    fbeResult.Private_orders = NewFieldModelVectorOrder(buffer, fbeResult.Private_wallet.FBEOffset() + fbeResult.Private_wallet.FBESize())
    fbeResult.Private_state = NewFieldModelState(buffer, fbeResult.Private_orders.FBEOffset() + fbeResult.Private_orders.FBESize())
    return &fbeResult
}

// Get the field size
func (fm *FieldModelPremiumAccount) FBESize() int { return 4 }

// Get the field body size
func (fm *FieldModelPremiumAccount) FBEBody() int {
    fbeResult := 4 + 4 +
        fm.Id.FBESize() +
        fm.Name.FBESize() +
        fm.Info.FBESize() +
        fm.Private_wallet.FBESize() +
        fm.Private_orders.FBESize() +
        fm.Private_state.FBESize() +
        0
    return fbeResult
}

// Get the field extra size
func (fm *FieldModelPremiumAccount) FBEExtra() int {
    if (fm.buffer.Offset() + fm.FBEOffset() + fm.FBESize()) > fm.buffer.Size() {
        return 0
    }

    fbeStructOffset := int(fbe.ReadUInt32(fm.buffer.Data(), fm.buffer.Offset() + fm.FBEOffset()))
    if (fbeStructOffset == 0) || ((fm.buffer.Offset() + fbeStructOffset + 4) > fm.buffer.Size()) {
        return 0
    }

    fm.buffer.Shift(fbeStructOffset)

    fbeResult := fm.FBEBody() +
        fm.Id.FBEExtra() +
        fm.Name.FBEExtra() +
        fm.Info.FBEExtra() +
        fm.Private_wallet.FBEExtra() +
        fm.Private_orders.FBEExtra() +
        fm.Private_state.FBEExtra() +
        0

    fm.buffer.Unshift(fbeStructOffset)

    return fbeResult
}

// Get the field type
func (fm *FieldModelPremiumAccount) FBEType() int { return 5 }

// Get the field offset
func (fm *FieldModelPremiumAccount) FBEOffset() int { return fm.offset }
// Set the field offset
func (fm *FieldModelPremiumAccount) SetFBEOffset(value int) { fm.offset = value }

// Shift the current field offset
func (fm *FieldModelPremiumAccount) FBEShift(size int) { fm.offset += size }
// Unshift the current field offset
func (fm *FieldModelPremiumAccount) FBEUnshift(size int) { fm.offset -= size }

// Check if the struct value is valid
func (fm *FieldModelPremiumAccount) Verify() bool { return fm.VerifyType(true) }

// Check if the struct value and its type are valid
func (fm *FieldModelPremiumAccount) VerifyType(fbeVerifyType bool) bool {
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
func (fm *FieldModelPremiumAccount) VerifyFields(fbeStructSize int) bool {
    fbeCurrentSize := 4 + 4

    if (fbeCurrentSize + fm.Id.FBESize()) > fbeStructSize {
        return true
    }
    if !fm.Id.Verify() {
        return false
    }
    fbeCurrentSize += fm.Id.FBESize()

    if (fbeCurrentSize + fm.Name.FBESize()) > fbeStructSize {
        return true
    }
    if !fm.Name.Verify() {
        return false
    }
    fbeCurrentSize += fm.Name.FBESize()

    if (fbeCurrentSize + fm.Info.FBESize()) > fbeStructSize {
        return true
    }
    if !fm.Info.Verify() {
        return false
    }
    fbeCurrentSize += fm.Info.FBESize()

    if (fbeCurrentSize + fm.Private_wallet.FBESize()) > fbeStructSize {
        return true
    }
    if !fm.Private_wallet.Verify() {
        return false
    }
    fbeCurrentSize += fm.Private_wallet.FBESize()

    if (fbeCurrentSize + fm.Private_orders.FBESize()) > fbeStructSize {
        return true
    }
    if !fm.Private_orders.Verify() {
        return false
    }
    fbeCurrentSize += fm.Private_orders.FBESize()

    if (fbeCurrentSize + fm.Private_state.FBESize()) > fbeStructSize {
        return true
    }
    if !fm.Private_state.Verify() {
        return false
    }
    fbeCurrentSize += fm.Private_state.FBESize()

    return true
}

// Get the struct value (begin phase)
func (fm *FieldModelPremiumAccount) GetBegin() (int, error) {
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
func (fm *FieldModelPremiumAccount) GetEnd(fbeBegin int) {
    fm.buffer.Unshift(fbeBegin)
}

// Get the struct value
func (fm *FieldModelPremiumAccount) Get() (*PremiumAccount, error) {
    fbeResult := NewPremiumAccount()
    return fbeResult, fm.GetValue(fbeResult)
}

// Get the struct value by the given pointer
func (fm *FieldModelPremiumAccount) GetValue(fbeValue *PremiumAccount) error {
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
func (fm *FieldModelPremiumAccount) GetFields(fbeValue *PremiumAccount, fbeStructSize int) {
    fbeCurrentSize := 4 + 4

    if (fbeCurrentSize + fm.Id.FBESize()) <= fbeStructSize {
        fbeValue.Id, _ = fm.Id.Get()
    } else {
        fbeValue.Id = 0
    }
    fbeCurrentSize += fm.Id.FBESize()

    if (fbeCurrentSize + fm.Name.FBESize()) <= fbeStructSize {
        fbeValue.Name, _ = fm.Name.Get()
    } else {
        fbeValue.Name = ""
    }
    fbeCurrentSize += fm.Name.FBESize()

    if (fbeCurrentSize + fm.Info.FBESize()) <= fbeStructSize {
        fbeValue.Info, _ = fm.Info.Get()
    } else {
        fbeValue.Info = ""
    }
    fbeCurrentSize += fm.Info.FBESize()

    if (fbeCurrentSize + fm.Private_wallet.FBESize()) <= fbeStructSize {
        _ = fm.Private_wallet.GetValue(&fbeValue.Private_wallet)
    } else {
        fbeValue.Private_wallet = *NewBalance()
    }
    fbeCurrentSize += fm.Private_wallet.FBESize()

    if (fbeCurrentSize + fm.Private_orders.FBESize()) <= fbeStructSize {
        fbeValue.Private_orders, _ = fm.Private_orders.Get()
    } else {
        fbeValue.Private_orders = make([]Order, 0)
    }
    fbeCurrentSize += fm.Private_orders.FBESize()

    if (fbeCurrentSize + fm.Private_state.FBESize()) <= fbeStructSize {
        _ = fm.Private_state.GetValueDefault(&fbeValue.Private_state, State_bad)
    } else {
        fbeValue.Private_state = State_bad
    }
    fbeCurrentSize += fm.Private_state.FBESize()
}

// Set the struct value (begin phase)
func (fm *FieldModelPremiumAccount) SetBegin() (int, error) {
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
func (fm *FieldModelPremiumAccount) SetEnd(fbeBegin int) {
    fm.buffer.Unshift(fbeBegin)
}

// Set the struct value
func (fm *FieldModelPremiumAccount) Set(fbeValue *PremiumAccount) error {
    fbeBegin, err := fm.SetBegin()
    if fbeBegin == 0 {
        return err
    }

    err = fm.SetFields(fbeValue)
    fm.SetEnd(fbeBegin)
    return err
}

// Set the struct fields values
func (fm *FieldModelPremiumAccount) SetFields(fbeValue *PremiumAccount) error {
    var err error = nil

    if err = fm.Id.Set(fbeValue.Id); err != nil {
        return err
    }
    if err = fm.Name.Set(fbeValue.Name); err != nil {
        return err
    }
    if err = fm.Info.Set(fbeValue.Info); err != nil {
        return err
    }
    if err = fm.Private_wallet.Set(&fbeValue.Private_wallet); err != nil {
        return err
    }
    if err = fm.Private_orders.Set(fbeValue.Private_orders); err != nil {
        return err
    }
    if err = fm.Private_state.Set(&fbeValue.Private_state); err != nil {
        return err
    }
    return err
}
