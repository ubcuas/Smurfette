#pragma once

#include <cstdint>

class UasTelem {
public:
    UasTelem();
    UasTelem(double latitude, double longitude, double altitude);
    void update(double latitude, double longitude, double altitude);

    double latitude();
    double longitude();
    double altitude();
    uint64_t timestamp();

private:
    double _latitude;
    double _longitude;
    double _altitude;
    uint64_t _timestamp;
};
