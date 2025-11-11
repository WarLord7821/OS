#!/bin/bash

# This script checks if a given number is prime.

# 1. Prompt the user for a number
read -p "Enter a number to check if it's prime: " num

# 2. Handle special cases. 0 and 1 are not prime.
if [ $num -lt 2 ]; then
  echo "$num is not a prime number."
  exit 0 # Exit the script
fi

# 3. We will use a flag variable. 1 means prime, 0 means not prime.
is_prime=1 

# 4. Loop from 2 up to (num / 2).
# We only need to check for divisors up to half of the number.
for (( i=2; i<=num/2; i++ ))
do
  # 5. Check if 'num' is divisible by 'i'
  # We use the modulo operator (%)
  if [ $((num % i)) -eq 0 ]; then
    # If the remainder is 0, it has a divisor, so it's not prime.
    is_prime=0
    break # Found a divisor, no need to check further. Exit the loop.
  fi
done

# 6. After the loop, check the flag
if [ $is_prime -eq 1 ]; then
  echo "$num is a prime number."
else
  echo "$num is not a prime number."
fi