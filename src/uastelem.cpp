#include "uastelem.h"
#include "utils.h"
#include <sstream>

UasTelem::UasTelem() {
    this->_timestamp = 0;
}

UasTelem::UasTelem(double latitude, double longitude, double altitude, double heading) :
    _latitude(latitude), _longitude(longitude), _altitude(altitude) {

    this->updateTimestamp();
}

std::string doubleToString(double number, int precision = 10) {
    std::ostringstream streamObj;

    streamObj << std::fixed << std::setprecision(precision);
    streamObj << number;

    return streamObj.str();
}

void UasTelem::update(double latitude, double longitude, double altitude, double heading) {
    this->_latitude = latitude;
    this->_latitudeString = doubleToString(latitude);

    this->_longitude = longitude;
    this->_longitudeString = doubleToString(longitude);

    this->_altitude = altitude;
    this->_altitudeString = doubleToString(altitude);

    this->_heading = heading;
    this->_headingString = doubleToString(heading);

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
