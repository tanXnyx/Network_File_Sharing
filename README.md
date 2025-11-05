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
