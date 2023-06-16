package main

import (
	"fbeproj/examples"
)

func main() {
	examples.Create()
	examples.Print()
	examples.PrintJSON()
	examples.SendReceive()
	examples.Serialization()
	examples.SerializationJSON()
	examples.SerializationFinal()
}
