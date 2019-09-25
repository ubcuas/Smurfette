#include <iostream>
#include <chrono>
#include <cstdint>
#include <thread>
#include <mutex>
#include "uastelem.h"
#include "utils.h"

constexpr int TLEM_THREAD_WAIT_MS = 1000;
constexpr int MAIN_THREAD_WAIT_MS = 1000;
constexpr int MAX_TIMESTAMP_DIFF_MS = 100;

std::mutex aircraftTelemMutex;
UasTelem aircraftSkylinkTelem;

std::mutex groundTelemMutex;
UasTelem groundSkylinkTelem;

void telemetryGatherThread(std::mutex* mutexPtr, UasTelem* skylinkTelemPtr) {
    while(true) {
        // Get telem from serial socket

        mutexPtr->lock();
        skylinkTelemPtr->update(0.0, 0.0, 0.0);
        mutexPtr->unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(TLEM_THREAD_WAIT_MS));
    }
}

bool isTelemTimestampValid(int64_t timestamp, int64_t telemTimestamp) {
    return (timestamp - telemTimestamp <= MAX_TIMESTAMP_DIFF_MS) ? true : false;
}

int main() {
    std::thread aircraftTelemThread (telemetryGatherThread, &aircraftTelemMutex, &aircraftSkylinkTelem);
    std::thread groundTelemThread (telemetryGatherThread, &groundTelemMutex, &groundSkylinkTelem);

    while(true) {
        UasTelem uasTelem;
        bool validTelem = false;
        int64_t timestamp = utils::timeSinceEpochMillisec();

        groundTelemMutex.lock();
        if (!validTelem && isTelemTimestampValid(timestamp, groundSkylinkTelem.timestamp())) {
            uasTelem = groundSkylinkTelem;
            validTelem = true;
        }
        groundTelemMutex.unlock();

        aircraftTelemMutex.lock();
        if (!validTelem && isTelemTimestampValid(timestamp, aircraftSkylinkTelem.timestamp())) {
            uasTelem = aircraftSkylinkTelem;
            validTelem = true;
        }
        aircraftTelemMutex.unlock();

        if (validTelem) {
            // Place telem into database
        } else {
            std::cout << "No valid TELEM found" << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(MAIN_THREAD_WAIT_MS));
    }

    aircraftTelemThread.join();
    groundTelemThread.join();

    return 0;
}
