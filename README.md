# Secure Capture 

This project allows clients to capture and transfer screenshots to a server. The server can handle normal screenshots. It also provides commands to control the server and client operations.

## Overview

- **Server**: Listens for incoming connections, processes screenshot requests, and handles normal or encrypted image transfers.
- **Client**: Connects to the server, sends screenshots or encrypted screenshots based on user input, and manages disconnection commands.

## Building the Project

Use the `Makefile` to build the server and client programs.

```bash
make server
make client
```

## Commands
### On the server
- QUIT: Terminates the server. Use this command to shut down the server.
- SHUTDOWN: Equivalent to QUIT.

### On the Client
- EXIT: Closes the client application.
- QUIT: Closes the client application.
- ss: Sends a normal screenshot to the server.
- securess: Sends an XOR-encrypted screenshot to the server. You will be prompted to enter an encryption key for this operation. Note that the server needs to decrypt the image in order to view it.
  
## How to use? 
1. Start the server
   ```./server```
2. Start the client
   ```./client```
   
The XOR encryption method used for securess is basic and should be improved for production use.
Ensure that both the server and client programs are properly configured and running before attempting to transfer screenshots.
