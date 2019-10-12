#pragma once

#include <cstdint>

class UasTelem {
public:
    UasTelem();
    UasTelem(double latitude, double longitude, double altitude, double heading);
    void update(double latitude, double longitude, double altitude, double heading);

    double latitude();
    double longitude();
    double altitude();
    double heading();
    uint64_t timestamp();

    double _latitude;
    double _longitude;
    double _altitude;
    double _heading;
    uint64_t _timestamp;

private:
    void updateTimestamp();
};
