#!/bin/bash

# This script prints the multiplication table for a number given by the user.

# 1. Prompt the user for a number
read -p "Enter a number to print its table: " num

echo "--- Multiplication Table of $num ---"

# 2. Use a 'for' loop to iterate from 1 to 10
# 'i' will take the value 1, 2, 3... up to 10
for i in {1..10}
do
  # 3. Calculate the result using arithmetic expansion $((...))
  result=$((num * i))
  
  # 4. Print the formatted line
  echo "$num x $i = $result"
done