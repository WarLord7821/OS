#!/bin/bash

# chmod +x cal.sh
#./cal.sh

# This script provides a menu-driven calculator.

# 1. Start an infinite 'while true' loop to keep showing the menu
while true
do
  # 2. Print the menu options
  echo ""
  echo "--- Menu-Driven Calculator ---"
  echo "1. Add"
  echo "2. Subtract"
  echo "3. Multiply"
  echo "4. Divide"
  echo "5. Exit"
  echo ""
  
  # 3. Read the user's choice
  read -p "Enter your choice (1-5): " choice

  # 4. Use a 'case' statement to decide what to do
  case $choice in
    1)
      # Addition
      read -p "Enter first number: " num1
      read -p "Enter second number: " num2
      result=$((num1 + num2))
      echo "Result: $num1 + $num2 = $result"
      ;;
    2)
      # Subtraction
      read -p "Enter first number: " num1
      read -p "Enter second number: " num2
      result=$((num1 - num2))
      echo "Result: $num1 - $num2 = $result"
      ;;
    3)
      # Multiplication
      read -p "Enter first number: " num1
      read -p "Enter second number: " num2
      result=$((num1 * num2))
      echo "Result: $num1 * $num2 = $result"
      ;;
    4.
      # Division
      read -p "Enter first number: " num1
      read -p "Enter second number: " num2
      
      # Check for division by zero
      if [ $num2 -eq 0 ]; then
        echo "Error: Cannot divide by zero."
      else
        result=$((num1 / num2))
        echo "Result: $num1 / $num2 = $result"
      fi
      ;;
    5)
      # Exit
      echo "Exiting calculator. Goodbye!"
      break # This 'break' command exits the 'while' loop
      ;;
    *)
      # Invalid option (anything not 1-5)
      echo "Invalid choice. Please enter a number between 1 and 5."
      ;;
  esac
  
  # The loop repeats from here
done