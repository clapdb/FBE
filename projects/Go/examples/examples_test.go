package examples

import "testing"

func TestExamples(t *testing.T) {
	ExampleCreate()
	ExamplePrint()
	ExamplePrintJSON()
	ExampleSendReceive()
	ExampleSerialization()
	ExampleSerializationJSON()
	ExampleSerializationFinal()
}
