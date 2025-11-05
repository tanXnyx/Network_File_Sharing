# C++ TCP Client-Server Chat using Docker Compose

This project sets up a basic TCP client-server chat application where both components are compiled and run inside separate **Linux containers** using the **POSIX Socket API** (`sys/socket.h`, `unistd.h`, etc.).

The client connects to the server using the service name `"server"` which is automatically resolved by the Docker network.

## 🚀 Prerequisites

1.  **Docker:** Docker Desktop (or Docker Engine) must be installed and running on your system.
2.  **Docker Compose:** Docker Compose V2 is typically included with Docker Desktop.
3.  **Project Structure:** Ensure the following files are in your project directory:
    * `server.cpp` (POSIX Server Code)
    * `client.cpp` (POSIX Client Code)
    * `Dockerfile.server`
    * `Dockerfile.client`
    * `docker-compose.yml`

---

## 🛠️ Execution Steps

Follow these steps in your **VS Code Integrated Terminal** (or any command line with Docker access) to build and run the application.

### Step 1: Build and Run the Services

The `docker-compose.yml` file defines the `server` and `client` services, sets up a shared bridge network (`socket-net`), and ensures the client starts only after the server.

Execute the following command to build the images and start the containers in detached mode (`-d`):

```bash
docker-compose up --build -d
Step 2: Verify the Server and Client Connection
Check the logs for the initial setup.

Bash

docker-compose logs
You should see output similar to this, confirming a successful connection:

server_1 | Server listening on port 8080...
server_1 | Client connected!
client_1 | Connected to server!
Step 3: Initiate Communication (Chat)
Since the client is an interactive application that requires user input, you must attach to its console to start chatting.

Find the running client container name:

Bash

docker-compose ps
(Look for the container name usually formatted as [project_folder_name]-client-1.)

Attach to the Client's console: Replace [CLIENT_NAME] with the actual name you found.

Bash

docker attach [CLIENT_NAME]
Start Chatting: The client will prompt you. Type a message and press Enter:

Enter message to send to server (or 'EXIT' to quit): Hello Server from Docker Client!
Step 4: Interact with the Server
To see the server's side of the conversation or to reply, you can monitor the server's output or attach to the server's console.

Monitor Server Logs (Recommended for receiving): Open a NEW Terminal window and monitor the server's logs:

Bash

docker-compose logs -f server
The server terminal will show:

Received from client: Hello Server from Docker Client!
Enter message to send to client (or 'EXIT' to quit): 
Send a Reply from Server: Type your reply into the server's log terminal and press Enter. The message will be echoed back to the client console.

Step 5: Clean Up
To stop and remove all services, networks, and volumes related to this project:

Bash

docker-compose down
