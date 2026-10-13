#!/bin/bash
# ==============================================================================
# Script: 02_user_login_duration.sh
# Description: Determines the period for which a specified user has been
#              working on the system using Linux utilities (who, w, last).
# Usage: ./02_user_login_duration.sh [username]
# ==============================================================================

TARGET_USER="$1"

if [ -z "$TARGET_USER" ]; then
    read -r -p "Enter username to inspect: " TARGET_USER
fi

if [ -z "$TARGET_USER" ]; then
    echo "Error: Username cannot be empty."
    exit 1
fi

echo "=========================================================="
echo " Checking Working Duration for User: '$TARGET_USER'"
echo "=========================================================="

# Check if user is currently logged in via 'who'
ACTIVE_INFO=$(who | grep -w "^$TARGET_USER")

if [ -n "$ACTIVE_INFO" ]; then
    echo "[STATUS] User '$TARGET_USER' is currently LOGGED IN."
    echo "Session details:"
    echo "$ACTIVE_INFO"
    echo ""
    echo "Active process & idle/login statistics (w):"
    w "$TARGET_USER" 2>/dev/null
else
    echo "[STATUS] User '$TARGET_USER' is currently NOT logged in."
fi

# Show historical session durations using 'last'
echo ""
echo "----------------------------------------------------------"
echo "Recent Login History and Session Durations (last):"
echo "----------------------------------------------------------"
last -n 5 "$TARGET_USER"
echo "=========================================================="
