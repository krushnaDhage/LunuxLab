import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

/**
 * TCPServer - Concurrent Multi-Threaded TCP Server
 * Spawns an independent worker thread for every connected client,
 * enabling simultaneous handling of multiple clients.
 */
public class TCPServer {
    private static final int PORT = 5001;

    public static void main(String[] args) {
        System.out.println("=== Concurrent TCP Server Started on Port " + PORT + " ===");

        try (ServerSocket serverSocket = new ServerSocket(PORT)) {
            while (true) {
                // Accept incoming client connection
                Socket clientSocket = serverSocket.accept();
                System.out.println("[Server] Accepted new client: " + 
                                   clientSocket.getRemoteSocketAddress());

                // Spawn a new thread to handle this client concurrently
                new Thread(new ClientHandler(clientSocket)).start();
            }
        } catch (Exception e) {
            System.err.println("[Server] Server error: " + e.getMessage());
        }
    }

    // Worker thread for each client connection
    private static class ClientHandler implements Runnable {
        private final Socket socket;

        public ClientHandler(Socket socket) {
            this.socket = socket;
        }

        @Override
        public void run() {
            String clientInfo = socket.getRemoteSocketAddress().toString();
            try (Socket s = this.socket;
                 BufferedReader reader = new BufferedReader(new InputStreamReader(s.getInputStream()));
                 PrintWriter writer = new PrintWriter(s.getOutputStream(), true)) {

                String message;
                // Read messages until client disconnects
                while ((message = reader.readLine()) != null) {
                    if (message.equalsIgnoreCase("exit") || message.equalsIgnoreCase("quit")) {
                        writer.println("Goodbye!");
                        break;
                    }
                    System.out.println("[Handler-" + Thread.currentThread().getId() + 
                                       " | " + clientInfo + "] Received: \"" + message + "\"");

                    // Send response
                    String response = "Concurrent Server Reply to [" + message + "] at " + 
                                      System.currentTimeMillis();
                    writer.println(response);
                }
            } catch (Exception e) {
                System.err.println("[Handler] Error with client " + clientInfo + ": " + e.getMessage());
            } finally {
                System.out.println("[Server] Client disconnected: " + clientInfo);
            }
        }
    }
}
