cmake_minimum_required(VERSION 3.10)

# Enable automatic moc, rcc, and uic processing
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt5 packages
find_package(Qt5 REQUIRED COMPONENTS Core Sql Widgets Qml)

add_subdirectory(entities)
add_subdirectory(repositories)
add_subdirectory(controllers)

# Add the main executable
add_executable(MyProject 
    main.cpp
    DatabaseManager.cpp
)

# Add QML resources
qt5_add_resources(QT_RESOURCES qml.qrc)

# Link libraries
target_link_libraries(MyProject PRIVATE repositories Qt5::Core Qt5::Sql Qt5::Widgets Qt5::Qml)
target_link_libraries(MyProject PRIVATE controllers Qt5::Core Qt5::Widgets Qt5::Qml)

# Ensure the database is created before building the project
add_dependencies(MyProject create_database)

# Add QML resources to the target
target_sources(MyProject PRIVATE ${QT_RESOURCES})
