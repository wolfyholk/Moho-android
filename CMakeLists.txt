
cmake_minimum_required(VERSION 3.16)
project(MohoClone VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

qt_add_executable(MohoClone main.cpp)

target_link_libraries(MohoClone PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

set_target_properties(MohoClone PROPERTIES
    QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android"
)
