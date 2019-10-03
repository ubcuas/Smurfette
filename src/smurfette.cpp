#include <chrono>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <thread>
#include "database.h"
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
    utils::TickClock tickClock (TLEM_THREAD_WAIT_MS);

    while(true) {
        // Get telem from serial socket

        mutexPtr->lock();
        skylinkTelemPtr->update(0.0, 0.0, 0.0);
        mutexPtr->unlock();

        tickClock.synchronize();
    }
}

bool isTelemTimestampValid(int64_t timestamp, int64_t telemTimestamp) {
    return (timestamp - telemTimestamp <= MAX_TIMESTAMP_DIFF_MS) ? true : false;
}

int main() {
    std::cout << "Starting connection to database" << std::endl;
    database::PGDatabase databaseConnection ("postgresql://172.17.0.2:5432");

    std::thread aircraftTelemThread (telemetryGatherThread, &aircraftTelemMutex, &aircraftSkylinkTelem);
    std::thread groundTelemThread (telemetryGatherThread, &groundTelemMutex, &groundSkylinkTelem);

    utils::TickClock tickClock (MAIN_THREAD_WAIT_MS);

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
            databaseConnection.addTelemItem(uasTelem);
        } else {
            std::cout << "No valid TELEM found" << std::endl;
        }

        tickClock.synchronize();
    }

    aircraftTelemThread.join();
    groundTelemThread.join();

    return 0;
}
