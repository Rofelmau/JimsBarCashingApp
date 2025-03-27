# Add the entity sources
add_library(entities
    Cocktail.cpp
    Discount.cpp
    DiscountType.cpp
    GeneralSettings.cpp
    Sale.cpp
    StatisticsData.cpp
    StatisticsDataByTime.cpp
    TemperatureCategory.cpp
    WeatherCondition.cpp
    WeatherData.cpp
)

# Include directories
target_include_directories(entities PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link Qt5 libraries
target_link_libraries(entities PRIVATE spdlog::spdlog Qt5::Core)