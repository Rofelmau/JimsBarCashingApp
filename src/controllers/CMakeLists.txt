# Add the repository sources
set(CONTROLLERS_SOURCES
    CheckoutScreenController.cpp
    CocktailsConfigurationScreenController.cpp
    DiscountsConfigurationScreenController.cpp
    GeneralSettingsScreenController.cpp
    StatisticsScreenController.cpp
)

set(CONTROLLERS_HEADERS
    DiscountsConfigurationScreenController.h
)

add_library(controllers ${CONTROLLERS_SOURCES} ${CONTROLLERS_HEADERS})

target_include_directories(controllers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ../repositories ../models)

target_link_libraries(controllers PRIVATE entities models spdlog::spdlog Qt5::Core Qt5::Sql)