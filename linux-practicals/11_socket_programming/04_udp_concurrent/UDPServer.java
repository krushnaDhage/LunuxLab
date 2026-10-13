import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.Arrays;

/**
 * UDPServer - Concurrent UDP Server
 * Receives incoming datagrams in a continuous loop and delegates each request
 * to a dedicated worker thread.
 *
 * Concurrency Safety Rule:
 * The server copies the byte buffer and sender details (address & port)
 * before dispatching to a worker thread to prevent data races and race conditions
 * on the shared receive buffer.
 */
public class UDPServer {
    private static final int PORT = 6001;
    private static final int BUFFER_SIZE = 1024;

    public static void main(String[] args) {
        System.out.println("=== Concurrent UDP Server Started on Port " + PORT + " ===");

        try (DatagramSocket socket = new DatagramSocket(PORT)) {
            while (true) {
                byte[] buffer = new byte[BUFFER_SIZE];
                DatagramPacket packet = new DatagramPacket(buffer, buffer.length);

                // Receive incoming datagram
                socket.receive(packet);

                // Defensive copy of payload and sender metadata to avoid buffer races
                byte[] payload = Arrays.copyOf(packet.getData(), packet.getLength());
                InetAddress clientAddress = packet.getAddress();
                int clientPort = packet.getPort();

                System.out.println("[Server] Dispatched packet from " + clientAddress + 
                                   ":" + clientPort + " to worker thread.");

                // Spawn worker thread for concurrent request processing
                new Thread(new PacketHandler(socket, payload, clientAddress, clientPort)).start();
            }
        } catch (Exception e) {
            System.err.println("[Server] Fatal UDP error: " + e.getMessage());
        }
    }

    private static class PacketHandler implements Runnable {
        private final DatagramSocket socket;
        private final byte[] data;
        private final InetAddress clientAddress;
        private final int clientPort;

        public PacketHandler(DatagramSocket socket, byte[] data, 
                             InetAddress clientAddress, int clientPort) {
            this.socket = socket;
            this.data = data;
            this.clientAddress = clientAddress;
            this.clientPort = clientPort;
        }

        @Override
        public void run() {
            try {
                String request = new String(data).trim();
                System.out.println("[Worker-" + Thread.currentThread().getId() + 
                                   "] Processing: \"" + request + "\" from " + 
                                   clientAddress + ":" + clientPort);

                // Simulate processing time
                Thread.sleep(500);

                String responseText = "Concurrent-UDP-Ack: Processed [" + request + 
                                      "] at " + System.currentTimeMillis();
                byte[] responseData = responseText.getBytes();

                DatagramPacket responsePacket = new DatagramPacket(
                    responseData, responseData.length, clientAddress, clientPort
                );

                // DatagramSocket.send() is thread-safe in Java
                socket.send(responsePacket);

                System.out.println("[Worker-" + Thread.currentThread().getId() + 
                                   "] Sent response to " + clientAddress + ":" + clientPort);
            } catch (Exception e) {
                System.err.println("[Worker] Error processing packet: " + e.getMessage());
            }
        }
    }
}
