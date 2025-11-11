#!/bin/bash

# This script reverses a string given by the user.

# 1. Prompt the user for a string
read -p "Enter a string to reverse: " input_string

# 2. Use the 'rev' command, which is a standard Linux utility for reversing.
# We 'echo' the string and 'pipe' it (|) as input to the 'rev' command.
reversed_string=$(echo "$input_string" | rev)

# 3. Print the result
echo "Original string: $input_string"
echo "Reversed string: $reversed_string"