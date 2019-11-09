#pragma once

#include <cstdint>
#include <iomanip>
#include <string>

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

    std::string _latitudeString;
    std::string _longitudeString;
    std::string _altitudeString;
    std::string _headingString;

private:
    double _latitude;
    double _longitude;
    double _altitude;
    double _heading;
    uint64_t _timestamp;

    void updateTimestamp();
};
