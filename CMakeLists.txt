project(tonton)
cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD              17)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)

add_subdirectory(fmt)

# Setup rplidar as a target that can be used in target_link_libraries
add_library(rplidar STATIC IMPORTED)

set_property(TARGET rplidar
  PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/rplidar_sdk/output/Linux/Release/libsl_lidar_sdk.a)

target_include_directories(rplidar
    INTERFACE
        ${CMAKE_SOURCE_DIR}/rplidar_sdk/sdk/include
        ${CMAKE_SOURCE_DIR}/rplidar_sdk/sdk/src
)

set(APP_NAME tonton)

add_executable(${APP_NAME}
    main.cpp
)

target_include_directories(${APP_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(${APP_NAME}
    PUBLIC
        rplidar
        fmt
)

install(TARGETS ${APP_NAME})