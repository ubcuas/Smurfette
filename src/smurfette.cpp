#include "database.h"
#include "skylink_connection.h"
#include "uastelem.h"
#include "utils.h"
#include <chrono>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <nlohmann/json.hpp>
#include <thread>

constexpr char SKYLINK_CONNECTION_AIR[] = "fakelink:65433";
constexpr char SKYLINK_CONNECTION_GROUND[] = "fakelink2:65434";
constexpr char DEFAULT_DB_CONNECTION_STRING[] = "postgresql://postgres:postgres@gcomx-db:5432";
constexpr int TLEM_THREAD_WAIT_MS = 1000;
constexpr int MAIN_THREAD_WAIT_MS = 1000;
constexpr int MAX_TIMESTAMP_DIFF_MS = 1000;

std::mutex aircraftTelemMutex;
UasTelem aircraftSkylinkTelem;

std::mutex groundTelemMutex;
UasTelem groundSkylinkTelem;

struct telemetryGatherParams {
    std::mutex* mutexPtr;
    UasTelem* skylinkTelemPtr;
    std::string skylinkConnectionString;
};

void telemetryGatherThread(struct telemetryGatherParams params) {
    using njson = nlohmann::json;

    utils::TickClock tickClock(TLEM_THREAD_WAIT_MS);
    const uintptr_t threadID = reinterpret_cast<uintptr_t>(params.mutexPtr);
    std::cout << "Starting thread with ID " << threadID << std::endl;
    std::cout << "Connecting to skylink @ " << params.skylinkConnectionString << std::endl;

    SkylinkConnection skylink(params.skylinkConnectionString);

    while (true) {
        // Get telem from serial socket
        std::string nextTelem = skylink.nextTelemetry();
        auto json = njson::parse(nextTelem);

        std::cout << json.dump() << std::endl;

        params.mutexPtr->lock();
        params.skylinkTelemPtr->update(json["latitude"].get<double>(), json["longitude"].get<double>(), json["altitude_agl_meters"].get<double>(), json["heading_degrees"].get<double>());

        params.mutexPtr->unlock();
        std::cout << "Telem Thread Tick " << threadID << std::endl;

        tickClock.synchronize();
    }
}

bool isTelemTimestampValid(int64_t timestamp, int64_t telemTimestamp) {
    return (timestamp - telemTimestamp <= MAX_TIMESTAMP_DIFF_MS) ? true : false;
}

void databaseInjectionThread() {
    utils::TickClock tickClock(MAIN_THREAD_WAIT_MS);
    std::cout << "Starting main thread" << std::endl;

    std::cout << "Starting connection to database" << std::endl;
    database::PGDatabase databaseConnection(DEFAULT_DB_CONNECTION_STRING);

    while (true) {
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

        // TODO: Pick latest not priority

        if (validTelem) {
            std::cout << "Valid TELEM" << std::endl;
            databaseConnection.addTelemItem(uasTelem);
        } else {
            std::cout << "No valid TELEM found" << std::endl;
        }

        tickClock.synchronize();
    }
}

int main(int argc, char* argv[]) {
    // struct telemetryGatherParams aircraftTelemParams = {&aircraftTelemMutex, &aircraftSkylinkTelem, SKYLINK_CONNECTION_AIR};
    struct telemetryGatherParams groundTelemParams = {&groundTelemMutex, &groundSkylinkTelem, SKYLINK_CONNECTION_GROUND};

    // std::thread aircraftTelemThread(telemetryGatherThread, aircraftTelemParams);
    std::thread groundTelemThread(telemetryGatherThread, groundTelemParams);
    std::thread mainThread(databaseInjectionThread);

    mainThread.join();
    groundTelemThread.join();
    // aircraftTelemThread.join();

    return 0;
}
