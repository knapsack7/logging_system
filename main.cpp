#include "logger.h" // Include the Logger header for thread-safe logging
#include <thread>   // Include thread support for multithreading

// ---------------------------------------------------------------------------------------
// workerFunction: Function executed by each thread to simulate logging tasks
//
// Parameters:
// - threadId: Unique identifier for the thread calling this function
//
// Functionality:
// - Logs 10 messages with the INFO level using the Logger instance.
// - Demonstrates the thread-safe logging capabilities of the Logger class.
// ---------------------------------------------------------------------------------------
void workerFunction(int threadId) {
    Logger& logger = Logger::getInstance(); // Access the singleton Logger instance
    for (int i = 0; i < 10; ++i) {
        // Log a message with INFO level, uniquely identifying the thread and message count
        logger.log(INFO, "Logger Thread " + std::to_string(threadId) + 
                           " is logging message " + std::to_string(i));
    }
}

// ---------------------------------------------------------------------------------------
// main: Entry point of the program
//
// Functionality:
// - Starts the Logger system.
// - Creates three worker threads to simulate concurrent logging.
// - Waits for all threads to complete logging tasks.
// - Gracefully shuts down the Logger system.
// ---------------------------------------------------------------------------------------
int main() {
    Logger& logger = Logger::getInstance(); // Access the singleton Logger instance
    logger.start(); // Start the Logger's worker thread

    // -----------------------------------------------------------------------------------
    // 1. Create multiple threads for concurrent logging
    // -----------------------------------------------------------------------------------
    std::thread t1(workerFunction, 1); // Create thread 1 to log messages
    std::thread t2(workerFunction, 2); // Create thread 2 to log messages
    std::thread t3(workerFunction, 3); // Create thread 3 to log messages

    // -----------------------------------------------------------------------------------
    // 2. Wait for all threads to complete their tasks
    // -----------------------------------------------------------------------------------
    t1.join(); // Wait for thread 1 to finish
    t2.join(); // Wait for thread 2 to finish
    t3.join(); // Wait for thread 3 to finish

    // -----------------------------------------------------------------------------------
    // 3. Stop the Logger system
    // -----------------------------------------------------------------------------------
    logger.stop(); // Gracefully shut down the Logger and ensure all logs are processed

    return 0; // Exit the program
}
