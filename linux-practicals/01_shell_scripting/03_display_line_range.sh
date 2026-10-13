#!/bin/bash
# ==============================================================================
# Script: 03_display_line_range.sh
# Description: Accepts a file name, starting line number, and ending line number
#              as arguments, and displays all lines between the given line numbers.
# Usage: ./03_display_line_range.sh <filename> <start_line> <end_line>
# ==============================================================================

if [ $# -ne 3 ]; then
    echo "Usage: $0 <filename> <start_line> <end_line>"
    echo "Example: $0 sample.txt 5 15"
    exit 1
fi

FILE="$1"
START_LINE="$2"
END_LINE="$3"

if [ ! -f "$FILE" ]; then
    echo "Error: File '$FILE' does not exist."
    exit 1
fi

# Validate line numbers are positive integers
if ! [[ "$START_LINE" =~ ^[1-9][0-9]*$ ]] || ! [[ "$END_LINE" =~ ^[1-9][0-9]*$ ]]; then
    echo "Error: Starting and ending lines must be positive integers."
    exit 1
fi

if [ "$START_LINE" -gt "$END_LINE" ]; then
    echo "Error: Start line ($START_LINE) cannot be greater than end line ($END_LINE)."
    exit 1
fi

TOTAL_LINES=$(wc -l < "$FILE")

echo "=========================================================="
echo " File: $FILE (Total lines: $TOTAL_LINES)"
echo " Printing lines $START_LINE to $END_LINE:"
echo "=========================================================="

sed -n "${START_LINE},${END_LINE}p" "$FILE"

echo "=========================================================="
