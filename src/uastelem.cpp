#include "uastelem.h"
#include "utils.h"

UasTelem::UasTelem() {
    this->_timestamp = 0;
}


UasTelem::UasTelem(double latitude, double longitude, double altitude, double heading):
    _latitude(latitude), _longitude(longitude), _altitude(altitude) {

    this->updateTimestamp();
}

void UasTelem::update(double latitude, double longitude, double altitude, double heading) {
    this->_latitude = latitude;
    this->_longitude = longitude;
    this->_altitude = altitude;
    this->_heading = heading;

    this->updateTimestamp();
}

void UasTelem::updateTimestamp() {
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

double UasTelem::heading() {
    return _heading;
}

uint64_t UasTelem::timestamp() {
    return _timestamp;
}
