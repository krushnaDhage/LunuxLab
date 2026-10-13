#!/bin/bash
# ==============================================================================
# Script: 10_password_validator.sh
# Description: Validates a password against standard complexity rules:
#              1. Minimum 10 characters
#              2. At least one uppercase letter
#              3. At least one lowercase letter
#              4. At least one digit (0-9)
#              5. At least one special character
#              6. No character repeated consecutively or in total > 3 times
#              Calculates a score: Weak, Medium, or Strong.
# ==============================================================================

if [ $# -ge 1 ]; then
    PASSWORD="$1"
else
    read -r -s -p "Enter password to validate: " PASSWORD
    echo ""
fi

if [ -z "$PASSWORD" ]; then
    echo "Error: Password cannot be empty."
    exit 1
fi

SCORE=0
LEN=${#PASSWORD}

echo "=========================================================="
echo "              PASSWORD STRENGTH EVALUATION                "
echo "=========================================================="

# 1. Length check (minimum 10 characters)
if [ "$LEN" -ge 10 ]; then
    echo "[PASS] Length is $LEN characters (>= 10)."
    SCORE=$((SCORE + 1))
else
    echo "[FAIL] Length is $LEN characters (Minimum 10 required)."
fi

# 2. Uppercase letter check
if echo "$PASSWORD" | grep -q '[A-Z]'; then
    echo "[PASS] Contains uppercase letter(s)."
    SCORE=$((SCORE + 1))
else
    echo "[FAIL] Missing uppercase letter."
fi

# 3. Lowercase letter check
if echo "$PASSWORD" | grep -q '[a-z]'; then
    echo "[PASS] Contains lowercase letter(s)."
    SCORE=$((SCORE + 1))
else
    echo "[FAIL] Missing lowercase letter."
fi

# 4. Number check
if echo "$PASSWORD" | grep -q '[0-9]'; then
    echo "[PASS] Contains number(s)."
    SCORE=$((SCORE + 1))
else
    echo "[FAIL] Missing number (0-9)."
fi

# 5. Special character check
if echo "$PASSWORD" | grep -q '[^a-zA-Z0-9]'; then
    echo "[PASS] Contains special character(s)."
    SCORE=$((SCORE + 1))
else
    echo "[FAIL] Missing special character (e.g. @, #, $, %, !)."
fi

# 6. Character repetition check (no character repeated more than 3 times)
MAX_REPEAT=0
for (( i=0; i<LEN; i++ )); do
    char="${PASSWORD:$i:1}"
    count=$(echo "$PASSWORD" | awk -F"$char" '{print NF-1}')
    if [ "$count" -gt "$MAX_REPEAT" ]; then
        MAX_REPEAT=$count
    fi
done

if [ "$MAX_REPEAT" -le 3 ]; then
    echo "[PASS] No character repeated more than 3 times (Max count: $MAX_REPEAT)."
    SCORE=$((SCORE + 1))
else
    echo "[FAIL] Repetition limit exceeded: A character repeats $MAX_REPEAT times (> 3)."
fi

echo "----------------------------------------------------------"
echo "Total Score: $SCORE / 6"

if [ "$SCORE" -ge 5 ]; then
    echo -e "Result: \e[32mSTRONG PASSWORD\e[0m"
elif [ "$SCORE" -ge 3 ]; then
    echo -e "Result: \e[33mMEDIUM PASSWORD\e[0m"
else
    echo -e "Result: \e[31mWEAK PASSWORD\e[0m"
fi
echo "=========================================================="
