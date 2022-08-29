project(localisation)

set(LIBRARY_NAME localisation)

add_library(${LIBRARY_NAME} 
    STATIC
        include/localisation_server.h
        include/localisation.grpc.pb.h
        include/localisation.pb.h
        include/agent.h
        src/localisation_server.cc
        src/localisation.grpc.pb.cc
        src/localisation.pb.cc
        src/agent.cc
)

find_package(gRPC CONFIG REQUIRED)
find_package(rplidar_sdk CONFIG REQUIRED)

target_link_libraries(${LIBRARY_NAME} 
    PUBLIC
        gRPC::grpc++
        gRPC::grpc
        rplidar_sdk
    PRIVATE
        fmt
)

target_include_directories(${LIBRARY_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

set_target_properties(${LIBRARY_NAME} 
    PROPERTIES
        PUBLIC_HEADER $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>       
)