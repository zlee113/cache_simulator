package main

import (
	"fmt"
	"os"
)


func main () {
	// Open a file for reading
	file, err := os.Open("multiplication_input.txt")
	if err != nil {
		fmt.Println("Error opening file:", err)
		return
	}
	defer file.Close()

	// Variables to store the read data
	var address string
	var t rune
	var value int

	// Read from file using Fscanf
	counts := make(map[string]int)
	for {
		_, err = fmt.Fscanf(file, "%s %c %d", &address, &t, &value)
		if err != nil {
			if err.Error() == "EOF" {
				break
			}
			fmt.Println("Error reading from file:", err)
			return
		}
		counts[address]++
	}

	// Print the final map
	fmt.Println("Final line counts:")
	for key, value := range counts {
		fmt.Printf("%s: %d\n", key, value)
	}
}
