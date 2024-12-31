# Multithreaded Logging System

## Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [Requirements](#requirements)
- [How It Works](#how-it-works)
- [Setup and Usage](#setup-and-usage)
- [Project Structure](#project-structure)
- [Example Output](#example-output)
- [License](#license)

---

## Introduction
This is a **Multithreaded Logging System** implemented in C++. The system efficiently handles logging in multi-threaded environments by ensuring thread safety and high performance using a dedicated logging thread and a thread-safe queue.

---

## Features
- Supports concurrent logging from multiple threads.
- Uses a thread-safe queue for handling log messages.
- Writes logs asynchronously to avoid blocking the main application.
- Adjustable log levels: `INFO`, `DEBUG`, `WARN`, `ERROR`.
- Logs are written to a file or console.

## Design Patterns Used

1. **Singleton Design Pattern**:
   - Ensures only one instance of the Logger exists throughout the application.

2. **Thread-Safe Logging**:
   - A mutex protects access to the log queue.
   - The worker thread processes logs from the queue and writes them to the file.

3. **Producer-Consumer Model**:
   - Main threads (producers) add log messages to the queue.
   - The worker thread (consumer) processes messages from the queue.

4. **Graceful Shutdown**:
   - The `stop()` method ensures all logs are processed before shutting down.

---

## Requirements
- **Operating System**: macOS/Linux/Windows
- **Compiler**: C++17 or later
- **Build Tools**: Make, CMake, or any equivalent tool.
- **Dependencies**: Standard Template Library (STL).

---

## How It Works
1. Multiple threads send log messages to a shared thread-safe queue.
2. A dedicated logging thread retrieves messages from the queue.
3. The logger writes messages to a log file or the console.
4. Ensures thread safety using mutex locks and condition variables.

---

## Setup and Usage

### 1. Clone the Repository
```bash
git clone https://github.com/username/multithreaded-logging-system.git
cd multithreaded-logging-system
