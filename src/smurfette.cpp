#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include "uastelem.h"
#include "utils.h"

constexpr int TLEM_THREAD_WAIT = 1000;
constexpr int MAIN_THREAD_WAIT = 1000;

std::mutex aircraftTelemMutex;
UasTelem aircraftSkylinkTelem;

std::mutex groundTelemMutex;
UasTelem groundSkylinkTelem;

void telemetryGatherThread(std::mutex* mutexPtr, UasTelem* skylinkTelemPtr) {
    while(true) {
        mutexPtr->lock();
        skylinkTelemPtr->update(0.0, 0.0, 0.0);
        mutexPtr->unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(TLEM_THREAD_WAIT));
    }
}

int main() {
    std::thread aircraftTelemThread (telemetryGatherThread, &aircraftTelemMutex, &aircraftSkylinkTelem);
    std::thread groundTelemThread (telemetryGatherThread, &groundTelemMutex, &groundSkylinkTelem);

    while(true) {
        std::cout << "Hello World" << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(MAIN_THREAD_WAIT));
    }

    aircraftTelemThread.join();
    groundTelemThread.join();

    return 0;
}
