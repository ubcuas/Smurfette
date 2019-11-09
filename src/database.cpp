#include "database.h"
#include <algorithm>
#include <iostream>

namespace database {

PGDatabase::PGDatabase(const std::string connectionString) {
    this->_databaseConnection = PQconnectdb(connectionString.c_str());
    if (PQstatus(this->_databaseConnection) == CONNECTION_OK) {
        std::cout << "Database Connected!" << std::endl;
    } else {
        std::cout << "Unable to connect to database" << std::endl;
    }
}

PGDatabase::~PGDatabase() {
    PQfinish(this->_databaseConnection);
}

bool PGDatabase::addTelemItem(UasTelem uasTelem) {
    constexpr size_t numParams = 4;

    const char* paramValues[numParams] = {
        uasTelem._latitudeString.data(),
        uasTelem._longitudeString.data(),
        uasTelem._altitudeString.data(),
        uasTelem._headingString.data(),
    };

    auto res = PQexecParams(this->_databaseConnection,
        "INSERT INTO interop_uastelemetry \
        (latitude, longitude, altitude_msl, uas_heading, uploaded, created_at) \
        VALUES \
        ($1::float8, $2::float8, $3::float8, $4::float8, false::bool, now())",
        numParams,
        NULL,
        paramValues,
        NULL,
        NULL,
        1);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Failed to add telem item to database: " << PQerrorMessage(this->_databaseConnection) << std::endl;
        PQclear(res);
        return false;
    } else {
        std::cout << "Added telem item to database." << std::endl;
        PQclear(res);
        return true;
    }
}

} // namespace database
