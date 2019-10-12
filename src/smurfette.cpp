#include <chrono>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <thread>
#include "database.h"
#include "uastelem.h"
#include "utils.h"

constexpr char DEFAULT_DB_CONNECTION_STRING[] = "postgresql://smurfette_db:5432";
constexpr int TLEM_THREAD_WAIT_MS = 1000;
constexpr int MAIN_THREAD_WAIT_MS = 1000;
constexpr int MAX_TIMESTAMP_DIFF_MS = 100;

std::mutex aircraftTelemMutex;
UasTelem aircraftSkylinkTelem;

std::mutex groundTelemMutex;
UasTelem groundSkylinkTelem;

void telemetryGatherThread(std::mutex* mutexPtr, UasTelem* skylinkTelemPtr) {
    utils::TickClock tickClock (TLEM_THREAD_WAIT_MS);
    const uintptr_t threadID = reinterpret_cast<uintptr_t>(mutexPtr);
    std::cout << "Starting thread with ID " << threadID << std::endl;

    while(true) {
        // Get telem from serial socket

        mutexPtr->lock();
        skylinkTelemPtr->update(0.0, 0.0, 0.0);
        mutexPtr->unlock();
        std::cout << "Telem Thread Tick " << threadID << std::endl;

        tickClock.synchronize();
    }
}

bool isTelemTimestampValid(int64_t timestamp, int64_t telemTimestamp) {
    return (timestamp - telemTimestamp <= MAX_TIMESTAMP_DIFF_MS) ? true : false;
}

void databaseInjectionThread() {
    utils::TickClock tickClock (MAIN_THREAD_WAIT_MS);
    std::cout << "Starting main thread" << std::endl;

    std::cout << "Starting connection to database" << std::endl;
    database::PGDatabase databaseConnection (DEFAULT_DB_CONNECTION_STRING);

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
            std::cout << "Valid TELEM" << std::endl;
            databaseConnection.addTelemItem(uasTelem);
        } else {
            std::cout << "No valid TELEM found" << std::endl;
        }

        tickClock.synchronize();
    }
}

int main(int argc, char *argv[]) {
    std::thread aircraftTelemThread (telemetryGatherThread, &aircraftTelemMutex, &aircraftSkylinkTelem);
    std::thread groundTelemThread (telemetryGatherThread, &groundTelemMutex, &groundSkylinkTelem);
    std::thread mainThread (databaseInjectionThread);

    mainThread.join();
    aircraftTelemThread.join();
    groundTelemThread.join();

    return 0;
}
