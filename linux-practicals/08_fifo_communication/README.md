# Topic 08: IPC: FIFO (Named Pipes)

## 1. Topic Overview & Purpose
A FIFO (First-In, First-Out), also known as a **Named Pipe**, provides a unidirectional byte-stream communication channel similar to an anonymous pipe. Unlike regular pipes, a FIFO exists as a special file in the file system created via `mkfifo()`. This allows completely unrelated processes to communicate by referencing the same filename path.

---

## 2. Key System Calls & Concepts
- `int mkfifo(const char *pathname, mode_t mode)`: Creates a FIFO special file in the file system.
- `open()`, `read()`, `write()`, `close()`: Standard POSIX file operations used on the FIFO path.
- **Blocking Behavior on Open**: By default, opening a FIFO for reading (`O_RDONLY`) blocks until another process opens it for writing (`O_WRONLY`), and vice versa. This automatically synchronizes sender and receiver startup.
- `unlink(const char *pathname)`: Removes the FIFO node from the file system.

---

## 3. Assignment Index & Filenames

| No. | Filename | Description |
|:---:|:---|:---|
| 1 | `01_fifo_sender.c` | Process A: Reads a text file and transmits its complete content through the FIFO |
| 2 | `02_fifo_receiver.c` | Process B: Receives data from the FIFO, saves it into an output file, and reports byte count |

---

## 4. Ubuntu Compilation & Execution Commands

```bash
# Compile both programs
gcc -Wall 01_fifo_sender.c -o 01_fifo_sender
gcc -Wall 02_fifo_receiver.c -o 02_fifo_receiver

# Execution in two terminals:
# Step 1: In Terminal 1, start the receiver (Process B):
./02_fifo_receiver received_output.txt

# Step 2: In Terminal 2, start the sender (Process A):
./01_fifo_sender sample_source.txt
```

---

## 5. Sample Output

### Terminal 1 (`02_fifo_receiver`):
```text
==========================================================
         FIFO FILE RECEIVER (Process B)                   
==========================================================
Output File: received_output.txt
FIFO Pipe  : my_test_fifo
Waiting for Sender (Process A) to connect...

[Receiver] Connected! Receiving data stream...
==========================================================
[Receiver] Data transfer complete!
           Total Bytes Received and Stored: 125 bytes
           Stored File                    : received_output.txt
==========================================================
```

### Terminal 2 (`01_fifo_sender`):
```text
==========================================================
         FIFO FILE SENDER (Process A)                     
==========================================================
Source File: sample_source.txt
FIFO Pipe  : my_test_fifo
Waiting for Receiver (Process B) to open the FIFO...

[Sender] Connected! Transferring file content...
==========================================================
[Sender] Transfer completed successfully!
         Total Bytes Sent: 125 bytes
==========================================================
```

---

## 6. Cleanup
```bash
rm -f 01_fifo_sender 02_fifo_receiver my_test_fifo sample_source.txt received_output.txt
```
