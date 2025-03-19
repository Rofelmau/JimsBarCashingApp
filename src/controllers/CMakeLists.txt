# Add the repository sources
add_library(controllers 
                        CheckoutScreenController.cpp
                        CocktailsConfigurationScreenController.cpp
                        GeneralSettingsScreenController.cpp
                        StatisticsScreenController.cpp
)

# Include directories
target_include_directories(controllers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ../repositories)

# Link libraries
target_link_libraries(controllers PRIVATE entities spdlog::spdlog Qt5::Core Qt5::Sql)