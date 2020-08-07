

# When in unit testing mode, we want buildfiles to still process OK.
# However, executables can't link correctly because mbed-os is stubbed out as headers.
# So, while libraries build normally, add_mbed_executable is a no-op.
function(add_mbed_executable EXECUTABLE)

	# unit tests active -- create a dummy target that only exists so setting properties on it doesn't fail.
	add_executable(${EXECUTABLE} ${MBED_CMAKE_SOURCE_DIR}/cmake/dummy.cpp)

	# prevent this target from being built during normal make commands
	set_property(TARGET ${EXECUTABLE} PROPERTY EXCLUDE_FROM_ALL TRUE)

endfunction(add_mbed_executable)

 
# Convenience function to add an executable containing GTest unit tests.
# CTest will be used to run the tests.
# When unit test mode is disabled, this becomes a no-op.
function(add_mbed_unit_test EXECUTABLE)
	add_executable(${EXECUTABLE} ${ARGN})
	target_link_libraries(${EXECUTABLE} GMock::Main)

	gtest_discover_tests(${EXECUTABLE})
endfunction(add_mbed_unit_test)