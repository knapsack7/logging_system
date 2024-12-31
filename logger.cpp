#include "logger.h"
#include <ctime>       // For timestamp generation
#include <sstream>     // For formatting log messages

// ---------------------------------------------------------------------------------------
// Logger.cpp - Implementation of the Thread-Safe Multithreaded Logging System
//
// This file provides the implementation for the Logger class defined in Logger.h.
//
// Features:
// 1. Singleton Design Pattern: Ensures a single Logger instance.
// 2. Thread-Safe Logging: Uses a mutex and condition variable for synchronized logging.
// 3. Producer-Consumer Model: Log producers add messages to the queue, and a consumer thread processes them.
// 4. Graceful Shutdown: Ensures all logs are processed before termination.
// 5. Log Formatting: Adds timestamps and log levels to each message.
//
// ---------------------------------------------------------------------------------------

// Initialize the Singleton Logger instance
Logger& Logger::getInstance() {
    static Logger instance; // Ensures only one instance exists (thread-safe in C++11+)
    return instance;
}

// Constructor: Initializes variables and opens the log file
Logger::Logger() : running(false) {
    logFile.open("log.txt", std::ios::out | std::ios::app); // Open the log file in append mode
    if (!logFile.is_open()) {
        std::cerr << "Failed to open log file!" << std::endl;
    }
}

// Destructor: Ensures resources are cleaned up
Logger::~Logger() {
    stop(); // Stop the logger and ensure all logs are processed
    if (logFile.is_open()) {
        logFile.close(); // Close the log file
    }
}

// Start the Logger: Launches the worker thread
void Logger::start() {
    running = true; // Set the running flag to true
    workerThread = std::thread(&Logger::processLogs, this); // Start the worker thread
}

// Stop the Logger: Gracefully shuts down the logging system
void Logger::stop() {
    {
        std::lock_guard<std::mutex> lock(queueMutex); // Lock the mutex to modify shared state
        running = false; // Indicate that the logger should stop
    }
    condition.notify_all(); // Notify the worker thread to process remaining logs
    if (workerThread.joinable()) {
        workerThread.join(); // Wait for the worker thread to finish
    }
}

// Add a log message to the queue
void Logger::log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(queueMutex); // Ensure thread-safe access to the queue
    logQueue.push(formatLog(level, message)); // Add the formatted log message to the queue
    condition.notify_one(); // Notify the worker thread
}

// Worker thread function: Processes log messages from the queue
void Logger::processLogs() {
    while (true) {
        std::unique_lock<std::mutex> lock(queueMutex); // Acquire lock for queue access
        condition.wait(lock, [this] { return !logQueue.empty() || !running; }); // Wait until logs are available or running stops

        while (!logQueue.empty()) { // Process all available log messages
            logFile << logQueue.front() << std::endl; // Write the log message to the file
            logQueue.pop(); // Remove the processed log from the queue
        }

        if (!running && logQueue.empty()) {
            break; // Exit the loop if the logger is stopping and the queue is empty
        }
    }
}

// Format a log message with timestamp and level
std::string Logger::formatLog(LogLevel level, const std::string& message) {
    std::ostringstream oss;
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now); // Get the local time

    // Add timestamp to the log
    oss << "[" << (1900 + localTime->tm_year) << "-" << (1 + localTime->tm_mon) << "-" << localTime->tm_mday << " "
        << localTime->tm_hour << ":" << localTime->tm_min << ":" << localTime->tm_sec << "] ";

    // Add log level to the log
    switch (level) {
        case INFO:    oss << "[INFO] "; break;
        case WARNING: oss << "[WARNING] "; break;
        case ERROR:   oss << "[ERROR] "; break;
    }

    // Add the actual log message
    oss << message;

    return oss.str(); // Return the formatted log message
}
