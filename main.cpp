#include "logger.h"
#include <thread>

void workerFunction(int threadId) {
    Logger& logger = Logger::getInstance();
    for (int i = 0; i < 10; ++i) {
        logger.log(INFO, "Thread " + std::to_string(threadId) + " is logging message " + std::to_string(i));
    }
}

int main() {
    Logger& logger = Logger::getInstance();
    logger.start();

    std::thread t1(workerFunction, 1);
    std::thread t2(workerFunction, 2);
    std::thread t3(workerFunction, 3);

    t1.join();
    t2.join();
    t3.join();

    logger.stop();
    return 0;
}
