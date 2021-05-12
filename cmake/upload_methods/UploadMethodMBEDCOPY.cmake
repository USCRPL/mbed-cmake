### Mbed USB Drive Manual Copy Upload Method
# This method directly copies the bin file over to MBED_PATH

set(MBED_PATH "" CACHE PATH "Path to mbed virtual device to upload to")

#always found on the device
set(UPLOAD_MBEDCOPY_FOUND TRUE)

function(gen_upload_target TARGET_NAME BIN_FILE)

    add_custom_target(flash-${TARGET_NAME}
            COMMAND ${CMAKE_COMMAND}
            -DBIN_FILE=${BIN_FILE}
            -DMBED_PATH=${MBED_PATH}
            -P ${MBED_CMAKE_SOURCE_DIR}/cmake/upload_methods/copy_bin_file.cmake)

    add_dependencies(flash-${TARGET_NAME} ${TARGET_NAME})

endfunction(gen_upload_target)