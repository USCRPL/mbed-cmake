# CMake toolchain file to set compiler for the gcc-arm toolchain

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_EXECUTABLE_SUFFIX elf)

# specify the cross compiler
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)

# remove some Mbed flags that shouldn't be in build commands (CMake will handle these)
list(REMOVE_ITEM MCU_COMPILE_OPTIONS -c -MMD)

# convert list to space-separated for CMAKE_C_FLAGS etc.
list_to_space_separated(CHIP_FLAGS ${MCU_COMPILE_OPTIONS})

set(CHIP_LINKER_FLAGS "")

# certain flags will only work if linking with Mbed OS, not for regular executables, so we need to strip those out so CMake can configure
set(MBED_LINK_OPTIONS "")

foreach(FLAG ${MCU_LINK_OPTIONS})
    if("${FLAG}" MATCHES "--wrap")
        list(APPEND MBED_LINK_OPTIONS "${FLAG}")
    elseif("${FLAG}" IN_LIST MCU_COMPILE_FLAGS)
        # duplicate flag, do nothing
    else()
        string(APPEND CHIP_LINKER_FLAGS " ${FLAG}")
    endif()
endforeach()

# also add config-specific link flags (these go with mbed-os since they need generator expressions)
foreach(BUILD_TYPE ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER "${BUILD_TYPE}" BUILD_TYPE_UCASE)
    foreach(OPTION ${MCU_LINK_OPTIONS_${BUILD_TYPE_UCASE}})
        if(NOT "${OPTION}" STREQUAL "")
            list(APPEND MBED_LINK_OPTIONS $<$<CONFIG:${BUILD_TYPE}>:${OPTION}>)
        endif()
    endforeach()
endforeach()

#note: these initialize the cache variables on first configure
set(CMAKE_C_FLAGS_INIT "${CHIP_FLAGS}")
set(CMAKE_CXX_FLAGS_INIT "${CHIP_FLAGS} -Wvla -fno-exceptions -fno-rtti")
set(CMAKE_ASM_FLAGS_INIT "${CHIP_FLAGS}")

set(CMAKE_EXE_LINKER_FLAGS_INIT "${CHIP_LINKER_FLAGS} --specs=nosys.specs -Wl,--start-group -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys -Wl,--end-group")
