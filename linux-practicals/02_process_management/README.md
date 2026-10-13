# Topic 02: Process Management: `fork()` and `exec()`

## 1. Topic Overview & Purpose
In Unix/Linux, process creation is separated into two fundamental operations:
1. `fork()`: Creates an exact duplicate child process with its own private address space (Copy-on-Write).
2. `exec()` family: Replaces the current process memory image with a new executable program.

This folder contains practical demonstrations of process lifecycles, PID/PPID relationships, Zombie and Orphan processes, variable isolation, shared file descriptors, and `exec()` semantics.

---

## 2. Key System Calls & Concepts
- `pid_t fork(void)`: Clones the calling process. Returns `0` to child, child's PID to parent, and `-1` on error.
- `pid_t getpid(void)`: Returns current process ID.
- `pid_t getppid(void)`: Returns parent process ID.
- `pid_t wait(int *status)`: Blocks parent until a child process terminates; reaps child state.
- `int execl(const char *path, const char *arg, ...)`: Replaces calling process image with the specified executable.
- **Zombie Process**: A terminated process whose parent has not yet called `wait()`. Remains in the process table in state `Z` (`[defunct]`).
- **Orphan Process**: A process whose parent has exited while it continues running. It is re-parented to `init` (PID 1) or the systemd subreaper.

---

## 3. Assignment Index & Filenames

| No. | Filename | Description |
|:---:|:---|:---|
| 1 | `01_basic_fork.c` | Basic `fork()` showing non-deterministic scheduling order |
| 2 | `02_fork_pid_ppid.c` | Verifies PID and PPID relationships using `sleep(5)` and `ps` |
| 3 | `03_zombie_process.c` | Demonstrates zombie creation (child terminates, parent sleeps without `wait()`) |
| 4 | `04_orphan_process.c` | Demonstrates orphan creation and adoption by init/systemd |
| 5 | `05_fork_variable_behavior.c` | Tests global and local variable independence across parent and child |
| 6 | `06_fork_file_writing.c` | Parent and child writing to shared file descriptor with and without `\n` |
| 7 | `07_exec_without_fork.c` | Calls `execl()` directly, showing image replacement and error return |
| 8 | `08_fork_exec_parent.c` | Calls `execl()` inside parent process while child executes |
| 9 | `09_fork_exec_child.c` | Standard Unix shell pattern: parent forks, child calls `execl()`, parent waits |
| 10 | `10_exec_file_descriptor.c` | Demonstrates file descriptor inheritance across `fork()` and `execl()` |

---

## 4. Ubuntu Compilation & Execution Commands

```bash
# Compile all programs using GCC
gcc -Wall 01_basic_fork.c -o 01_basic_fork
gcc -Wall 02_fork_pid_ppid.c -o 02_fork_pid_ppid
gcc -Wall 03_zombie_process.c -o 03_zombie_process
gcc -Wall 04_orphan_process.c -o 04_orphan_process
gcc -Wall 05_fork_variable_behavior.c -o 05_fork_variable_behavior
gcc -Wall 06_fork_file_writing.c -o 06_fork_file_writing
gcc -Wall 07_exec_without_fork.c -o 07_exec_without_fork
gcc -Wall 08_fork_exec_parent.c -o 08_fork_exec_parent
gcc -Wall 09_fork_exec_child.c -o 09_fork_exec_child
gcc -Wall 10_exec_file_descriptor.c -o 10_exec_file_descriptor

# Run programs individually:
./01_basic_fork
./02_fork_pid_ppid
./03_zombie_process
./04_orphan_process
./05_fork_variable_behavior
./06_fork_file_writing
./07_exec_without_fork
./07_exec_without_fork invalid
./08_fork_exec_parent
./09_fork_exec_child
./10_exec_file_descriptor
```

---

## 5. Sample Output & Observations

### `01_basic_fork.c`
```text
Before fork(): Starting single process (PID: 12345)
[Parent Process] Hello from Parent! (My PID: 12345, Created Child PID: 12346)
Common exit: Process with PID 12345 terminating.
[Child Process] Hello from Child! (My PID: 12346, Parent PID: 12345)
Common exit: Process with PID 12346 terminating.
```
*Observation*: Parent or child may print first depending on CPU scheduling.

### `03_zombie_process.c`
In Terminal 1:
```text
[Parent] Child has terminated but Parent is sleeping for 12 seconds.
[Parent] Run 'ps -l' or 'ps aux | grep defunct' in another terminal.
```
In Terminal 2 (`ps -l`):
```text
F S   UID   PID  PPID  C PRI  NI ADDR SZ WCHAN  TTY          TIME CMD
0 Z  1000 12380 12379  0  80   0 -     0 -      pts/1    00:00:00 03_zombie <defunct>
```

---

## 6. Important Code Explanations
- **Variable Behavior (`05`)**: When child modifies `global_var` and `local_var`, it modifies its own private copy. The parent's values remain unchanged.
- **File Writing (`06`)**: Because parent and child share the same open file description table entry, they share the file offset pointer. Text appends in sequence rather than overwriting.
- **File Descriptors Across Exec (`10`)**: File descriptors remain open across `exec()` unless explicitly configured with `O_CLOEXEC` / `FD_CLOEXEC`.

---

## 7. Cleanup
```bash
rm -f 01_basic_fork 02_fork_pid_ppid 03_zombie_process 04_orphan_process \
      05_fork_variable_behavior 06_fork_file_writing 07_exec_without_fork \
      08_fork_exec_parent 09_fork_exec_child 10_exec_file_descriptor \
      output_no_newline.txt output_with_newline.txt exec_fd_test.txt
```
