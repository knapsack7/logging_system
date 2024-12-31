#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <mutex>
#include <queue>
#include <thread>
#include <condition_variable>
#include <fstream>
#include <atomic>
#include <iostream>

enum LogLevel { INFO, WARNING, ERROR };

class Logger {
public:
    static Logger& getInstance();
    void log(LogLevel level, const std::string& message);

    void start();
    void stop();

private:
    Logger(); // Private Constructor
    ~Logger(); // Destructor to clean up resources

    void processLogs(); // Worker thread function
    std::string formatLog(LogLevel level, const std::string& message);

    std::queue<std::string> logQueue;
    std::mutex queueMutex;
    std::condition_variable condition;
    std::atomic<bool> running;
    std::thread workerThread;
    std::ofstream logFile;
};

#endif // LOGGER_H
