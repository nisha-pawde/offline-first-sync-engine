# Offline-First Data Synchronization Engine

## 🚀 Overview

This project implements an offline-first data synchronization system using Linux system calls and IPC mechanisms.

---

## 🎯 Objectives

- Demonstrate Linux system calls
- Implement IPC mechanisms
- Ensure reliable synchronization using ACK

---

## 🧠 Architecture Diagram

```mermaid
graph TD

A[Supervisor] --> B[Local Node]
A --> C[Sync Manager]
A --> D[Central Server]
A --> E[Logger]

B -->|Shared Memory| C
C -->|Message Queue (DATA)| D
D -->|Message Queue (ACK)| C
C -->|Pipe| D

B -->|FIFO| E
C -->|FIFO| E
D -->|FIFO| E

B --> F[local_data.txt]
D --> G[central_data.txt]
E --> H[system.log]
