add_executable(main
    main.cpp
)

target_link_libraries(main 
    gameConfigLoader 
    gameInstance
    gameInstanceManager
    cpp-tree-sitter
    userManager
    networking
)

install(TARGETS main
    RUNTIME DESTINATION bin
)

add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(server-manager)