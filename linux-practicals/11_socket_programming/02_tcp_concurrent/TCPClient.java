import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

/**
 * TCPClient - Concurrent TCP Client
 * Connects to concurrent TCPServer on port 5001.
 * Supports interactive conversation (sending multiple lines) until 'exit'.
 */
public class TCPClient {
    private static final String SERVER_HOST = "127.0.0.1";
    private static final int SERVER_PORT = 5001;

    public static void main(String[] args) {
        String host = (args.length > 0) ? args[0] : SERVER_HOST;
        int port = (args.length > 1) ? Integer.parseInt(args[1]) : SERVER_PORT;

        System.out.println("=== Concurrent TCP Client ===");
        System.out.println("Connecting to " + host + ":" + port + "...");

        try (Socket socket = new Socket(host, port);
             BufferedReader consoleReader = new BufferedReader(new InputStreamReader(System.in));
             BufferedReader socketReader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
             PrintWriter socketWriter = new PrintWriter(socket.getOutputStream(), true)) {

            System.out.println("Connected to Concurrent Server!");
            System.out.println("Type messages to send (Type 'exit' to quit):\n");

            String line;
            while (true) {
                System.out.print("Client > ");
                line = consoleReader.readLine();
                if (line == null) break;

                // Send line to server
                socketWriter.println(line);

                // Receive response
                String response = socketReader.readLine();
                System.out.println("Server > " + response);

                if (line.equalsIgnoreCase("exit") || line.equalsIgnoreCase("quit")) {
                    break;
                }
            }
        } catch (Exception e) {
            System.err.println("[Client] Error: " + e.getMessage());
        }

        System.out.println("=== Client Terminated ===");
    }
}
