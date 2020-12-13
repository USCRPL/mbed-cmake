# ----------------------------------------------
# Load the upload method that the user selects

if(NOT DEFINED UPLOAD_METHOD_DEFAULT)
	message(WARNING "Please define UPLOAD_METHOD_DEFAULT in your CMakeLists.txt to the upload method you wish to use for this project")
endif()

set(UPLOAD_METHOD "${UPLOAD_METHOD_DEFAULT}" CACHE STRING "Method for uploading programs to the mbed")

include(UploadMethod${UPLOAD_METHOD})

if(NOT "${UPLOAD_${UPLOAD_METHOD}_FOUND}")
	message(FATAL_ERROR "The upload method ${UPLOAD_METHOD} does not have the dependencies needed to run on this machine.")
endif()

if(NOT "${${UPLOAD_METHOD}_UPLOAD_ENABLED}")
	message(FATAL_ERROR "The upload method ${UPLOAD_METHOD} is not enabled in the config code for this target -- set ${UPLOAD_METHOD}_UPLOAD_ENABLED to TRUE to enable it.")
endif()

message(STATUS "Board upload method set to ${UPLOAD_METHOD}")

# ----------------------------------------------
# Generate gdbinit if needed

# path where the gdbinit file will be written
set(GDBINIT_PATH ${CMAKE_CURRENT_BINARY_DIR}/mbed-cmake.gdbinit)

if(UPLOAD_SUPPORTS_DEBUG)
	# create init file for GDB client
	file(GENERATE OUTPUT ${GDBINIT_PATH} CONTENT
"# connect to GDB server
target remote localhost:${GDB_PORT}
")
endif()