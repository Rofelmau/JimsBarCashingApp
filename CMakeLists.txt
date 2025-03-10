cmake_minimum_required(VERSION 3.5)

project(MyQtApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(SOURCES
    src/main.cpp
    src/cocktailmodel.cpp
)

set(HEADERS
    src/cocktailmodel.h
)

qt5_add_resources(QT_RESOURCES src/resources.qrc src/qml.qrc)

configure_file(${CMAKE_SOURCE_DIR}/src/cocktails.json ${CMAKE_BINARY_DIR}/cocktails.json COPYONLY)

add_executable(MyQtApp
    ${SOURCES}
    ${HEADERS}
    ${QT_RESOURCES}
)

target_link_libraries(MyQtApp Qt5::Core Qt5::Quick)
