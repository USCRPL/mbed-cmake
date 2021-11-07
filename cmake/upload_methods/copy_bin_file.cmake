
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
# file(COPY "${BIN_FILE}" DESTINATION "${MBED_PATH}")
# file(COPY "${BIN_FILE}" DESTINATION "/mnt/d")
execute_process(COMMAND cp "${BIN_FILE}" "${MBED_PATH}")
