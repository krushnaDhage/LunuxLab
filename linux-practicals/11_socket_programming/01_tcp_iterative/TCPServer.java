import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

/**
 * TCPServer - Iterative TCP Server
 * Handles one client connection at a time. The next client must wait
 * until the current client disconnects.
 */
public class TCPServer {
    private static final int PORT = 5000;

    public static void main(String[] args) {
        System.out.println("=== Iterative TCP Server Started on Port " + PORT + " ===");

        try (ServerSocket serverSocket = new ServerSocket(PORT)) {
            while (true) {
                System.out.println("\n[Server] Waiting for an incoming connection...");
                
                // Blocks until a client connects
                try (Socket clientSocket = serverSocket.accept()) {
                    System.out.println("[Server] Accepted connection from: " + 
                                       clientSocket.getRemoteSocketAddress());

                    BufferedReader reader = new BufferedReader(
                        new InputStreamReader(clientSocket.getInputStream())
                    );
                    PrintWriter writer = new PrintWriter(
                        clientSocket.getOutputStream(), true
                    );

                    // Read client message
                    String clientMessage = reader.readLine();
                    System.out.println("[Server] Received: \"" + clientMessage + "\"");

                    // Send response back
                    String response = "Echo from Iterative Server: " + 
                                      (clientMessage != null ? clientMessage.toUpperCase() : "");
                    writer.println(response);
                    System.out.println("[Server] Sent response. Connection closing.");
                } catch (Exception e) {
                    System.err.println("[Server] Error handling client: " + e.getMessage());
                }
            }
        } catch (Exception e) {
            System.err.println("[Server] Server error: " + e.getMessage());
        }
    }
}
