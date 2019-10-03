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
    auto res = PQexec(this->_databaseConnection, "BEGIN");

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