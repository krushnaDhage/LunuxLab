#!/bin/bash
# ==============================================================================
# Script: 01_uppercase_files.sh
# Description: Converts file names to uppercase.
#              Accepts one or more files OR a directory as argument.
#              Provides argument validation, space handling, and collision protection.
# Usage:
#   ./01_uppercase_files.sh <file1> [file2 ...]
#   ./01_uppercase_files.sh [directory_path]
# ==============================================================================

rename_to_upper() {
    local target_path="$1"
    local dir_part
    dir_part=$(dirname "$target_path")
    local base_part
    base_part=$(basename "$target_path")

    # Convert base filename to uppercase
    local upper_base
    upper_base=$(echo "$base_part" | tr '[:lower:]' '[:upper:]')

    # If already uppercase, skip
    if [ "$base_part" = "$upper_base" ]; then
        echo "[INFO] '$base_part' is already uppercase. Skipping."
        return 0
    fi

    local new_path="$dir_part/$upper_base"

    # Collision check: verify if uppercase file already exists
    if [ -e "$new_path" ]; then
        echo "[COLLISION] '$new_path' already exists!"
        read -r -p "Overwrite '$new_path'? (y/N): " confirm
        case "$confirm" in
            [yY][eE][sS]|[yY])
                mv -f -- "$target_path" "$new_path"
                echo "[SUCCESS] Overwritten: '$base_part' -> '$upper_base'"
                ;;
            *)
                echo "[SKIPPED] Kept '$base_part' to avoid data loss."
                ;;
        esac
    else
        mv -- "$target_path" "$new_path"
        echo "[SUCCESS] Renamed: '$base_part' -> '$upper_base'"
    fi
}

# Argument validation
if [ $# -eq 0 ]; then
    echo "Usage: $0 <filename1> [filename2 ...]"
    echo "       $0 <directory_path>"
    exit 1
fi

echo "=== Converting Filenames to Uppercase ==="

# Check if single argument is a directory
if [ $# -eq 1 ] && [ -d "$1" ]; then
    TARGET_DIR="$1"
    echo "Processing all files in directory: '$TARGET_DIR'"
    for item in "$TARGET_DIR"/*; do
        if [ -f "$item" ]; then
            rename_to_upper "$item"
        fi
    done
else
    # Process files passed as arguments
    for file in "$@"; do
        if [ ! -e "$file" ]; then
            echo "[SKIP] '$file' does not exist."
            continue
        fi
        if [ -d "$file" ]; then
            echo "[INFO] '$file' is a directory. Renaming files within it:"
            for inner in "$file"/*; do
                [ -f "$inner" ] && rename_to_upper "$inner"
            done
        else
            rename_to_upper "$file"
        fi
    done
fi

echo "=== Finished ==="
