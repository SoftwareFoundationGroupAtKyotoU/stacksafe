function(export_compile_commands)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON PARENT_SCOPE)
  set(file "compile_commands.json")
  set(source "${CMAKE_BINARY_DIR}/${file}")
  add_custom_target(export-compile-commands
    COMMAND cp "${source}" "${file}"
    DEPENDS "${source}"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMENT "export ${file}"
    )
endfunction()
