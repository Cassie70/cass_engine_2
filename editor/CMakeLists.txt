add_executable(editor
    main.cpp
 )

target_link_libraries(editor PRIVATE engine)

add_custom_command(TARGET editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:editor>/assets
)