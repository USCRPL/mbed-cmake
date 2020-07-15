
# Note: some code originally from here: https://os.mbed.com/cookbook/mbed-cmake

# needed for memory map script
check_python_package(intelhex HAVE_INTELHEX)
check_python_package(prettytable HAVE_PRETTYTABLE)

set(CAN_RUN_MEMAP TRUE)
if(NOT (HAVE_INTELHEX AND HAVE_PRETTYTABLE))
	message(WARNING "Unable to run the memory mapper due to missing Python dependencies.")
	set(CAN_RUN_MEMAP FALSE)
endif()

#custom function to add a mbed executable and generate mbed source files
function(add_mbed_executable EXECUTABLE)
	
	add_executable(${EXECUTABLE} ${ARGN})

	target_link_libraries(${EXECUTABLE} mbed-os)
	
	set(BIN_FILE ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE}.bin)
	set(MAP_FILE ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE}.map)

	# generate .bin firmware file
	add_custom_command(
		TARGET ${EXECUTABLE} POST_BUILD
		COMMAND arm-none-eabi-objcopy -O binary $<TARGET_FILE:${EXECUTABLE}> ${BIN_FILE}
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Objcopying ${EXECUTABLE} to mbed firmware ${BIN_FILE}")
	
	# add link options to generate memory map
	target_link_libraries(${EXECUTABLE} -Wl,\"-Map=${MAP_FILE}\",--cref)

	set_property(DIRECTORY PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${BIN_FILE} ${EXECUTABLE}.map)

	if(CAN_RUN_MEMAP)
		add_custom_command(
			TARGET ${EXECUTABLE} POST_BUILD
			COMMAND ${Python3_EXECUTABLE} ${MBED_CMAKE_SOURCE_DIR}/scripts/memap/memap.py -t GCC_ARM ${MAP_FILE}
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			COMMENT "Displaying memory map for ${EXECUTABLE}")
	endif()

	# add upload target
	gen_upload_target(${EXECUTABLE} ${BIN_FILE})

endfunction(add_mbed_executable)

 
