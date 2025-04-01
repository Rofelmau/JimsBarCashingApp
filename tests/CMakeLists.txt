# Download and include Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# Add this line to force shared runtime
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Find Qt5 packages
find_package(Qt5 REQUIRED COMPONENTS Core Sql Widgets Qml)

# Ensure Qt plugins are available
get_target_property(Qt5Core_LOCATION Qt5::Core LOCATION)
get_filename_component(Qt5_PLUGIN_DIR ${Qt5Core_LOCATION} DIRECTORY)
set(Qt5_PLUGIN_DIR ${Qt5_PLUGIN_DIR}/../plugins) # Dynamisch den Plugin-Pfad ermitteln
get_filename_component(Qt5_PLUGIN_DIR ${Qt5_PLUGIN_DIR} ABSOLUTE)

# Zusätzliche Überprüfung und alternative Fallbacks
if(NOT EXISTS ${Qt5_PLUGIN_DIR}/sqldrivers)
  message(WARNING "Qt plugin directory not found at ${Qt5_PLUGIN_DIR}/sqldrivers. Trying alternative paths.")
  if(EXISTS "/usr/lib/qt/plugins/sqldrivers")
    set(Qt5_PLUGIN_DIR "/usr/lib/qt/plugins")
  elseif(EXISTS "/usr/lib/x86_64-linux-gnu/qt5/plugins/sqldrivers")
    set(Qt5_PLUGIN_DIR "/usr/lib/x86_64-linux-gnu/qt5/plugins")
  elseif(EXISTS "/usr/lib/arm-linux-gnueabihf/qt5/plugins/sqldrivers")
    set(Qt5_PLUGIN_DIR "/usr/lib/arm-linux-gnueabihf/qt5/plugins")
  elseif(DEFINED ENV{QT_PLUGIN_PATH} AND EXISTS "$ENV{QT_PLUGIN_PATH}/sqldrivers")
    set(Qt5_PLUGIN_DIR "$ENV{QT_PLUGIN_PATH}")
  else()
    message(FATAL_ERROR "Qt plugin directory could not be found. Please ensure Qt is correctly installed.")
  endif()
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)

add_executable(unit_tests
  test_main.cpp
  test_DatabaseManager.cpp
  test_Cocktail.cpp
  ../src/DatabaseManager.cpp
  ../src/entities/Cocktail.cpp
  ../src/Logger.cpp
)

target_include_directories(unit_tests PRIVATE ${spdlog_SOURCE_DIR}/include)

target_link_libraries(unit_tests PRIVATE
  gtest
  gtest_main
  Qt5::Core
  Qt5::Sql
  Qt5::Widgets
  Qt5::Qml
  spdlog::spdlog
)

# Add runtime environment for Qt plugins
add_custom_command(TARGET unit_tests POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${Qt5_PLUGIN_DIR}/sqldrivers $<TARGET_FILE_DIR:unit_tests>/sqldrivers
)

include(GoogleTest)
gtest_discover_tests(unit_tests)
