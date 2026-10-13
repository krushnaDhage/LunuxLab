#!/bin/bash
# ==============================================================================
# Script: 06_linux_quiz_system.sh
# Description: Terminal-based Linux Quiz System featuring:
#              - Multiple-choice questions (MCQs)
#              - Per-question countdown timer (10s)
#              - Negative marking (+2 for correct, -1 for wrong, 0 for timeout)
#              - Final score report
# ==============================================================================

TIME_LIMIT=10
SCORE=0
CORRECT=0
WRONG=0
TIMEOUT=0

ask() {
    local q="$1"
    local a="$2"
    local b="$3"
    local c="$4"
    local d="$5"
    local ans="$6"

    echo ""
    echo "$q"
    echo "  A) $a"
    echo "  B) $b"
    echo "  C) $c"
    echo "  D) $d"
    echo -n "Answer [Time: ${TIME_LIMIT}s]: "

    user_ans=""
    if read -t "$TIME_LIMIT" -r user_ans; then
        user_ans=$(echo "$user_ans" | tr '[:lower:]' '[:upper:]')
        if [ "$user_ans" = "$ans" ]; then
            echo "-> Correct! (+2)"
            SCORE=$((SCORE + 2))
            CORRECT=$((CORRECT + 1))
        else
            echo "-> Incorrect! (-1) [Correct: $ans]"
            SCORE=$((SCORE - 1))
            WRONG=$((WRONG + 1))
        fi
    else
        echo -e "\n-> Time's up! (0)"
        TIMEOUT=$((TIMEOUT + 1))
    fi
}

echo "=========================================================="
echo "                 LINUX PRACTICAL QUIZ                     "
echo "=========================================================="
echo "Rules: +2 for correct, -1 for wrong, 10s per question."

ask "1. Which system call creates a new process in Linux?" "exec()" "fork()" "clone()" "wait()" "B"
ask "2. What signal is sent by pressing Ctrl+C?" "SIGKILL" "SIGSTOP" "SIGINT" "SIGTERM" "C"
ask "3. What is the IPC mechanism for fast shared memory in Linux?" "pipe" "FIFO" "shmget" "socket" "C"

echo ""
echo "=========================================================="
echo "                    FINAL QUIZ REPORT                     "
echo "=========================================================="
echo "Correct Answers : $CORRECT (+ $((CORRECT * 2)))"
echo "Wrong Answers   : $WRONG (- $WRONG)"
echo "Timed Out       : $TIMEOUT (0)"
echo "Total Score     : $SCORE / 6"
echo "=========================================================="
