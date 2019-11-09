#include "skylink_connection.h"
#include <boost/array.hpp>
#include <iostream>
#include <string>
#include <vector>

struct connectionStruct {
    std::string hostname;
    std::string port;
};

struct connectionStruct parseConnectionString(const std::string connectionString) {

    std::string hostname = connectionString.substr(0, connectionString.find(":"));
    std::string port = connectionString.substr(connectionString.find(":") + 1, connectionString.size());

    return {hostname, port};
}

SkylinkConnection::SkylinkConnection(const std::string connectionString) {
    asio::io_context io_context;

    auto connectionPack = parseConnectionString(connectionString);

    asio::ip::tcp::resolver resolver(io_context);
    asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(asio::ip::tcp::v4(), connectionPack.hostname, connectionPack.port);

    this->_socket = std::make_shared<asio::ip::tcp::socket>(io_context);
    this->_socket->connect(*endpoints);
}

SkylinkConnection::~SkylinkConnection() {
}

std::string SkylinkConnection::nextTelemetry() {
    std::array<char, 128> readBuf;
    std::string readChars(this->_savedBuffer);
    this->_savedBuffer.clear();

    for (;;) {
        size_t len = this->_socket->read_some(asio::buffer(readBuf));
        readChars.append(readBuf.begin(), len);

        size_t endOfPayload = readChars.find("\n");
        if (endOfPayload != std::string::npos) {
            // Save the tail end of the chars
            this->_savedBuffer.append(readChars, endOfPayload + 1, readChars.size());

            // Erase the tail end
            readChars.erase(endOfPayload, readChars.size());
            break;
        }
    }

    return readChars;
}