#include "uastelem.h"
#include "utils.h"

UasTelem::UasTelem() {
    this->_timestamp = 0;
}

UasTelem::UasTelem(double latitude, double longitude, double altitude):
    _latitude(latitude), _longitude(longitude), _altitude(altitude) {
    this->_timestamp = utils::timeSinceEpochMillisec();
}

void UasTelem::update(double latitude, double longitude, double altitude) {
    this->_latitude = latitude;
    this->_longitude = longitude;
    this->_altitude = altitude;

    this->_timestamp = utils::timeSinceEpochMillisec();
}

double UasTelem::latitude() {
    return _latitude;
}

double UasTelem::longitude() {
    return _longitude;
}

double UasTelem::altitude() {
    return _altitude;
}

uint64_t UasTelem::timestamp() {
    return _timestamp;
}
