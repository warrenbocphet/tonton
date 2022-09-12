set(LIBRARY_NAME visualiser)

add_library(${LIBRARY_NAME} 
    STATIC
        include/visualiser.h
        src/visualiser.cc
)

target_link_libraries(${LIBRARY_NAME} 
    PUBLIC
        matplot
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