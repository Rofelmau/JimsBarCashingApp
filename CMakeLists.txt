cmake_minimum_required(VERSION 3.10)

project(MyProject)

set(CMAKE_CXX_STANDARD 17)

# Find Qt5 packages
find_package(Qt5 COMPONENTS Core Sql REQUIRED)

# Add subdirectories
add_subdirectory(src)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/database)
include_directories(${CMAKE_SOURCE_DIR}/src/repositories)
include_directories(${CMAKE_SOURCE_DIR}/src/managers)  # Add this line

configure_file(${CMAKE_SOURCE_DIR}/src/database/schema.sql ${CMAKE_BINARY_DIR}/schema.sql COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/src/database/data.sql ${CMAKE_BINARY_DIR}/data.sql COPYONLY)

message(STATUS "Schema SQL file: ${CMAKE_BINARY_DIR}/schema.sql")
message(STATUS "Data SQL file: ${CMAKE_BINARY_DIR}/data.sql")

set(SCHEMA_FILE "${CMAKE_BINARY_DIR}/schema.sql")
set(DATA_FILE "${CMAKE_BINARY_DIR}/data.sql")
set(DB_FILE "${CMAKE_BINARY_DIR}/cocktails.db")

# Custom command to create the database
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/cocktails.db
    COMMAND sqlite3 ${CMAKE_BINARY_DIR}/cocktails.db < ${CMAKE_BINARY_DIR}/schema.sql
    COMMAND sqlite3 ${CMAKE_BINARY_DIR}/cocktails.db < ${CMAKE_BINARY_DIR}/data.sql
    DEPENDS ${CMAKE_SOURCE_DIR}/src/database/schema.sql ${CMAKE_SOURCE_DIR}/src/database/data.sql
    COMMENT "Creating and populating the database"
)

# Ensure the database is created before building the project
add_custom_target(create_database ALL DEPENDS ${CMAKE_BINARY_DIR}/cocktails.db)