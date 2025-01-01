//----------------------------------------------------------------------------------------
// The #ifndef LOGGER_H directive to prevent multiple inclusions of the same header
// file during compilation.
//----------------------------------------------------------------------------------------
#ifndef LOGGER_H
#define LOGGER_H

// ---------------------------------------------------------------------------------------
// Logger.h - Advanced Thread-Safe Multithreaded Logging System with Log Levels
//
// Features:
// 1. Singleton Design Pattern:
//    - Ensures only one instance of the Logger exists throughout the application.
//
// 2. Thread-Safe Logging:
//    - A mutex protects access to the log queue.
//    - A worker thread processes logs from the queue and writes them to the file.
//
// 3. Log Levels:
//    - Supports different log levels: INFO, WARNING, and ERROR.
//    - Log messages are prefixed with their respective levels in the output.
//
// 4. Producer-Consumer Model:
//    - Main threads (producers) add log messages to the queue.
//    - The worker thread (consumer) processes messages from the queue. The 
//    Producer:
//       - The Threads, workerFunction() threads in main.cpp are producers.
//       - These threads call the Logger::log() method to enqueue log messages into the
//         logQueue.
//    Consumer:
//       - The Logger class internally creates a WorkerThread when the start() method
//         is called.
//       - This workerThread acts as a a consumer, continuously monitoring the logQueue
//         for new log messages.
//       - When a log message is found in the queue, the workerThread dequeues it and writes it
//         to the log file.
//
// 5. Key Features of the Model:
//        Queue-Based Communication:
//          The logQueue acts as a shared resource between producers and the consumer.
//          Producers (application threads) add log messages to the queue, and the consumer (worker thread)
//          processes them.
//        Synchronization:
//          A mutex (queueMutex) ensures thread-safe access to the logQueue, preventing race conditions when
//          multiple threads interact with the queue.
//          A condition variable (condition) allows the workerThread to efficiently wait for new log messages
//          without busy-waiting.
//        Decoupling of Producers and Consumers:
//          Producers can enqueue messages at any time without worrying about the file-writing process, as the
//          consumer thread handles the process independently.
// 6. Why use this model ?
//        Efficiency:
//          Producers (main threads) don't block while waiting for the log file to be written. They quickly
//          enqueue the message and move on.
//        Scalability:
//          Multiple producers can simultaneously log messages, while a single consumer thread serially processes
//          and writes them to ensure consistency.
//        Thread Safety:
//          The model ensures no conflicts between the threads accessing the shared logQueue.
// 7. Graceful Shutdown:
//    - The stop() method ensures all logs are processed before shutting down.
//
// Usage:
// 1. Use Logger::getInstance() to get the singleton instance.
// 2. Call log(LogLevel::INFO, "message") to add messages to the log queue with a specific level.
// 3. Call start() at the beginning of the application to initialize logging.
// 4. Call stop() before exiting the application to gracefully shutdown the logger.
//
// ---------------------------------------------------------------------------------------

#include <string>               // For handling log messages
#include <mutex>                // For thread-safe access to shared resources
#include <queue>                // For storing log messages in a queue
#include <thread>               // For running the worker thread
#include <condition_variable>   // For signaling the worker thread
#include <fstream>              // For writing logs to a file
#include <atomic>               // For managing the logger's running state
#include <iostream>             // For console debugging (optional)

// Enum to define log levels
enum LogLevel { INFO, WARNING, ERROR };

class Logger {
public:
    // Returns the singleton instance of Logger
    static Logger& getInstance();

    // Adds a log message with a specified log level to the queue
    void log(LogLevel level, const std::string& message);

    // Starts the logger by initializing the worker thread
    void start();

    // Stops the logger and ensures all pending messages are processed
    void stop();

private:
    Logger();                   // Private constructor for Singleton Design Pattern
    ~Logger();                  // Destructor to clean up resources

    // Function for the worker thread to process log messages from the queue
    void processLogs();

    // Helper to format log messages with timestamp and log level
    std::string formatLog(LogLevel level, const std::string& message);

    std::queue<std::string> logQueue;    // Queue to store log messages
    std::mutex queueMutex;               // Mutex to protect access to the log queue
    std::condition_variable condition;   // Condition variable to notify the worker thread
    std::atomic<bool> running;           // Atomic flag to indicate if the logger is running
    std::thread workerThread;            // Worker thread for processing log messages
    std::ofstream logFile;               // File stream for writing log messages
};

#endif // LOGGER_H
