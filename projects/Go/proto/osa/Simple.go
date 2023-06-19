//------------------------------------------------------------------------------
// Automatically generated by the Fast Binary Encoding compiler, do not modify!
// https://github.com/chronoxor/FastBinaryEncoding
// Source: osa.fbe
// FBE version: 1.11.0.0
//------------------------------------------------------------------------------

package osa

import "fmt"
import "strconv"
import "strings"
import "errors"
import "fbeproj/proto/fbe"

// Workaround for Go unused imports issue
var _ = errors.New
var _ = fbe.Version

// Workaround for Go unused imports issue
var _ = fmt.Print
var _ = strconv.FormatInt

// Simple key
type SimpleKey struct {
}

// Convert Simple flags key to string
func (k *SimpleKey) String() string {
    var sb strings.Builder
    sb.WriteString("SimpleKey(")
    sb.WriteString(")")
    return sb.String()
}

// Simple struct
type Simple struct {
    Name string `json:"name"`
    Depth int32 `json:"depth"`
    Sa [1]Extra `json:"sa"`
    Sex Sex `json:"sex"`
}

// Create a new Simple struct
func NewSimple() *Simple {
    return &Simple{
        Name: "",
        Depth: 0,
        Sa: [1]Extra{},
        Sex: *NewSex(),
    }
}

// Create a new Simple struct from the given field values
func NewSimpleFromFieldValues(Name string, Depth int32, Sa [1]Extra, Sex Sex) *Simple {
    return &Simple{Name, Depth, Sa, Sex}
}

// Create a new Simple struct from JSON
func NewSimpleFromJSON(buffer []byte) (*Simple, error) {
    result := *NewSimple()
    err := fbe.Json.Unmarshal(buffer, &result)
    if err != nil {
        return nil, err
    }
    return &result, nil
}

// Struct shallow copy
func (s *Simple) Copy() *Simple {
    var result = *s
    return &result
}

// Struct deep clone
func (s *Simple) Clone() *Simple {
    // Serialize the struct to the FBE stream
    writer := NewSimpleModel(fbe.NewEmptyBuffer())
    _, _ = writer.Serialize(s)

    // Deserialize the struct from the FBE stream
    reader := NewSimpleModel(writer.Buffer())
    result, _, _ := reader.Deserialize()
    return result
}

// Get the struct key
func (s *Simple) Key() SimpleKey {
    return SimpleKey{
    }
}

// Convert struct to optional
func (s *Simple) Optional() *Simple {
    return s
}

// Get the FBE type
func (s *Simple) FBEType() int { return 2 }

// Convert struct to string
func (s *Simple) String() string {
    var sb strings.Builder
    sb.WriteString("Simple(")
    sb.WriteString("name=")
    sb.WriteString("\"" + s.Name + "\"")
    sb.WriteString(",depth=")
    sb.WriteString(strconv.FormatInt(int64(s.Depth), 10))
    sb.WriteString(",sa=")
    if true {
        first := true
        sb.WriteString("[" + strconv.FormatInt(int64(len(s.Sa)), 10) + "][")
        for _, v := range s.Sa {
            if first { sb.WriteString("") } else { sb.WriteString(",") }
            sb.WriteString(v.String())
            first = false
        }
        sb.WriteString("]")
    } else {
        sb.WriteString(",sa=[0][]")
    }
    sb.WriteString(",sex=")
    sb.WriteString(s.Sex.String())
    sb.WriteString(")")
    return sb.String()
}

// Convert struct to JSON
func (s *Simple) JSON() ([]byte, error) {
    return fbe.Json.Marshal(s)
}
