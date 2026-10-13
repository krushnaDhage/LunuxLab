# Topic 04: File System Internals and File Locking

## 1. Topic Overview & Purpose
Linux organizes all data, devices, and IPC channels using the virtual file system (VFS). Understanding inode metadata, file types, and file locking mechanisms is essential for reliable systems programming.
This module demonstrates extracting file attributes via `fstat()`, detecting the 7 POSIX file types using `lstat()`, and managing file access concurrency using `flock()`.

---

## 2. Key System Calls & Structures
- `int fstat(int fd, struct stat *buf)`: Obtains status of an already opened file descriptor.
- `int lstat(const char *pathname, struct stat *buf)`: Similar to `stat()`, but returns information about the symbolic link itself rather than the target file.
- `int flock(int fd, int operation)`: Applies or removes an advisory lock on an open file:
  - `LOCK_SH`: Shared lock (multiple readers).
  - `LOCK_EX`: Exclusive lock (single writer).
  - `LOCK_UN`: Unlock.
  - `LOCK_NB`: Non-blocking flag.

---

## 3. Assignment Index & Filenames

| No. | Filename | Description |
|:---:|:---|:---|
| 1 | `01_file_statistics_fstat.c` | Uses `fstat()` to display Inode, UID, GID, FAP (permissions), and file type |
| 2 | `02_linux_file_types.c` | Inspects and identifies all 7 Linux file types (regular, dir, char, block, fifo, link, socket) |
| 3 | `03_file_locking_flock.c` | Applies `flock()`, checks mandatory locking criteria (SGID on, group-exec off), and explains advisory behavior |

---

## 4. Ubuntu Compilation & Execution Commands

```bash
gcc -Wall 01_file_statistics_fstat.c -o 01_file_statistics_fstat
gcc -Wall 02_linux_file_types.c -o 02_linux_file_types
gcc -Wall 03_file_locking_flock.c -o 03_file_locking_flock

# Run programs:
./01_file_statistics_fstat /etc/passwd
./02_linux_file_types
./03_file_locking_flock lock_test.txt
```

---

## 5. Sample Output & Analysis

### `01_file_statistics_fstat.c`
```text
==========================================================
               FILE STATISTICS (fstat)                    
==========================================================
 Target Path : /etc/passwd
 1. Inode    : 262145
 2. UID      : 0
 3. GID      : 0
 4. FAP      : 0644 (Octal Permissions)
 5. File Type: Regular File
==========================================================
```

### Advisory vs. Mandatory Locking Explanation
- **Advisory Locking**: Default in Linux (`flock()` and `fcntl()`). Cooperating processes explicitly call locking APIs before accessing the file. If an uncooperative process ignores the lock, the kernel does NOT prevent standard `read()` or `write()`.
- **Mandatory Locking**: The OS kernel blocks unauthorized `read()` and `write()` calls even from processes that never called a locking API.
  - Historical Unix requirement: File must have `S_ISGID` ON and `S_IXGRP` OFF, on a filesystem mounted with `-o mand`.
  - **Linux 5.15+ Update**: Mandatory locking has been officially removed from the Linux kernel because of race conditions and system lockups.

---

## 6. Cleanup
```bash
rm -f 01_file_statistics_fstat 02_linux_file_types 03_file_locking_flock lock_test.txt
```
