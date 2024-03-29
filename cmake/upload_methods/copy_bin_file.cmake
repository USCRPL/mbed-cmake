
# CMake script which is invoked at install time to copy a bin file onto the mbed

# arguments:
# BIN_FILE     bin file to install
# MBED_PATH    path to Mbed virtual thumb drive on the computer

if(NOT (IS_DIRECTORY "${MBED_PATH}" AND EXISTS "${MBED_PATH}"))
	message(FATAL_ERROR "No mbed detected at MBED_PATH \"${MBED_PATH}\" \
Did you set MBED_PATH in CMake?
Did you try turning it on and off again?
Is it plugged in?")
	return()
endif()

# remove all other bin files on the device
file(GLOB OLD_BIN_FILES "${MBED_PATH}/*.bin")

foreach(FILE ${OLD_BIN_FILES})
	file(REMOVE "${FILE}")
	message(STATUS "Removed old bin file: ${FILE}")
endforeach()

# now copy the new bin file in place
message(STATUS "Installing to mbed (${MBED_PATH}): ${BIN_FILE}")

# if we have the cp program, use that because it seems to have better behavior on Windows and WSL
# with mounted paths (see mbed-cmake#32).  This may require people to install cp manually
# on Windows if they have issues.
find_program(CP_COMMAND NAMES cp)
if(CP_COMMAND)
	message("${CP_COMMAND} \"${BIN_FILE}\" \"${MBED_PATH}\"")
	execute_process(COMMAND ${CP_COMMAND} "${BIN_FILE}" "${MBED_PATH}")
else()
	file(COPY "${BIN_FILE}" DESTINATION "${MBED_PATH}")
endif()
