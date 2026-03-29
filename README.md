# Offline-First Edge Data Sync Engine (Embedded Linux)

## Problem Statement
This project simulates an IoT/Embedded Linux edge device that continues to store data locally when offline and synchronizes it with a central server once connectivity is restored.

In many real-world embedded systems, network connectivity may be unstable or unavailable. Instead of stopping operation, such systems must continue collecting and storing data locally and later synchronize it safely.

## Objectives
- Simulate an offline-first edge device
- Implement a multi-process Linux system
- Use IPC for communication between processes
- Use threads for concurrent synchronization tasks
- Use signals for lifecycle control
- Use Linux file I/O system calls for storage and logging

## Linux Concepts Used
- fork() for process creation
- pthread_create() for threading
- FIFO (named pipes) for IPC
- signal() for lifecycle control
- open(), read(), write(), close() for file operations
- lseek() for metadata/header updates

## Process Design
- Supervisor (Parent Process)
- Local Node (Data generator)
- Sync Manager (Handles sync)
- Central Server (Stores synced data)
- Logger (Logs all events)

## Thread Design
The Sync Manager uses:
- Network Monitor Thread
- Sync Worker Thread

## IPC Design
Named Pipes (FIFO) are used for inter-process communication.

### IPC Mapping
- Local Node → Sync Manager
- Sync Manager → Central Server
- Central Server → Sync Manager (ACK)
- All Processes → Logger

## Signal Handling
- SIGINT → graceful shutdown
- SIGUSR1 → trigger immediate synchronization
- SIGTERM → terminate child processes cleanly

## File I/O and Data Management
Files used:
- data/local_data.txt
- data/central_data.txt
- data/meta.txt
- logs/system.log

## Use of lseek()
The project uses lseek() to update metadata such as total records and last synced record without rewriting the full file.

## Data Flow Diagram

[Generate Record]
      |
      v
[Local Node]
      |
      v
[Store Locally]
      |
      v
[Sync Manager Checks Pending Records]
      |
      v
[Send to Central Server]
      |
      v
[ACK Received?] ---No---> [Retry Later]
      |
     Yes
      |
      v
[Mark as Synced]
      |
      v
[Logger Stores Event]

## Architecture Diagram

                +------------------+
                |    Supervisor    |
                +--------+---------+
                         |
    ---------------------------------------------------
    |                  |                 |            |
    v                  v                 v            v
+-----------+   +--------------+   +-------------+  +---------+
| Local Node|   | Sync Manager |   |CentralServer|  | Logger  |
+-----------+   +--------------+   +-------------+  +---------+
     |                 |                 |              |
     v                 v                 v              v
 local_data.txt   pending queue     central_data.txt  system.log

## Build Instructions
make

## Run Instructions
./supervisor

## Sample Output
[Supervisor] Starting all modules...
[Local Node] Stored record ID 101 locally
[Sync Manager] Pending record ID 101 found
[Sync Manager] Sending record ID 101 to Central Server
[Central Server] Record ID 101 stored successfully
[Central Server] ACK sent for record ID 101
[Sync Manager] Record ID 101 marked as SYNCED
[Logger] Event logged

## Future Scope
- Retry backoff mechanism
- Conflict resolution
- Socket-based remote synchronization
- Better fault tolerance and crash recovery

## Author
Nisha Pawde
