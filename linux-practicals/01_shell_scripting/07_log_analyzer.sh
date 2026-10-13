#!/bin/bash
# ==============================================================================
# Script: 07_log_analyzer.sh
# Description: Analyzes a Linux log file (e.g. /var/log/auth.log or syslog).
#              Tasks:
#              1. Counts ERROR, WARNING, and INFO messages using grep.
#              2. Displays the top 10 IP addresses using grep, sort, and uniq.
#              3. Finds failed login attempts using grep, awk, and cut.
#              4. Generates a structured summary report.
#
# Log Format Assumption:
#   Standard Linux Syslog / auth.log format:
#   <Month> <Day> <Time> <Hostname> <Service[PID]>: <Message>
#   e.g.: "Oct 14 01:00:01 server sshd[101]: Failed password for invalid user admin from 192.168.1.10 port 4001"
#
# Usage: ./07_log_analyzer.sh [path_to_logfile]
# ==============================================================================

LOG_FILE="$1"

# If no file provided or file does not exist, generate a demonstration sample log
if [ -z "$LOG_FILE" ] || [ ! -f "$LOG_FILE" ]; then
    LOG_FILE="sample_demo.log"
    echo "[INFO] No valid input file specified. Creating demo log '$LOG_FILE'..."
    cat << 'EOF' > "$LOG_FILE"
Oct 14 01:00:01 ubuntu sshd[101]: Failed password for invalid user admin from 192.168.1.10 port 4001 ssh2
Oct 14 01:00:05 ubuntu systemd[1]: INFO: Network interface eth0 up and running
Oct 14 01:00:10 ubuntu sshd[102]: Failed password for root from 192.168.1.10 port 4002 ssh2
Oct 14 01:00:15 ubuntu kernel: WARNING: Disk space low on /dev/sda1
Oct 14 01:00:20 ubuntu app[201]: ERROR: Database connection pool exhausted
Oct 14 01:00:25 ubuntu sshd[103]: Failed password for user test from 10.0.0.5 port 5001 ssh2
Oct 14 01:00:30 ubuntu systemd[1]: INFO: Backup job completed successfully
Oct 14 01:00:35 ubuntu kernel: WARNING: High CPU temperature detected
Oct 14 01:00:40 ubuntu app[201]: ERROR: Socket read timed out
Oct 14 01:00:45 ubuntu sshd[104]: Failed password for invalid user guest from 192.168.1.10 port 4003 ssh2
Oct 14 01:00:50 ubuntu sshd[105]: Accepted publickey for ubuntu from 192.168.1.25 port 51234 ssh2
EOF
fi

echo "=========================================================="
echo "          LINUX LOG FILE ANALYZER & REPORT                "
echo "=========================================================="
echo "Target Log File : $LOG_FILE"
echo "Total Log Lines : $(wc -l < "$LOG_FILE")"
echo "Format Model    : Standard Linux Syslog / auth.log"
echo "=========================================================="

# 1. Message severity counts using grep
info_cnt=$(grep -i -c "INFO" "$LOG_FILE" || echo 0)
warn_cnt=$(grep -i -c "WARNING" "$LOG_FILE" || echo 0)
err_cnt=$(grep -i -c "ERROR" "$LOG_FILE" || echo 0)

echo ""
echo "--- 1. Message Severity Counts (grep -c) ---"
printf "  %-12s : %d\n" "INFO" "$info_cnt"
printf "  %-12s : %d\n" "WARNING" "$warn_cnt"
printf "  %-12s : %d\n" "ERROR" "$err_cnt"

# 2. Top 10 IP addresses using grep, sort, uniq
echo ""
echo "--- 2. Top 10 IP Addresses (grep, sort, uniq) ---"
ip_matches=$(grep -oE '\b([0-9]{1,3}\.){3}[0-9]{1,3}\b' "$LOG_FILE" | sort | uniq -c | sort -nr | head -n 10)
if [ -n "$ip_matches" ]; then
    printf "  %-8s %-20s\n" "Hits" "IP Address"
    echo "  -----------------------------"
    echo "$ip_matches" | while read -r hits ip; do
        printf "  %-8s %-20s\n" "$hits" "$ip"
    done
else
    echo "  No IPv4 addresses found in this log."
fi

# 3. Failed login attempts using grep, cut, awk
echo ""
echo "--- 3. Failed Login Attempts (grep, awk, cut) ---"
failed_lines=$(grep -i "Failed password" "$LOG_FILE" || true)
failed_cnt=$(echo "$failed_lines" | grep -v '^$' | wc -l || echo 0)

echo "  Total Failed Attempts: $failed_cnt"
if [ "$failed_cnt" -gt 0 ]; then
    echo "  Details (Extracted via cut & awk):"
    # Using cut to extract timestamp and awk to extract username/IP
    echo "$failed_lines" | while IFS= read -r line; do
        timestamp=$(echo "$line" | cut -d' ' -f1-3)
        user_info=$(echo "$line" | awk -F'for ' '{print $2}' | cut -d' ' -f1-2)
        ip_info=$(echo "$line" | awk -F'from ' '{print $2}' | cut -d' ' -f1)
        echo "   [FAIL] Time: $timestamp | Target: $user_info | Source IP: $ip_info"
    done
fi

# 4. Final Summary Report
echo ""
echo "=========================================================="
echo "                    SUMMARY REPORT                        "
echo "=========================================================="
echo " Analyzed File    : $LOG_FILE"
echo " Total Errors     : $err_cnt"
echo " Total Warnings   : $warn_cnt"
echo " Total Info       : $info_cnt"
echo " Failed Logins    : $failed_cnt"
echo " Security Status  : $([ "$failed_cnt" -gt 3 ] && echo "ALERT: Multiple failed logins!" || echo "OK")"
echo "=========================================================="
