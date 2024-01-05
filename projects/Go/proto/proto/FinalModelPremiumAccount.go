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

// Fast Binary Encoding PremiumAccount final model
type FinalModelPremiumAccount struct {
    buffer *fbe.Buffer  // Final model buffer
    offset int          // Final model buffer offset

    Id *fbe.FinalModelInt32
    Name *fbe.FinalModelString
    Info *fbe.FinalModelString
    Private_wallet *FinalModelBalance
    Private_orders *FinalModelVectorOrder
    Private_state *FinalModelState
}

// Create a new PremiumAccount final model
func NewFinalModelPremiumAccount(buffer *fbe.Buffer, offset int) *FinalModelPremiumAccount {
    fbeResult := FinalModelPremiumAccount{buffer: buffer, offset: offset}
    fbeResult.Id = fbe.NewFinalModelInt32(buffer, 0)
    fbeResult.Name = fbe.NewFinalModelString(buffer, 0)
    fbeResult.Info = fbe.NewFinalModelString(buffer, 0)
    fbeResult.Private_wallet = NewFinalModelBalance(buffer, 0)
    fbeResult.Private_orders = NewFinalModelVectorOrder(buffer, 0)
    fbeResult.Private_state = NewFinalModelState(buffer, 0)
    return &fbeResult
}

// Get the allocation size
func (fm *FinalModelPremiumAccount) FBEAllocationSize(fbeValue *PremiumAccount) int {
    fbeResult := 0 +
        fm.Id.FBEAllocationSize(fbeValue.Id) +
        fm.Name.FBEAllocationSize(fbeValue.Name) +
        fm.Info.FBEAllocationSize(fbeValue.Info) +
        fm.Private_wallet.FBEAllocationSize(&fbeValue.Private_wallet) +
        fm.Private_orders.FBEAllocationSize(fbeValue.Private_orders) +
        fm.Private_state.FBEAllocationSize(&fbeValue.Private_state) +
        0
    return fbeResult
}

// Get the final size
func (fm *FinalModelPremiumAccount) FBESize() int { return 0 }

// Get the final extra size
func (fm *FinalModelPremiumAccount) FBEExtra() int { return 0 }

// Get the final type
func (fm *FinalModelPremiumAccount) FBEType() int { return 5 }

// Get the final offset
func (fm *FinalModelPremiumAccount) FBEOffset() int { return fm.offset }
// Set the final offset
func (fm *FinalModelPremiumAccount) SetFBEOffset(value int) { fm.offset = value }

// Shift the current final offset
func (fm *FinalModelPremiumAccount) FBEShift(size int) { fm.offset += size }
// Unshift the current final offset
func (fm *FinalModelPremiumAccount) FBEUnshift(size int) { fm.offset -= size }

// Check if the struct value is valid
func (fm *FinalModelPremiumAccount) Verify() int {
    fm.buffer.Shift(fm.FBEOffset())
    fbeResult := fm.VerifyFields()
    fm.buffer.Unshift(fm.FBEOffset())
    return fbeResult
}

// Check if the struct fields are valid
func (fm *FinalModelPremiumAccount) VerifyFields() int {
    fbeCurrentOffset := 0
    fbeFieldSize := 0


    fm.Id.SetFBEOffset(fbeCurrentOffset)
    if fbeFieldSize = fm.Id.Verify(); fbeFieldSize == fbe.MaxInt {
        return fbe.MaxInt
    }
    fbeCurrentOffset += fbeFieldSize

    fm.Name.SetFBEOffset(fbeCurrentOffset)
    if fbeFieldSize = fm.Name.Verify(); fbeFieldSize == fbe.MaxInt {
        return fbe.MaxInt
    }
    fbeCurrentOffset += fbeFieldSize

    fm.Info.SetFBEOffset(fbeCurrentOffset)
    if fbeFieldSize = fm.Info.Verify(); fbeFieldSize == fbe.MaxInt {
        return fbe.MaxInt
    }
    fbeCurrentOffset += fbeFieldSize

    fm.Private_wallet.SetFBEOffset(fbeCurrentOffset)
    if fbeFieldSize = fm.Private_wallet.Verify(); fbeFieldSize == fbe.MaxInt {
        return fbe.MaxInt
    }
    fbeCurrentOffset += fbeFieldSize

    fm.Private_orders.SetFBEOffset(fbeCurrentOffset)
    if fbeFieldSize = fm.Private_orders.Verify(); fbeFieldSize == fbe.MaxInt {
        return fbe.MaxInt
    }
    fbeCurrentOffset += fbeFieldSize

    fm.Private_state.SetFBEOffset(fbeCurrentOffset)
    if fbeFieldSize = fm.Private_state.Verify(); fbeFieldSize == fbe.MaxInt {
        return fbe.MaxInt
    }
    fbeCurrentOffset += fbeFieldSize

    return fbeCurrentOffset
}

// Get the struct value
func (fm *FinalModelPremiumAccount) Get() (*PremiumAccount, int, error) {
    fbeResult := NewPremiumAccount()
    fbeSize, err := fm.GetValue(fbeResult)
    return fbeResult, fbeSize, err
}

// Get the struct value by the given pointer
func (fm *FinalModelPremiumAccount) GetValue(fbeValue *PremiumAccount) (int, error) {
    fm.buffer.Shift(fm.FBEOffset())
    fbeSize, err := fm.GetFields(fbeValue)
    fm.buffer.Unshift(fm.FBEOffset())
    return fbeSize, err
}

// Get the struct fields values
func (fm *FinalModelPremiumAccount) GetFields(fbeValue *PremiumAccount) (int, error) {
    var err error = nil
    fbeCurrentSize := 0
    fbeCurrentOffset := 0
    fbeFieldSize := 0

    fm.Id.SetFBEOffset(fbeCurrentOffset)
    if fbeValue.Id, fbeFieldSize, err = fm.Id.Get(); err != nil {
        return fbeCurrentSize, err
    }
    fbeCurrentOffset += fbeFieldSize
    fbeCurrentSize += fbeFieldSize

    fm.Name.SetFBEOffset(fbeCurrentOffset)
    if fbeValue.Name, fbeFieldSize, err = fm.Name.Get(); err != nil {
        return fbeCurrentSize, err
    }
    fbeCurrentOffset += fbeFieldSize
    fbeCurrentSize += fbeFieldSize

    fm.Info.SetFBEOffset(fbeCurrentOffset)
    if fbeValue.Info, fbeFieldSize, err = fm.Info.Get(); err != nil {
        return fbeCurrentSize, err
    }
    fbeCurrentOffset += fbeFieldSize
    fbeCurrentSize += fbeFieldSize

    fm.Private_wallet.SetFBEOffset(fbeCurrentOffset)
    if fbeFieldSize, err = fm.Private_wallet.GetValue(&fbeValue.Private_wallet); err != nil {
        return fbeCurrentSize, err
    }
    fbeCurrentOffset += fbeFieldSize
    fbeCurrentSize += fbeFieldSize

    fm.Private_orders.SetFBEOffset(fbeCurrentOffset)
    if fbeValue.Private_orders, fbeFieldSize, err = fm.Private_orders.Get(); err != nil {
        return fbeCurrentSize, err
    }
    fbeCurrentOffset += fbeFieldSize
    fbeCurrentSize += fbeFieldSize

    fm.Private_state.SetFBEOffset(fbeCurrentOffset)
    if fbeFieldSize, err = fm.Private_state.GetValue(&fbeValue.Private_state); err != nil {
        return fbeCurrentSize, err
    }
    fbeCurrentOffset += fbeFieldSize
    fbeCurrentSize += fbeFieldSize

    return fbeCurrentSize, err
}

// Set the struct value
func (fm *FinalModelPremiumAccount) Set(fbeValue *PremiumAccount) (int, error) {
    fm.buffer.Shift(fm.FBEOffset())
    fbeResult, err := fm.SetFields(fbeValue)
    fm.buffer.Unshift(fm.FBEOffset())
    return fbeResult, err
}

// Set the struct fields values
func (fm *FinalModelPremiumAccount) SetFields(fbeValue *PremiumAccount) (int, error) {
    var err error = nil
    fbeCurrentSize := 0
    fbeCurrentOffset := 0
    fbeFieldSize := 0

    fm.Id.SetFBEOffset(fbeCurrentOffset)
    if fbeFieldSize, err = fm.Id.Set(fbeValue.Id); err != nil {
        return fbeCurrentSize, err
    }
    fbeCurrentOffset += fbeFieldSize
    fbeCurrentSize += fbeFieldSize

    fm.Name.SetFBEOffset(fbeCurrentOffset)
    if fbeFieldSize, err = fm.Name.Set(fbeValue.Name); err != nil {
        return fbeCurrentSize, err
    }
    fbeCurrentOffset += fbeFieldSize
    fbeCurrentSize += fbeFieldSize

    fm.Info.SetFBEOffset(fbeCurrentOffset)
    if fbeFieldSize, err = fm.Info.Set(fbeValue.Info); err != nil {
        return fbeCurrentSize, err
    }
    fbeCurrentOffset += fbeFieldSize
    fbeCurrentSize += fbeFieldSize

    fm.Private_wallet.SetFBEOffset(fbeCurrentOffset)
    if fbeFieldSize, err = fm.Private_wallet.Set(&fbeValue.Private_wallet); err != nil {
        return fbeCurrentSize, err
    }
    fbeCurrentOffset += fbeFieldSize
    fbeCurrentSize += fbeFieldSize

    fm.Private_orders.SetFBEOffset(fbeCurrentOffset)
    if fbeFieldSize, err = fm.Private_orders.Set(fbeValue.Private_orders); err != nil {
        return fbeCurrentSize, err
    }
    fbeCurrentOffset += fbeFieldSize
    fbeCurrentSize += fbeFieldSize

    fm.Private_state.SetFBEOffset(fbeCurrentOffset)
    if fbeFieldSize, err = fm.Private_state.Set(&fbeValue.Private_state); err != nil {
        return fbeCurrentSize, err
    }
    fbeCurrentOffset += fbeFieldSize
    fbeCurrentSize += fbeFieldSize

    return fbeCurrentSize, err
}
