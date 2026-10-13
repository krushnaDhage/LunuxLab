# Topic 06: IPC: Message Queues

## 1. Topic Overview & Purpose
System V Message Queues provide a kernel-managed linked list of messages. Unlike byte streams (pipes/FIFOs), message queues transmit discrete, structured messages with associated message types (`mtype`), allowing processes to filter and selectively retrieve messages in arbitrary or priority order.
This topic implements a private two-way conversation between two processes using a single System V message queue.

---

## 2. Key System Calls & Structures
- `key_t ftok(const char *pathname, int proj_id)`: Generates a System V IPC key from a path and project identifier.
- `int msgget(key_t key, int msgflg)`: Obtains or creates a message queue ID.
- `int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg)`: Enqueues a message.
- `ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg)`: Dequeues a message of type `msgtyp`.
- `int msgctl(int msqid, int cmd, struct msqid_ds *buf)`: Control operations, including `IPC_RMID` to destroy the queue.
- **Message Structure**:
  ```c
  struct msgbuf {
      long mtype;       /* message type, must be > 0 */
      char mtext[128];  /* message data */
  };
  ```

---

## 3. Assignment Index & Filenames

| No. | Filename | Description |
|:---:|:---|:---|
| 1 | `01_sender.c` | Process 1: Creates queue, sends "Are you hearing me?", receives reply, sends "I can hear you too", removes queue |
| 2 | `02_receiver.c` | Process 2: Connects to queue, receives first message, sends "Loud and Clear", receives final message |

---

## 4. Ubuntu Compilation & Execution Commands

```bash
# Compile both programs
gcc -Wall 01_sender.c -o 01_sender
gcc -Wall 02_receiver.c -o 02_receiver

# Execution across two terminals:
# Step 1: In Terminal 1, start Process 1 (Sender):
./01_sender

# Step 2: In Terminal 2, start Process 2 (Receiver):
./02_receiver
```

---

## 5. Sample Output

### Terminal 1 (`01_sender`):
```text
==========================================================
     PROCESS 1 (SENDER) - MESSAGE QUEUE (ID: 32768)       
==========================================================
[Process 1] Sending message: "Are you hearing me?"
[Process 1] Waiting for reply from Process 2...
[Process 1] Received reply: "Loud and Clear"
[Process 1] Sending final message: "I can hear you too"

[Process 1] Conversation sequence complete.
[Process 1] Sleeping 2 seconds to allow Process 2 to receive final message...
[Process 1] Message queue removed from system.
```

### Terminal 2 (`02_receiver`):
```text
==========================================================
     PROCESS 2 (RECEIVER) - MESSAGE QUEUE (ID: 32768)     
==========================================================
[Process 2] Waiting for message from Process 1...
[Process 2] Received message: "Are you hearing me?"
[Process 2] Replying with: "Loud and Clear"
[Process 2] Waiting for final confirmation from Process 1...
[Process 2] Received final message: "I can hear you too"

[Process 2] Conversation completed successfully.
```

---

## 6. Important Notes & IPC Cleanup
- **Selective Retrieval by `mtype`**:
  - `mtype = 1`: Used for messages directed from Process 1 to Process 2.
  - `mtype = 2`: Used for the reply directed from Process 2 to Process 1.
  This prevents a process from accidentally reading back its own message.
- **Checking Active Queues in Linux**:
  ```bash
  ipcs -q
  ```
- **Manually Removing Stuck Queues**:
  ```bash
  ipcrm -q <msqid>
  ```
- **Cleanup Executables**:
  ```bash
  rm -f 01_sender 02_receiver
  ```
