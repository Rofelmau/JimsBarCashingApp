cmake_minimum_required(VERSION 3.16)

project(MyProject)

set(CMAKE_CXX_STANDARD 17)

# Check for sqlite3 CLI tool
find_program(SQLITE3_EXECUTABLE sqlite3)

if(NOT SQLITE3_EXECUTABLE)
  message(FATAL_ERROR "The sqlite3 command-line tool was not found. Please install it before building.")
endif()

# Add subdirectories
add_subdirectory(src)

# Add Google Test
enable_testing()
add_subdirectory(tests)

# Custom command to create the database
configure_file(${CMAKE_SOURCE_DIR}/src/database/schema.sql ${CMAKE_BINARY_DIR}/schema.sql COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/src/database/data.sql ${CMAKE_BINARY_DIR}/data.sql COPYONLY)

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/cocktails.db
    COMMAND ${SQLITE3_EXECUTABLE} ${CMAKE_BINARY_DIR}/cocktails.db < ${CMAKE_BINARY_DIR}/schema.sql
    COMMAND ${SQLITE3_EXECUTABLE} ${CMAKE_BINARY_DIR}/cocktails.db < ${CMAKE_BINARY_DIR}/data.sql
    DEPENDS ${CMAKE_SOURCE_DIR}/src/database/schema.sql ${CMAKE_SOURCE_DIR}/src/database/data.sql
    COMMENT "Creating and populating the database"
)

add_custom_target(create_database ALL DEPENDS ${CMAKE_BINARY_DIR}/cocktails.db)
