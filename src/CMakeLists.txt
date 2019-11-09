cmake_minimum_required( VERSION 3.10 )
set( CMAKE_CXX_STANDARD 17 )
project( smurfette )


# JSON library dependency
set( JSON_BuildTests OFF CACHE INTERNAL "" )
set( JSON_Install OFF CACHE INTERNAL "" )
add_subdirectory( thirdparty/nlohmann_json )


# Main smurfette executable
add_executable( smurfette  smurfette.cpp uastelem.cpp utils.cpp database.cpp database.cpp skylink_connection.cpp )
target_compile_options( smurfette PUBLIC "-Wall" )

target_include_directories( smurfette SYSTEM PRIVATE "thirdparty/nlohmann_json/include/" "thirdparty/asio-1.12.2/include" )
target_link_libraries( smurfette PUBLIC pq PRIVATE nlohmann_json::nlohmann_json )
