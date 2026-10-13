# Topic 01: Shell Scripting

## 1. Topic Overview & Purpose
Shell scripting allows Linux users and system administrators to automate repetitive tasks, manipulate text files, inspect system processes, and manage file systems using Bash. This module covers essential scripting techniques frequently tested in college practical examinations.

## 2. Key Concepts & Linux Commands
- **Control Flow**: `if-else`, `for`, `while`, `case`.
- **String Manipulation**: Parameter expansion `${#str}`, `${str:pos:len}`, `tr`, `cut`.
- **File & Stream Operations**: Redirection (`>`, `>>`), pipes (`|`), `grep`, `sed`, `awk`, `find`.
- **System Utilities**: `who`, `w`, `last`, `md5sum`, `wc`.

---

## 3. Assignment Index & Filenames

| No. | Filename | Description |
|:---:|:---|:---|
| 1 | `01_uppercase_files.sh` | Converts given filenames to uppercase with overwrite protection |
| 2 | `02_user_login_duration.sh` | Calculates active session and historical login duration for a user |
| 3 | `03_display_line_range.sh` | Prints lines between specified start and end numbers from a file |
| 4 | `04_delete_matching_lines.sh` | Removes lines matching a word (with output file or confirmed in-place backup) |
| 5 | `05_string_operations.sh` | Extracts substrings and calculates string lengths |
| 6 | `06_linux_quiz_system.sh` | Interactive MCQ quiz with countdown timer and negative marking |
| 7 | `07_log_analyzer.sh` | Parses logs for ERROR/WARN/INFO, top IPs, and failed logins |
| 8 | `08_duplicate_file_finder.sh` | Finds duplicate files recursively by MD5 hash with delete prompt |
| 9 | `09_file_organizer.sh` | Categorizes files into Images, PDFs, Videos, Docs, Archives, Others |
| 10 | `10_password_validator.sh` | Validates password complexity rules and computes strength score |

---

## 4. Prerequisites
Make all scripts executable before running on Ubuntu:
```bash
chmod +x *.sh
```

---

## 5. Ubuntu Execution Commands

```bash
# 1. Uppercase Files
touch file1.txt file2.txt
./01_uppercase_files.sh file1.txt file2.txt

# 2. User Login Duration
./02_user_login_duration.sh $(whoami)

# 3. Display Line Range
./03_display_line_range.sh /etc/passwd 1 5

# 4. Delete Matching Lines
echo -e "apple\nbanana\napple pie" > fruit.txt
./04_delete_matching_lines.sh apple fruit.txt

# 5. String Operations
./05_string_operations.sh "OperatingSystem" 0 9

# 6. Linux Quiz System
./06_linux_quiz_system.sh

# 7. Log Analyzer
./07_log_analyzer.sh

# 8. Duplicate File Finder
./08_duplicate_file_finder.sh .

# 9. File Organizer
./09_file_organizer.sh .

# 10. Password Validator
./10_password_validator.sh "P@ssw0rd2026"
```

---

## 6. Sample Output

### Running `03_display_line_range.sh`:
```text
==========================================================
 File: /etc/passwd (Total lines: 45)
 Extracting lines from 1 to 3
==========================================================
--- Lines 1 to 3 Content ---
root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
bin:x:2:2:bin:/bin:/usr/sbin/nologin
--- End of Line Range ---
```

### Running `10_password_validator.sh`:
```text
==========================================================
              PASSWORD STRENGTH EVALUATION                
==========================================================
[PASS] Length is 12 characters (>= 10).
[PASS] Contains uppercase letter(s).
[PASS] Contains lowercase letter(s).
[PASS] Contains number(s).
[PASS] Contains special character(s).
[PASS] No character repeated more than 3 times (Max count: 1).
----------------------------------------------------------
Total Score: 6 / 6
Result: STRONG PASSWORD
==========================================================
```

---

## 7. Explanation of Important Logic
- **Collision Protection (`01`)**: Checks if target file `[ -e "$upper_name" ]` exists before `mv` to avoid accidental data loss.
- **Line Range (`03`)**: Uses `sed -n "${START_LINE},${END_LINE}p" "$FILE"` which efficiently extracts lines without loading the whole file into memory.
- **Safe Line Deletion (`04`)**: Generates a `.bak` copy and writes clean lines via `grep -v` to a temporary file before replacing the target.
- **Timed Quiz Input (`06`)**: Uses `read -t 15` to capture user input within 15 seconds; returns non-zero code on timeout.
- **Hashing (`08`)**: Computes `md5sum` on each file and finds duplicate hashes with `sort | uniq -d`.

---

## 8. Common Errors & Troubleshooting
1. **Permission Denied**: Run `chmod +x <script_name>.sh`.
2. **Windows CRLF line endings (`\r: command not found`)**: If scripts were cloned from Windows, convert line endings using `sed -i 's/\r$//' *.sh` or `dos2unix *.sh`.
3. **Empty Output in Log Analyzer**: Provide a valid path to `/var/log/syslog` or run without arguments to let the script generate a sample log file automatically.
