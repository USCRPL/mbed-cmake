# mbed-cmake requires CMake >= 3.12 for the new FindPython3 module
cmake_policy(VERSION 3.12)

message(STATUS [==[ _       _    ____      ______   ___                      ____    _       _         _        _      _   ______  ]==])
message(STATUS [==[| \     / |  /  _  \   /  ____) /  _ \_                 / ____)  | \     / |       / \      / \    / | /  ____) ]==])
message(STATUS [==[|  \   /  |  | |_|  |  | |____  | | \_ \    ______    /  /       |  \   /  |      / _ \     | |   / /  | |____  ]==])
message(STATUS [==[|   \ /   |  |     /   |  ____) | |   | |  (______)  |  |        |   \ /   |     / /_\ \    |  _ / /   |  ____) ]==])
message(STATUS [==[| |\ - /| |  |  _   \  | |      | |  _| |             \  \       | |\ - /| |    / _____ \   |  _  \    | |      ]==])
message(STATUS [==[| | \_/ | |  | |_|   | | |____  | |_/ _/                \ \___   | | \_/ | |   / /     \ \  | |  \ \   | |____  ]==])
message(STATUS [==[\_/     \_/  \_____ /  \______) \___ /                   \____)  \_/     \_/  |_/       \_| \_/   \_/  \______) ]==])
message(STATUS "")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

include(Shorthand)
include(Utils)
include(CheckPythonPackage)
include(FindPackageHandleStandardArgs)

# build configuration
# -------------------------------------------------------------
# default to RelWithDebInfo to match MBed OS default
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
		"Type of build, options are: Debug, RelWithDebInfo, Release" FORCE)
endif()

# Allowed build types are Debug and Release
set(CMAKE_CONFIGURATION_TYPES Debug RelWithDebInfo Release)

# check configuration files
# -------------------------------------------------------------

set(MBED_CMAKE_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

# relative to CMakeLists.txt inside mbed-cmake
if(NOT DEFINED MBED_CMAKE_GENERATED_CONFIG_RELPATH)
	set(MBED_CMAKE_GENERATED_CONFIG_RELPATH "../mbed-cmake-config")
endif()
set(MBED_CMAKE_GENERATED_CONFIG_PATH ${MBED_CMAKE_SOURCE_DIR}/${MBED_CMAKE_GENERATED_CONFIG_RELPATH})

if(NOT EXISTS ${MBED_CMAKE_GENERATED_CONFIG_PATH}/cmake/MBedOSConfig.cmake)
	message(FATAL_ERROR "MBed config files and headers do not exist!  You need to run mbed-cmake/configure_for_target.py from the top source dir!")
endif()


# load compilers and flags
# -------------------------------------------------------------
include(mbed_gcc_arm_toolchain)

# search for and load compilers
enable_language(C CXX ASM)

# Set config-specific flags
# Note: unlike CMAKE_C_FLAGS etc, we need to set these AFTER the project() call in order to override CMake's default values for these flags.
# CMAKE_C_FLAGS_INIT etc need to be set BEFORE the project() call so that it's used when CMake scans compiler properties.
foreach(BUILD_TYPE RELWITHDEBINFO DEBUG RELEASE)
	list_to_space_separated(CMAKE_C_FLAGS_${BUILD_TYPE} ${MCU_COMPILE_OPTIONS_${BUILD_TYPE}})
	list_to_space_separated(CMAKE_CXX_FLAGS_${BUILD_TYPE} ${MCU_COMPILE_OPTIONS_${BUILD_TYPE}})
	list_to_space_separated(CMAKE_ASM_FLAGS_${BUILD_TYPE} ${MCU_COMPILE_OPTIONS_${BUILD_TYPE}})
endforeach()

# Set language standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS TRUE)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS TRUE)

# find python (used for memap and several upload methods)
# -------------------------------------------------------------

find_package(Python3 COMPONENTS Interpreter REQUIRED)


# load the MBed CMake functions
# -------------------------------------------------------------

include(MBedExecutable)

# Configure upload methods
# -------------------------------------------------------------

# find upload tools
find_package(OpenOCD)
find_package(JLINK)

include(UploadMethods)

# add MBed OS source
# -------------------------------------------------------------

set(MBED_CMAKE_CONFIG_HEADERS_PATH ${MBED_CMAKE_GENERATED_CONFIG_PATH}/config-headers)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/mbed-src) #first get MBed standard library

# build report
# -------------------------------------------------------------
function(mbed_cmake_print_build_report)

	message(STATUS "---- Completed configuration of ${PROJECT_NAME} ----")

	# build type
	message(STATUS ">> Current Build Type: ${CMAKE_BUILD_TYPE}")

	# build flags
	string(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_UCASE)
	message(STATUS ">> C   Compile Flags (Global): ${CMAKE_C_FLAGS}")
	message(STATUS ">> C   Compile Flags (For ${CMAKE_BUILD_TYPE}): ${CMAKE_C_FLAGS_${CMAKE_BUILD_TYPE_UCASE}}")
	message(STATUS ">> CXX Compile Flags (Global): ${CMAKE_CXX_FLAGS}")
	message(STATUS ">> CXX Compile Flags (For ${CMAKE_BUILD_TYPE}): ${CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE_UCASE}}")
	message(STATUS ">> Linker Flags: ${CMAKE_EXE_LINKER_FLAGS}")

endfunction(mbed_cmake_print_build_report)