set(MODELS_SOURCES
    DiscountModel.cpp
)

set(MODELS_HEADERS
    DiscountModel.h
)

add_library(models ${MODELS_SOURCES} ${MODELS_HEADERS})
target_link_libraries(models Qt5::Core entities)
target_include_directories(models PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ../entities)
