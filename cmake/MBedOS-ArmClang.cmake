# This file is called by mbed-src/CMakeLists.txt when the ArmClang compiler is used.

project(mbed-os)

get_filename_component(LINKER_SCRIPT_FILENAME "${MBED_LINKER_SCRIPT}" NAME_WE)
get_filename_component(LINKER_SCRIPT_DIR ${MBED_LINKER_SCRIPT} DIRECTORY)
get_filename_component(LINKER_SCRIPT_EXT ${MBED_LINKER_SCRIPT} LAST_EXT)

set(PREPROCESSED_LINKER_SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/${LINKER_SCRIPT_FILENAME}_fixed${LINKER_SCRIPT_EXT})

# for Arm Compiler, all we need to do is replace the shebang at the top with the correct command line
set(PREPROCESS_ARGS ${MCU_COMPILE_OPTIONS} ${ARMCLANG_CPU_OPTION} -E -x c)

# build the shebang command following the logic in arm.py
list_to_space_separated(SHEBANG_COMMAND ${PREPROCESS_ARGS})
set(SHEBANG_COMMAND "#! \"${CMAKE_C_COMPILER}\" ${SHEBANG_COMMAND} -I\"${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_SCRIPT_DIR}\"")

# now replace it into the file
file(READ ${MBED_LINKER_SCRIPT} LINKER_SCRIPT_CONTENTS)
string(REGEX REPLACE "^#![^\n]+(.*)$" "${SHEBANG_COMMAND}\\1" PREPROC_LINKER_SCRIPT_CONTENTS "${LINKER_SCRIPT_CONTENTS}")
file(GENERATE OUTPUT ${PREPROCESSED_LINKER_SCRIPT} CONTENT "${PREPROC_LINKER_SCRIPT_CONTENTS}")

# force all compilations to include the compile definitions files
# note: SHELL prefixes are a workaround for an annoying CMake issue: https://gitlab.kitware.com/cmake/cmake/issues/15826
set(MBED_COMPILE_OPTIONS "SHELL:-include \"${MBED_CMAKE_CONFIG_HEADERS_PATH}/mbed_config.h\"" "SHELL:-include \"${MBED_CMAKE_CONFIG_HEADERS_PATH}/mbed_target_config.h\"")

# convert include args for passing to preprocessor manually
set(PREPROCESSOR_INCLUDE_ARGS
	-include ${MBED_CMAKE_CONFIG_HEADERS_PATH}/mbed_config.h
	-include ${MBED_CMAKE_CONFIG_HEADERS_PATH}/mbed_target_config.h)

foreach(INCLUDE_DIR ${MBED_INCLUDE_DIRS})
	list(APPEND PREPROCESSOR_INCLUDE_ARGS "-I${INCLUDE_DIR}")
endforeach()

# Annoyingly, the ARMC6 assembler does not do preprocessing.  So we have to give it a little help.
# This finds all assembler files (.S) in the source list, and adds a custom command to preprocess them.
# Also, the assembler files need to be linked as objects into the final version (something with the provided linker scripts???),
# they can't be in a .a file, so we have to build them separately.
set(OLD_MBED_SOURCE_FILES ${MBED_SOURCE_FILES})
set(MBED_SOURCE_FILES "")
set(ASM_SOURCE_FILES "")
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
			COMMAND ${CMAKE_C_COMPILER} ${PREPROCESS_ARGS} ${PREPROCESSOR_INCLUDE_ARGS} ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILE} -o ${PREPROCESSED_SOURCE_FILE}
			COMMENT "Preprocessing ${SOURCE_FILE}"
			VERBATIM)

		list(APPEND ASM_SOURCE_FILES ${PREPROCESSED_SOURCE_FILE})

	else()
		# not ASM file, keep
		list(APPEND MBED_SOURCE_FILES ${SOURCE_FILE})
	endif()
endforeach()

# Let's just say that Arm Compiler is not a big fan of .a libraries, at least when they contain things like
# weak symbols and code that calls ASM.
# I tried using a static library at first, and that just sent things into a massive bootloop that poured a
# deluge of crash reports through my terminal.
# Soooo... let's NOT do that.

add_library(mbed-os-obj OBJECT ${ASM_SOURCE_FILES} ${MBED_SOURCE_FILES})
target_compile_options(mbed-os-obj PUBLIC ${MBED_COMPILE_OPTIONS})
target_include_directories(mbed-os-obj PUBLIC ${MBED_INCLUDE_DIRS})

# create final library target (interface which contains objects, link options, and compile flags)
add_library(mbed-os INTERFACE)
target_link_libraries(mbed-os INTERFACE
	--scatter ${PREPROCESSED_LINKER_SCRIPT}
	${MBED_LINK_OPTIONS})
target_sources(mbed-os INTERFACE $<TARGET_OBJECTS:mbed-os-obj>)
target_compile_options(mbed-os INTERFACE ${MBED_COMPILE_OPTIONS})
target_include_directories(mbed-os INTERFACE ${MBED_INCLUDE_DIRS})