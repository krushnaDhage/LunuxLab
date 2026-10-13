#!/bin/bash
# ==============================================================================
# Script: 04_delete_matching_lines.sh
# Description: Removes lines containing a specified word from a file.
#              Preserves the original file unless the user explicitly confirms
#              in-place modification, or saves filtered contents to an output file.
# Usage: ./04_delete_matching_lines.sh <word> <file1> [file2 ...]
# ==============================================================================

if [ $# -lt 2 ]; then
    echo "Usage: $0 <word> <file1> [file2 ...]"
    echo "Example: $0 DEBUG server.log"
    exit 1
fi

WORD="$1"
shift

echo "=========================================================="
echo " Deleting Lines Containing Word: '$WORD'"
echo "=========================================================="

for file in "$@"; do
    if [ ! -f "$file" ]; then
        echo "[SKIP] '$file' is not a regular file or does not exist."
        continue
    fi

    # Count matching lines
    match_count=$(grep -c -F "$WORD" "$file" 2>/dev/null || echo 0)
    if [ "$match_count" -eq 0 ]; then
        echo "[INFO] No lines containing '$WORD' found in '$file'. File unchanged."
        continue
    fi

    echo ""
    echo "Found $match_count line(s) containing '$WORD' in '$file'."
    echo "Choose action for '$file':"
    echo "  1) Save filtered content to new output file (Original preserved)"
    echo "  2) In-place modification (Overwrites original after backup)"
    echo "  3) Cancel and skip this file"
    read -r -p "Select option (1/2/3) [Default 1]: " opt
    opt=${opt:-1}

    case "$opt" in
        1)
            out_file="${file%.*}_cleaned.${file##*.}"
            [ "$out_file" = "$file" ] && out_file="${file}.cleaned"
            grep -v -F "$WORD" "$file" > "$out_file"
            echo "[SUCCESS] Filtered lines saved to '$out_file'."
            echo "          Original file '$file' preserved unchanged."
            ;;
        2)
            read -r -p "Are you sure you want to modify '$file' in-place? (y/N): " confirm_inplace
            case "$confirm_inplace" in
                [yY][eE][sS]|[yY])
                    cp -- "$file" "${file}.bak"
                    echo "[BACKUP] Backup copy created at '${file}.bak'."
                    tmp_file=$(mktemp)
                    grep -v -F "$WORD" "$file" > "$tmp_file"
                    mv -f "$tmp_file" "$file"
                    echo "[SUCCESS] '$file' updated in-place ($match_count line(s) deleted)."
                    ;;
                *)
                    echo "[CANCELLED] In-place modification aborted. '$file' unchanged."
                    ;;
            esac
            ;;
        *)
            echo "[SKIPPED] Skipped '$file'."
            ;;
    esac
done

echo ""
echo "=========================================================="
echo "Operation completed."
