import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

/**
 * UDPClient - Concurrent UDP Client
 * Sends multiple datagrams or interactive inputs to concurrent UDPServer on port 6001.
 */
public class UDPClient {
    private static final String SERVER_HOST = "127.0.0.1";
    private static final int SERVER_PORT = 6001;
    private static final int BUFFER_SIZE = 1024;

    public static void main(String[] args) {
        String host = (args.length > 0) ? args[0] : SERVER_HOST;
        int port = (args.length > 1) ? Integer.parseInt(args[1]) : SERVER_PORT;

        System.out.println("=== Concurrent UDP Client ===");
        System.out.println("Target: " + host + ":" + port);

        try (DatagramSocket socket = new DatagramSocket();
             BufferedReader consoleReader = new BufferedReader(new InputStreamReader(System.in))) {

            // Set socket timeout so client doesn't hang forever if packet drops
            socket.setSoTimeout(3000);

            InetAddress serverAddress = InetAddress.getByName(host);

            System.out.println("Type messages to send (Type 'exit' to quit):\n");

            while (true) {
                System.out.print("Client > ");
                String message = consoleReader.readLine();
                if (message == null || message.equalsIgnoreCase("exit")) {
                    break;
                }

                byte[] sendData = message.getBytes();
                DatagramPacket sendPacket = new DatagramPacket(
                    sendData, sendData.length, serverAddress, port
                );
                socket.send(sendPacket);

                // Receive response
                try {
                    byte[] receiveData = new byte[BUFFER_SIZE];
                    DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
                    socket.receive(receivePacket);

                    String response = new String(receivePacket.getData(), 0, receivePacket.getLength());
                    System.out.println("Server > " + response);
                } catch (Exception timeoutEx) {
                    System.out.println("[Timeout] No response received within 3 seconds.");
                }
            }

        } catch (Exception e) {
            System.err.println("[Client] UDP error: " + e.getMessage());
        }

        System.out.println("=== Client Exited ===");
    }
}
