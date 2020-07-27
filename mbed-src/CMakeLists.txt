
# include correct mbed-cmake buildfile based on toolchain
if(MBED_UNITTESTS)
	include(MBedOS-UnitTests)
elseif("${MBED_TOOLCHAIN_NAME}" STREQUAL "ARMC6")
	include(MBedOS-ArmClang)
elseif("${MBED_TOOLCHAIN_NAME}" STREQUAL "GCC_ARM")
	include(MBedOS-GCCArm)
endif()


