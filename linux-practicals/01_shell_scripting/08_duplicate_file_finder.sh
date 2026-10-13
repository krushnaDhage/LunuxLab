#!/bin/bash
# ==============================================================================
# Script: 08_duplicate_file_finder.sh
# Description: Recursively scans a directory for duplicate files using md5sum checksums.
#              Lists duplicate files and asks for confirmation before deleting.
# Usage: ./08_duplicate_file_finder.sh [directory]
# ==============================================================================

DIR="${1:-.}"

if [ ! -d "$DIR" ]; then
    echo "Error: Directory '$DIR' does not exist."
    exit 1
fi

echo "=========================================================="
echo "          RECURSIVE DUPLICATE FILE FINDER                 "
echo "=========================================================="
echo "Scanning: $(realpath "$DIR")"

temp_checksums=$(mktemp)
trap 'rm -f "$temp_checksums"' EXIT

# Calculate md5sum for all regular files
find "$DIR" -type f -exec md5sum {} + 2>/dev/null > "$temp_checksums"

# Find hashes that appear more than once
dup_hashes=$(awk '{print $1}' "$temp_checksums" | sort | uniq -d)

if [ -z "$dup_hashes" ]; then
    echo "No duplicate files found."
    exit 0
fi

echo ""
echo "Duplicate files found:"
for hash in $dup_hashes; do
    echo "----------------------------------------------------------"
    echo "Checksum: $hash"
    
    files=$(grep "^$hash" "$temp_checksums" | awk '{$1=""; print $0}' | sed 's/^ //')
    
    # Show original and duplicates
    first=1
    duplicates_to_delete=()
    while IFS= read -r f; do
        if [ $first -eq 1 ]; then
            echo " [ORIGINAL]   $f"
            first=0
        else
            echo " [DUPLICATE]  $f"
            duplicates_to_delete+=("$f")
        fi
    done <<< "$files"

    # Confirm deletion
    read -r -p "Delete these duplicate copies? (y/N): " confirm
    case "$confirm" in
        [yY][eE][sS]|[yY])
            for d in "${duplicates_to_delete[@]}"; do
                rm -f -- "$d"
                echo "   -> Deleted: $d"
            done
            ;;
        *)
            echo "   -> Kept."
            ;;
    esac
done

echo "=========================================================="
