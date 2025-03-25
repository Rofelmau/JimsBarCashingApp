find_package(Qt5 REQUIRED COMPONENTS Core Gui Quick Network Location)

# Add the repository sources
add_library(services
    LocationService.cpp
    WeatherService.cpp
)

# Include directories
# target_include_directories(services PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ../entities)

# Link libraries
target_link_libraries(services PRIVATE Qt5::Core Qt5::Gui Qt5::Quick Qt5::Network Qt5::Location spdlog::spdlog)
