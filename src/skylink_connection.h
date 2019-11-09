#pragma once

#include <asio.hpp>
#include <memory>
#include <mutex>
#include <string>

class SkylinkConnection {
public:
    SkylinkConnection(const std::string connectionString);
    ~SkylinkConnection();
    std::string nextTelemetry();

private:
    std::string _savedBuffer;
    std::shared_ptr<asio::ip::tcp::socket> _socket;
};