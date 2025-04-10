cmake_minimum_required(VERSION 3.10)

# Enable automatic moc, rcc, and uic processing
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt5 packages
find_package(Qt5 REQUIRED COMPONENTS Core Sql Widgets Qml Quick)

# Fetch spdlog
include(FetchContent)
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.12.0
)
FetchContent_MakeAvailable(spdlog)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/database)
include_directories(${CMAKE_SOURCE_DIR}/src/repositories)
include_directories(${CMAKE_SOURCE_DIR}/src/services)
include_directories(${spdlog_INCLUDE_DIRS})
include_directories(${spdlog_SOURCE_DIR}/include)

# Add subdirectories
add_subdirectory(entities)
add_subdirectory(repositories)
add_subdirectory(controllers)
add_subdirectory(models)
add_subdirectory(services)

# Add the main executable
add_executable(MyProject 
    main.cpp
    DatabaseManager.cpp
    Logger.cpp
)

# Add QML resources
qt5_add_resources(QT_RESOURCES qml.qrc)

# Link libraries
target_link_libraries(MyProject PRIVATE repositories Qt5::Sql)
target_link_libraries(MyProject PRIVATE controllers)
target_link_libraries(MyProject PRIVATE spdlog::spdlog Qt5::Core Qt5::Widgets Qt5::Qml Qt5::Quick)

target_include_directories(MyProject PRIVATE ${spdlog_SOURCE_DIR}/include)

# Ensure the database is created before building the project
add_dependencies(MyProject create_database)

# Add QML resources to the target
target_sources(MyProject PRIVATE ${QT_RESOURCES})
