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
type PremiumAccountFinalModel struct {
    // Model buffer
    buffer *fbe.Buffer

    // Final model
    model *FinalModelPremiumAccount
}

// Create a new PremiumAccount final model
func NewPremiumAccountFinalModel(buffer *fbe.Buffer) *PremiumAccountFinalModel {
    return &PremiumAccountFinalModel{buffer: buffer, model: NewFinalModelPremiumAccount(buffer, 8)}
}

// Get the final model buffer
func (m *PremiumAccountFinalModel) Buffer() *fbe.Buffer { return m.buffer }
// Get the final model
func (m *PremiumAccountFinalModel) Model() *FinalModelPremiumAccount { return m.model }

// // Get the final model type
func (m *PremiumAccountFinalModel) FBEType() int { return m.model.FBEType() }

// Check if the struct value is valid
func (m *PremiumAccountFinalModel) Verify() bool {
    if (m.buffer.Offset() + m.model.FBEOffset() - 4) > m.buffer.Size() {
        return false
    }

    fbeStructSize := int(fbe.ReadUInt32(m.buffer.Data(), m.buffer.Offset() + m.model.FBEOffset() - 8))
    fbeStructType := int(fbe.ReadUInt32(m.buffer.Data(), m.buffer.Offset() + m.model.FBEOffset() - 4))
    if (fbeStructSize <= 0) || (fbeStructType != m.FBEType()) {
        return false
    }

    return (8 + m.model.Verify()) == fbeStructSize
}

// Create a new final model (begin phase)
func (m *PremiumAccountFinalModel) CreateBegin() int {
    fbeBegin := m.buffer.Allocate(4 + m.model.FBESize())
    return fbeBegin
}

// Create a new final model (end phase)
func (m *PremiumAccountFinalModel) CreateEnd(fbeBegin int) int {
    fbeEnd := m.buffer.Size()
    fbeFullSize := fbeEnd - fbeBegin
    fbe.WriteUInt32(m.buffer.Data(), m.buffer.Offset() + m.model.FBEOffset() - 4, uint32(fbeFullSize))
    return fbeFullSize
}

// Serialize the struct value
func (m *PremiumAccountFinalModel) Serialize(value *PremiumAccount) (int, error) {
    fbeInitialSize := m.buffer.Size()

    fbeStructType := m.FBEType()
    fbeStructSize := 8 + m.model.FBEAllocationSize(value)
    fbeStructOffset := m.buffer.Allocate(fbeStructSize) - m.buffer.Offset()
    if (m.buffer.Offset() + fbeStructOffset + fbeStructSize) > m.buffer.Size() {
        return 0, errors.New("model is broken")
    }

    fbeStructSize, err := m.model.Set(value)
    fbeStructSize += 8
    m.buffer.Resize(fbeInitialSize + fbeStructSize)

    fbe.WriteUInt32(m.buffer.Data(), m.buffer.Offset() + m.model.FBEOffset() - 8, uint32(fbeStructSize))
    fbe.WriteUInt32(m.buffer.Data(), m.buffer.Offset() + m.model.FBEOffset() - 4, uint32(fbeStructType))

    return fbeStructSize, err
}

// Deserialize the struct value
func (m *PremiumAccountFinalModel) Deserialize() (*PremiumAccount, int, error) {
    value := NewPremiumAccount()
    fbeFullSize, err := m.DeserializeValue(value)
    return value, fbeFullSize, err
}

// Deserialize the struct value by the given pointer
func (m *PremiumAccountFinalModel) DeserializeValue(value *PremiumAccount) (int, error) {
    if (m.buffer.Offset() + m.model.FBEOffset()) > m.buffer.Size() {
        value = NewPremiumAccount()
        return 0, errors.New("model is broken")
    }

    fbeStructSize := int(fbe.ReadUInt32(m.buffer.Data(), m.buffer.Offset() + m.model.FBEOffset() - 8))
    fbeStructType := int(fbe.ReadUInt32(m.buffer.Data(), m.buffer.Offset() + m.model.FBEOffset() - 4))
    if (fbeStructSize <= 0) || (fbeStructType != m.FBEType()) {
        value = NewPremiumAccount()
        return 8, errors.New("model is broken")
    }

    fbeStructSize, err := m.model.GetValue(value)
    return 8 + fbeStructSize, err
}

// Move to the next struct value
func (m *PremiumAccountFinalModel) Next(prev int) {
    m.model.FBEShift(prev)
}
