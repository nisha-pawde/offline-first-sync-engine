# Offline-First Data Synchronization Engine

## 🚀 Overview

This project implements an offline-first data synchronization system using Linux system calls and IPC mechanisms.

---

## 🎯 Objectives

- Demonstrate Linux system calls
- Implement IPC mechanisms
- Ensure reliable synchronization using ACK

--

## 🧠 Architecture Diagram

```mermaid
graph LR
    A[Supervisor] --> B[Local Node]
    A --> C[Sync Manager]
    A --> D[Central Server]
    A --> E[Logger]
    B --> C
    C --> D
    D --> C
    B --> E
    C --> E
    D --> E
    B --> F[local_data.txt]
    D --> G[central_data.txt]
    E --> H[system.log]
