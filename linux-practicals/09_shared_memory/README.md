# Topic 09: IPC: Shared Memory

## 1. Topic Overview & Purpose
Shared Memory is the fastest form of Interprocess Communication (IPC) available in Linux. Instead of copying data between user space and kernel space through system calls like `read()` and `write()`, the kernel maps a common region of physical memory into the address spaces of multiple processes.
Processes read and write directly to RAM pointers, achieving maximum data throughput.

---

## 2. Key System Calls & APIs
- `int shmget(key_t key, size_t size, int shmflg)`: Allocates a System V shared memory segment.
- `void *shmat(int shmid, const void *shmaddr, int shmflg)`: Attaches the shared segment to the calling process's address space.
- `int shmdt(const void *shmaddr)`: Detaches the segment from the calling process's address space.
- `int shmctl(int shmid, int cmd, struct shmid_ds *buf)`: Control operations, including `IPC_RMID` to mark the segment for deletion once all processes detach.

---

## 3. Assignment Index & Filenames

| No. | Filename | Description |
|:---:|:---|:---|
| 1 | `01_shared_memory_string.c` | Shares a simple string between processes running concurrently |
| 2 | `02_file_to_shared_memory.c` | Reads a small file and writes its content and length metadata into shared memory |
| 3 | `03_shared_memory_to_file.c` | Reads shared memory content and saves it into a destination file, then destroys the segment |

---

## 4. Ubuntu Compilation & Execution Commands

```bash
# Compile all programs
gcc -Wall 01_shared_memory_string.c -o 01_shared_memory_string
gcc -Wall 02_file_to_shared_memory.c -o 02_file_to_shared_memory
gcc -Wall 03_shared_memory_to_file.c -o 03_shared_memory_to_file

# Experiment 1: String Sharing
./01_shared_memory_string

# Experiment 2: File to Shared Memory and back
# Step A: Load file into shared memory
./02_file_to_shared_memory
# Step B: Read shared memory into output file
./03_shared_memory_to_file output_recovered.txt
cat output_recovered.txt
```

---

## 5. Sample Output

### `01_shared_memory_string.c`
```text
==========================================================
         SHARED MEMORY STRING SHARING DEMO                
==========================================================
Created Shared Memory Segment (shmid: 65536, size: 1024 bytes)
[Parent PID 20450] Wrote message into shared memory:
               "Greetings from Parent Process via System V Shared Memory!"
[Child PID 20451] Attached to shared memory.
[Child PID 20451] Read string from shared memory:
              "Greetings from Parent Process via System V Shared Memory!"
[Parent] Cleaned up and removed shared memory segment 65536.
==========================================================
```

---

## 6. Linux IPC Monitoring & Cleanup
- View active shared memory segments:
  ```bash
  ipcs -m
  ```
- Remove a stuck shared memory segment manually:
  ```bash
  ipcrm -m <shmid>
  ```
- Cleanup binaries:
  ```bash
  rm -f 01_shared_memory_string 02_file_to_shared_memory 03_shared_memory_to_file output_recovered.txt
  ```
