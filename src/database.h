#pragma once

#include "uastelem.h"
#include <libpq-fe.h>
#include <string>

namespace database {

class PGDatabase {
public:
    PGDatabase(const std::string connectionString);
    ~PGDatabase();
    bool addTelemItem(UasTelem uasTelem);

private:
    PGconn* _databaseConnection;
};

} // namespace database