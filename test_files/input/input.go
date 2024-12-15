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

	var new_address string
	var new_t rune
	var new_value int
	// Read from file using Fscanf
	for {
		_, err = fmt.Fscanf(file, "%s %c %d", &address, &t, &value)
		if err != nil {
			if err.Error() == "EOF" {
				break
			}
			fmt.Println("Error reading from file:", err)
			return
		}

		if t == 'R' {
			for {
				_, err = fmt.Fscanf(file, "%s %c %d", &new_address, &new_t, &new_value)
				if err != nil {
					if err.Error() == "EOF" {
						break
					}
					fmt.Println("Error reading from file:", err)
					return
				}
				if (value == new_value && address == new_address) {
					fmt.Println("WORKS")
				}
			}
		}


	}

}

