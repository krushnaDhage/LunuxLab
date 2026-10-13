# Topic 05: IPC: Signals

## 1. Topic Overview & Purpose
Signals are asynchronous notifications sent by the Linux kernel or processes to inform a target process about an event (such as hardware exceptions, user interrupts, timers, or interprocess messages).
This module covers signal registration using `signal()`, delivering signals using `kill()`, trapping keyboard interrupts (`SIGINT`), and implementing timers with `alarm()`.

---

## 2. Key System Calls & Signals
- `sighandler_t signal(int signum, sighandler_t handler)`: Sets a disposition for signal `signum`.
- `int kill(pid_t pid, int sig)`: Sends signal `sig` to process `pid`.
- `unsigned int alarm(unsigned int seconds)`: Delivers `SIGALRM` after the specified number of real-time seconds.
- `int pause(void)`: Suspends the process until any signal is received.
- **Key Signals**:
  - `SIGINT` (2): Terminal interrupt (Ctrl+C).
  - `SIGALRM` (14): Timer expiration from `alarm()`.
  - `SIGUSR1` (10) & `SIGUSR2` (12): User-defined application signals.
  - `SIGTERM` (15): Graceful termination request.
  - *Note*: `SIGKILL` (9) and `SIGSTOP` (19) cannot be caught, blocked, or ignored.

---

## 3. Assignment Index & Filenames

| No. | Filename | Description |
|:---:|:---|:---|
| 1 | `01_parent_child_sigusr1.c` | Child installs handler; parent waits 3s and sends `SIGUSR1` via `kill()` |
| 2 | `02_handle_ctrl_c.c` | Traps `SIGINT` (Ctrl+C) without terminating the program |
| 3 | `03_alarm_input_timeout.c` | Enforces a 10-second input timeout using `alarm()` and `SIGALRM` |
| 4 | `04_interprocess_signal_communication.c` | Two-terminal IPC using `kill()` and user-defined signals |

---

## 4. Ubuntu Compilation & Execution Commands

```bash
gcc -Wall 01_parent_child_sigusr1.c -o 01_parent_child_sigusr1
gcc -Wall 02_handle_ctrl_c.c -o 02_handle_ctrl_c
gcc -Wall 03_alarm_input_timeout.c -o 03_alarm_input_timeout
gcc -Wall 04_interprocess_signal_communication.c -o 04_interprocess_signal_communication

# Run programs:
./01_parent_child_sigusr1
./02_handle_ctrl_c
./03_alarm_input_timeout

# For two-terminal communication:
# Terminal 1:
./04_interprocess_signal_communication
# Terminal 2:
./04_interprocess_signal_communication
```

---

## 5. Sample Output

### `01_parent_child_sigusr1.c`
```text
[Child PID 15201] Signal handler installed. Waiting for SIGUSR1...
[Parent PID 15200] Created child PID 15201. Waiting for 3 seconds...
[Parent] Sending SIGUSR1 to child process 15201 via kill()...
[Child Handler] Parent has sent SIGUSR1
[Child PID 15201] Resumed after signal. Terminating normally.
[Parent] Child has finished. Parent exiting.
```

### `03_alarm_input_timeout.c` (When user waits > 10 seconds)
```text
==========================================================
         INPUT TIMEOUT DEMONSTRATION (alarm)              
==========================================================
You have 10 seconds to enter your name...

Enter your name: 

[TIMEOUT] Time limit of 10 seconds exceeded!
          No input received from user. Terminating program.
```

---

## 6. Cleanup
```bash
rm -f 01_parent_child_sigusr1 02_handle_ctrl_c \
      03_alarm_input_timeout 04_interprocess_signal_communication
```
