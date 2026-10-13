#!/bin/bash
# ==============================================================================
# Script: 05_string_operations.sh
# Description: Demonstrates fundamental string operations in Bash:
#              (a) Extract a substring from a given string.
#              (b) Find the length of a given string.
# Usage: ./05_string_operations.sh [string] [start_index] [length]
# ==============================================================================

if [ $# -ge 3 ]; then
    STR="$1"
    START="$2"
    LEN="$3"
else
    read -r -p "Enter string: " STR
    read -r -p "Enter start index (0-based): " START
    read -r -p "Enter length of substring: " LEN
fi

echo "=========================================================="
echo " Input String : \"$STR\""
echo "----------------------------------------------------------"

# (a) String length
str_length=${#STR}
echo " (a) String Length: $str_length"

# (b) Substring extraction
substr="${STR:$START:$LEN}"
echo " (b) Extracted Substring (from index $START, length $LEN): \"$substr\""
echo "=========================================================="
