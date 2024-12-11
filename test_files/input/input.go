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
	_, err = fmt.Fscanf(file, "%s %c %d", &address, &t, &value)
	if err != nil {
		fmt.Println("Error reading from file:", err)
		return
	}

	// Print the results
	fmt.Printf("Address: %s\nType: %c\nValue: %d\n", address, t, value)
}
