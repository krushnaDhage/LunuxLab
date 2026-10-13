# Topic 10: IPC: Semaphores & Producer-Consumer

## 1. Topic Overview & Purpose
A semaphore is a synchronization primitive consisting of an integer counter and two atomic operations:
- **P operation (Wait / Proberen)**: Decrements the semaphore value. If the value is 0, the calling process blocks until it becomes greater than 0.
- **V operation (Signal / Verhogen)**: Increments the semaphore value, waking up any blocked processes.

This module provides separate System V programs for initializing a semaphore, running P operations, and running V operations on the same shared semaphore set. It also demonstrates solving the bounded-buffer Producer-Consumer problem using semaphores.

---

## 2. Key System Calls & APIs
- **System V Semaphores**:
  - `int semget(key_t key, int nsems, int semflg)`: Obtains or creates a semaphore set.
  - `int semctl(int semid, int semnum, int cmd, ...)`: Controls semaphore values (e.g. `SETVAL`, `IPC_RMID`).
  - `int semop(int semid, struct sembuf *sops, size_t nsops)`: Performs atomic operations (`sem_op = -1` for P, `+1` for V).
- **POSIX Semaphores**:
  - `sem_init()`, `sem_wait()`, `sem_post()`, `sem_destroy()`.

---

## 3. Assignment Index & Filenames

| No. | Filename | Description |
|:---:|:---|:---|
| 1 | `01_semaphore_initialize.c` | Initializes a System V semaphore with an initial value and displays its ID |
| 2 | `02_semaphore_p_operation.c` | Performs the P (Wait / Decrement) operation on the shared semaphore |
| 3 | `03_semaphore_v_operation.c` | Performs the V (Signal / Increment) operation on the shared semaphore |
| 4 | `04_producer_consumer.c` | Implements the classic bounded-buffer Producer-Consumer problem |

---

## 4. Ubuntu Compilation & Execution Commands

```bash
# Compile System V programs
gcc -Wall 01_semaphore_initialize.c -o 01_semaphore_initialize
gcc -Wall 02_semaphore_p_operation.c -o 02_semaphore_p_operation
gcc -Wall 03_semaphore_v_operation.c -o 03_semaphore_v_operation

# Compile Producer-Consumer (requires -pthread)
gcc -Wall -pthread 04_producer_consumer.c -o 04_producer_consumer

# Demonstration of System V Semaphore (Step-by-step):
# Step 1: Initialize semaphore to 1
./01_semaphore_initialize 1

# Step 2: In Terminal 1, run P operation (acquires lock, sem becomes 0):
./02_semaphore_p_operation

# Step 3: Run P operation again in Terminal 2 (it will block because sem is 0):
./02_semaphore_p_operation

# Step 4: In Terminal 3, run V operation (increments sem, unblocking Terminal 2):
./03_semaphore_v_operation

# Run Producer-Consumer problem:
./04_producer_consumer
```

---

## 5. Sample Output

### `04_producer_consumer.c`
```text
==========================================================
         PRODUCER-CONSUMER PROBLEM (SEMAPHORES)           
==========================================================
Buffer Size: 5 slots | Total items to exchange: 10

[Producer] Produced Item  1 -> Placed at Slot 0
  [Consumer] Consumed Item  1 <- Retrieved from Slot 0
[Producer] Produced Item  2 -> Placed at Slot 1
[Producer] Produced Item  3 -> Placed at Slot 2
  [Consumer] Consumed Item  2 <- Retrieved from Slot 1
  [Consumer] Consumed Item  3 <- Retrieved from Slot 2
...
[Done] All 10 items produced and consumed safely!
==========================================================
```

---

## 6. Linux Semaphore Cleanup
- List active semaphores:
  ```bash
  ipcs -s
  ```
- Remove a semaphore manually:
  ```bash
  ipcrm -s <semid>
  ```
- Clean binaries:
  ```bash
  rm -f 01_semaphore_initialize 02_semaphore_p_operation \
        03_semaphore_v_operation 04_producer_consumer
  ```
