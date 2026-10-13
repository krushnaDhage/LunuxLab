# Topic 11: Socket Programming (Java Only)

## 1. Topic Overview & Purpose
Network socket programming allows interprocess communication across local boundaries and computer networks.
As required by the assignment guidelines, all socket practicals are implemented **exclusively in Java** using standard JDK networking APIs (`java.net.*` and `java.io.*`). No external frameworks (like Spring Boot) or build tools (like Maven/Gradle) are used.

This topic covers four practical paradigms:
1. **Iterative TCP**: Connection-oriented, handles one client at a time.
2. **Concurrent TCP**: Connection-oriented, handles multiple clients concurrently via multi-threading.
3. **Iterative UDP**: Connectionless datagram exchange, sequential packet handling.
4. **Concurrent UDP**: Connectionless datagram exchange, parallel worker threads with defensive buffer copying.

---

## 2. Key Java Networking Classes & Concepts
- `ServerSocket`: TCP server class that listens on a port and accepts incoming stream connections via `accept()`.
- `Socket`: TCP client/communication endpoint providing input and output streams.
- `DatagramSocket`: UDP endpoint used to send and receive independent packet datagrams.
- `DatagramPacket`: Self-contained message packet containing payload bytes, length, destination IP address, and port.
- **Iterative vs Concurrent**:
  - *Iterative*: The server handles one client connection/request to completion before accepting the next.
  - *Concurrent*: The main thread accepts requests and hands each one off to a worker `Thread`, enabling parallel processing.

---

## 3. Subfolder Index & Filenames

```text
11_socket_programming/
├── 01_tcp_iterative/
│   ├── TCPServer.java   (Port: 5000)
│   └── TCPClient.java
├── 02_tcp_concurrent/
│   ├── TCPServer.java   (Port: 5001)
│   └── TCPClient.java
├── 03_udp_iterative/
│   ├── UDPServer.java   (Port: 6000)
│   └── UDPClient.java
└── 04_udp_concurrent/
    ├── UDPServer.java   (Port: 6001)
    └── UDPClient.java
```

---

## 4. Ubuntu Compilation & Execution Commands

### Prerequisites on Ubuntu:
```bash
sudo apt update
sudo apt install default-jdk -y
java -version
```

### A. Iterative TCP (`01_tcp_iterative/`)
```bash
cd 01_tcp_iterative
javac TCPServer.java TCPClient.java

# Terminal 1 (Start Server first):
java TCPServer

# Terminal 2 (Run Client):
java TCPClient
```

### B. Concurrent TCP (`02_tcp_concurrent/`)
```bash
cd 02_tcp_concurrent
javac TCPServer.java TCPClient.java

# Terminal 1 (Start Server):
java TCPServer

# Terminal 2 (Client 1):
java TCPClient

# Terminal 3 (Client 2 - concurrently connected):
java TCPClient
```

### C. Iterative UDP (`03_udp_iterative/`)
```bash
cd 03_udp_iterative
javac UDPServer.java UDPClient.java

# Terminal 1 (Start UDP Server):
java UDPServer

# Terminal 2 (Send Datagram):
java UDPClient
```

### D. Concurrent UDP (`04_udp_concurrent/`)
```bash
cd 04_udp_concurrent
javac UDPServer.java UDPClient.java

# Terminal 1 (Start Server):
java UDPServer

# Terminal 2 (Client 1):
java UDPClient

# Terminal 3 (Client 2):
java UDPClient
```

---

## 5. Running Across Two Separate Machines
To communicate between two distinct machines over LAN/Wi-Fi:
1. Find the server machine's IP address on Ubuntu:
   ```bash
   ip a | grep inet
   # e.g., 192.168.1.150
   ```
2. Ensure the firewall allows the port:
   ```bash
   sudo ufw allow 5000/tcp
   sudo ufw allow 5001/tcp
   sudo ufw allow 6000/udp
   sudo ufw allow 6001/udp
   ```
3. Run the client on the second machine passing the server IP:
   ```bash
   java TCPClient 192.168.1.150 5001
   java UDPClient 192.168.1.150 6001
   ```

---

## 6. Sample Output

### Concurrent TCP (`02_tcp_concurrent`):
Server output:
```text
=== Concurrent TCP Server Started on Port 5001 ===
[Server] Accepted new client: /127.0.0.1:48210
[Handler-14 | /127.0.0.1:48210] Received: "Hello from Client 1"
[Server] Accepted new client: /127.0.0.1:48214
[Handler-15 | /127.0.0.1:48214] Received: "Hello from Client 2"
```

### Concurrent UDP (`04_udp_concurrent`):
Server output:
```text
=== Concurrent UDP Server Started on Port 6001 ===
[Server] Dispatched packet from /127.0.0.1:51234 to worker thread.
[Worker-16] Processing: "UDP message from terminal A" from /127.0.0.1:51234
[Worker-16] Sent response to /127.0.0.1:51234
```

---

## 7. Important Technical Notes
- **Defensive Packet Copying in Concurrent UDP**: Unlike TCP streams where each socket connection has its own stream, UDP datagrams are received on a single socket into a shared byte buffer. If worker threads share the receive buffer directly, the next incoming packet would overwrite earlier data. In `04_udp_concurrent/UDPServer.java`, `Arrays.copyOf()` creates an isolated copy of the payload for each worker thread, eliminating data races.
- **Port Reuse**: Always start the Server before starting Clients. If a server terminates abruptly, wait a few seconds or use `kill -9 $(lsof -t -i:PORT)` if the port remains in `TIME_WAIT`.

---

## 8. Cleanup
```bash
find . -name "*.class" -type f -delete
```
