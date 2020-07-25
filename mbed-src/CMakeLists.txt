
# include correct mbed-cmake buildfile based on toolchain
if("${MBED_TOOLCHAIN_NAME}" STREQUAL "ARMC6")
	include(MBedOS-ArmClang)
elseif("${MBED_TOOLCHAIN_NAME}" STREQUAL "GCC_ARM")
	include(MBedOS-GCCArm)
endif()


