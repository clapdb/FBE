package examples

import "fmt"
import "fbeproj/proto/test"

func ExamplePrintJSON() {
	json, _ := test.NewStructSimple().JSON()
	fmt.Println(string(json))
	fmt.Println()

	json, _ = test.NewStructOptional().JSON()
	fmt.Println(string(json))
	fmt.Println()

	json, _ = test.NewStructNested().JSON()
	fmt.Println(string(json))
	fmt.Println()
}
