# Script to auto-generate VS Code debug configurations from target names


set(VSCODE_LAUNCH_JSON_PATH ${CMAKE_SOURCE_DIR}/.vscode/launch.json)

# Add header of file (has to be cache so we can access it from subdirs)
set(VSCODE_LAUNCH_JSON_CONTENT 
"{
    \"configurations\": ["
    CACHE INTERNAL "Content of VS Code launch.json" FORCE)


function(gen_vs_code_debug_configuration CMAKE_TARGET)

    # Create name (combine target name, Mbed target, and build config to generate a unique string)
    set(CONFIG_NAME "mbed-cmake gdbserver ${CMAKE_TARGET} ${MBED_TARGET_NAME} ${CMAKE_BUILD_TYPE}")

    # from here: https://stackoverflow.com/questions/38089178/is-it-possible-to-attach-to-a-remote-gdb-target-with-vscode
    set(VSCODE_LAUNCH_JSON_CONTENT "${VSCODE_LAUNCH_JSON_CONTENT}
        {
            \"type\": \"gdb\",
            \"request\": \"attach\",
            \"name\": \"${CONFIG_NAME}\",
            \"executable\": \"$<TARGET_FILE:${CMAKE_TARGET}>\",
            \"target\": \"localhost:${GDB_PORT}\",
            \"remote\": true,
            \"cwd\": \"\${workspaceRoot}\",
            \"gdbpath\": \"arm-none-eabi-gdb\"
        },"
        CACHE INTERNAL "" FORCE)

endfunction(gen_vs_code_debug_configuration)

# Take all generated debug configurations and write them to launch.json.
function(vs_code_writeout_configs)
    
    # Add footer
    set(VSCODE_LAUNCH_JSON_CONTENT "${VSCODE_LAUNCH_JSON_CONTENT}
    ]
}"
    CACHE INTERNAL "" FORCE)

    file(GENERATE OUTPUT ${VSCODE_LAUNCH_JSON_PATH} CONTENT ${VSCODE_LAUNCH_JSON_CONTENT})
endfunction(vs_code_writeout_configs)