# Topic 07: IPC: Pipes

## 1. Topic Overview & Purpose
An anonymous pipe is a unidirectional, byte-stream IPC channel managed by the Linux kernel in memory.
Because a single pipe allows data flow in only one direction (from write-end to read-end), full two-way communication between related processes (e.g. parent and child) requires **two pipes**:
1. Parent-to-Child pipe
2. Child-to-Parent pipe

---

## 2. Key System Calls & Concepts
- `int pipe(int pipefd[2])`: Creates a pipe.
  - `pipefd[0]`: Read end.
  - `pipefd[1]`: Write end.
- `read()` & `write()`: Standard POSIX I/O calls operating on pipe descriptors.
- **Closing Unused Ends**: Each process must close the pipe ends it does not use. If the write end of a pipe remains open anywhere, `read()` will block rather than returning `0` (EOF).
- **SIGPIPE**: Writing to a pipe with no active read end triggers a `SIGPIPE` signal (terminating the process unless handled).

---

## 3. Assignment Index & Filenames

| No. | Filename | Description |
|:---:|:---|:---|
| 1 | `01_parent_child_sum.c` | Parent sends N integers via Pipe 1; Child calculates sum and returns result via Pipe 2 |

---

## 4. Ubuntu Compilation & Execution Commands

```bash
# Compile using GCC
gcc -Wall 01_parent_child_sum.c -o 01_parent_child_sum

# Run program:
./01_parent_child_sum
```

---

## 5. Sample Output

```text
==========================================================
       PARENT-CHILD TWO-WAY PIPE COMMUNICATION            
==========================================================
Enter the count of integers (N): 5
Enter 5 integers separated by spaces:
10 25 30 15 20
[Child PID 18234] Received 5 numbers. Calculated sum = 100
----------------------------------------------------------
[Parent PID 18233] Final Sum received from Child: 100
==========================================================
```

---

## 6. Cleanup
```bash
rm -f 01_parent_child_sum
```
