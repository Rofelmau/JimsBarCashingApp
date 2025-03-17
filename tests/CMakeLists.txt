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

add_executable(unit_tests
  test_main.cpp
  test_DatabaseManager.cpp
  test_Cocktail.cpp
  ../src/DatabaseManager.cpp
  ../src/entities/Cocktail.cpp
)

target_link_libraries(unit_tests
  gtest
  gtest_main
  Qt5::Core
  Qt5::Sql
  Qt5::Widgets
  Qt5::Qml
)

include(GoogleTest)
gtest_discover_tests(unit_tests)
