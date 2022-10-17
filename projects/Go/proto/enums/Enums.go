//------------------------------------------------------------------------------
// Automatically generated by the Fast Binary Encoding compiler, do not modify!
// https://github.com/chronoxor/FastBinaryEncoding
// Source: enums.fbe
// FBE version: 1.10.0.0
//------------------------------------------------------------------------------

package enums

import "fmt"
import "strconv"
import "strings"
import "errors"
import "../fbe"

// Workaround for Go unused imports issue
var _ = errors.New
var _ = fbe.Version

// Workaround for Go unused imports issue
var _ = fmt.Print
var _ = strconv.FormatInt

// Enums key
type EnumsKey struct {
}

// Convert Enums flags key to string
func (k *EnumsKey) String() string {
    var sb strings.Builder
    sb.WriteString("EnumsKey(")
    sb.WriteString(")")
    return sb.String()
}

// Enums struct
type Enums struct {
    Byte0 EnumByte `json:"byte0"`
    Byte1 EnumByte `json:"byte1"`
    Byte2 EnumByte `json:"byte2"`
    Byte3 EnumByte `json:"byte3"`
    Byte4 EnumByte `json:"byte4"`
    Byte5 EnumByte `json:"byte5"`
    Char0 EnumChar `json:"char0"`
    Char1 EnumChar `json:"char1"`
    Char2 EnumChar `json:"char2"`
    Char3 EnumChar `json:"char3"`
    Char4 EnumChar `json:"char4"`
    Char5 EnumChar `json:"char5"`
    Wchar0 EnumWChar `json:"wchar0"`
    Wchar1 EnumWChar `json:"wchar1"`
    Wchar2 EnumWChar `json:"wchar2"`
    Wchar3 EnumWChar `json:"wchar3"`
    Wchar4 EnumWChar `json:"wchar4"`
    Wchar5 EnumWChar `json:"wchar5"`
    Int8b0 EnumInt8 `json:"int8b0"`
    Int8b1 EnumInt8 `json:"int8b1"`
    Int8b2 EnumInt8 `json:"int8b2"`
    Int8b3 EnumInt8 `json:"int8b3"`
    Int8b4 EnumInt8 `json:"int8b4"`
    Int8b5 EnumInt8 `json:"int8b5"`
    Uint8b0 EnumUInt8 `json:"uint8b0"`
    Uint8b1 EnumUInt8 `json:"uint8b1"`
    Uint8b2 EnumUInt8 `json:"uint8b2"`
    Uint8b3 EnumUInt8 `json:"uint8b3"`
    Uint8b4 EnumUInt8 `json:"uint8b4"`
    Uint8b5 EnumUInt8 `json:"uint8b5"`
    Int16b0 EnumInt16 `json:"int16b0"`
    Int16b1 EnumInt16 `json:"int16b1"`
    Int16b2 EnumInt16 `json:"int16b2"`
    Int16b3 EnumInt16 `json:"int16b3"`
    Int16b4 EnumInt16 `json:"int16b4"`
    Int16b5 EnumInt16 `json:"int16b5"`
    Uint16b0 EnumUInt16 `json:"uint16b0"`
    Uint16b1 EnumUInt16 `json:"uint16b1"`
    Uint16b2 EnumUInt16 `json:"uint16b2"`
    Uint16b3 EnumUInt16 `json:"uint16b3"`
    Uint16b4 EnumUInt16 `json:"uint16b4"`
    Uint16b5 EnumUInt16 `json:"uint16b5"`
    Int32b0 EnumInt32 `json:"int32b0"`
    Int32b1 EnumInt32 `json:"int32b1"`
    Int32b2 EnumInt32 `json:"int32b2"`
    Int32b3 EnumInt32 `json:"int32b3"`
    Int32b4 EnumInt32 `json:"int32b4"`
    Int32b5 EnumInt32 `json:"int32b5"`
    Uint32b0 EnumUInt32 `json:"uint32b0"`
    Uint32b1 EnumUInt32 `json:"uint32b1"`
    Uint32b2 EnumUInt32 `json:"uint32b2"`
    Uint32b3 EnumUInt32 `json:"uint32b3"`
    Uint32b4 EnumUInt32 `json:"uint32b4"`
    Uint32b5 EnumUInt32 `json:"uint32b5"`
    Int64b0 EnumInt64 `json:"int64b0"`
    Int64b1 EnumInt64 `json:"int64b1"`
    Int64b2 EnumInt64 `json:"int64b2"`
    Int64b3 EnumInt64 `json:"int64b3"`
    Int64b4 EnumInt64 `json:"int64b4"`
    Int64b5 EnumInt64 `json:"int64b5"`
    Uint64b0 EnumUInt64 `json:"uint64b0"`
    Uint64b1 EnumUInt64 `json:"uint64b1"`
    Uint64b2 EnumUInt64 `json:"uint64b2"`
    Uint64b3 EnumUInt64 `json:"uint64b3"`
    Uint64b4 EnumUInt64 `json:"uint64b4"`
    Uint64b5 EnumUInt64 `json:"uint64b5"`
}

// Create a new Enums struct
func NewEnums() *Enums {
    return &Enums{
        Byte0: EnumByte_ENUM_VALUE_0,
        Byte1: EnumByte_ENUM_VALUE_1,
        Byte2: EnumByte_ENUM_VALUE_2,
        Byte3: EnumByte_ENUM_VALUE_3,
        Byte4: EnumByte_ENUM_VALUE_4,
        Byte5: EnumByte_ENUM_VALUE_5,
        Char0: EnumChar_ENUM_VALUE_0,
        Char1: EnumChar_ENUM_VALUE_1,
        Char2: EnumChar_ENUM_VALUE_2,
        Char3: EnumChar_ENUM_VALUE_3,
        Char4: EnumChar_ENUM_VALUE_4,
        Char5: EnumChar_ENUM_VALUE_5,
        Wchar0: EnumWChar_ENUM_VALUE_0,
        Wchar1: EnumWChar_ENUM_VALUE_1,
        Wchar2: EnumWChar_ENUM_VALUE_2,
        Wchar3: EnumWChar_ENUM_VALUE_3,
        Wchar4: EnumWChar_ENUM_VALUE_4,
        Wchar5: EnumWChar_ENUM_VALUE_5,
        Int8b0: EnumInt8_ENUM_VALUE_0,
        Int8b1: EnumInt8_ENUM_VALUE_1,
        Int8b2: EnumInt8_ENUM_VALUE_2,
        Int8b3: EnumInt8_ENUM_VALUE_3,
        Int8b4: EnumInt8_ENUM_VALUE_4,
        Int8b5: EnumInt8_ENUM_VALUE_5,
        Uint8b0: EnumUInt8_ENUM_VALUE_0,
        Uint8b1: EnumUInt8_ENUM_VALUE_1,
        Uint8b2: EnumUInt8_ENUM_VALUE_2,
        Uint8b3: EnumUInt8_ENUM_VALUE_3,
        Uint8b4: EnumUInt8_ENUM_VALUE_4,
        Uint8b5: EnumUInt8_ENUM_VALUE_5,
        Int16b0: EnumInt16_ENUM_VALUE_0,
        Int16b1: EnumInt16_ENUM_VALUE_1,
        Int16b2: EnumInt16_ENUM_VALUE_2,
        Int16b3: EnumInt16_ENUM_VALUE_3,
        Int16b4: EnumInt16_ENUM_VALUE_4,
        Int16b5: EnumInt16_ENUM_VALUE_5,
        Uint16b0: EnumUInt16_ENUM_VALUE_0,
        Uint16b1: EnumUInt16_ENUM_VALUE_1,
        Uint16b2: EnumUInt16_ENUM_VALUE_2,
        Uint16b3: EnumUInt16_ENUM_VALUE_3,
        Uint16b4: EnumUInt16_ENUM_VALUE_4,
        Uint16b5: EnumUInt16_ENUM_VALUE_5,
        Int32b0: EnumInt32_ENUM_VALUE_0,
        Int32b1: EnumInt32_ENUM_VALUE_1,
        Int32b2: EnumInt32_ENUM_VALUE_2,
        Int32b3: EnumInt32_ENUM_VALUE_3,
        Int32b4: EnumInt32_ENUM_VALUE_4,
        Int32b5: EnumInt32_ENUM_VALUE_5,
        Uint32b0: EnumUInt32_ENUM_VALUE_0,
        Uint32b1: EnumUInt32_ENUM_VALUE_1,
        Uint32b2: EnumUInt32_ENUM_VALUE_2,
        Uint32b3: EnumUInt32_ENUM_VALUE_3,
        Uint32b4: EnumUInt32_ENUM_VALUE_4,
        Uint32b5: EnumUInt32_ENUM_VALUE_5,
        Int64b0: EnumInt64_ENUM_VALUE_0,
        Int64b1: EnumInt64_ENUM_VALUE_1,
        Int64b2: EnumInt64_ENUM_VALUE_2,
        Int64b3: EnumInt64_ENUM_VALUE_3,
        Int64b4: EnumInt64_ENUM_VALUE_4,
        Int64b5: EnumInt64_ENUM_VALUE_5,
        Uint64b0: EnumUInt64_ENUM_VALUE_0,
        Uint64b1: EnumUInt64_ENUM_VALUE_1,
        Uint64b2: EnumUInt64_ENUM_VALUE_2,
        Uint64b3: EnumUInt64_ENUM_VALUE_3,
        Uint64b4: EnumUInt64_ENUM_VALUE_4,
        Uint64b5: EnumUInt64_ENUM_VALUE_5,
    }
}

// Create a new Enums struct from the given field values
func NewEnumsFromFieldValues(Byte0 EnumByte, Byte1 EnumByte, Byte2 EnumByte, Byte3 EnumByte, Byte4 EnumByte, Byte5 EnumByte, Char0 EnumChar, Char1 EnumChar, Char2 EnumChar, Char3 EnumChar, Char4 EnumChar, Char5 EnumChar, Wchar0 EnumWChar, Wchar1 EnumWChar, Wchar2 EnumWChar, Wchar3 EnumWChar, Wchar4 EnumWChar, Wchar5 EnumWChar, Int8b0 EnumInt8, Int8b1 EnumInt8, Int8b2 EnumInt8, Int8b3 EnumInt8, Int8b4 EnumInt8, Int8b5 EnumInt8, Uint8b0 EnumUInt8, Uint8b1 EnumUInt8, Uint8b2 EnumUInt8, Uint8b3 EnumUInt8, Uint8b4 EnumUInt8, Uint8b5 EnumUInt8, Int16b0 EnumInt16, Int16b1 EnumInt16, Int16b2 EnumInt16, Int16b3 EnumInt16, Int16b4 EnumInt16, Int16b5 EnumInt16, Uint16b0 EnumUInt16, Uint16b1 EnumUInt16, Uint16b2 EnumUInt16, Uint16b3 EnumUInt16, Uint16b4 EnumUInt16, Uint16b5 EnumUInt16, Int32b0 EnumInt32, Int32b1 EnumInt32, Int32b2 EnumInt32, Int32b3 EnumInt32, Int32b4 EnumInt32, Int32b5 EnumInt32, Uint32b0 EnumUInt32, Uint32b1 EnumUInt32, Uint32b2 EnumUInt32, Uint32b3 EnumUInt32, Uint32b4 EnumUInt32, Uint32b5 EnumUInt32, Int64b0 EnumInt64, Int64b1 EnumInt64, Int64b2 EnumInt64, Int64b3 EnumInt64, Int64b4 EnumInt64, Int64b5 EnumInt64, Uint64b0 EnumUInt64, Uint64b1 EnumUInt64, Uint64b2 EnumUInt64, Uint64b3 EnumUInt64, Uint64b4 EnumUInt64, Uint64b5 EnumUInt64) *Enums {
    return &Enums{Byte0, Byte1, Byte2, Byte3, Byte4, Byte5, Char0, Char1, Char2, Char3, Char4, Char5, Wchar0, Wchar1, Wchar2, Wchar3, Wchar4, Wchar5, Int8b0, Int8b1, Int8b2, Int8b3, Int8b4, Int8b5, Uint8b0, Uint8b1, Uint8b2, Uint8b3, Uint8b4, Uint8b5, Int16b0, Int16b1, Int16b2, Int16b3, Int16b4, Int16b5, Uint16b0, Uint16b1, Uint16b2, Uint16b3, Uint16b4, Uint16b5, Int32b0, Int32b1, Int32b2, Int32b3, Int32b4, Int32b5, Uint32b0, Uint32b1, Uint32b2, Uint32b3, Uint32b4, Uint32b5, Int64b0, Int64b1, Int64b2, Int64b3, Int64b4, Int64b5, Uint64b0, Uint64b1, Uint64b2, Uint64b3, Uint64b4, Uint64b5}
}

// Create a new Enums struct from JSON
func NewEnumsFromJSON(buffer []byte) (*Enums, error) {
    result := *NewEnums()
    err := fbe.Json.Unmarshal(buffer, &result)
    if err != nil {
        return nil, err
    }
    return &result, nil
}

// Struct shallow copy
func (s *Enums) Copy() *Enums {
    var result = *s
    return &result
}

// Struct deep clone
func (s *Enums) Clone() *Enums {
    // Serialize the struct to the FBE stream
    writer := NewEnumsModel(fbe.NewEmptyBuffer())
    _, _ = writer.Serialize(s)

    // Deserialize the struct from the FBE stream
    reader := NewEnumsModel(writer.Buffer())
    result, _, _ := reader.Deserialize()
    return result
}

// Get the struct key
func (s *Enums) Key() EnumsKey {
    return EnumsKey{
    }
}

// Convert struct to optional
func (s *Enums) Optional() *Enums {
    return s
}

// Get the FBE type
func (s *Enums) FBEType() int { return 1 }

// Convert struct to string
func (s *Enums) String() string {
    var sb strings.Builder
    sb.WriteString("Enums(")
    sb.WriteString("byte0=")
    sb.WriteString(s.Byte0.String())
    sb.WriteString(",byte1=")
    sb.WriteString(s.Byte1.String())
    sb.WriteString(",byte2=")
    sb.WriteString(s.Byte2.String())
    sb.WriteString(",byte3=")
    sb.WriteString(s.Byte3.String())
    sb.WriteString(",byte4=")
    sb.WriteString(s.Byte4.String())
    sb.WriteString(",byte5=")
    sb.WriteString(s.Byte5.String())
    sb.WriteString(",char0=")
    sb.WriteString(s.Char0.String())
    sb.WriteString(",char1=")
    sb.WriteString(s.Char1.String())
    sb.WriteString(",char2=")
    sb.WriteString(s.Char2.String())
    sb.WriteString(",char3=")
    sb.WriteString(s.Char3.String())
    sb.WriteString(",char4=")
    sb.WriteString(s.Char4.String())
    sb.WriteString(",char5=")
    sb.WriteString(s.Char5.String())
    sb.WriteString(",wchar0=")
    sb.WriteString(s.Wchar0.String())
    sb.WriteString(",wchar1=")
    sb.WriteString(s.Wchar1.String())
    sb.WriteString(",wchar2=")
    sb.WriteString(s.Wchar2.String())
    sb.WriteString(",wchar3=")
    sb.WriteString(s.Wchar3.String())
    sb.WriteString(",wchar4=")
    sb.WriteString(s.Wchar4.String())
    sb.WriteString(",wchar5=")
    sb.WriteString(s.Wchar5.String())
    sb.WriteString(",int8b0=")
    sb.WriteString(s.Int8b0.String())
    sb.WriteString(",int8b1=")
    sb.WriteString(s.Int8b1.String())
    sb.WriteString(",int8b2=")
    sb.WriteString(s.Int8b2.String())
    sb.WriteString(",int8b3=")
    sb.WriteString(s.Int8b3.String())
    sb.WriteString(",int8b4=")
    sb.WriteString(s.Int8b4.String())
    sb.WriteString(",int8b5=")
    sb.WriteString(s.Int8b5.String())
    sb.WriteString(",uint8b0=")
    sb.WriteString(s.Uint8b0.String())
    sb.WriteString(",uint8b1=")
    sb.WriteString(s.Uint8b1.String())
    sb.WriteString(",uint8b2=")
    sb.WriteString(s.Uint8b2.String())
    sb.WriteString(",uint8b3=")
    sb.WriteString(s.Uint8b3.String())
    sb.WriteString(",uint8b4=")
    sb.WriteString(s.Uint8b4.String())
    sb.WriteString(",uint8b5=")
    sb.WriteString(s.Uint8b5.String())
    sb.WriteString(",int16b0=")
    sb.WriteString(s.Int16b0.String())
    sb.WriteString(",int16b1=")
    sb.WriteString(s.Int16b1.String())
    sb.WriteString(",int16b2=")
    sb.WriteString(s.Int16b2.String())
    sb.WriteString(",int16b3=")
    sb.WriteString(s.Int16b3.String())
    sb.WriteString(",int16b4=")
    sb.WriteString(s.Int16b4.String())
    sb.WriteString(",int16b5=")
    sb.WriteString(s.Int16b5.String())
    sb.WriteString(",uint16b0=")
    sb.WriteString(s.Uint16b0.String())
    sb.WriteString(",uint16b1=")
    sb.WriteString(s.Uint16b1.String())
    sb.WriteString(",uint16b2=")
    sb.WriteString(s.Uint16b2.String())
    sb.WriteString(",uint16b3=")
    sb.WriteString(s.Uint16b3.String())
    sb.WriteString(",uint16b4=")
    sb.WriteString(s.Uint16b4.String())
    sb.WriteString(",uint16b5=")
    sb.WriteString(s.Uint16b5.String())
    sb.WriteString(",int32b0=")
    sb.WriteString(s.Int32b0.String())
    sb.WriteString(",int32b1=")
    sb.WriteString(s.Int32b1.String())
    sb.WriteString(",int32b2=")
    sb.WriteString(s.Int32b2.String())
    sb.WriteString(",int32b3=")
    sb.WriteString(s.Int32b3.String())
    sb.WriteString(",int32b4=")
    sb.WriteString(s.Int32b4.String())
    sb.WriteString(",int32b5=")
    sb.WriteString(s.Int32b5.String())
    sb.WriteString(",uint32b0=")
    sb.WriteString(s.Uint32b0.String())
    sb.WriteString(",uint32b1=")
    sb.WriteString(s.Uint32b1.String())
    sb.WriteString(",uint32b2=")
    sb.WriteString(s.Uint32b2.String())
    sb.WriteString(",uint32b3=")
    sb.WriteString(s.Uint32b3.String())
    sb.WriteString(",uint32b4=")
    sb.WriteString(s.Uint32b4.String())
    sb.WriteString(",uint32b5=")
    sb.WriteString(s.Uint32b5.String())
    sb.WriteString(",int64b0=")
    sb.WriteString(s.Int64b0.String())
    sb.WriteString(",int64b1=")
    sb.WriteString(s.Int64b1.String())
    sb.WriteString(",int64b2=")
    sb.WriteString(s.Int64b2.String())
    sb.WriteString(",int64b3=")
    sb.WriteString(s.Int64b3.String())
    sb.WriteString(",int64b4=")
    sb.WriteString(s.Int64b4.String())
    sb.WriteString(",int64b5=")
    sb.WriteString(s.Int64b5.String())
    sb.WriteString(",uint64b0=")
    sb.WriteString(s.Uint64b0.String())
    sb.WriteString(",uint64b1=")
    sb.WriteString(s.Uint64b1.String())
    sb.WriteString(",uint64b2=")
    sb.WriteString(s.Uint64b2.String())
    sb.WriteString(",uint64b3=")
    sb.WriteString(s.Uint64b3.String())
    sb.WriteString(",uint64b4=")
    sb.WriteString(s.Uint64b4.String())
    sb.WriteString(",uint64b5=")
    sb.WriteString(s.Uint64b5.String())
    sb.WriteString(")")
    return sb.String()
}

// Convert struct to JSON
func (s *Enums) JSON() ([]byte, error) {
    return fbe.Json.Marshal(s)
}