
# include correct mbed-cmake buildfile based on toolchain
if(MBED_UNITTESTS)
	include(MbedOS-UnitTests)
elseif("${MBED_TOOLCHAIN_NAME}" STREQUAL "ARMC6")
	include(MbedOS-ArmClang)
elseif("${MBED_TOOLCHAIN_NAME}" STREQUAL "GCC_ARM")
	include(MbedOS-GCCArm)
endif()


