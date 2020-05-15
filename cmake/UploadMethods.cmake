# CMakeLists file to handle the different upload methods to the hamster board

# Here's a list of currently added upload methods, and all the parameters they accept.
# Things labeled as parameters should be set in your buildscript, as they will be constant for a given board.
# However, things labeled options are things the user may need to configure at runtime and are created as cache variables.

# MBED - Simple uploader that uploads code via an MBed device's semihosting USB port.
#   Options:
#     MBED_PATH - Drive letter or local path that the MBed is mounted to.  No default
# JLINK - Uploader that connects via a J-Link JTAG box.  Supports both flashing and debugging.
#   Parameters:
#     JLINK_CPU_NAME - Name that your processor is known by to J-link.  These are listed here: https://www.segger.com/downloads/supported-devices.php
#     JLINK_RESET_TARGET - Whether to reset the target after upload.
#     JLINK_JTAG_SPEED - Clock speed of the JTAG connection.  Accepts either a speed in kHz or "adaptive" to automatically determine speed using the RTCK pin.
#   Options:
#     GDB_PORT - Network port that JLink gdbserver and gdb will be run on.
# NXPPROG - Uploader that uses the bootloader on certain NXP 1XXX and 2XXX processors.  Requires ISP and reset pins to be connected to the serial port for automatic flashing.
#           Specifically, a low on RESET should be triggered by a high on DTR, and a low on ISP should be triggered by a low on RTS.
#   Options:
#     NXP_COM_PORT - Serial port that the NXP processor is accessible on.
#   Parameters:
#     NXP_BAUD - Baudrate to talk to the bootloader over.
#     NXP_OSCFREQ - The bootloader needs to be sent the chip's internal oscillator frequency in order to operate.  See datasheet for details.
# PYOCD - Uploader that uses the pyOCD flashing and debugging program.
#   Parameters:
#     PYOCD_JTAG_SPEED - Clock speed of the JTAG connection.  Default is in Hz, but can use k and M suffixes for MHz and GHz

# Step 1: determine supported upload methods
# -------------------------------------------------------------

# Start with NONE upload method which does not allow any uploading
set(SUPPORTED_UPLOAD_METHODS NONE)

# MBed is always supported
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
                -P ${CMAKE_SOURCE_DIR}/cmake/install_bin_file.cmake)

        add_dependencies(flash-${TARGET_NAME} ${TARGET_NAME})

    endfunction(gen_upload_target)

elseif("${UPLOAD_METHOD}" STREQUAL "JLINK")

    set(GENERATE_GDBINIT TRUE)

    function(gen_upload_target TARGET_NAME BIN_FILE)

        set(JLINK_RESET_COMMAND "")
        if(JLINK_RESET_TARGET)
            set(JLINK_RESET_COMMAND "r")
        endif()

        # create command file for j-link
        set(COMMAND_FILE_PATH ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/flash-${TARGET_NAME}.jlink)
        file(GENERATE OUTPUT ${COMMAND_FILE_PATH} CONTENT
"
loadbin ${BIN_FILE}, 0x0
${JLINK_RESET_COMMAND}
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
        -Device LPC1768 
        -Speed 4000 
        -endian little 
        -if JTAG 
        -JTAGConf -1,-1 
        -LocalhostOnly
        -noIR
        -port ${GDB_PORT}
        )

elseif("${UPLOAD_METHOD}" STREQUAL "PYOCD")

    set(GENERATE_GDBINIT TRUE)

    function(gen_upload_target TARGET_NAME BIN_FILE)

        add_custom_target(flash-${TARGET_NAME}
            COMMENT "Flashing ${TARGET_NAME} with pyOCD..."
            COMMAND ${Python3_EXECUTABLE}
            -m pyocd
            flash
            -v
            --no-wait
            -t ${MBED_TARGET_NAME}
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
        -t ${MBED_TARGET_NAME}
        -f 4000k
        -p ${GDB_PORT}
        --persist
        --semihosting
        )

elseif("${UPLOAD_METHOD}" STREQUAL "OPENOCD")

    set(GENERATE_GDBINIT TRUE)

    set(OPENOCD_CHIP_CONFIG_COMMANDS
        -f ${OpenOCD_SCRIPT_DIR}/interface/cmsis-dap.cfg
        -f ${OpenOCD_SCRIPT_DIR}/target/lpc17xx.cfg
        -c "gdb_memory_map disable")

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

    set(COM_PORT "" CACHE STRING "COM port for bootloader upload.  Should be \"COMXX\" on Windows, and /dev/ttyXX on Linux/Mac")

    message(STATUS "COM port for bootloader upload: ${COM_PORT}")

    function(gen_upload_target TARGET_NAME BIN_FILE)

        if("${COM_PORT}" STREQUAL "")
            add_custom_target(flash-${TARGET_NAME}
                    COMMAND ${CMAKE_COMMAND}
                    -E echo "ERROR: Cannot flash, no COM_PORT configured!")
        else()
            add_custom_target(flash-${TARGET_NAME}
                COMMENT "Flashing ${TARGET_NAME} through bootloader..."
                COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/nxpprog.py
                    --control
                    --oscfreq=14748
                    --baud=115200
                    ${COM_PORT}
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