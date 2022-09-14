project(tonton)
cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD              17)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)

add_subdirectory(third_party/CLI11)
add_subdirectory(third_party/fmt)
set(gRPC_BUILD_CSHARP_EXT OFF)
set(gRPC_BACKWARDS_COMPATIBILITY_MODE OFF)
set(gRPC_BUILD_GRPC_CSHARP_PLUGIN OFF)
set(gRPC_BUILD_GRPC_NODE_PLUGIN OFF)
set(gRPC_BUILD_GRPC_OBJECTIVE_C_PLUGIN OFF)
set(gRPC_BUILD_GRPC_PHP_PLUGIN OFF)
set(gRPC_BUILD_GRPC_PYTHON_PLUGIN OFF)
set(gRPC_BUILD_GRPC_RUBY_PLUGIN OFF)
add_subdirectory(third_party/grpc)
add_subdirectory(third_party/rplidar_sdk)
add_subdirectory(third_party/matplotplusplus)

add_subdirectory(apps)

install(TARGETS grpc_cpp_plugin
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)