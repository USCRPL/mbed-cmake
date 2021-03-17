
# Note: some code originally from here: https://os.mbed.com/cookbook/mbed-cmake

# needed for memory map script
check_python_package(intelhex HAVE_INTELHEX)
check_python_package(prettytable HAVE_PRETTYTABLE)

set(CAN_RUN_MEMAP TRUE)
if(NOT (HAVE_INTELHEX AND HAVE_PRETTYTABLE))
	message(WARNING "Unable to run the memory mapper due to missing Python dependencies.")
	set(CAN_RUN_MEMAP FALSE)
endif()

# Why doesn't memap use the same toolchain names as the rest of the Mbed build system?
# Don't ask me!
if("${MBED_TOOLCHAIN_NAME}" STREQUAL "ARMC6")
	set(MEMAP_TOOLCHAIN_NAME "ARM_STD")
else()
	set(MEMAP_TOOLCHAIN_NAME ${MBED_TOOLCHAIN_NAME})
endif()

# figure out objcopy command
get_filename_component(TOOLCHAIN_BIN_DIR ${CMAKE_C_COMPILER} DIRECTORY)

if("${MBED_TOOLCHAIN_NAME}" STREQUAL "ARMC6")
	set(OBJCOPY_NAME fromelf)
elseif("${MBED_TOOLCHAIN_NAME}" STREQUAL "GCC_ARM")
	set(OBJCOPY_NAME arm-none-eabi-objcopy)
endif()

find_program(
	OBJCOPY_EXECUTABLE
	NAMES ${OBJCOPY_NAME}
	HINTS ${TOOLCHAIN_BIN_DIR}
	DOC "Path to objcopy executable")

if(NOT EXISTS ${OBJCOPY_EXECUTABLE})
	message(FATAL_ERROR "Failed to find objcopy executable!  Please set OBJCOPY_EXECUTABLE to the path to ${OBJCOPY_NAME}")
endif()

#custom function to add a mbed executable and generate mbed source files
function(add_mbed_executable EXECUTABLE)

	add_executable(${EXECUTABLE} ${ARGN})

	target_link_libraries(${EXECUTABLE} mbed-os)

	set(BIN_FILE ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE}.bin)
	set(HEX_FILE ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE}.hex)

	if("${MBED_TOOLCHAIN_NAME}" STREQUAL "ARMC6")
		# the ArmClang CMake platform files automatically generate a memory map
		target_link_libraries(${EXECUTABLE} --info=totals --map)
		set(MAP_FILE ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE}${CMAKE_EXECUTABLE_SUFFIX}.map)

		set(OBJCOPY_ELF_TO_BIN_COMMAND ${OBJCOPY_EXECUTABLE} --bin -o ${BIN_FILE} $<TARGET_FILE:${EXECUTABLE}>)
		set(OBJCOPY_ELF_TO_HEX_COMMAND ${OBJCOPY_EXECUTABLE} --i32 -o ${HEX_FILE} $<TARGET_FILE:${EXECUTABLE}>)

	elseif("${MBED_TOOLCHAIN_NAME}" STREQUAL "GCC_ARM")

		set(MAP_FILE ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE}.map)

		# add link options to generate memory map
		target_link_libraries(${EXECUTABLE} -Wl,\"-Map=${MAP_FILE}\",--cref)

		set(OBJCOPY_ELF_TO_BIN_COMMAND ${OBJCOPY_EXECUTABLE} -O binary $<TARGET_FILE:${EXECUTABLE}> ${BIN_FILE})
		set(OBJCOPY_ELF_TO_HEX_COMMAND ${OBJCOPY_EXECUTABLE} -O ihex   $<TARGET_FILE:${EXECUTABLE}> ${HEX_FILE})

	endif()

	# generate .bin firmware file
	add_custom_command(
		TARGET ${EXECUTABLE} POST_BUILD
		COMMAND ${OBJCOPY_ELF_TO_BIN_COMMAND}
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Objcopying ${EXECUTABLE} to mbed firmware ${BIN_FILE}")

	# generate .hex firmware file
	add_custom_command(
		TARGET ${EXECUTABLE} POST_BUILD
		COMMAND ${OBJCOPY_ELF_TO_HEX_COMMAND}
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Objcopying ${EXECUTABLE} to mbed firmware ${HEX_FILE}")

	set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${BIN_FILE} ${MAP_FILE} ${HEX_FILE})

	if(CAN_RUN_MEMAP)
		add_custom_command(
			TARGET ${EXECUTABLE} POST_BUILD
			COMMAND ${Python3_EXECUTABLE} ${MBED_CMAKE_SOURCE_DIR}/mbed-src/tools/memap.py -t ${MEMAP_TOOLCHAIN_NAME} ${MAP_FILE}
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			COMMENT "Displaying memory map for ${EXECUTABLE}")
	endif()

	# add upload target
	gen_upload_target(${EXECUTABLE} ${BIN_FILE} ${HEX_FILE})

	# add debug target
	if(UPLOAD_SUPPORTS_DEBUG)

		# Generate IDE debug config if supported
		if(MBED_CMAKE_CLION)
			gen_clion_debug_configuration(${EXECUTABLE})
		elseif(MBED_CMAKE_VS_CODE)
			gen_vs_code_debug_configuration(${EXECUTABLE})
		else()

			# failing that, generate for command-line GDB
			add_custom_target(debug-${EXECUTABLE}
				COMMENT "starting GDB to debug ${EXECUTABLE}..."
				COMMAND arm-none-eabi-gdb
				--command=${GDBINIT_PATH}
				$<TARGET_FILE:${EXECUTABLE}>
				USES_TERMINAL)
		endif()
	endif()

endfunction(add_mbed_executable)

# When in not unit testing mode, we want buildfiles to still process OK.
# However, unit tests can't build for the MBed itself.
# So, add_mbed_unit_test is a no-op.
function(add_mbed_unit_test EXECUTABLE)

	# unit tests active -- create a dummy target that only exists so setting properties on it doesn't fail.
	add_executable(${EXECUTABLE} ${MBED_CMAKE_SOURCE_DIR}/cmake/dummy.cpp)

	# prevent this target from being built during normal make commands
	set_property(TARGET ${EXECUTABLE} PROPERTY EXCLUDE_FROM_ALL TRUE)

endfunction(add_mbed_unit_test)
