# C++ TCP Client-Server Chat using Docker Compose 💬

This project deploys an interactive TCP client-server chat application where both components are compiled and run inside isolated **Linux containers** using the **POSIX Socket API** (`sys/socket.h`, `unistd.h`, etc.).

The client connects to the server using the service name `"server"`, which is automatically resolved by the Docker network.

---

## 🚀 Prerequisites

Ensure you have the following installed and configured on your system:

1.  **Docker Desktop/Engine:** Installed and running.
2.  **Docker Compose:** Available (usually included with Docker Desktop).
3.  **Project Files:** All five required files must be in your working directory:
    * `server.cpp`
    * `client.cpp`
    * `Dockerfile.server`
    * `Dockerfile.client`
    * `docker-compose.yml`

---

## 🛠️ Step-by-Step Execution

All commands should be run from the project's root directory using your **VS Code Integrated Terminal** or command line.

### Step 1: Build and Launch Containers

This command builds the images for both services and starts them in the background (`-d`).

```bash
docker-compose up --build -d
Got it. You want the complete, consolidated README.md code, including the prerequisites, the full execution steps, and the necessary markdown formatting.Here is the entire README.md file:Markdown# C++ TCP Client-Server Chat using Docker Compose 💬

This project deploys an interactive TCP client-server chat application where both components are compiled and run inside isolated **Linux containers** using the **POSIX Socket API** (`sys/socket.h`, `unistd.h`, etc.).

The client connects to the server using the service name `"server"`, which is automatically resolved by the Docker network.

---

## 🚀 Prerequisites

Ensure you have the following installed and configured on your system:

1.  **Docker Desktop/Engine:** Installed and running.
2.  **Docker Compose:** Available (usually included with Docker Desktop).
3.  **Project Files:** All five required files must be in your working directory:
    * `server.cpp`
    * `client.cpp`
    * `Dockerfile.server`
    * `Dockerfile.client`
    * `docker-compose.yml`

---

## 🛠️ Step-by-Step Execution

All commands should be run from the project's root directory using your **VS Code Integrated Terminal** or command line.

### Step 1: Build and Launch Containers

This command builds the images for both services and starts them in the background (`-d`).

```bash
docker-compose up --build -d
Command PartPurposeupCreates and starts the defined services.--buildEnsures fresh images are built before starting.-dRuns containers in detached (background) mode.Step 2: Verify Server StatusCheck the server logs to confirm it has started and is listening on port 8080.Bashdocker-compose logs server
Expected Output:server_1 | Server listening on port 8080...
server_1 | Client connected!  <-- This confirms the client has successfully started and connected.
Step 3: Initiate Communication (Client Interaction)To send messages from the client, you must attach your terminal session directly to the running client container.Find the client container's full name:Bashdocker-compose ps
Attach to the client container: Replace [CLIENT_NAME] with the actual container name found in the previous step (e.g., myproject-client-1).Bashdocker attach [CLIENT_NAME]
Send Messages: The client prompt will appear. Type your messages and press Enter.Connected to server!
Enter message to send to server (or 'EXIT' to quit): Hello from Docker Client!
Step 4: View Server Output and ReplyOpen a NEW Terminal window.Monitor Server Logs (for replies): Monitor the server's output using the follow flag (-f).Bashdocker-compose logs -f server
Send a Reply: The server will prompt you. Type a message directly into this terminal (where the prompt is visible) and press Enter to send a reply back to the client.Received from client: Hello from Docker Client!
Enter message to send to client (or 'EXIT' to quit): I received your message!
Step 5: Clean UpWhen you are finished, stop and remove all services, networks, and volumes related to this project.Bashdocker-compose down
