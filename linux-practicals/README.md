# Linux System Programming — Practical Repository

A comprehensive, student-friendly Linux System Programming practical codebase organized strictly topic-wise for Ubuntu Linux. This repository satisfies all experiments, exercises, and sub-questions from the course curriculum.

> **Environment Note**: This repository contains complete, logically verified source code and documentation. All code is designed to be executed directly on **Ubuntu Linux (20.04 / 22.04 / 24.04 LTS)**.

---

## 📁 Repository Directory Structure

```text
linux-practicals/
├── README.md                               # Root repository overview & master guide
├── 01_shell_scripting/                     # Topic 01: Bash Shell Scripting
│   ├── README.md
│   ├── 01_uppercase_files.sh
│   ├── 02_user_login_duration.sh
│   ├── 03_display_line_range.sh
│   ├── 04_delete_matching_lines.sh
│   ├── 05_string_operations.sh
│   ├── 06_linux_quiz_system.sh
│   ├── 07_log_analyzer.sh
│   ├── 08_duplicate_file_finder.sh
│   ├── 09_file_organizer.sh
│   └── 10_password_validator.sh
├── 02_process_management/                  # Topic 02: fork(), exec(), process lifecycle
│   ├── README.md
│   ├── 01_basic_fork.c
│   ├── 02_fork_pid_ppid.c
│   ├── 03_zombie_process.c
│   ├── 04_orphan_process.c
│   ├── 05_fork_variable_behavior.c
│   ├── 06_fork_file_writing.c
│   ├── 07_exec_without_fork.c
│   ├── 08_fork_exec_parent.c
│   ├── 09_fork_exec_child.c
│   └── 10_exec_file_descriptor.c
├── 03_multithreading/                      # Topic 03: POSIX Threads (pthreads)
│   ├── README.md
│   ├── 01_odd_even_prime_threads.c
│   ├── 02_shared_loop_variable.c
│   ├── 03_thread_parameters_counter.c
│   ├── 04_mutex_counter.c
│   └── 05_alternating_thread_file_write.c
├── 04_file_system_and_locking/             # Topic 04: VFS internals & file locking
│   ├── README.md
│   ├── 01_file_statistics_fstat.c
│   ├── 02_linux_file_types.c
│   └── 03_file_locking_flock.c
├── 05_signals/                             # Topic 05: Asynchronous IPC via Signals
│   ├── README.md
│   ├── 01_parent_child_sigusr1.c
│   ├── 02_handle_ctrl_c.c
│   ├── 03_alarm_input_timeout.c
│   └── 04_interprocess_signal_communication.c
├── 06_message_queues/                      # Topic 06: System V Message Queues
│   ├── README.md
│   ├── 01_sender.c
│   └── 02_receiver.c
├── 07_pipes/                               # Topic 07: Anonymous Pipes
│   ├── README.md
│   └── 01_parent_child_sum.c
├── 08_fifo_communication/                  # Topic 08: Named Pipes (FIFOs)
│   ├── README.md
│   ├── 01_fifo_sender.c
│   └── 02_fifo_receiver.c
├── 09_shared_memory/                       # Topic 09: System V Shared Memory
│   ├── README.md
│   ├── 01_shared_memory_string.c
│   ├── 02_file_to_shared_memory.c
│   └── 03_shared_memory_to_file.c
├── 10_semaphores/                          # Topic 10: System V & POSIX Semaphores
│   ├── README.md
│   ├── 01_semaphore_initialize.c
│   ├── 02_semaphore_p_operation.c
│   ├── 03_semaphore_v_operation.c
│   └── 04_producer_consumer.c
└── 11_socket_programming/                  # Topic 11: Java Socket Programming
    ├── README.md
    ├── 01_tcp_iterative/
    │   ├── TCPServer.java
    │   └── TCPClient.java
    ├── 02_tcp_concurrent/
    │   ├── TCPServer.java
    │   └── TCPClient.java
    ├── 03_udp_iterative/
    │   ├── UDPServer.java
    │   └── UDPClient.java
    └── 04_udp_concurrent/
        ├── UDPServer.java
        └── UDPClient.java
```

---

## 🛠️ Ubuntu Environment Setup & Toolchain

To prepare an Ubuntu workstation or VM to compile and run all assignments:

```bash
# Update package indices
sudo apt update

# Install GCC, Make, standard C libraries, and build tools
sudo apt install build-essential -y

# Install OpenJDK for Java socket practicals
sudo apt install default-jdk -y

# Verify installations
gcc --version
javac -version
java -version
bash --version
```

---

## 🚀 Quick Execution Guide

### 1. Shell Scripts (`01_shell_scripting/`)
Ensure execute permissions are granted:
```bash
cd 01_shell_scripting
chmod +x *.sh
./01_uppercase_files.sh <filenames...>
```

### 2. Standard C Programs (`02`, `04`, `05`, `06`, `07`, `08`, `09`, `10`)
Compile using `gcc -Wall`:
```bash
gcc -Wall <program.c> -o <binary_name>
./<binary_name>
```

### 3. Multithreaded C Programs (`03_multithreading/` and `10_semaphores/04_producer_consumer.c`)
Link the POSIX thread library using `-pthread`:
```bash
gcc -Wall -pthread <program.c> -o <binary_name>
./<binary_name>
```

### 4. Java Socket Programs (`11_socket_programming/`)
Compile and run with JDK:
```bash
cd 11_socket_programming/01_tcp_iterative
javac TCPServer.java TCPClient.java
# Run server:
java TCPServer
# In another terminal, run client:
java TCPClient
```

---

## ✅ Complete Assignment Coverage Checklist

| Topic | Experiment / Objective | Language | Implementation File | Status |
|:---|:---|:---:|:---|:---:|
| **1. Shell Scripting** | 1. Filename uppercase conversion & collision check | Bash | `01_uppercase_files.sh` | Completed |
| | 2. User login duration & active session inspect | Bash | `02_user_login_duration.sh` | Completed |
| | 3. Display line range from file | Bash | `03_display_line_range.sh` | Completed |
| | 4. Delete lines containing word with backup | Bash | `04_delete_matching_lines.sh` | Completed |
| | 5. Substring extraction & string length | Bash | `05_string_operations.sh` | Completed |
| | 6. Linux quiz system with timer & negative marking | Bash | `06_linux_quiz_system.sh` | Completed |
| | 7. Log file analyzer (grep, awk, sort, uniq, cut) | Bash | `07_log_analyzer.sh` | Completed |
| | 8. Recursive duplicate finder via md5sum | Bash | `08_duplicate_file_finder.sh` | Completed |
| | 9. File organizer into category folders | Bash | `09_file_organizer.sh` | Completed |
| | 10. Password complexity validation & scoring | Bash | `10_password_validator.sh` | Completed |
| **2. Process Management** | 1. Basic `fork()` & scheduling order | C | `01_basic_fork.c` | Completed |
| | 2. PID, PPID & sleep verification | C | `02_fork_pid_ppid.c` | Completed |
| | 3. Zombie process creation & ps check | C | `03_zombie_process.c` | Completed |
| | 4. Orphan process creation & init adoption | C | `04_orphan_process.c` | Completed |
| | 5. Global & local variable memory isolation | C | `05_fork_variable_behavior.c` | Completed |
| | 6. File writing after `fork()` (newline test) | C | `06_fork_file_writing.c` | Completed |
| | 7. `execl()` without `fork()` & error return | C | `07_exec_without_fork.c` | Completed |
| | 8. `execl()` in parent process | C | `08_fork_exec_parent.c` | Completed |
| | 9. `execl()` in child process (shell pattern) | C | `09_fork_exec_child.c` | Completed |
| | 10. File descriptor inheritance across `exec()` | C | `10_exec_file_descriptor.c` | Completed |
| **3. Multithreading** | 1. Odd, even, prime threads & averages | C (pthreads) | `01_odd_even_prime_threads.c` | Completed |
| | 2. Shared loop-variable address race experiment | C (pthreads) | `02_shared_loop_variable.c` | Completed |
| | 3. Thread parameters & unsynchronized counter | C (pthreads) | `03_thread_parameters_counter.c` | Completed |
| | 4. Mutex-protected counter (atomicity vs order) | C (pthreads) | `04_mutex_counter.c` | Completed |
| | 5. Alternating file write via condition variable | C (pthreads) | `05_alternating_thread_file_write.c` | Completed |
| **4. File System & Locking** | 1. File statistics via `fstat()` | C | `01_file_statistics_fstat.c` | Completed |
| | 2. Identification of 7 Linux file types | C | `02_linux_file_types.c` | Completed |
| | 3. `flock()` locking & mandatory check | C | `03_file_locking_flock.c` | Completed |
| **5. Signals** | 1. Parent sends `SIGUSR1` to child via `kill()` | C | `01_parent_child_sigusr1.c` | Completed |
| | 2. Trap `SIGINT` (Ctrl+C) without quitting | C | `02_handle_ctrl_c.c` | Completed |
| | 3. Input timeout via `alarm()` & `SIGALRM` | C | `03_alarm_input_timeout.c` | Completed |
| | 4. Two-terminal signal communication | C | `04_interprocess_signal_communication.c` | Completed |
| **6. Message Queues** | 1. Two-way conversation: Process 1 (Sender) | C (Sys V IPC) | `01_sender.c` | Completed |
| | 2. Two-way conversation: Process 2 (Receiver) | C (Sys V IPC) | `02_receiver.c` | Completed |
| **7. Pipes** | 1. Parent-child two-way integer sum | C (POSIX) | `01_parent_child_sum.c` | Completed |
| **8. FIFO Communication** | 1. Process A: FIFO text file sender | C (POSIX) | `01_fifo_sender.c` | Completed |
| | 2. Process B: FIFO text file receiver | C (POSIX) | `02_fifo_receiver.c` | Completed |
| **9. Shared Memory** | 1. Interprocess string transfer via shared memory | C (Sys V IPC) | `01_shared_memory_string.c` | Completed |
| | 2. Copy small file into shared memory | C (Sys V IPC) | `02_file_to_shared_memory.c` | Completed |
| | 3. Read shared memory and write to file | C (Sys V IPC) | `03_shared_memory_to_file.c` | Completed |
| **10. Semaphores** | 1. Initialize semaphore & print ID | C (Sys V IPC) | `01_semaphore_initialize.c` | Completed |
| | 2. Perform P operation (wait / decrement) | C (Sys V IPC) | `02_semaphore_p_operation.c` | Completed |
| | 3. Perform V operation (signal / increment) | C (Sys V IPC) | `03_semaphore_v_operation.c` | Completed |
| | 4. Producer-Consumer problem | C (POSIX) | `04_producer_consumer.c` | Completed |
| **11. Sockets (Java Only)** | 1. Iterative TCP Client & Server | Java | `01_tcp_iterative/` | Completed |
| | 2. Concurrent Multi-threaded TCP Client & Server | Java | `02_tcp_concurrent/` | Completed |
| | 3. Iterative UDP Client & Server | Java | `03_udp_iterative/` | Completed |
| | 4. Concurrent Multi-threaded UDP Client & Server | Java | `04_udp_concurrent/` | Completed |

---

## 💡 Practical Examination & Viva Tips
1. **Pointers & Memory Isolation in `fork()`**: Remember that memory is NOT shared after `fork()`. Linux employs Copy-on-Write (COW). Both processes start with identical memory contents, but modifications in one process remain private to that process.
2. **Advisory vs. Mandatory Locking**: Modern Linux kernels (5.15+) do not support mandatory locking. `flock()` is strictly advisory: processes must voluntarily check the lock.
3. **Pipes are Unidirectional**: A single anonymous pipe can only flow data one way. For request-response interaction, you must allocate two pipes.
4. **Message Queues vs. Streams**: Pipes and FIFOs are unformatted byte streams. Message queues preserve discrete message boundaries and support message typing (`mtype`).
5. **UDP Concurrency Safety**: UDP servers must copy incoming packet buffer contents (`Arrays.copyOf`) before passing them to worker threads because a single `DatagramSocket` reuses the same buffer for subsequent calls to `receive()`.
