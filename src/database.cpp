#include <algorithm>
#include <iostream>
#include "database.h"

namespace database {

PGDatabase::PGDatabase(std::string connectionString) {
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
    constexpr size_t numParams = 3;

    const char* paramValues [numParams] = {
        reinterpret_cast<char *>(&uasTelem._latitude),
        reinterpret_cast<char *>(&uasTelem._longitude),
        reinterpret_cast<char *>(&uasTelem._altitude),
    };

    const int paramLengths [numParams] = {
        sizeof(uasTelem._latitude),
        sizeof(uasTelem._longitude),
        sizeof(uasTelem._altitude),
    };

    // Set all fields to binary
    int paramFormats [numParams];
    std::fill_n(paramFormats, numParams, 1);

    auto res = PQexecParams(this->_databaseConnection,
                            "INSERT INTO table (latitude, longtitude, altitude) VALUES ($1::double precision, $2::double precision, $3::double precision)",
                            numParams,
                            NULL,
                            paramValues,
                            paramLengths,
                            paramFormats,
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

}  // namespace database