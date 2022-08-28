project(tonton)
cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD              17)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)

include(ExternalProject)


# Builds absl project from the git submodule.
ExternalProject_Add(absl
  PREFIX absl
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/grpc/third_party/abseil-cpp"
  CMAKE_CACHE_ARGS
        -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=TRUE
        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/absl
)

# Builds c-ares project from the git submodule.
ExternalProject_Add(c-ares
  PREFIX c-ares
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/grpc/third_party/cares/cares"
  CMAKE_CACHE_ARGS
        -DCARES_SHARED:BOOL=OFF
        -DCARES_STATIC:BOOL=ON
        -DCARES_STATIC_PIC:BOOL=ON
        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/c-ares
)

# Builds protobuf project from the git submodule.
ExternalProject_Add(protobuf
  PREFIX protobuf
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/grpc/third_party/protobuf"
  CMAKE_CACHE_ARGS
        -Dprotobuf_BUILD_TESTS:BOOL=OFF
        -Dprotobuf_WITH_ZLIB:BOOL=OFF
        -Dprotobuf_MSVC_STATIC_RUNTIME:BOOL=OFF
        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/protobuf
)

# Builds re2 project from the git submodule.
ExternalProject_Add(re2
  PREFIX re2
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/grpc/third_party/re2"
  CMAKE_CACHE_ARGS
        -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=TRUE
        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/re2
)

# Builds zlib project from the git submodule.
ExternalProject_Add(zlib
  PREFIX zlib
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/grpc/third_party/zlib"
  CMAKE_CACHE_ARGS
        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/zlib
)

# if OPENSSL_ROOT_DIR is set, propagate that hint path to the external projects with OpenSSL dependency.
set(_CMAKE_ARGS_OPENSSL_ROOT_DIR "")
if (OPENSSL_ROOT_DIR)
  set(_CMAKE_ARGS_OPENSSL_ROOT_DIR "-DOPENSSL_ROOT_DIR:PATH=${OPENSSL_ROOT_DIR}")
endif()

# Builds gRPC based on locally checked-out sources and set arguments so that all the dependencies
# are correctly located.
ExternalProject_Add(grpc
  PREFIX grpc
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/grpc"
  CMAKE_CACHE_ARGS
        -DgRPC_INSTALL:BOOL=ON
        -DgRPC_BUILD_TESTS:BOOL=OFF
        -DgRPC_BUILD_MSVC_MP_COUNT:STRING=-1
        -DgRPC_PROTOBUF_PROVIDER:STRING=package
        -DgRPC_PROTOBUF_PACKAGE_TYPE:STRING=CONFIG
        -DProtobuf_DIR:PATH=${CMAKE_CURRENT_BINARY_DIR}/protobuf/lib/cmake/protobuf
        -DgRPC_RE2_PROVIDER:STRING=package
        -Dre2_DIR:STRING=${CMAKE_CURRENT_BINARY_DIR}/re2/lib/cmake/re2
        -DgRPC_ZLIB_PROVIDER:STRING=package
        -DZLIB_ROOT:STRING=${CMAKE_CURRENT_BINARY_DIR}/zlib
        -DgRPC_ABSL_PROVIDER:STRING=package
        -Dabsl_DIR:STRING=${CMAKE_CURRENT_BINARY_DIR}/absl/lib/cmake/absl
        -DgRPC_CARES_PROVIDER:STRING=package
        -Dc-ares_DIR:PATH=${CMAKE_CURRENT_BINARY_DIR}/c-ares/lib/cmake/c-ares
        -DgRPC_SSL_PROVIDER:STRING=package
        ${_CMAKE_ARGS_OPENSSL_ROOT_DIR}
        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/grpc
  DEPENDS c-ares protobuf re2 zlib absl
)

# Setup rplidar as a target that can be used in target_link_libraries
add_library(rplidar STATIC IMPORTED)

set_property(TARGET rplidar
  PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/third_party/rplidar_sdk/output/Linux/Release/libsl_lidar_sdk.a)

target_include_directories(rplidar
    INTERFACE
        ${CMAKE_SOURCE_DIR}/rplidar_sdk/sdk/include
        ${CMAKE_SOURCE_DIR}/rplidar_sdk/sdk/src
)
ExternalProject_Add(localisation
  PREFIX libs
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/localisation"
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/tonton/localisation"
  INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/install"
  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}/tonton
  INSTALL_COMMAND "" # When this is an empty string, no intsall command.
  CMAKE_CACHE_ARGS
        -DProtobuf_DIR:PATH=${CMAKE_CURRENT_BINARY_DIR}/protobuf/lib/cmake/protobuf
        -Dc-ares_DIR:PATH=${CMAKE_CURRENT_BINARY_DIR}/c-ares/lib/cmake/c-ares
        -Dre2_DIR:STRING=${CMAKE_CURRENT_BINARY_DIR}/re2/lib/cmake/re2
        -DZLIB_ROOT:STRING=${CMAKE_CURRENT_BINARY_DIR}/zlib
        -Dabsl_DIR:STRING=${CMAKE_CURRENT_BINARY_DIR}/absl/lib/cmake/absl
        ${_CMAKE_ARGS_OPENSSL_ROOT_DIR}
        -DgRPC_DIR:PATH=${CMAKE_CURRENT_BINARY_DIR}/grpc/lib/cmake/grpc
  DEPENDS protobuf grpc
)
# 