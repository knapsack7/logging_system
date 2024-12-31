#include "logger.h"
#include <chrono>
#include <sstream>

// Singleton instance
Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

// Constructor
Logger::Logger() : running(false), logFile("logs.txt", std::ios::app) {
    if (!logFile.is_open()) {
        throw std::runtime_error("Failed to open log file!");
    }
}

// Destructor
Logger::~Logger() {
    stop();
    if (logFile.is_open()) {
        logFile.close();
    }
}

// Start logging system
void Logger::start() {
    running.store(true);
    workerThread = std::thread(&Logger::processLogs, this);
}

// Stop logging system
void Logger::stop() {
    running.store(false);
    condition.notify_all(); // Wake up the worker thread
    if (workerThread.joinable()) {
        workerThread.join();
    }
}

// Log a message
void Logger::log(LogLevel level, const std::string& message) {
    std::string formattedLog = formatLog(level, message);
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        logQueue.push(formattedLog);
    }
    condition.notify_one(); // Notify the worker thread
}

// Worker thread function
void Logger::processLogs() {
    while (running.load() || !logQueue.empty()) {
        std::unique_lock<std::mutex> lock(queueMutex);
        condition.wait(lock, [this]() {
            return !logQueue.empty() || !running.load();
        });

        while (!logQueue.empty()) {
            std::string logMessage = logQueue.front();
            logQueue.pop();
            lock.unlock();

            logFile << logMessage << std::endl;
            logFile.flush(); // Ensure log is written to disk
            lock.lock();
        }
    }
}

// Format log message
std::string Logger::formatLog(LogLevel level, const std::string& message) {
    std::ostringstream oss;
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    oss << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S");

    switch (level) {
        case INFO:    oss << " [INFO]    "; break;
        case WARNING: oss << " [WARNING] "; break;
        case ERROR:   oss << " [ERROR]   "; break;
    }

    oss << message;
    return oss.str();
}
