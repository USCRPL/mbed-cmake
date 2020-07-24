# CMake toolchain file to set compiler for the ArmClang toolchain

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
set(CMAKE_C_COMPILER armclang)
set(CMAKE_CXX_COMPILER armclang)

set(CMAKE_C_COMPILER_ID ARMClang)
set(CMAKE_CXX_COMPILER_ID ARMClang)

set(OLD_MCU_COMPILE_OPTIONS ${MCU_COMPILE_OPTIONS})
set(MCU_COMPILE_OPTIONS "")
foreach(OPTION ${OLD_MCU_COMPILE_OPTIONS})

    # remove some MBed flags that shouldn't be in build commands (CMake auto adds these)
    if(("${OPTION}" STREQUAL "-c") OR
        ("${OPTION}" STREQUAL "-MMD"))
        # don't add
    elseif("${OPTION}" MATCHES "--target=(.*)")

        # parse out CPU target and tell CMake about it
        set(CMAKE_C_COMPILER_TARGET ${CMAKE_MATCH_1})
        set(CMAKE_CXX_COMPILER_TARGET ${CMAKE_MATCH_1})
        set(CMAKE_ASM_COMPILER_TARGET ${CMAKE_MATCH_1})

        list(APPEND MCU_COMPILE_OPTIONS ${OPTION})

    elseif("${OPTION}" MATCHES "-mcpu=(.*)")

        # parse out CPU name and set CMAKE_SYSTEM_PROCESSOR to it to prevent CMake error
        set(CMAKE_SYSTEM_PROCESSOR ${CMAKE_MATCH_1})
    else()

        # no special handling for this arg
        list(APPEND MCU_COMPILE_OPTIONS ${OPTION})
    endif()
endforeach()

#set(OLD_MCU_LINK_OPTIONS ${MCU_LINK_OPTIONS})
#set(MCU_LINK_OPTIONS "")
#foreach(OPTION ${OLD_MCU_LINK_OPTIONS})
#    # mbed-studio provides armlink flags directly, but cmake links using armclang as the driver.
#    list(APPEND MCU_LINK_OPTIONS -Wl,${OPTION})
#endforeach()

# convert list to space-separated for CMAKE_C_FLAGS etc.
list_to_space_separated(CHIP_FLAGS ${MCU_COMPILE_OPTIONS})

printvar(MCU_COMPILE_OPTIONS)

set(CHIP_LINKER_FLAGS "")

# certain flags will only work if linking with MBed OS, not for regular executables, so we need to strip those out so CMake can configure
set(MBED_LINK_OPTIONS "")

foreach(FLAG ${MCU_LINK_OPTIONS})
    if("${FLAG}" IN_LIST MCU_COMPILE_FLAGS)
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
set(CMAKE_CXX_FLAGS_INIT "${CHIP_FLAGS}")
set(CMAKE_ASM_FLAGS_INIT "${CHIP_FLAGS}")

set(CMAKE_EXE_LINKER_FLAGS_INIT "${CHIP_LINKER_FLAGS}")
