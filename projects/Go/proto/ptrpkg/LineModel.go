//------------------------------------------------------------------------------
// Automatically generated by the Fast Binary Encoding compiler, do not modify!
// https://github.com/chronoxor/FastBinaryEncoding
// Source: ptrpkg.fbe
// FBE version: 1.11.0.0
//------------------------------------------------------------------------------

package ptrpkg

import "errors"
import "fbeproj/proto/fbe"
import "fbeproj/proto/variants_ptr"

// Workaround for Go unused imports issue
var _ = errors.New
var _ = fbe.Version
var _ = variants_ptr.Version

// Fast Binary Encoding Line model
type LineModel struct {
    // Model buffer
    buffer *fbe.Buffer

    // Field model
    model *FieldModelLine
}

// Create a new Line model
func NewLineModel(buffer *fbe.Buffer) *LineModel {
    return &LineModel{buffer: buffer, model: NewFieldModelLine(buffer, 4)}
}

// Get the model buffer
func (m *LineModel) Buffer() *fbe.Buffer { return m.buffer }
// Get the field model
func (m *LineModel) Model() *FieldModelLine { return m.model }

// Get the model size
func (m *LineModel) FBESize() int { return m.model.FBESize() + m.model.FBEExtra() }
// // Get the model type
func (m *LineModel) FBEType() int { return m.model.FBEType() }

// Check if the struct value is valid
func (m *LineModel) Verify() bool {
    if (m.buffer.Offset() + m.model.FBEOffset() - 4) > m.buffer.Size() {
        return false
    }

    fbeFullSize := int(fbe.ReadUInt32(m.buffer.Data(), m.buffer.Offset() + m.model.FBEOffset() - 4))
    if fbeFullSize < m.model.FBESize() {
        return false
    }

    return m.model.Verify()
}

// Create a new model (begin phase)
func (m *LineModel) CreateBegin() int {
    fbeBegin := m.buffer.Allocate(4 + m.model.FBESize())
    return fbeBegin
}

// Create a new model (end phase)
func (m *LineModel) CreateEnd(fbeBegin int) int {
    fbeEnd := m.buffer.Size()
    fbeFullSize := fbeEnd - fbeBegin
    fbe.WriteUInt32(m.buffer.Data(), m.buffer.Offset() + m.model.FBEOffset() - 4, uint32(fbeFullSize))
    return fbeFullSize
}

// Serialize the struct value
func (m *LineModel) Serialize(value *Line) (int, error) {
    fbeBegin := m.CreateBegin()
    err := m.model.Set(value)
    fbeFullSize := m.CreateEnd(fbeBegin)
    return fbeFullSize, err
}

// Deserialize the struct value
func (m *LineModel) Deserialize() (*Line, int, error) {
    value := NewLine()
    fbeFullSize, err := m.DeserializeValue(value)
    return value, fbeFullSize, err
}

// Deserialize the struct value by the given pointer
func (m *LineModel) DeserializeValue(value *Line) (int, error) {
    if (m.buffer.Offset() + m.model.FBEOffset() - 4) > m.buffer.Size() {
        value = NewLine()
        return 0, nil
    }

    fbeFullSize := int(fbe.ReadUInt32(m.buffer.Data(), m.buffer.Offset() + m.model.FBEOffset() - 4))
    if fbeFullSize < m.model.FBESize() {
        value = NewLine()
        return 0, errors.New("model is broken")
    }

    err := m.model.GetValue(value)
    return fbeFullSize, err
}

// Move to the next struct value
func (m *LineModel) Next(prev int) {
    m.model.FBEShift(prev)
}