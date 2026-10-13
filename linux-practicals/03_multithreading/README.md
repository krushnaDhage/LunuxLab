# Topic 03: Multithreading with Pthreads

## 1. Topic Overview & Purpose
Threads are lightweight units of execution within a single process. Unlike processes created via `fork()`, all threads of a process share the same heap, global variables, and open file descriptors, but each thread has its own stack and program counter.
This topic covers POSIX Threads (pthreads) in Linux, data races, mutual exclusion with mutexes, and strict alternation using condition variables.

---

## 2. Key POSIX Thread APIs
- `pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg)`: Creates a new thread.
- `pthread_join(pthread_t thread, void **retval)`: Waits for the specified thread to terminate.
- `pthread_mutex_lock(pthread_mutex_t *mutex)` / `pthread_mutex_unlock(...)`: Acquires and releases a mutex lock.
- `pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)`: Atomically releases mutex and sleeps until signaled.
- `pthread_cond_signal(pthread_cond_t *cond)`: Wakes up at least one waiting thread.

---

## 3. Assignment Index & Filenames

| No. | Filename | Description |
|:---:|:---|:---|
| 1 | `01_odd_even_prime_threads.c` | Creates 3 threads to compute averages of odd, even, and prime numbers |
| 2 | `02_shared_loop_variable.c` | Classroom experiment passing `&i` to threads; demonstrates data race |
| 3 | `03_thread_parameters_counter.c` | Two threads incrementing unsynchronized shared counter (inconsistencies) |
| 4 | `04_mutex_counter.c` | Mutex-synchronized counter; explains atomicity vs ordering |
| 5 | `05_alternating_thread_file_write.c` | Strictly alternating writes to a file using mutex and condition variables |

---

## 4. Ubuntu Compilation & Execution Commands

> **Note**: Always link the POSIX thread library using `-pthread`.

```bash
gcc -Wall -pthread 01_odd_even_prime_threads.c -o 01_odd_even_prime_threads
gcc -Wall -pthread 02_shared_loop_variable.c -o 02_shared_loop_variable
gcc -Wall -pthread 03_thread_parameters_counter.c -o 03_thread_parameters_counter
gcc -Wall -pthread 04_mutex_counter.c -o 04_mutex_counter
gcc -Wall -pthread 05_alternating_thread_file_write.c -o 05_alternating_thread_file_write

# Run the programs:
./01_odd_even_prime_threads
./02_shared_loop_variable
./03_thread_parameters_counter
./04_mutex_counter
./05_alternating_thread_file_write
cat alternating_output.txt
```

---

## 5. Sample Output & Key Explanations

### `02_shared_loop_variable.c`
```text
=== Passing Address of Loop Variable (&i) ===
Thread ID: 139824855799552, Value of i received = 5
Thread ID: 139824847406848, Value of i received = 5
Thread ID: 139824839014144, Value of i received = 5
Thread ID: 139824830621440, Value of i received = 5
Thread ID: 139824822228736, Value of i received = 5
```
*Why this happens*: Passing `&i` gives each thread a pointer to the same variable on `main`'s stack. The loop finishes incrementing `i` to 5 before child threads execute their print statement.

### `04_mutex_counter.c` vs `05_alternating_thread_file_write.c`
- **Does Mutex alone guarantee alternating order?** NO. A mutex only ensures mutual exclusion (preventing race conditions so the final counter is accurate). The OS scheduler decides which thread acquires the mutex next.
- **How to guarantee strict alternation?** Use a condition variable (`pthread_cond_t`) with a state flag (`turn`) as demonstrated in `05_alternating_thread_file_write.c`.

---

## 6. Cleanup
```bash
rm -f 01_odd_even_prime_threads 02_shared_loop_variable \
      03_thread_parameters_counter 04_mutex_counter \
      05_alternating_thread_file_write alternating_output.txt
```
