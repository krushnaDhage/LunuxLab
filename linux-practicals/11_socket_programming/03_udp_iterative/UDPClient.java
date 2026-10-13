import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

/**
 * UDPClient - Iterative UDP Client
 * Sends a message via DatagramPacket to UDPServer at port 6000,
 * awaits the response packet, and exits.
 */
public class UDPClient {
    private static final String SERVER_HOST = "127.0.0.1";
    private static final int SERVER_PORT = 6000;
    private static final int BUFFER_SIZE = 1024;

    public static void main(String[] args) {
        String host = (args.length > 0) ? args[0] : SERVER_HOST;
        int port = (args.length > 1) ? Integer.parseInt(args[1]) : SERVER_PORT;

        System.out.println("=== Iterative UDP Client ===");

        try (DatagramSocket socket = new DatagramSocket();
             BufferedReader consoleReader = new BufferedReader(new InputStreamReader(System.in))) {

            System.out.print("Enter message to send over UDP: ");
            String message = consoleReader.readLine();
            if (message == null || message.trim().isEmpty()) {
                message = "Hello UDP Server";
            }

            InetAddress serverAddress = InetAddress.getByName(host);
            byte[] sendData = message.getBytes();

            // Construct and send datagram packet
            DatagramPacket sendPacket = new DatagramPacket(
                sendData, sendData.length, serverAddress, port
            );
            socket.send(sendPacket);
            System.out.println("[Client] Sent UDP packet to " + host + ":" + port);

            // Receive response
            byte[] receiveData = new byte[BUFFER_SIZE];
            DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
            socket.receive(receivePacket);

            String reply = new String(receivePacket.getData(), 0, receivePacket.getLength());
            System.out.println("[Client] Server response: \"" + reply + "\"");

        } catch (Exception e) {
            System.err.println("[Client] UDP error: " + e.getMessage());
        }

        System.out.println("=== Client Terminated ===");
    }
}
