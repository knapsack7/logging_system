# Multithreaded Logging System

This project implements a multithreaded logging system in C++. The logger handles log messages from multiple threads and ensures that they are logged in a thread-safe manner.

## Features

- Thread-safe logging: Uses mutexes and condition variables to manage concurrent access to log messages.
- Supports multiple threads logging simultaneously.
- Simple and efficient logging to the console.

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

## Files Included

- `main.cpp`: Contains the implementation of the logger and the main function that demonstrates its usage.
- `Makefile`: Used for compiling the project.

## How to Build and Run

### Prerequisites

Ensure you have a C++ compiler (like g++) installed on your machine. This project uses C++17 standards.

### Build the Project

1. Open a terminal.
2. Navigate to the project directory.
3. Run the following command to compile the project:

   ```bash
   make
