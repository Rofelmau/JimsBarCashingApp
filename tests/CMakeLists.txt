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

include(GoogleTest)
gtest_discover_tests(unit_tests)
