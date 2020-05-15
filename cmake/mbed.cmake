
# Note: some code originally from here: https://os.mbed.com/cookbook/mbed-cmake

#custom function to add a mbed executable and generate mbed source files
function(add_mbed_executable EXECUTABLE)
	
	add_executable(${EXECUTABLE} ${ARGN})

	target_link_libraries(${EXECUTABLE} mbed-os)
	
	set(BIN_FILE ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE}.bin)
	set(MAP_FILE ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE}.map)
	
	# add link options to generate memory map
	target_link_libraries(${EXECUTABLE} -Wl,\"-Map=${MAP_FILE}\",--cref)

	# generate .bin firmware file
	add_custom_command(
		TARGET ${EXECUTABLE} POST_BUILD
		COMMAND arm-none-eabi-objcopy -O binary $<TARGET_FILE:${EXECUTABLE}> ${BIN_FILE}
		COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/memap/memap.py -t GCC_ARM ${MAP_FILE}
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Objcopying ${EXECUTABLE} to mbed firmware ${BIN_FILE}")
		
	set_property(DIRECTORY PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${BIN_FILE} ${EXECUTABLE}.map)
	
	# add upload target
	gen_upload_target(${EXECUTABLE} ${BIN_FILE})

endfunction(add_mbed_executable)

 
