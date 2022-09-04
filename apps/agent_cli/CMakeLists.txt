set(APP_NAME agent_cli)

add_executable(${APP_NAME}
    src/agent_cli.cc
)

target_link_libraries(${APP_NAME} 
    PUBLIC
        CLI11::CLI11
        localisation
)

target_include_directories(${APP_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

install(TARGETS ${APP_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
