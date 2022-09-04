project(tonton)
cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD              17)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)

add_subdirectory(third_party/CLI11)
add_subdirectory(third_party/fmt)
add_subdirectory(third_party/grpc)
add_subdirectory(third_party/rplidar_sdk)

add_subdirectory(apps)

install(TARGETS grpc_cpp_plugin
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)