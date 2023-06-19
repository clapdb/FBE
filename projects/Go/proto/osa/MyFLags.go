//------------------------------------------------------------------------------
// Automatically generated by the Fast Binary Encoding compiler, do not modify!
// https://github.com/chronoxor/FastBinaryEncoding
// Source: osa.fbe
// FBE version: 1.11.0.0
//------------------------------------------------------------------------------

package osa

import "errors"
import "fbeproj/proto/fbe"

// Workaround for Go unused imports issue
var _ = errors.New
var _ = fbe.Version

// MyFLags enum key
type MyFLagsKey int32

// Convert MyFLags enum key to string
func (k MyFLagsKey) String() string {
    return MyFLags(k).String()
}

// MyFLags enum
type MyFLags int32

// MyFLags enum values
//noinspection GoSnakeCaseUsage
const (
    MyFLags_flag0 = MyFLags(0x00 + 0)
    MyFLags_flag1 = MyFLags(0x01 + 0)
    MyFLags_flag2 = MyFLags(0x02 + 0)
)

// Create a new MyFLags enum
func NewMyFLags() *MyFLags {
    return new(MyFLags)
}

// Create a new MyFLags enum from the given value
func NewMyFLagsFromValue(value int32) *MyFLags {
    result := MyFLags(value)
    return &result
}

// Get the enum key
func (e MyFLags) Key() MyFLagsKey {
    return MyFLagsKey(e)
}

// Convert enum to optional
func (e *MyFLags) Optional() *MyFLags {
    return e
}

// Convert enum to string
func (e MyFLags) String() string {
    if e == MyFLags_flag0 {
        return "flag0"
    }
    if e == MyFLags_flag1 {
        return "flag1"
    }
    if e == MyFLags_flag2 {
        return "flag2"
    }
    return "<unknown>"
}

// Convert enum to JSON
func (e MyFLags) MarshalJSON() ([]byte, error) {
    value := int32(e)
    return fbe.Json.Marshal(&value)
}

// Convert JSON to enum
func (e *MyFLags) UnmarshalJSON(buffer []byte) error {
    var result int32
    err := fbe.Json.Unmarshal(buffer, &result)
    if err != nil {
        return err
    }
    *e = MyFLags(result)
    return nil
}
