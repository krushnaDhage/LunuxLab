import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

/**
 * TCPClient - Iterative TCP Client
 * Connects to TCPServer on localhost:5000, sends a message,
 * prints the response, and disconnects.
 */
public class TCPClient {
    private static final String SERVER_HOST = "127.0.0.1";
    private static final int SERVER_PORT = 5000;

    public static void main(String[] args) {
        String host = (args.length > 0) ? args[0] : SERVER_HOST;
        int port = (args.length > 1) ? Integer.parseInt(args[1]) : SERVER_PORT;

        System.out.println("=== Iterative TCP Client ===");
        System.out.println("Connecting to server at " + host + ":" + port + "...");

        try (Socket socket = new Socket(host, port);
             BufferedReader consoleReader = new BufferedReader(new InputStreamReader(System.in));
             BufferedReader socketReader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
             PrintWriter socketWriter = new PrintWriter(socket.getOutputStream(), true)) {

            System.out.print("Enter message to send to server: ");
            String message = consoleReader.readLine();
            if (message == null || message.trim().isEmpty()) {
                message = "Hello Iterative Server";
            }

            // Send message to server
            socketWriter.println(message);
            System.out.println("[Client] Message sent: \"" + message + "\"");

            // Read server response
            String response = socketReader.readLine();
            System.out.println("[Client] Server response: \"" + response + "\"");

        } catch (Exception e) {
            System.err.println("[Client] Connection error: " + e.getMessage());
        }

        System.out.println("=== Client Finished ===");
    }
}
