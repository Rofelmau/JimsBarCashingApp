# Add the repository sources
add_library(repositories
    CashBalanceRepository.cpp
    CocktailRepository.cpp
    DiscountsRepository.cpp
    SalesRepository.cpp
    SettingsRepository.cpp
    WeatherRepository.cpp
)

# Include directories
target_include_directories(repositories PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ../entities)

# Link libraries
target_link_libraries(repositories PRIVATE spdlog::spdlog Qt5::Core Qt5::Sql)