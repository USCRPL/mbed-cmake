# This file is called by mbed-src/CMakeLists.txt when the GCC Arm compiler is used.

project(mbed-os)

# add command to preprocess linker script
get_filename_component(LINKER_SCRIPT_FILENAME "${MBED_LINKER_SCRIPT}" NAME_WE)
get_filename_component(LINKER_SCRIPT_DIR ${MBED_LINKER_SCRIPT} DIRECTORY)
set(LINKER_SCRIPT_EXT ".ld")

set(PREPROCESSED_LINKER_SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/${LINKER_SCRIPT_FILENAME}_preprocessed${LINKER_SCRIPT_EXT})

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

# force all compilations to include the compile definitions files
# note: SHELL prefixes are a workaround for an annoying CMake issue: https://gitlab.kitware.com/cmake/cmake/issues/15826
set(MBED_COMPILE_OPTIONS "SHELL:-include \"${MBED_CMAKE_CONFIG_HEADERS_PATH}/mbed_config.h\"" "SHELL:-include \"${MBED_CMAKE_CONFIG_HEADERS_PATH}/mbed_target_config.h\"")

# create the static library for the rest of the source files
add_library(mbed-os-static STATIC ${MBED_SOURCE_FILES})
target_compile_options(mbed-os-static PUBLIC ${MBED_COMPILE_OPTIONS})
target_include_directories(mbed-os-static PUBLIC ${MBED_INCLUDE_DIRS})

# make sure linker script gets built before anything trying to use mbed-os
add_dependencies(mbed-os-static mbed-linker-script)

# disable some annoying warnings during the Mbed build
target_compile_options(mbed-os-static PRIVATE
	$<$<COMPILE_LANGUAGE:CXX>:-Wno-reorder>
	-Wno-unused-function
	-Wno-unused-variable
	-Wno-enum-compare
	-Wno-attributes)

# create final library target (wraps actual library target in -Wl,--whole-archive [which is needed for weak symbols to work])
add_library(mbed-os INTERFACE)

target_link_libraries(mbed-os INTERFACE
	-Wl,--whole-archive
	mbed-os-static
	-Wl,--no-whole-archive
	-T\"${PREPROCESSED_LINKER_SCRIPT}\"
	${MBED_LINK_OPTIONS})
