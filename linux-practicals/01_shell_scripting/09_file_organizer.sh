#!/bin/bash
# ==============================================================================
# Script: 09_file_organizer.sh
# Description: Automatically organizes files in a directory into folders:
#              - Images    (jpg, png, gif, svg)
#              - PDFs      (pdf)
#              - Videos    (mp4, mkv, avi)
#              - Documents (docx, doc, txt, xls, ppt)
#              - Archives  (zip, tar, gz, 7z)
#              - Others    (remaining types)
# Usage: ./09_file_organizer.sh [directory]
# ==============================================================================

DIR="${1:-.}"

if [ ! -d "$DIR" ]; then
    echo "Error: Directory '$DIR' does not exist."
    exit 1
fi

echo "Organizing files in directory: $(realpath "$DIR")"

# Create category directories
mkdir -p "$DIR/Images" "$DIR/PDFs" "$DIR/Videos" "$DIR/Documents" "$DIR/Archives" "$DIR/Others"

SCRIPT_NAME=$(basename "$0")

for file in "$DIR"/*; do
    [ -f "$file" ] || continue
    fname=$(basename "$file")

    # Do not move the script itself or sample logs
    if [ "$fname" = "$SCRIPT_NAME" ]; then
        continue
    fi

    ext="${fname##*.}"
    ext_lower=$(echo "$ext" | tr '[:upper:]' '[:lower:]')

    if [ "$fname" = "$ext" ]; then
        dest="Others"
    else
        case "$ext_lower" in
            jpg|jpeg|png|gif|svg|webp)
                dest="Images"
                ;;
            pdf)
                dest="PDFs"
                ;;
            mp4|mkv|avi|mov)
                dest="Videos"
                ;;
            doc|docx|txt|xls|xlsx|ppt|pptx)
                dest="Documents"
                ;;
            zip|tar|gz|7z|bz2)
                dest="Archives"
                ;;
            *)
                dest="Others"
                ;;
        esac
    fi

    if [ -e "$DIR/$dest/$fname" ]; then
        echo "[COLLISION] '$fname' already exists in '$dest/'. Keeping original."
    else
        mv -- "$file" "$DIR/$dest/"
        echo "Moved '$fname' -> $dest/"
    fi
done

echo "Organization completed."
