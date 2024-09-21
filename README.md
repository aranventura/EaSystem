# EäSystem

# EäSystem

This project aims to implement a distributed system named Eä System, inspired by a fictional universe. It involves creating a communication system between different processes located across various physical machines using Linux operating systems and TCP/IP networking. The implementation is divided into four primary phases, with each phase incrementally adding more functionality to the system.

![image](https://github.com/user-attachments/assets/6a9d170b-ef3b-4e90-897a-1c6ba1159833)

Phase 1: Starting the Sons of Iluvatar
In this phase, the main objective is to create the initial process, named IluvatarSon. This process will read a configuration file provided as a parameter and perform the following tasks:
- Configuration Parsing: The process will read the configuration file to obtain details like the username, directory paths, and server IP and ports.
- Command Line Interface: IluvatarSon will operate in command-line mode and support extended commands, such as:
UPDATE USERS: Updates the list of connected users from the Arda server.
LIST USERS: Displays the current list of users connected to the Arda server.
SEND MSG <user> “<text>”: Sends a text message to another user.
SEND FILE <user> <file>: Sends a file to another user.
EXIT: Disconnects from the server and terminates the process.
In addition, IluvatarSon should support all standard Linux commands available in the system. The command interface must be stable and properly handle user input and errors, for example, if Arda is not up, the main process of Iluvatar should shut down.

![image](https://github.com/user-attachments/assets/541b85ad-835f-47f8-8b7f-3a04d378c521)


Phase 2: Connecting Arda
In the second phase, the focus is on establishing the initial connectivity between the IluvatarSon processes and the central server named Arda. The implementation involves:
- Server Initialization: The Arda server starts and reads its configuration to initialize its network settings.
- Persistent Connections: IluvatarSon processes will connect to the Arda server upon starting and maintain the connection throughout their lifecycle.
- User Management: The Arda server will maintain a list of all connected IluvatarSon processes. This list can be updated or queried using the appropriate commands (UPDATE LIST, LIST USERS).
- Error Handling: The system should gracefully handle connection drops and other errors by notifying connected processes and cleaning up resources.

![image](https://github.com/user-attachments/assets/f4cd728d-6dc1-4f7a-a4fe-486fdab7364f)

Phase 3: We Love Sockets
This extends the functionality to enable communication between IluvatarSon processes located on different physical machines:
- Inter-Machine Communication: Implement message and file transfer between IluvatarSon processes running on separate machines using sockets.
- Protocol Compliance: The communication must adhere to the predefined protocol structure for message and file transfer.
- Verification: File transfers should be verified using MD5 checksums to ensure integrity.

![image](https://github.com/user-attachments/assets/e2a97b2c-e498-4c56-a8c6-d9a35e1b15f3)

Phase 4: Friendly Neighbors
The final mandatory phase focuses on optimizing communication between IluvatarSon processes running on the same physical machine:
- Intra-Machine Communication: Implement communication between IluvatarSon processes on the same machine without using sockets.
- Custom Communication Method: Design and use an efficient inter-process communication mechanism (e.g., shared memory, pipes) as an alternative to sockets.
- Protocol Adjustments: Ensure compatibility with the overall communication protocol, adapting it as necessary for local communication.

![image](https://github.com/user-attachments/assets/73a04354-c693-4374-b07e-ee091d9020dd)

