project(mbed-os)

# add command to preprocess linker script
get_filename_component(LINKER_SCRIPT_FILENAME "${MBED_LINKER_SCRIPT}" NAME_WE)
get_filename_component(LINKER_SCRIPT_DIR ${MBED_LINKER_SCRIPT} DIRECTORY)
get_filename_component(LINKER_SCRIPT_EXT ${MBED_LINKER_SCRIPT} LAST_EXT)

set(PREPROCESSED_LINKER_SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/${LINKER_SCRIPT_FILENAME}_preprocessed${LINKER_SCRIPT_EXT})

if("${MBED_TOOLCHAIN_NAME}" STREQUAL "ARMC6")

	# for Arm Compiler, all we need to do is replace the shebang at the top with the correct command line
	set(PREPROCESS_COMMAND ${CMAKE_C_COMPILER} ${MCU_COMPILE_OPTIONS} ${ARMCLANG_CPU_OPTION} -E -x c)

	# build the shebang command following the logic in arm.py
	list_to_space_separated(SHEBANG_COMMAND ${PREPROCESS_COMMAND})
	set(SHEBANG_COMMAND "#! ${SHEBANG_COMMAND} -I\"${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_SCRIPT_DIR}\"")

	# now replace it into the file
	file(READ ${MBED_LINKER_SCRIPT} LINKER_SCRIPT_CONTENTS)
	string(REGEX REPLACE "^#![^\n]+(.*)$" "${SHEBANG_COMMAND}\\1" PREPROC_LINKER_SCRIPT_CONTENTS ${LINKER_SCRIPT_CONTENTS})
	file(GENERATE OUTPUT ${PREPROCESSED_LINKER_SCRIPT} CONTENT ${PREPROC_LINKER_SCRIPT_CONTENTS})

elseif("${MBED_TOOLCHAIN_NAME}" STREQUAL "GCC_ARM")

	# NOTE: clearly the linker script is not assembly!  We give -x assembler-with-cpp to force GCC to try and compile the file
	# as if it was assembly.  It only gets to the preprocessor stage because of -E, and everything works fine.

	set(PREPROCESS_ONLY_FLAGS -x assembler-with-cpp -E -Wp,-P ${CMAKE_EXE_LINKER_FLAGS})

	# This is kind of a hack, but without it we would have to have a seperate facility for finding the preprocessor executable.
	# We also needed to pass the linker flags because they can include defines needed in the linker script
	add_custom_command(
		OUTPUT ${PREPROCESSED_LINKER_SCRIPT}
		COMMAND ${CMAKE_CXX_COMPILER} ${PREPROCESS_ONLY_FLAGS} ${CMAKE_CURRENT_SOURCE_DIR}/${MBED_LINKER_SCRIPT} -o ${PREPROCESSED_LINKER_SCRIPT}
		MAIN_DEPENDENCY ${MBED_LINKER_SCRIPT}
		COMMENT "Preprocessing linker script ${MBED_LINKER_SCRIPT} -> ${PREPROCESSED_LINKER_SCRIPT}"
		VERBATIM)

	# add target so we can force this command to be run before mbed is compiled
	add_custom_target(
		mbed-linker-script
		DEPENDS ${PREPROCESSED_LINKER_SCRIPT})

endif()


# force all compilations to include the compile definitions files
# note: SHELL prefixes are a workaround for an annoying CMake issue: https://gitlab.kitware.com/cmake/cmake/issues/15826
if(NOT DEFINED MBED_CMAKE_CONFIG_HEADERS_PATH)
	set(MBED_CMAKE_CONFIG_HEADERS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/config-headers)
endif()

set(MBED_COMPILE_OPTIONS "SHELL:-include \"${MBED_CMAKE_CONFIG_HEADERS_PATH}/mbed_config.h\"" "SHELL:-include \"${MBED_CMAKE_CONFIG_HEADERS_PATH}/mbed_target_config.h\"")

if("${MBED_TOOLCHAIN_NAME}" STREQUAL "ARMC6")

	# Annoyingly, the ARMC6 assembler does not do preprocessing.  So we have to give it a little help.
	# This finds all assembler files (.S) in the source list, and adds a custom command to preprocess them.
	set(OLD_MBED_SOURCE_FILES ${MBED_SOURCE_FILES})
	set(MBED_SOURCE_FILES "")
	foreach(SOURCE_FILE ${OLD_MBED_SOURCE_FILES})
		get_filename_component(SOURCE_EXT ${SOURCE_FILE} LAST_EXT)

		if("${SOURCE_EXT}" STREQUAL ".S")

			# ASM file -- time to go DIY
			get_filename_component(SOURCE_FILENAME ${SOURCE_FILE} NAME_WE)
			get_filename_component(SOURCE_DIR ${SOURCE_FILE} DIRECTORY)

			set(PREPROCESSED_SOURCE_FILE_DIR  ${CMAKE_CURRENT_BINARY_DIR}/${SOURCE_DIR})
			file(MAKE_DIRECTORY ${PREPROCESSED_SOURCE_FILE_DIR})
			set(PREPROCESSED_SOURCE_FILE ${PREPROCESSED_SOURCE_FILE_DIR}/${SOURCE_FILENAME}_preproc.S)

			add_custom_command(
				OUTPUT ${PREPROCESSED_SOURCE_FILE}
				MAIN_DEPENDENCY ${SOURCE_FILE}
				COMMAND ${PREPROCESS_COMMAND} ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILE} -o ${PREPROCESSED_SOURCE_FILE}
				COMMENT "Preprocessing ${SOURCE_FILE}"
				VERBATIM)

			list(APPEND MBED_SOURCE_FILES ${PREPROCESSED_SOURCE_FILE})

		else()
			# not ASM file, keep
			list(APPEND MBED_SOURCE_FILES ${SOURCE_FILE})
		endif()
	endforeach()

endif()

# create the static library for the rest of the source files
add_library(mbed-os-static STATIC ${MBED_SOURCE_FILES})
target_compile_options(mbed-os-static PUBLIC ${MBED_COMPILE_OPTIONS})
target_include_directories(mbed-os-static PUBLIC ${MBED_INCLUDE_DIRS})

# make sure linker script gets built before anything trying to use mbed-os
if("${MBED_TOOLCHAIN_NAME}" STREQUAL "GCC_ARM")
	add_dependencies(mbed-os-static mbed-linker-script)
endif()

if("${MBED_TOOLCHAIN_NAME}" STREQUAL "GCC_ARM")
	# disable some annoying warnings during the MBed build
	target_compile_options(mbed-os-static PRIVATE
		$<$<COMPILE_LANGUAGE:CXX>:-Wno-reorder>
		-Wno-unused-function
		-Wno-unused-variable
		-Wno-enum-compare
		-Wno-attributes)
endif()

# create final library target (wraps actual library target in -Wl,--whole-archive [which is needed for weak symbols to work])
add_library(mbed-os INTERFACE)
target_link_libraries(mbed-os INTERFACE
	-Wl,--whole-archive
	mbed-os-static
	-Wl,--no-whole-archive
	-T\"${PREPROCESSED_LINKER_SCRIPT}\"
	${MBED_LINK_OPTIONS})