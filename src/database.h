#pragma once
#include <string>
#include <libpq-fe.h>
#include "uastelem.h"

namespace database {

class PGDatabase {
public:
    PGDatabase(std::string connectionString);
    ~PGDatabase();
    bool addTelemItem(UasTelem uasTelem);

private:
    PGconn* _databaseConnection;
};

}  // namespace database