# CMakeLists file to handle the different upload methods to mbed targets

# Step 1: determine supported upload methods
# -------------------------------------------------------------

# Start with NONE upload method which does not allow any uploading
set(SUPPORTED_UPLOAD_METHODS NONE)

# Mbed is always supported
list(APPEND SUPPORTED_UPLOAD_METHODS MBED)

# JLINK requires the J-link package
if(JLINK_FOUND)
    list(APPEND SUPPORTED_UPLOAD_METHODS JLINK)
endif()

# NXPPROG requires pyserial
check_python_package(serial HAVE_PYSERIAL)
if(HAVE_PYSERIAL)
    list(APPEND SUPPORTED_UPLOAD_METHODS NXPPROG)
endif()

# pyOCD requires pyocd
check_python_package(pyocd HAVE_PYOCD)
if(HAVE_PYOCD)
    list(APPEND SUPPORTED_UPLOAD_METHODS PYOCD)
endif()

# OPENOCD required openocd
if(OpenOCD_FOUND)
    list(APPEND SUPPORTED_UPLOAD_METHODS OPENOCD)
endif()


# decide default method based on what can work
# -------------------------------------------------------------


set(UPLOAD_METHOD "${UPLOAD_METHOD_DEFAULT}" CACHE STRING "Method for uploading programs to the mbed")

if((NOT "${UPLOAD_METHOD}" STREQUAL "NONE") AND (NOT "${${UPLOAD_METHOD}_UPLOAD_ENABLED}"))
    message(FATAL_ERROR "The upload method ${UPLOAD_METHOD} is not enabled for this project.")
endif()

if(NOT "${UPLOAD_METHOD}" IN_LIST SUPPORTED_UPLOAD_METHODS)
    message(FATAL_ERROR "The upload method ${UPLOAD_METHOD} cannot run on this system due to missing prerequisites.  See its docs for details.")
endif()

message(STATUS "Board upload method set to ${UPLOAD_METHOD}")


# path where the gdbinit file will be written
set(GDBINIT_PATH ${CMAKE_CURRENT_BINARY_DIR}/mbed-cmake.gdbinit)


# create function to upload code
# -------------------------------------------------------------
if("${UPLOAD_METHOD}" STREQUAL NONE)
    function(gen_upload_target TARGET_NAME BIN_FILE)
        add_custom_target(flash-${TARGET_NAME}
                COMMAND ${CMAKE_COMMAND}
                -E echo "Cannot flash, no UPLOAD_METHOD configured!")

        add_dependencies(flash-${TARGET_NAME} ${TARGET_NAME})

    endfunction(gen_upload_target)

elseif("${UPLOAD_METHOD}" STREQUAL MBED)

    set(MBED_PATH "" CACHE PATH "Path to mbed virtual device to upload to")

    function(gen_upload_target TARGET_NAME BIN_FILE)

        add_custom_target(flash-${TARGET_NAME}
                COMMAND ${CMAKE_COMMAND}
                -DBIN_FILE=${BIN_FILE}
                -DMBED_PATH=${MBED_PATH}
                -P ${MBED_CMAKE_SOURCE_DIR}/cmake/install_bin_file.cmake)

        add_dependencies(flash-${TARGET_NAME} ${TARGET_NAME})

    endfunction(gen_upload_target)

elseif("${UPLOAD_METHOD}" STREQUAL "JLINK")

    set(GENERATE_GDBINIT TRUE)

    function(gen_upload_target TARGET_NAME BIN_FILE)

        # create command file for j-link
        set(COMMAND_FILE_PATH ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/flash-${TARGET_NAME}.jlink)
        file(GENERATE OUTPUT ${COMMAND_FILE_PATH} CONTENT
"
loadbin ${BIN_FILE}, 0x0
r
exit
")
        add_custom_target(flash-${TARGET_NAME}
                COMMENT "Flashing ${TARGET_NAME} with J-Link..."
                COMMAND ${JLINK}
                -Device ${JLINK_CPU_NAME}
                -Speed ${JLINK_JTAG_SPEED}
                -if JTAG
                -JTAGConf -1,-1
                -AutoConnect 1
                -ExitOnError
                -CommandFile ${COMMAND_FILE_PATH})


        add_dependencies(flash-${TARGET_NAME} ${TARGET_NAME})

        # create debug target
        add_custom_target(debug-${TARGET_NAME}
                COMMENT "starting GDB to debug ${TARGET_NAME}..."
                COMMAND arm-none-eabi-gdb
                --command=${GDBINIT_PATH}
                $<TARGET_FILE:${EXECUTABLE}>)


        add_dependencies(debug-${TARGET_NAME} ${TARGET_NAME})

    endfunction(gen_upload_target)

    # also create a target to run GDB server
    add_custom_target(start-gdbserver
        COMMENT "Starting J-Link GDB server"
        COMMAND 
        "${JLINK_GDBSERVER}" 
        -Select USB 
        -Device ${JLINK_CPU_NAME}
        -Speed ${JLINK_JTAG_SPEED}
        -endian little 
        -if JTAG 
        -JTAGConf -1,-1 
        -LocalhostOnly
        -noIR
        -port ${GDB_PORT}
        )

elseif("${UPLOAD_METHOD}" STREQUAL "PYOCD")

    set(GENERATE_GDBINIT TRUE)

    set(PYOCD_PROBE_ARGS "")
    if(DEFINED PYOCD_PROBE_UID)
        if(NOT "${PYOCD_PROBE_UID}" STREQUAL "")
            set(PYOCD_PROBE_ARGS --probe ${PYOCD_PROBE_UID})
        endif()
    endif()

    function(gen_upload_target TARGET_NAME BIN_FILE)

        add_custom_target(flash-${TARGET_NAME}
            COMMENT "Flashing ${TARGET_NAME} with pyOCD..."
            COMMAND ${Python3_EXECUTABLE}
            -m pyocd
            flash
            -v
            --no-wait
            -t ${PYOCD_TARGET_NAME}
            ${PYOCD_PROBE_ARGS}
            ${BIN_FILE})


        add_dependencies(flash-${TARGET_NAME} ${TARGET_NAME})

        # create debug target
        add_custom_target(debug-${TARGET_NAME}
            COMMENT "starting GDB to debug ${TARGET_NAME}..."
            COMMAND arm-none-eabi-gdb
            --command=${GDBINIT_PATH}
            $<TARGET_FILE:${EXECUTABLE}>)


        add_dependencies(debug-${TARGET_NAME} ${TARGET_NAME})

    endfunction(gen_upload_target)

    # also create a target to run GDB server
    add_custom_target(start-gdbserver
        COMMENT "Starting pyOCD GDB server"
        COMMAND
        ${Python3_EXECUTABLE}
        -m pyocd
        gdbserver
        -v
        --no-wait
        -t ${PYOCD_TARGET_NAME}
        ${PYOCD_PROBE_ARGS}
        -f ${PYOCD_JTAG_SPEED}
        -p ${GDB_PORT}
        --persist
        --semihosting
        )

elseif("${UPLOAD_METHOD}" STREQUAL "OPENOCD")

    set(GENERATE_GDBINIT TRUE)

    function(gen_upload_target TARGET_NAME BIN_FILE)

        add_custom_target(flash-${TARGET_NAME}
            COMMENT "Flashing ${TARGET_NAME} with OpenOCD..."
            COMMAND ${OpenOCD}
            ${OPENOCD_CHIP_CONFIG_COMMANDS}
            -c init
            -c "reset init"
            -c "program ${BIN_FILE} reset exit")


        add_dependencies(flash-${TARGET_NAME} ${TARGET_NAME})

        # create debug target
        add_custom_target(debug-${TARGET_NAME}
            COMMENT "starting GDB to debug ${TARGET_NAME}..."
            COMMAND arm-none-eabi-gdb
            --command=${GDBINIT_PATH}
            $<TARGET_FILE:${EXECUTABLE}>)


        add_dependencies(debug-${TARGET_NAME} ${TARGET_NAME})

    endfunction(gen_upload_target)

    # also create a target to run GDB server
    add_custom_target(start-gdbserver
        COMMENT "Starting OpenOCD GDB server"
        COMMAND
        ${OpenOCD}
        ${OPENOCD_CHIP_CONFIG_COMMANDS}
        -c "gdb_port ${GDB_PORT}"
        -c init
        -c "reset init"
        )



elseif("${UPLOAD_METHOD}" STREQUAL NXPPROG)

    set(NXPPROG_COM_PORT "" CACHE STRING "COM port for bootloader upload.  Should be \"COMXX\" on Windows, and /dev/ttyXX on Linux/Mac")

    message(STATUS "COM port for bootloader upload: ${NXPPROG_COM_PORT}")

    function(gen_upload_target TARGET_NAME BIN_FILE)

        if("${NXPPROG_COM_PORT}" STREQUAL "")
            add_custom_target(flash-${TARGET_NAME}
                    COMMAND ${CMAKE_COMMAND}
                    -E echo "ERROR: Cannot flash, no COM_PORT configured!")
        else()
            add_custom_target(flash-${TARGET_NAME}
                COMMENT "Flashing ${TARGET_NAME} through bootloader..."
                COMMAND ${Python3_EXECUTABLE} ${MBED_CMAKE_SOURCE_DIR}/scripts/nxpprog.py
                    --control
                    --oscfreq=${NXPPROG_OSCFREQ}
                    --baud=${NXPPROG_BAUD}
                    ${NXPPROG_COM_PORT}
                    ${BIN_FILE})
        endif()

        add_dependencies(flash-${TARGET_NAME} ${TARGET_NAME})

    endfunction(gen_upload_target)

endif()

# Common actions
# -------------------------------------------------------------


if(GENERATE_GDBINIT)

    # create init file for GDB client
    file(GENERATE OUTPUT ${GDBINIT_PATH} CONTENT
        "
# connect to GDB server
target remote localhost:${GDB_PORT}
")
endif()
