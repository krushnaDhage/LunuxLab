import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

/**
 * UDPServer - Iterative UDP Server
 * Uses connectionless DatagramSocket and DatagramPacket.
 * Processes requests sequentially one packet at a time.
 */
public class UDPServer {
    private static final int PORT = 6000;
    private static final int BUFFER_SIZE = 1024;

    public static void main(String[] args) {
        System.out.println("=== Iterative UDP Server Listening on Port " + PORT + " ===");

        try (DatagramSocket socket = new DatagramSocket(PORT)) {
            byte[] receiveBuffer = new byte[BUFFER_SIZE];

            while (true) {
                // Prepare packet to receive data
                DatagramPacket receivePacket = new DatagramPacket(receiveBuffer, receiveBuffer.length);
                System.out.println("\n[Server] Waiting for UDP datagram...");

                socket.receive(receivePacket); // Blocks until a packet arrives

                String message = new String(
                    receivePacket.getData(), 0, receivePacket.getLength()
                ).trim();

                InetAddress clientAddress = receivePacket.getAddress();
                int clientPort = receivePacket.getPort();

                System.out.println("[Server] Received from " + clientAddress + ":" + clientPort + 
                                   " -> \"" + message + "\"");

                // Prepare and send response datagram
                String replyText = "UDP-Echo: " + message.toUpperCase();
                byte[] replyData = replyText.getBytes();

                DatagramPacket replyPacket = new DatagramPacket(
                    replyData, replyData.length, clientAddress, clientPort
                );

                socket.send(replyPacket);
                System.out.println("[Server] Sent response to " + clientAddress + ":" + clientPort);
            }
        } catch (Exception e) {
            System.err.println("[Server] UDP Server error: " + e.getMessage());
        }
    }
}
